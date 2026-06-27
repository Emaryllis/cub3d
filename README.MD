_This project has been created as part of the 42 curriculum by egoh._

# cub3D

An optimized, high-performance 3D raycasting engine and single-pass map interpreter written from scratch in C, inspired by the mechanics of Wolfenstein 3D.

## Description

This project implements a real-time pseudo-3D perspective rendering engine using vector-based raycasting mechanics alongside an optimized single-pass configuration file compiler.

### Technical Highlights

#### 1. Vector-Based DDA Grid Traversal
Instead of relying on slow, uniform ray marching or expensive trigonometric calculations, the rendering pipeline utilizes a high-throughput Digital Differential Analysis (DDA) loop to find wall intersections.
* Every frame, ray vectors are calculated using matrix-free 2D linear algebra (`ray->dir_x = plyr->dir_x + plyr->plane_x * camera_x`). This replaces slow trigonometric tracking with highly efficient O(1) operations.
* By pre-calculating the exact ray hypotenuse scaling factors (`delta_dist_x/y = fabs(1.0 / dir_x/y)`), the loop bypasses incremental steps. The DDA engine compares total distances (`side_dist_x/y`), allowing the ray to jump cleanly from one discrete tile boundary directly to the next until a wall hit is detected.

#### 2. Distortion Correction & Aspect-Ratio Stability
Interacting closely with walls in a pixel grid introduces perspective warping and rendering bugs. This engine corrects these spatial artifacts natively through targeted vector mechanics:
* True Euclidean distance creates curved, fishbowl distortion. This engine isolates the absolute perpendicular distance (`ray->perp_wall_dist`) from the wall intersection point back to the flat player camera plane vector, ensuring all wall lines remain perfectly straight.
* Standard raycasters experience severe horizontal stretching or visual popping the exact moment a wall fills a screen column vertically (clamping draw limits to screen bounds). This engine tracks the true unclamped virtual line height (`ray->line_height`), forcing vertical texture sampling increments to scale proportionally with horizontal vectors even when a wall extends far off-screen.

#### 3. Single-Pass Streaming Parser & Bitmask Validator
The map interpreter is a custom-engineered single-pass pipeline that streams configuration files character-by-character, structuring data and validating map integrity simultaneously. This completely eliminates the need for secondary traversal passes or resource-heavy recursive flood-fills.
* The interpreter tracks tile contexts using a lightweight coordinate-mapped state system. It defers boundary validation for future coordinates by assigning specific rules to tiles (e.g., State 2 = current space must be a wall, State 3 = next row at this column position must be a wall).
* To minimize memory overhead, validation prerequisites are packed into a compressed bitwise array (`bit_valid`). Low-level bit-shifting (`>>`) and bit-masking (`&`) isolate 2-bit state structures within standard bytes, avoiding the allocation of heavy tracking grids.

### Design Philosophy: Pseudo-Object-Oriented C

To manage complexity and prevent structural bloat, directories function as domain-isolated class packages utilizing temporary contexts with strictly decoupled lifecycles:
* Source files are strictly compartmentalized into specialized workspace folders (`src/parse/` and `src/render/`). Internal helper operations are declared as static inside these folders, exposing only unified public entry points (`parse_file()` and `render()`) to clear dependencies between subsystems.
* Rather than mixing data validation tracking, parsing states, and runtime graphics fields into a single massive game structure, modules execute inside localized, short-lived structs. The parsing and validation framework operates completely within its own ephemeral context (`t_p_map`).
* The moment configuration validation succeeds, the internal parsing variables are immediately compiled down into a clean, flat runtime grid, and the entire initialization structure is freed from memory. This guarantees that runtime graphics processes remain completely unburdened by tracking states that are no longer necessary.

### Codebase Organization

#### Core Rendering Module
* `render_ray.c` & `render.h` manages the core raycasting sequence, tracking vector directions, `delta_dist`, `side_dist` progression, and calculating `perp_wall_dist`.
* `render_tex.c` handles affine texture coordinate mapping (`wall_x`) and fixes aspect shearing using virtual `line_height` scaling.
* `render_move.c` & `render_init.c` implements matrix-free 2D vector rotation and axis-aligned bounding box (AABB) collision checks against hitboxes.

#### Parsing & Validation Module
* `parse_map.c` streams map configurations sequentially, translating ASCII data directly into validation states.
* `parse_map_bitmask.c` manages bit-packing and state extraction logic for the `bit_valid` array.
* `parse_map_utils.c` & `parse_struct.c` manages dynamic map dimension scaling, row boundary tracking, multi-pass elimination logic and map dimensional array transformation.

## Instructions

### Controls

| Action | Control |
| :--- | :--- |
| **Move Forward/Backward** | **W / S** |
| **Strafe Left/Right** | **A / D** |
| **Rotate Camera** | **Left / Right Arrow / Mouse Movement** |
| **Exit Application** | **ESC / Window Close** |

### Compilation

The project features a dual-build setup: a modern CMake layout configuration alongside a standard-compliant GNU Makefile matching the mandatory subject constraints.

```bash
# Option A: Compilation via Native Makefile (Mandatory Target)
make

# Option B: Compilation via CMake (Developer Target)
cmake -S . -B cmake-build-debug/
cmake --build cmake-build-debug/
```

### Execution

Execute the compiled binary alongside a valid map configuration path utilizing the `.cub` file extension:

```bash
# Option A: After Native Makefile (Mandatory Target)
./cub3d test.cub

# Option B: After CMake (Developer Target)
./cub3d.sh test.cub
```

### Memory Leak Verification

The program handles cascading resource teardowns to guarantee clean, leak-free returns across all execution paths. To run the tracking automated profiling targets:

```bash
cmake --build cmake-build-debug/ --target leak_check
```

## Resources

* [**Amanatides, J., & Woo, A. (1987): "A Fast Voxel Traversal Algorithm for Ray Tracing"**](http://www.cse.yorku.ca/~amana/research/grid.pdf) — Theoretical foundation for discrete grid-stepping logic and digital differential analysis (DDA) without uniform spatial sampling overhead.
* [**Heckbert, P. S. (1986): "Survey of Texture Mapping"**](https://graphicsinterface.org/wp-content/uploads/2015/03/gi1986-37.pdf) — Foundational computer graphics text outlining affine coordinate mapping, spatial projections, and texture coordinate sampling over discrete screen spans.
* [**Bailey, D. G., & Johnston, C. T. (2007): "Single Pass Connected Components Analysis"**](https://www-ist.massey.ac.nz/dbailey/sprg/pdfs/2007_IVCNZ_282.pdf) — Spatial data processing paper detailing how to analyze grid topologies in a single pass by caching row-neighborhood contexts and deferring boundary validations until row boundaries change, minimizing memory footprints in stream-oriented environments.
* [**Ullman, J. D. (2006): "Syntax-Directed Translation Schemes"**](http://infolab.stanford.edu/~ullman/dragon/slides2.pdf) — Stanford InfoLab architectural guide for single-pass input parsing and evaluation, where evaluation of downstream structural validation rules is performed efficiently on-the-fly without needing an explicit multi-pass parse tree structure.
* [**MinilibX Source Repository**](https://github.com/42paris/minilibx-linux) — Target software graphics API specifying windowing abstractions, OS subsystem event hooks, and direct pixel-buffer manipulation constraints.
