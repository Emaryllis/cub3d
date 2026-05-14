## How to run the project:

```bash
cmake -S . -B cmake-build-debug/
cmake --build cmake-build-debug/
./cub3d.sh
```

## How to test for leaks:
```bash
cmake --build cmake-build-debug/ --target leak_check
```