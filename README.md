# Psych Engine

## 3D cross-platform game engine

Psych is currently under active development. It aims to be a cross-platform 3D engine with a simple GUI and be highly configurable.

## Open Source

Psych is licensed under MIT and will always be free.

## Background and Disclaimer

This engine is a personal project that may very well one day be worth it to somebody. However, it takes a lot of insperation from other engines
such as [Hazel](https://github.com/TheCherno/Hazel) and diverges at points were my studies have lead me. I am also a student and have spent maybe
two years of actual programming, less in c++. As a result, you should not expect the code quality or architectural design to be of any quality.
I am open to learn and this project is my way of forcing myself into a proper codebase that is mine.

## Contributing

I don't have any formal process for contributing as of yet. Feel free to suggest what ever.
Try to follow the architecture of the engine. It is what it is.

## Building With CMake

The engine uses CMake 3.20 or newer. Configure from the repository root:

```bash
cmake -S . -B build
```

Build the default target:

```bash
cmake --build build --parallel
```

The `PsychEngine` library is always configured; there is no separate `BUILD_ENGINE` option at the moment. By default, the non-editor `App` frontend is configured. To configure the editor frontend instead:

```bash
cmake -S . -B build -DPSYCH_ENGINE_BUILD_EDITOR=ON
cmake --build build --target Editor --parallel
```

`BUILD_EDITOR=ON` is still accepted as a backwards-compatible alias for `PSYCH_ENGINE_BUILD_EDITOR=ON`.

Common targets:

```bash
cmake --build build --target PsychEngine --parallel
cmake --build build --target Editor --parallel
cmake --build build --target App --parallel
cmake --build build --target PsychEngineTests --parallel
```

`Editor` exists only when configured with `-DPSYCH_ENGINE_BUILD_EDITOR=ON`. `App` exists only when the editor option is off.

### CMake Options

| Option                           | Default | Description                                                                |
| -------------------------------- | ------- | -------------------------------------------------------------------------- |
| `PSYCH_ENGINE_BUILD_EDITOR`      | `OFF`   | Configure the `Editor` executable instead of the default `App` executable. |
| `PSYCH_ENGINE_BUILD_TESTS`       | `OFF`   | Configure the `PsychEngineTests` target and CTest integration.             |
| `PSYCH_ENABLE_ASSERTS`           | `ON`    | Enable engine runtime assertions.                                          |
| `PSYCH_ENABLE_PROFILING`         | `ON`    | Enable profiling instrumentation.                                          |
| `PSYCH_ENABLE_SANITIZERS`        | `OFF`   | Add AddressSanitizer/UndefinedBehaviorSanitizer flags to `PsychEngine`.    |
| `PSYCH_ENGINE_ENABLE_CLANG_TIDY` | `OFF`   | Run `clang-tidy` while compiling `PsychEngine` and `Editor` sources.       |
| `BUILD_SHARED_LIBS`              | `OFF`   | Standard CMake option used by dependencies that respect it.                |

Build type can be set with standard CMake values:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel
```

### Tests

Configure tests and build the test binary:

```bash
cmake -S . -B build-tests -DPSYCH_ENGINE_BUILD_TESTS=ON
cmake --build build-tests --target PsychEngineTests --parallel
```

Run tests through CTest:

```bash
ctest --test-dir build-tests --output-on-failure
```

### Clang-Tidy

Configure with clang-tidy enabled:

```bash
cmake -S . -B build-tidy -DPSYCH_ENGINE_ENABLE_CLANG_TIDY=ON -DPSYCH_ENGINE_BUILD_EDITOR=ON
cmake --build build-tidy --target Editor --parallel
```

This applies clang-tidy only to the engine/editor targets that call the project helper, not to vendored dependency targets.

### Sanitizers

Configure a sanitizer build:

```bash
cmake -S . -B build-asan -DGE_ENABLE_SANITIZERS=ON -DPSYCH_ENGINE_BUILD_EDITOR=ON
cmake --build build-asan --target Editor --parallel
```
