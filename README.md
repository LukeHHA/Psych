# Game Engine

## iCloud Duplicate Cleaner

`icloud_duplicate_cleaner.py` is a Python 3 standard-library utility for reviewing iCloud-style numbered duplicates such as `Document 2.pdf`, `main 3.cpp`, or `Assets 2`.

Start with a dry run:

```bash
python3 icloud_duplicate_cleaner.py /path/to/root
```

For stronger verification, include hashes and export a JSON report:

```bash
python3 icloud_duplicate_cleaner.py /path/to/root --hash --export-report report.json
```

To clean up safely, use interactive mode. Accepted items are moved to quarantine, preserving their relative paths:

```bash
python3 icloud_duplicate_cleaner.py /path/to/root --interactive --hash
```

To choose the quarantine location explicitly:

```bash
python3 icloud_duplicate_cleaner.py /path/to/root --interactive --hash --quarantine /path/to/quarantine
```

Batch quarantine is also available, but it still requires typing `MOVE` before anything is moved:

```bash
python3 icloud_duplicate_cleaner.py /path/to/root --quarantine /path/to/quarantine --min-confidence high
```

If you already reviewed the JSON report and want to quarantine every displayed candidate, including low-confidence candidates, use the explicit batch override:

```bash
python3 icloud_duplicate_cleaner.py /path/to/root --hash --quarantine /path/to/quarantine --min-confidence low --allow-low-confidence-batch
```

That command still requires typing `MOVE LOW CONFIDENCE` once before anything is moved.

Safety notes:

- The default mode is dry-run; nothing is moved without explicit cleanup flags and confirmation.
- Low-confidence candidates are not moved by normal batch cleanup; `--allow-low-confidence-batch` is only intended for a reviewed report.
- Folder candidates are included by default; use `--files-only` to restrict scanning to files.
- Symlinked directories are not followed.
- Review the report before moving anything, especially when not using `--hash`.
