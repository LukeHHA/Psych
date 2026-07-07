#!/usr/bin/env python3
"""Generate C++ accessors for binary resources.

Example:
  python tools/embed_resources.py \
    --namespace psych::embedded \
    --output-header build/generated/EmbeddedResources.h \
    --output-source build/generated/EmbeddedResources.cpp \
    Resources/Fonts/JetBrainsMonoNerdFont-Regular.ttf=JetBrainsMonoNerdFontRegular
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path


def parse_resource(value: str) -> tuple[Path, str]:
    separator = "=" if "=" in value else ":"
    if separator not in value:
        raise argparse.ArgumentTypeError(
            f"resource '{value}' must use '<path>=<symbol>'"
        )

    raw_path, symbol = value.rsplit(separator, 1)
    if not raw_path or not symbol:
        raise argparse.ArgumentTypeError(
            f"resource '{value}' must use '<path>=<symbol>'"
        )

    if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol):
        raise argparse.ArgumentTypeError(
            f"resource symbol '{symbol}' is not a valid C++ identifier"
        )

    return Path(raw_path), symbol


def namespace_open(namespace: str) -> str:
    return f"namespace {namespace}\n{{"


def namespace_close(namespace: str) -> str:
    return f"}} // namespace {namespace}"


def write_header(path: Path, namespace: str, resources: list[tuple[Path, str]]) -> None:
    lines = [
        "#pragma once",
        "",
        '#include "Resources/BinaryBuffer.h"',
        "",
        namespace_open(namespace),
        "",
    ]

    for _, symbol in resources:
        lines.append(f"[[nodiscard]] BinaryBuffer {symbol}();")

    lines.extend(["", namespace_close(namespace), ""])
    path.write_text("\n".join(lines), encoding="utf-8")


def byte_lines(data: bytes) -> list[str]:
    lines: list[str] = []
    for index in range(0, len(data), 12):
        chunk = data[index : index + 12]
        values = ", ".join(f"0x{byte:02x}" for byte in chunk)
        lines.append(f"  {values},")
    return lines


def write_source(
    path: Path,
    header_name: str,
    namespace: str,
    resources: list[tuple[Path, str]],
) -> None:
    lines = [
        f'#include "{header_name}"',
        "",
        namespace_open(namespace),
        "namespace",
        "{",
        "",
    ]

    for resource_path, symbol in resources:
        data = resource_path.read_bytes()
        lines.append(f"alignas(16) unsigned char {symbol}Data[] = {{")
        lines.extend(byte_lines(data))
        lines.append("};")
        lines.append("")

    lines.extend(["} // namespace", ""])

    for _, symbol in resources:
        lines.append(f"BinaryBuffer {symbol}()")
        lines.append("{")
        lines.append(f"  return BinaryBuffer{{{symbol}Data, sizeof({symbol}Data)}};")
        lines.append("}")
        lines.append("")

    lines.extend([namespace_close(namespace), ""])
    path.write_text("\n".join(lines), encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--namespace", required=True)
    parser.add_argument("--output-header", required=True, type=Path)
    parser.add_argument("--output-source", required=True, type=Path)
    parser.add_argument("resources", nargs="+", type=parse_resource)
    args = parser.parse_args()

    output_header: Path = args.output_header
    output_source: Path = args.output_source
    output_header.parent.mkdir(parents=True, exist_ok=True)
    output_source.parent.mkdir(parents=True, exist_ok=True)

    for resource_path, _ in args.resources:
        if not resource_path.is_file():
            parser.error(f"resource file does not exist: {resource_path}")

    write_header(output_header, args.namespace, args.resources)
    write_source(output_source, output_header.name, args.namespace, args.resources)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
