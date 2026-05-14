cmake_minimum_required(VERSION 3.10)
project(cub3d VERSION 1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# --------------------------------------------------------
# OPTIONS
# --------------------------------------------------------

set(AUTO_FETCH_SUBMODULES   ON) # Auto init/update git submodules at configure time
set(ENABLE_ASAN             OFF) # Enable AddressSanitizer
set(ENABLE_C_FLAGS          OFF) # Enable additional compiler warnings
set(CUB_FILE "${CMAKE_SOURCE_DIR}/test.cub" CACHE FILEPATH "Path to .cub file")

# --------------------------------------------------------
# INITIALIZATION
# --------------------------------------------------------

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")
file(GLOB HEADERS "${CMAKE_SOURCE_DIR}/main.h")
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set(WRAPPER "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.sh")
if (ENABLE_C_FLAGS)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
endif ()
message(STATUS "ENABLE_C_FLAGS=${ENABLE_C_FLAGS}")

# --------------------------------------------------------
# VALGRIND LEAK CHECK
# --------------------------------------------------------
add_custom_target(leak_check
        COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $<TARGET_FILE:${PROJECT_NAME}> "${CUB_FILE}"
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Leak Check with Valgrind"
)

if (AUTO_FETCH_SUBMODULES)
    find_package(Git QUIET)
    if (GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
        set(SUBMODULES_NEED_UPDATE FALSE)

        if (NOT EXISTS "${CMAKE_SOURCE_DIR}/minilibx-linux/Makefile")
            set(SUBMODULES_NEED_UPDATE TRUE)
            message(STATUS "MiniLibX missing or incomplete; will init/update submodules.")
        endif ()

        if (SUBMODULES_NEED_UPDATE)
            execute_process(
                    COMMAND "${GIT_EXECUTABLE}" submodule sync --recursive
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    RESULT_VARIABLE GIT_SYNC_RESULT
            )
            execute_process(
                    COMMAND "${GIT_EXECUTABLE}" submodule update --init --recursive --force
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    RESULT_VARIABLE GIT_SUBMOD_RESULT
            )
            if ((NOT GIT_SYNC_RESULT EQUAL 0) OR (NOT GIT_SUBMOD_RESULT EQUAL 0))
                message(FATAL_ERROR
                        "Failed to sync/init submodules.\n"
                        "Run manually:\n"
                        "  git submodule sync --recursive\n"
                        "  git submodule update --init --recursive --force")
            endif ()
        endif ()
    else ()
        message(WARNING "Git not found or not a git checkout; cannot auto-fetch submodules.")
    endif ()
endif ()

set(MLX_DIR "${CMAKE_SOURCE_DIR}/minilibx-linux")
set(MLX_LIB "${MLX_DIR}/libmlx.a")

add_custom_command(
        OUTPUT "${MLX_LIB}"
        COMMAND ./configure
        WORKING_DIRECTORY "${MLX_DIR}"
        COMMENT "Configuring MiniLibX"
        VERBATIM
)
add_custom_target(build_mlx DEPENDS "${MLX_LIB}")
add_dependencies(${PROJECT_NAME} build_mlx)

# --------------------------------------------------------
# ADDRESS SANITIZER
# --------------------------------------------------------
file(WRITE ${WRAPPER} "#!/bin/bash\n")
if (ENABLE_ASAN AND CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    message(STATUS "Enabling AddressSanitizer...")
    message(STATUS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:-fsanitize=address -O1 -fno-omit-frame-pointer>
    )
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:-fsanitize=address>
    )
    file(APPEND ${WRAPPER} "export ASAN_OPTIONS=\"halt_on_error=0:verbosity=1\"\n")
else ()
    file(APPEND ${WRAPPER} "unset ASAN_OPTIONS\n")
endif ()
message(STATUS "Creating wrapper...")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND echo "exec $<TARGET_FILE:${PROJECT_NAME}> \"\$@\"" >> ${WRAPPER}
        COMMAND chmod +x ${WRAPPER}
        VERBATIM
)
message("${WRAPPER}")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${MLX_LIB} X11 Xext m)