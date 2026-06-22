#!/usr/bin/env python3
"""Safely detect and quarantine iCloud-style duplicate files and folders.

Examples:
    python3 icloud_duplicate_cleaner.py /path/to/root
    python3 icloud_duplicate_cleaner.py /path/to/root --hash --export-report report.json
    python3 icloud_duplicate_cleaner.py /path/to/root --interactive --hash
    python3 icloud_duplicate_cleaner.py /path/to/root --quarantine /path/to/quarantine --min-confidence high
    python3 icloud_duplicate_cleaner.py /path/to/root --hash --quarantine /path/to/quarantine --allow-low-confidence-batch

The default mode is a dry run. Nothing is moved unless you explicitly choose an
interactive cleanup or provide --quarantine and confirm the batch prompt.
"""

from __future__ import annotations

import argparse
import datetime as _dt
import hashlib
import json
import os
from dataclasses import dataclass, field
from pathlib import Path, PurePath
import re
import shutil
import stat
import sys
from typing import Dict, Iterable, List, Optional, Sequence, Tuple


DEFAULT_QUARANTINE_NAME = ".icloud_duplicate_quarantine"
HASH_CHUNK_SIZE = 1024 * 1024
MTIME_CLOSE_SECONDS = 2.0
MTIME_NEAR_SECONDS = 24 * 60 * 60

CONFIDENCE_RANK = {"low": 0, "medium": 1, "high": 2}

# iCloud-style duplicate counters normally start at " 2". Requiring a matching
# original in the same parent directory is the main guard against false positives
# such as "Episode 2.mkv" when "Episode.mkv" does not exist.
NUMBERED_SUFFIX_RE = re.compile(r"^(?P<base>.+) (?P<number>(?:[2-9]\d*|1\d+))$")


@dataclass
class DuplicateCandidate:
    candidate_path: Path
    original_path: Path
    item_type: str
    confidence: str
    reasons: List[str]
    candidate_size: Optional[int] = None
    original_size: Optional[int] = None
    candidate_mtime: Optional[float] = None
    original_mtime: Optional[float] = None
    candidate_file_count: Optional[int] = None
    original_file_count: Optional[int] = None
    candidate_hash: Optional[str] = None
    original_hash: Optional[str] = None
    errors: List[str] = field(default_factory=list)

    @property
    def parent_path(self) -> Path:
        return self.candidate_path.parent

    def to_json_dict(self) -> Dict[str, object]:
        return {
            "candidate_path": str(self.candidate_path),
            "original_path": str(self.original_path),
            "parent_path": str(self.parent_path),
            "type": self.item_type,
            "confidence": self.confidence,
            "reasons": self.reasons,
            "candidate_size": self.candidate_size,
            "original_size": self.original_size,
            "candidate_mtime": timestamp_to_iso(self.candidate_mtime),
            "original_mtime": timestamp_to_iso(self.original_mtime),
            "candidate_file_count": self.candidate_file_count,
            "original_file_count": self.original_file_count,
            "candidate_hash": self.candidate_hash,
            "original_hash": self.original_hash,
            "errors": self.errors,
        }


@dataclass
class FileStats:
    size: Optional[int] = None
    mtime: Optional[float] = None
    sha256: Optional[str] = None
    errors: List[str] = field(default_factory=list)


@dataclass
class FolderStats:
    total_size: int = 0
    file_count: int = 0
    latest_mtime: Optional[float] = None
    digest: Optional[str] = None
    errors: List[str] = field(default_factory=list)


@dataclass
class MoveResult:
    candidate: DuplicateCandidate
    destination: Optional[Path]
    moved: bool
    message: str


def timestamp_to_iso(timestamp: Optional[float]) -> Optional[str]:
    if timestamp is None:
        return None
    return _dt.datetime.fromtimestamp(timestamp).isoformat(timespec="seconds")


def format_timestamp(timestamp: Optional[float]) -> str:
    if timestamp is None:
        return "unknown"
    return _dt.datetime.fromtimestamp(timestamp).strftime("%Y-%m-%d %H:%M:%S")


def format_bytes(size: Optional[int]) -> str:
    if size is None:
        return "unknown"
    value = float(size)
    for unit in ("B", "KiB", "MiB", "GiB", "TiB"):
        if value < 1024 or unit == "TiB":
            if unit == "B":
                return f"{int(value)} {unit}"
            return f"{value:.1f} {unit}"
        value /= 1024
    return f"{size} B"


def original_name_for_candidate(name: str, is_file: bool) -> Optional[str]:
    """Return the original name that a numbered iCloud duplicate would match.

    For files, the numeric suffix is expected before the extension chain:
    "Document 2.pdf" -> "Document.pdf" and "archive 2.tar.gz" ->
    "archive.tar.gz". For folders and extensionless files, it is removed from
    the end of the name: "Assets 2" -> "Assets".
    """
    suffix = ""
    base_name = name

    if is_file:
        suffixes = "".join(PurePath(name).suffixes)
        if suffixes:
            suffix = suffixes
            base_name = name[: -len(suffixes)]

    match = NUMBERED_SUFFIX_RE.match(base_name)
    if not match:
        return None

    original_base = match.group("base")
    if not original_base:
        return None
    return f"{original_base}{suffix}"


def safe_lstat(path: Path) -> Tuple[Optional[os.stat_result], Optional[str]]:
    try:
        return path.lstat(), None
    except OSError as exc:
        return None, f"{path}: {exc}"


def collect_file_stats(path: Path, use_hash: bool) -> FileStats:
    stats = FileStats()
    st, error = safe_lstat(path)
    if error:
        stats.errors.append(error)
        return stats

    assert st is not None
    if stat.S_ISLNK(st.st_mode):
        stats.errors.append(f"{path}: symlink skipped")
        return stats
    if not stat.S_ISREG(st.st_mode):
        stats.errors.append(f"{path}: not a regular file")
        return stats

    stats.size = st.st_size
    stats.mtime = st.st_mtime

    if use_hash:
        digest, hash_error = sha256_file(path)
        if hash_error:
            stats.errors.append(hash_error)
        else:
            stats.sha256 = digest

    return stats


def sha256_file(path: Path) -> Tuple[Optional[str], Optional[str]]:
    digest = hashlib.sha256()
    try:
        with path.open("rb") as handle:
            for chunk in iter(lambda: handle.read(HASH_CHUNK_SIZE), b""):
                digest.update(chunk)
    except OSError as exc:
        return None, f"{path}: unable to read for hash: {exc}"
    return digest.hexdigest(), None


def collect_folder_stats(path: Path, use_hash: bool) -> FolderStats:
    stats = FolderStats()
    root_stat, root_error = safe_lstat(path)
    if root_error:
        stats.errors.append(root_error)
        return stats

    assert root_stat is not None
    if stat.S_ISLNK(root_stat.st_mode):
        stats.errors.append(f"{path}: symlinked directory skipped")
        return stats
    if not stat.S_ISDIR(root_stat.st_mode):
        stats.errors.append(f"{path}: not a directory")
        return stats

    stats.latest_mtime = root_stat.st_mtime
    folder_digest = hashlib.sha256() if use_hash else None

    def on_walk_error(exc: OSError) -> None:
        stats.errors.append(f"{exc.filename}: {exc}")

    for dirpath_text, dirnames, filenames in os.walk(path, topdown=True, onerror=on_walk_error, followlinks=False):
        dirpath = Path(dirpath_text)
        dirnames.sort()
        filenames.sort()

        kept_dirnames: List[str] = []
        for dirname in dirnames:
            child = dirpath / dirname
            child_stat, child_error = safe_lstat(child)
            if child_error:
                stats.errors.append(child_error)
                continue
            assert child_stat is not None
            if stat.S_ISLNK(child_stat.st_mode):
                stats.errors.append(f"{child}: symlinked directory skipped")
                continue
            if not stat.S_ISDIR(child_stat.st_mode):
                stats.errors.append(f"{child}: not a directory; skipped")
                continue
            stats.latest_mtime = max_optional_mtime(stats.latest_mtime, child_stat.st_mtime)
            kept_dirnames.append(dirname)
        dirnames[:] = kept_dirnames

        for filename in filenames:
            child = dirpath / filename
            child_stat, child_error = safe_lstat(child)
            if child_error:
                stats.errors.append(child_error)
                continue
            assert child_stat is not None
            if stat.S_ISLNK(child_stat.st_mode):
                stats.errors.append(f"{child}: symlinked file skipped")
                continue
            if not stat.S_ISREG(child_stat.st_mode):
                stats.errors.append(f"{child}: not a regular file; skipped")
                continue

            stats.file_count += 1
            stats.total_size += child_stat.st_size
            stats.latest_mtime = max_optional_mtime(stats.latest_mtime, child_stat.st_mtime)

            if folder_digest is not None:
                relative = child.relative_to(path).as_posix()
                file_digest, hash_error = sha256_file(child)
                if hash_error:
                    stats.errors.append(hash_error)
                    continue
                assert file_digest is not None
                folder_digest.update(b"FILE\0")
                folder_digest.update(relative.encode("utf-8", errors="surrogateescape"))
                folder_digest.update(b"\0")
                folder_digest.update(str(child_stat.st_size).encode("ascii"))
                folder_digest.update(b"\0")
                folder_digest.update(file_digest.encode("ascii"))
                folder_digest.update(b"\0")

    if folder_digest is not None:
        stats.digest = folder_digest.hexdigest()

    return stats


def max_optional_mtime(current: Optional[float], new_value: float) -> float:
    if current is None:
        return new_value
    return max(current, new_value)


def compare_files(candidate_path: Path, original_path: Path, use_hash: bool) -> DuplicateCandidate:
    candidate_stats = collect_file_stats(candidate_path, use_hash)
    original_stats = collect_file_stats(original_path, use_hash)
    errors = candidate_stats.errors + original_stats.errors

    confidence, reasons = file_confidence(candidate_stats, original_stats, use_hash)
    if errors:
        reasons.append("one or more file checks failed; review manually")
        if confidence == "high":
            confidence = "medium"

    return DuplicateCandidate(
        candidate_path=candidate_path,
        original_path=original_path,
        item_type="file",
        confidence=confidence,
        reasons=reasons,
        candidate_size=candidate_stats.size,
        original_size=original_stats.size,
        candidate_mtime=candidate_stats.mtime,
        original_mtime=original_stats.mtime,
        candidate_hash=candidate_stats.sha256,
        original_hash=original_stats.sha256,
        errors=errors,
    )


def file_confidence(candidate: FileStats, original: FileStats, use_hash: bool) -> Tuple[str, List[str]]:
    reasons: List[str] = []

    if use_hash and candidate.sha256 and original.sha256:
        if candidate.sha256 == original.sha256:
            reasons.append("SHA-256 hashes match")
            return "high", reasons
        reasons.append("SHA-256 hashes differ")
        return "low", reasons

    same_size = candidate.size is not None and candidate.size == original.size
    mtime_delta = modified_time_delta(candidate.mtime, original.mtime)

    if same_size:
        reasons.append("file sizes match")
    elif candidate.size is not None and original.size is not None:
        reasons.append("file sizes differ")
    else:
        reasons.append("file size could not be fully compared")

    if mtime_delta is None:
        reasons.append("modified times could not be fully compared")
        return ("medium" if same_size else "low"), reasons

    if mtime_delta <= MTIME_CLOSE_SECONDS:
        reasons.append(f"modified times match within {MTIME_CLOSE_SECONDS:.0f} seconds")
        return ("high" if same_size else "low"), reasons

    if mtime_delta <= MTIME_NEAR_SECONDS:
        reasons.append("modified times are within 24 hours")
        return ("medium" if same_size else "low"), reasons

    reasons.append("modified times differ by more than 24 hours")
    return ("low" if same_size else "low"), reasons


def compare_folders(candidate_path: Path, original_path: Path, use_hash: bool) -> DuplicateCandidate:
    candidate_stats = collect_folder_stats(candidate_path, use_hash)
    original_stats = collect_folder_stats(original_path, use_hash)
    errors = candidate_stats.errors + original_stats.errors

    confidence, reasons = folder_confidence(candidate_stats, original_stats, use_hash)
    if errors:
        reasons.append("one or more folder entries could not be checked; review manually")
        if confidence == "high":
            confidence = "medium"

    return DuplicateCandidate(
        candidate_path=candidate_path,
        original_path=original_path,
        item_type="folder",
        confidence=confidence,
        reasons=reasons,
        candidate_size=candidate_stats.total_size,
        original_size=original_stats.total_size,
        candidate_mtime=candidate_stats.latest_mtime,
        original_mtime=original_stats.latest_mtime,
        candidate_file_count=candidate_stats.file_count,
        original_file_count=original_stats.file_count,
        candidate_hash=candidate_stats.digest,
        original_hash=original_stats.digest,
        errors=errors,
    )


def folder_confidence(candidate: FolderStats, original: FolderStats, use_hash: bool) -> Tuple[str, List[str]]:
    reasons: List[str] = []
    same_count = candidate.file_count == original.file_count
    same_size = candidate.total_size == original.total_size

    if same_count:
        reasons.append("recursive file counts match")
    else:
        reasons.append("recursive file counts differ")

    if same_size:
        reasons.append("recursive total sizes match")
    else:
        reasons.append("recursive total sizes differ")

    if use_hash and candidate.digest and original.digest:
        if candidate.digest == original.digest and same_count and same_size:
            reasons.append("contained file hashes and relative paths match")
            return "high", reasons
        reasons.append("contained file hashes or relative paths differ")
        return "low", reasons

    if same_count and same_size:
        reasons.append("folder content was not hashed; run with --hash for stronger verification")
        return "medium", reasons

    return "low", reasons


def modified_time_delta(candidate_mtime: Optional[float], original_mtime: Optional[float]) -> Optional[float]:
    if candidate_mtime is None or original_mtime is None:
        return None
    return abs(candidate_mtime - original_mtime)


def scan_for_duplicates(
    root: Path,
    include_folders: bool,
    use_hash: bool,
    excluded_dirs: Sequence[Path],
) -> Tuple[List[DuplicateCandidate], List[str]]:
    candidates: List[DuplicateCandidate] = []
    warnings: List[str] = []
    excluded_resolved = [resolve_for_compare(path) for path in excluded_dirs]

    def on_walk_error(exc: OSError) -> None:
        warnings.append(f"{exc.filename}: {exc}")

    for dirpath_text, dirnames, filenames in os.walk(root, topdown=True, onerror=on_walk_error, followlinks=False):
        parent = Path(dirpath_text)
        dirnames.sort()
        filenames.sort()

        kept_dirnames: List[str] = []
        for dirname in dirnames:
            child_dir = parent / dirname
            if is_excluded_path(child_dir, excluded_resolved):
                continue
            if child_dir.is_symlink():
                warnings.append(f"{child_dir}: symlinked directory not followed")
                continue
            kept_dirnames.append(dirname)
        dirnames[:] = kept_dirnames

        filename_set = set(filenames)
        dirname_set = set(kept_dirnames)

        for filename in filenames:
            candidate_path = parent / filename
            if candidate_path.is_symlink():
                warnings.append(f"{candidate_path}: symlinked file skipped")
                continue

            original_name = original_name_for_candidate(filename, is_file=True)
            if not original_name or original_name not in filename_set:
                continue

            original_path = parent / original_name
            if original_path.is_symlink():
                warnings.append(f"{original_path}: original is a symlink; skipped")
                continue

            candidates.append(compare_files(candidate_path, original_path, use_hash))

        if include_folders:
            for dirname in kept_dirnames:
                candidate_path = parent / dirname
                original_name = original_name_for_candidate(dirname, is_file=False)
                if not original_name or original_name not in dirname_set:
                    continue

                original_path = parent / original_name
                if original_path.is_symlink():
                    warnings.append(f"{original_path}: original is a symlink; skipped")
                    continue

                candidates.append(compare_folders(candidate_path, original_path, use_hash))

    candidates.sort(key=lambda item: (str(item.parent_path), item.item_type, str(item.candidate_path)))
    return candidates, warnings


def resolve_for_compare(path: Path) -> Path:
    try:
        return path.resolve()
    except OSError:
        return path.absolute()


def is_excluded_path(path: Path, excluded_resolved: Sequence[Path]) -> bool:
    resolved = resolve_for_compare(path)
    return any(path_is_relative_to(resolved, excluded) for excluded in excluded_resolved)


def path_is_relative_to(path: Path, parent: Path) -> bool:
    try:
        path.relative_to(parent)
        return True
    except ValueError:
        return False


def filter_by_confidence(candidates: Iterable[DuplicateCandidate], minimum: str) -> List[DuplicateCandidate]:
    minimum_rank = CONFIDENCE_RANK[minimum]
    return [candidate for candidate in candidates if CONFIDENCE_RANK[candidate.confidence] >= minimum_rank]


def group_by_parent(candidates: Iterable[DuplicateCandidate]) -> Dict[Path, List[DuplicateCandidate]]:
    grouped: Dict[Path, List[DuplicateCandidate]] = {}
    for candidate in candidates:
        grouped.setdefault(candidate.parent_path, []).append(candidate)
    return dict(sorted(grouped.items(), key=lambda item: str(item[0])))


def render_report(
    candidates: Sequence[DuplicateCandidate],
    warnings: Sequence[str],
    root: Path,
    dry_run: bool,
    quarantine_root: Path,
) -> None:
    print(f"Root: {root}")
    print(f"Mode: {'dry-run' if dry_run else 'cleanup available after confirmation'}")
    print(f"Quarantine: {quarantine_root}")
    print(f"Candidates shown: {len(candidates)}")
    print()

    if not candidates:
        print("No matching iCloud-style duplicate candidates found.")
    else:
        for parent, parent_candidates in group_by_parent(candidates).items():
            print(f"Parent: {parent}")
            for candidate in parent_candidates:
                print(format_candidate(candidate))
            print()

    if warnings:
        print("Warnings:")
        for warning in warnings:
            print(f"  - {warning}")
        print()


def format_candidate(candidate: DuplicateCandidate) -> str:
    lines = [
        f"  [{candidate.confidence.upper()}] {candidate.item_type}",
        f"    candidate: {candidate.candidate_path}",
        f"    original:  {candidate.original_path}",
        f"    size:      {size_comparison_text(candidate)}",
        f"    modified:  {mtime_comparison_text(candidate)}",
        f"    reason:    {'; '.join(candidate.reasons)}",
    ]

    if candidate.item_type == "folder":
        lines.insert(4, f"    files:     {candidate.candidate_file_count} vs {candidate.original_file_count}")

    if candidate.candidate_hash or candidate.original_hash:
        lines.append(f"    hash:      {short_hash(candidate.candidate_hash)} vs {short_hash(candidate.original_hash)}")

    for error in candidate.errors:
        lines.append(f"    error:     {error}")

    return "\n".join(lines)


def size_comparison_text(candidate: DuplicateCandidate) -> str:
    same_text = "same" if candidate.candidate_size == candidate.original_size else "different"
    return (
        f"{same_text} "
        f"({format_bytes(candidate.candidate_size)} candidate vs {format_bytes(candidate.original_size)} original)"
    )


def mtime_comparison_text(candidate: DuplicateCandidate) -> str:
    delta = modified_time_delta(candidate.candidate_mtime, candidate.original_mtime)
    if delta is None:
        delta_text = "delta unknown"
    else:
        delta_text = f"delta {format_seconds(delta)}"
    return (
        f"{format_timestamp(candidate.candidate_mtime)} candidate vs "
        f"{format_timestamp(candidate.original_mtime)} original; {delta_text}"
    )


def format_seconds(seconds: float) -> str:
    if seconds < 60:
        return f"{seconds:.0f}s"
    if seconds < 60 * 60:
        return f"{seconds / 60:.1f}m"
    if seconds < 24 * 60 * 60:
        return f"{seconds / 3600:.1f}h"
    return f"{seconds / (24 * 60 * 60):.1f}d"


def short_hash(value: Optional[str]) -> str:
    if not value:
        return "not computed"
    return value[:16]


def export_report(path: Path, root: Path, candidates: Sequence[DuplicateCandidate], warnings: Sequence[str]) -> None:
    report = {
        "generated_at": _dt.datetime.now().isoformat(timespec="seconds"),
        "root": str(root),
        "candidate_count": len(candidates),
        "candidates": [candidate.to_json_dict() for candidate in candidates],
        "warnings": list(warnings),
    }
    try:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(report, indent=2), encoding="utf-8")
    except OSError as exc:
        raise RuntimeError(f"failed to export report to {path}: {exc}") from exc


def cleanup_candidates(
    candidates: Sequence[DuplicateCandidate],
    root: Path,
    quarantine_root: Path,
    dry_run: bool,
    interactive: bool,
    allow_low_confidence_batch: bool,
) -> List[MoveResult]:
    if not candidates:
        return []

    if interactive:
        return interactive_cleanup(candidates, root, quarantine_root, dry_run)

    if dry_run:
        print("Dry run only. Pass --interactive or --quarantine to move reviewed candidates.")
        return []

    return batch_cleanup(candidates, root, quarantine_root, allow_low_confidence_batch)


def interactive_cleanup(
    candidates: Sequence[DuplicateCandidate],
    root: Path,
    quarantine_root: Path,
    dry_run: bool,
) -> List[MoveResult]:
    results: List[MoveResult] = []
    index = 0
    while index < len(candidates):
        candidate = candidates[index]
        print(format_candidate(candidate))
        print("    action:    [y] move to quarantine  [n] skip  [a] accept all remaining high-confidence  [q] quit")
        try:
            answer = input("> ").strip().lower()
        except EOFError:
            print("Input closed; stopping interactive cleanup.")
            break

        if answer == "q":
            break
        if answer == "n" or answer == "":
            index += 1
            continue
        if answer == "a":
            remaining_high = [item for item in candidates[index:] if item.confidence == "high"]
            print(f"Accepting {len(remaining_high)} remaining high-confidence candidates.")
            for high_candidate in remaining_high:
                result = move_to_quarantine(high_candidate, root, quarantine_root, dry_run)
                results.append(result)
                print(result.message)
            break
        if answer == "y":
            if candidate.confidence == "low":
                print("Low-confidence candidates require extra confirmation. Type LOW to move this item.")
                try:
                    low_answer = input("> ").strip()
                except EOFError:
                    print("Input closed; skipping low-confidence candidate.")
                    index += 1
                    continue
                if low_answer != "LOW":
                    print("Skipped.")
                    index += 1
                    continue
            result = move_to_quarantine(candidate, root, quarantine_root, dry_run)
            results.append(result)
            print(result.message)
            index += 1
            continue

        print("Please enter y, n, a, or q.")

    return results


def batch_cleanup(
    candidates: Sequence[DuplicateCandidate],
    root: Path,
    quarantine_root: Path,
    allow_low_confidence_batch: bool,
) -> List[MoveResult]:
    low_confidence_count = sum(1 for candidate in candidates if candidate.confidence == "low")
    movable = list(candidates) if allow_low_confidence_batch else [
        candidate for candidate in candidates if candidate.confidence != "low"
    ]
    skipped_low = len(candidates) - len(movable)

    if skipped_low:
        print(f"Skipping {skipped_low} low-confidence candidates; they are never moved automatically.")
    if not movable:
        print("No medium- or high-confidence candidates are available for batch quarantine.")
        return []

    print(f"About to move {len(movable)} candidates into quarantine:")
    print(f"  {quarantine_root}")
    if allow_low_confidence_batch and low_confidence_count:
        print(f"This includes {low_confidence_count} low-confidence candidates.")
        print("Type MOVE LOW CONFIDENCE to continue, or anything else to abort.")
        required_answer = "MOVE LOW CONFIDENCE"
    else:
        print("Type MOVE to continue, or anything else to abort.")
        required_answer = "MOVE"
    try:
        answer = input("> ").strip()
    except EOFError:
        print("Input closed; aborting cleanup.")
        return []

    if answer != required_answer:
        print("Cleanup aborted.")
        return []

    results = []
    for candidate in movable:
        result = move_to_quarantine(candidate, root, quarantine_root, dry_run=False)
        results.append(result)
        print(result.message)
    return results


def move_to_quarantine(candidate: DuplicateCandidate, root: Path, quarantine_root: Path, dry_run: bool) -> MoveResult:
    candidate_path = candidate.candidate_path
    if not os.path.lexists(candidate_path):
        return MoveResult(candidate, None, False, f"Skipped missing candidate: {candidate_path}")

    try:
        relative_path = candidate_path.relative_to(root)
    except ValueError:
        return MoveResult(candidate, None, False, f"Skipped candidate outside root: {candidate_path}")

    destination = quarantine_root / relative_path

    if path_is_relative_to(resolve_for_compare(destination), resolve_for_compare(candidate_path)):
        return MoveResult(candidate, destination, False, f"Skipped unsafe destination inside candidate: {destination}")

    destination = unique_destination(destination, candidate.item_type)

    if dry_run:
        return MoveResult(candidate, destination, False, f"DRY-RUN: would move {candidate_path} -> {destination}")

    try:
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.move(str(candidate_path), str(destination))
    except OSError as exc:
        return MoveResult(candidate, destination, False, f"Failed to move {candidate_path}: {exc}")

    return MoveResult(candidate, destination, True, f"Moved {candidate_path} -> {destination}")


def unique_destination(destination: Path, item_type: str) -> Path:
    if not os.path.lexists(destination):
        return destination

    for counter in range(1, 10_000):
        if item_type == "file":
            suffixes = "".join(destination.suffixes)
            if suffixes:
                stem = destination.name[: -len(suffixes)]
                candidate = destination.with_name(f"{stem}.quarantine-{counter}{suffixes}")
            else:
                candidate = destination.with_name(f"{destination.name}.quarantine-{counter}")
        else:
            candidate = destination.with_name(f"{destination.name}.quarantine-{counter}")

        if not os.path.lexists(candidate):
            return candidate

    raise RuntimeError(f"could not find a free quarantine path for {destination}")


def summarize_moves(results: Sequence[MoveResult]) -> None:
    if not results:
        return
    moved = sum(1 for result in results if result.moved)
    skipped = len(results) - moved
    print()
    print(f"Cleanup summary: moved {moved}, skipped or failed {skipped}.")


def validate_root(path: Path) -> Path:
    raw_root = path.expanduser()
    if raw_root.is_symlink():
        raise ValueError(f"root path must not be a symlink: {raw_root}")

    try:
        root = raw_root.resolve(strict=True)
    except OSError as exc:
        raise ValueError(f"invalid root path {path}: {exc}") from exc

    if not root.is_dir():
        raise ValueError(f"root path is not a directory: {root}")
    return root


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Safely detect and quarantine iCloud-style numbered duplicate files and folders."
    )
    parser.add_argument("root", type=Path, help="Root directory to scan recursively.")
    parser.add_argument("--dry-run", action="store_true", help="Only report candidates; do not move anything.")
    parser.add_argument(
        "--interactive",
        action="store_true",
        help="Review candidates one by one before moving accepted items to quarantine.",
    )
    parser.add_argument(
        "--quarantine",
        type=Path,
        help=(
            "Quarantine directory for accepted candidates. Without --interactive this enables batch cleanup "
            "after typing MOVE."
        ),
    )
    parser.add_argument("--hash", action="store_true", help="Compare SHA-256 hashes for stronger confidence.")
    parser.add_argument(
        "--include-folders",
        action="store_true",
        help="Include folder duplicate candidates. This is the default; the flag is accepted for explicitness.",
    )
    parser.add_argument("--files-only", action="store_true", help="Only scan file candidates; ignore folder candidates.")
    parser.add_argument(
        "--min-confidence",
        choices=("high", "medium", "low"),
        default="low",
        help="Only show candidates at or above this confidence level. Default: low.",
    )
    parser.add_argument(
        "--allow-low-confidence-batch",
        action="store_true",
        help=(
            "Allow non-interactive quarantine of low-confidence candidates after typing "
            "'MOVE LOW CONFIDENCE'. Use only after reviewing the report."
        ),
    )
    parser.add_argument("--export-report", type=Path, help="Write the displayed report to a JSON file.")
    return parser


def main(argv: Optional[Sequence[str]] = None) -> int:
    parser = build_arg_parser()
    args = parser.parse_args(argv)

    if args.include_folders and args.files_only:
        parser.error("--include-folders and --files-only cannot be used together")

    try:
        root = validate_root(args.root)
    except ValueError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 2

    quarantine_root = args.quarantine.expanduser() if args.quarantine else root / DEFAULT_QUARANTINE_NAME
    quarantine_root = quarantine_root.resolve() if quarantine_root.exists() else quarantine_root.absolute()
    dry_run = args.dry_run or (not args.interactive and args.quarantine is None)
    include_folders = not args.files_only

    all_candidates, warnings = scan_for_duplicates(
        root=root,
        include_folders=include_folders,
        use_hash=args.hash,
        excluded_dirs=[quarantine_root],
    )
    candidates = filter_by_confidence(all_candidates, args.min_confidence)

    render_report(candidates, warnings, root, dry_run, quarantine_root)

    if args.export_report:
        try:
            export_report(args.export_report.expanduser(), root, candidates, warnings)
        except RuntimeError as exc:
            print(f"Error: {exc}", file=sys.stderr)
            return 1
        print(f"Report exported to {args.export_report.expanduser()}")
        print()

    results = cleanup_candidates(
        candidates=candidates,
        root=root,
        quarantine_root=quarantine_root,
        dry_run=dry_run,
        interactive=args.interactive,
        allow_low_confidence_batch=args.allow_low_confidence_batch,
    )
    summarize_moves(results)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
