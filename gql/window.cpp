#include "window.h"

#include <glad/glad.h>

#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

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

void gql::window::internal_mouse_pos_listener(GLFWwindow *window, double x, double y) {
  auto self = reinterpret_cast<gql::window*>(glfwGetWindowUserPointer(window));
  self->emit_mouse_move(event::mouse_move {
    .x = x,
    .y = y
  });
}

void gql::window::internal_mouse_button_listener(GLFWwindow *window, int button, int action, int mods) {
  auto self = reinterpret_cast<gql::window*>(glfwGetWindowUserPointer(window));
  if (action == GLFW_PRESS) {
    self->emit_mouse_button_press(event::mouse_button_press {
      .button = button
    });
  } else if (action == GLFW_RELEASE) {
    self->emit_mouse_button_release(event::mouse_button_release {
      .button = button
    });
  }
}

gql::window::window(std::uint16_t width, std::uint16_t height, std::string_view title) : width(width), height(height) {
  handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

  glfwMakeContextCurrent(handle);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glfwSetWindowUserPointer(handle, this);
  glfwSetKeyCallback(handle, window::internal_key_listener);
  glfwSetCursorPosCallback(handle, window::internal_mouse_pos_listener);
  glfwSetMouseButtonCallback(handle, window::internal_mouse_button_listener);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *)handle, true);
  ImGui_ImplOpenGL3_Init("#version 450");
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

gql::window::tick_wrapper::~tick_wrapper() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(handle);
}
