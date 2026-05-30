# Contributing to loopin-ambience-mode

First off, thank you for considering contributing to loopin-ambience-mode! It's people like you that make it a great tool for everyone.

## How Can I Contribute?

### Reporting Bugs
* Check the existing issues to see if the bug has already been reported.
* If not, open a new issue. Clearly describe the problem, including steps to reproduce it and your environment (OS, hardware, etc.).

### Suggesting Enhancements
* Open a new issue and describe the enhancement you'd like to see and why it would be useful.

### Pull Requests
1. **Fork** the repository on GitHub.
2. **Clone** your fork locally.
3. **Create a branch** for your changes: `git checkout -b feature/your-feature-name` or `bugfix/your-bug-name`.
4. **Make your changes**.
5. **Commit** your changes with clear and descriptive commit messages.
6. **Push** your branch to your fork on GitHub.
7. **Submit a Pull Request** to the `main` branch of the original repository.

## Coding Style

To keep the codebase clean and consistent, please follow these guidelines for C++:

- **Standard**: C++17
- **Naming**:
  - Classes: `PascalCase`
  - Methods and functions: `camelCase`
  - Variables: `camelCase`
  - Constants and Enums: `SCREAMING_SNAKE_CASE`
  - Files: `PascalCase.cpp` / `PascalCase.h`
- **Indentation**: Use 4 spaces (no tabs).
- **Braces**: Place braces on their own lines for classes and functions. For control flow (if, while, etc.), follow the style prevalent in the file.
- **Documentation**: Use Doxygen-style comments for public APIs.
- **Headers**: Use `#pragma once` for header guards.

## Build and Test

Before submitting a PR, ensure that the project builds successfully on your platform and that any existing tests pass.

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

Thank you for your contributions!
