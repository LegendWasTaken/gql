#include "window.h"

#include <glad/glad.h>

gql::window::init::init() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
}

void gql::window::internal_key_listener(GLFWwindow *window, int key, int scancode, int action, int mods) {
  auto self = reinterpret_cast<gql::window*>(glfwGetWindowUserPointer(window));
  if (action == GLFW_PRESS) {
    self->emit_key_press(event::key_press {
      .key = key
    });
  } else if (action == GLFW_RELEASE) {
    self->emit_key_release(event::key_release {
      .key = key
    });
  }
}

gql::window::window(std::uint16_t width, std::uint16_t height, std::string_view title) : width(width), height(height) {
  handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

  glfwMakeContextCurrent(handle);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glfwSetWindowUserPointer(handle, this);
  glfwSetKeyCallback(handle, window::internal_key_listener);
}

bool gql::window::should_close() const {
  return glfwWindowShouldClose(handle);
}

gql::window::tick_wrapper gql::window::tick() {
  return gql::window::tick_wrapper(handle);
}

gql::window::tick_wrapper::tick_wrapper(GLFWwindow *handle) : handle(handle) {
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwPollEvents();
}

gql::window::tick_wrapper::~tick_wrapper() {
  glfwSwapBuffers(handle);
}
