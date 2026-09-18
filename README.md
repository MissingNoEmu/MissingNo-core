# MissingNo

Platform-independent, dependency-free Game Boy emulation library written in C99. No libc used.

## Usage

Add MissingNo to your CMake project with `add_subdirectory()`, and link against the `MissingNo::MissingNo` target:

```cmake
add_subdirectory(path/to/MissingNo)
target_link_libraries(your_target PRIVATE MissingNo::MissingNo)
```
```C
#include <MissingNo/MissingNo.h>
```

## Platform Types

Edit `MissingNo.h` to make sure definitions of:
- `mn_u8` matches your platform's 8-bit unsigned integer (exact width)
- `mn_u16` matches your platform's 16-bit unsigned integer (exact width)
- `mn_size` matches your platform's `size_t` equivalent

## License

This project is licensed under the MIT license. See LICENSE file for more details.
