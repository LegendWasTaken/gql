#ifndef GQL_GL_H
#define GQL_GL_H

#include <vector>
#include <filesystem>

#include <glad/glad.h>
#include <fstream>

#include <fmt/format.h>

namespace gql {
struct shader {
  std::filesystem::path path;
  std::uint32_t handle;
};

namespace detail {
[[nodiscard]] std::string read_file(std::filesystem::path path) {
  auto file_buffer = std::ifstream(path);
  auto string_buffer = std::stringstream();
  return string_buffer << file_buffer.rdbuf(), string_buffer.str();
}

[[nodiscard]] shader create_shader(std::string_view path) {
  if (std::filesystem::exists(path)) {
    const auto source = detail::read_file(path);

    auto type = GLuint();
    if (path.ends_with(".vert"))
      type = GL_VERTEX_SHADER;
    else if (path.ends_with(".frag"))
      type = GL_FRAGMENT_SHADER;
    else if (path.ends_with(".comp"))
      type = GL_COMPUTE_SHADER;

    const auto handle = glCreateShader(type);
    const auto source_str = source.c_str();
    glShaderSource(handle, 1, &source_str, nullptr);
    glCompileShader(handle);

    auto success = std::int32_t();
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

    if (success == GL_TRUE) {
      return shader {
        .path = path,
        .handle = handle};
    } else {
      auto log = std::array<char, 513>();
      glGetShaderInfoLog(handle, 512, nullptr, log.data());
      fmt::print("failed to create shader {} because {}", path, log.data());
    }
  } else {
    fmt::print("shader path doesnt exist");
  }
}

void load_shader_into(std::string_view path, std::vector<shader> &shaders) {
  shaders.push_back(create_shader(path));
}
}

struct program {
  std::vector<shader> shaders = {};
  std::uint32_t handle = -1;
};

template<typename ...Args>
[[nodiscard]] program create_program(Args ...shader_paths) {
  static_assert(sizeof...(shader_paths) <= 2 && "Only 2 shaders are supported per program");
  auto program = gql::program();
  program.handle = glCreateProgram();
  (detail::load_shader_into(shader_paths, program.shaders), ...);

  for (const auto shader : program.shaders)
    glAttachShader(program.handle, shader.handle);

  glLinkProgram(program.handle);

  auto success = int();
  glGetProgramiv(program.handle, GL_LINK_STATUS, &success);

  if (!success) {
    auto log = std::array<char, 513>();
    glGetProgramInfoLog(program.handle, 512, nullptr, log.data());
    fmt::print("failed to link program {}", log.data());
  }

  return program;
}

void use(gql::program &program) {
  glUseProgram(program.handle);
}

void reload(gql::shader &shader) {
  glDeleteShader(shader.handle);
  shader = detail::create_shader(shader.path.string());
}

void reload(gql::program &program) {
  glDeleteProgram(program.handle);
  for (auto &shader : program.shaders) {
    reload(shader);
  }
  program.handle = glCreateProgram();

  for (const auto& shader : program.shaders)
    glAttachShader(program.handle, shader.handle);

  glLinkProgram(program.handle);

  auto success = int();
  glGetProgramiv(program.handle, GL_LINK_STATUS, &success);

  if (!success) {
    auto log = std::array<char, 513>();
    glGetProgramInfoLog(program.handle, 512, nullptr, log.data());
    fmt::print("failed to link program {}", log.data());
  }
}
}

#endif //GQL_GL_H
