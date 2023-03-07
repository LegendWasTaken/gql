include(FetchContent)

option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_INSTALL "" OFF)
option(GLFW_BUILD_EXAMPLES "" OFF)
option(FMT_PEDANTIC  "" OFF)
option(FMT_WERROR   "" OFF)
option(FMT_DOC   "" OFF)
option(FMT_TEST   "" OFF)
option(FMT_FUZZ   "" OFF)
option(FMT_CUDA_TEST   "" OFF)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG  3.3.8)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 7.1.3
)

FetchContent_MakeAvailable(glfw fmt)

# ImGui doesn't provide it's own CMakeLists, so we create it ourselves
file(GLOB_RECURSE imgui_src
        "imgui/*.h"
        "imgui/*.cpp")
add_library(ImGui ${imgui_src})

target_include_directories(ImGui PUBLIC imgui)
target_link_libraries(ImGui PUBLIC glfw)
target_compile_definitions(ImGui PUBLIC -DGLFW_INCLUDE_NONE -DIMGUI_IMPL_OPENGL_LOADER_GLAD)

add_library(glad glad/glad.c)
target_include_directories(glad PUBLIC ../external)
