# lingmo-utils

Lingmo Desktop - Utility library.

## Overview

`lingmo-utils` provides common utility functions for Lingmo Desktop components:

- **Logger**: Structured logging with categories, levels, and Qt integration
- **FileUtils**: File operations with progress callbacks and MIME detection
- **XdgUtils**: XDG Base Directory path resolution
- **Process**: Process spawning and output capture

## Dependencies

- Qt 6 (Core)

## Building

```bash
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build
ctest --test-dir build
```

## License

GPL-2.0-or-later
