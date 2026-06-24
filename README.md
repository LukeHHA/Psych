# Game Engine

## Building With CMake

The engine uses CMake 3.20 or newer. Configure from the repository root:

```bash
cmake -S . -B build
```

Build the default target:

```bash
cmake --build build --parallel
```

The `GameEngine` library is always configured; there is no separate `BUILD_ENGINE` option at the moment. By default, the non-editor `App` frontend is configured. To configure the editor frontend instead:

```bash
cmake -S . -B build -DGAME_ENGINE_BUILD_EDITOR=ON
cmake --build build --target Editor --parallel
```

`BUILD_EDITOR=ON` is still accepted as a backwards-compatible alias for `GAME_ENGINE_BUILD_EDITOR=ON`.

Common targets:

```bash
cmake --build build --target GameEngine --parallel
cmake --build build --target Editor --parallel
cmake --build build --target App --parallel
cmake --build build --target GameEngineTests --parallel
```

`Editor` exists only when configured with `-DGAME_ENGINE_BUILD_EDITOR=ON`. `App` exists only when the editor option is off.

### CMake Options

| Option | Default | Description |
| --- | --- | --- |
| `GAME_ENGINE_BUILD_EDITOR` | `OFF` | Configure the `Editor` executable instead of the default `App` executable. |
| `GAME_ENGINE_BUILD_TESTS` | `OFF` | Configure the `GameEngineTests` target and CTest integration. |
| `GE_ENABLE_ASSERTS` | `ON` | Enable engine runtime assertions. |
| `GE_ENABLE_PROFILING` | `ON` | Enable profiling instrumentation. |
| `GE_ENABLE_SANITIZERS` | `OFF` | Add AddressSanitizer/UndefinedBehaviorSanitizer flags to `GameEngine`. |
| `GAME_ENGINE_ENABLE_CLANG_TIDY` | `OFF` | Run `clang-tidy` while compiling `GameEngine` and `Editor` sources. |
| `BUILD_SHARED_LIBS` | `OFF` | Standard CMake option used by dependencies that respect it. |

Build type can be set with standard CMake values:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel
```

### Tests

Configure tests and build the test binary:

```bash
cmake -S . -B build-tests -DGAME_ENGINE_BUILD_TESTS=ON
cmake --build build-tests --target GameEngineTests --parallel
```

Run tests through CTest:

```bash
ctest --test-dir build-tests --output-on-failure
```

### Clang-Tidy

Configure with clang-tidy enabled:

```bash
cmake -S . -B build-tidy -DGAME_ENGINE_ENABLE_CLANG_TIDY=ON -DGAME_ENGINE_BUILD_EDITOR=ON
cmake --build build-tidy --target Editor --parallel
```

This applies clang-tidy only to the engine/editor targets that call the project helper, not to vendored dependency targets.

### Sanitizers

Configure a sanitizer build:

```bash
cmake -S . -B build-asan -DGE_ENABLE_SANITIZERS=ON -DGAME_ENGINE_BUILD_EDITOR=ON
cmake --build build-asan --target Editor --parallel
```

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
