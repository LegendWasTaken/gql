#ifndef GQL_IMGUI_H
#define GQL_IMGUI_H

#include <glad/glad.h>

#include <imgui.h>

#include <string_view>

namespace gql {
namespace detail {
template <typename T>
concept can_be_slider_t = requires(T param) {
  requires std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, double>;
};

template <typename T>
void display_widget(T &widget) {
  ImGui::Begin(widget.name().data());
  widget.render();
  ImGui::End();
}

template<template<class> class T, class U>
inline void display_component(T<U> &component) = delete;

template <typename T>
inline void display_component(T &component) = delete;
}

template <typename T> requires detail::can_be_slider_t<T>
struct slider {
private:
  T min;
  T max;
  std::string_view name;
  T val;
public:
  slider(T min, T max, std::string_view name, T def) : min(min), max(max), name(name), val(def) { }

  [[nodiscard]] const char *label() const noexcept { return name.data(); }

  [[nodiscard]] T* ptr() { return &val; }

  [[nodiscard]] T min_val() const noexcept { return min; }
  [[nodiscard]] T max_val() const noexcept { return max; }
};

template <typename T>
struct input {
private:
  T val;
  std::string_view name;
public:
  input(std::string_view name, T def) : name(name), val(def) { }

  [[nodiscard]] const char *label() const noexcept { return name.data(); }

  [[nodiscard]] T* ptr() { return &val; }
};

struct radio {
private:
  bool val;
  std::string_view name;
public:
  radio(std::string_view name, bool def) : name(name), val(def) { }

  [[nodiscard]] const char *label() const noexcept { return name.data(); }

  [[nodiscard]] bool* ptr() { return &val; }
};

template <typename ...Args>
void display_widgets(Args& ...args) {
  (detail::display_widget(args), ...);
}

template <typename ...Args>
inline void component_block(Args &...args) {
  (detail::display_component(args), ...);
}

template <typename ...Args>
inline void collapsable_block(std::string_view title, Args &...args) {
  if (ImGui::CollapsingHeader(title.data())) {
    component_block(args...);
  }
}
}

namespace gql::detail {
template <>
inline void display_component<slider, float>(slider<float> &component) {
  ImGui::SliderFloat(component.label(), component.ptr(), component.min_val(), component.max_val());
}

template <>
inline void display_component<slider, int>(slider<int> &component) {
  ImGui::SliderInt(component.label(), component.ptr(), component.min_val(), component.max_val());
}

template <>
inline void display_component<input, float>(input<float> &component) {
  ImGui::InputFloat(component.label(), component.ptr());
}

template <>
inline void display_component<input, int>(input<int> &component) {
  ImGui::InputInt(component.label(), component.ptr());
}

template <>
inline void display_component<radio>(radio &component) {
  if (ImGui::RadioButton(component.label(), *component.ptr())) {
    *component.ptr() = !*component.ptr();
  }
}
}

#endif //GQL_IMGUI_H
