#ifndef GQL_WINDOW_H
#define GQL_WINDOW_H

#include <GLFW/glfw3.h>

#include <cstdint>
#include <string_view>
#include <type_traits>
#include <functional>

#include <gql/event.h>

namespace gql::detail {
template <typename T>
concept is_keyboard_event_consumer = requires(T t, event::key_press press, event::key_release release) {
  {t.consume(press)} -> std::same_as<void>;
  {t.consume(release)} -> std::same_as<void>;
};
}

namespace gql {
class window {
private:
  GLFWwindow *handle;
  std::uint16_t width;
  std::uint16_t height;

  std::function<void(event::key_press)> emit_key_press = []([[maybe_unused]] event::key_press){};
  std::function<void(event::key_release)> emit_key_release = []([[maybe_unused]] event::key_release){};

  static void internal_key_listener(GLFWwindow *window, int key, int scancode, int action, int mods);

public:
  struct init {
    init();
  };

  window(std::uint16_t width, std::uint16_t height, std::string_view title);

  [[nodiscard]] bool should_close() const;

  struct tick_wrapper {
  private:
    GLFWwindow *handle;
  public:
    explicit tick_wrapper(GLFWwindow *handle);

    ~tick_wrapper();
  };
  [[nodiscard]] tick_wrapper tick();

  template <typename T> requires detail::is_keyboard_event_consumer<T>
  void keyboard_events(T *event_handler) {
    emit_key_press = [event_handler](event::key_press event) {
      event_handler->consume(event);
    };
    emit_key_release = [event_handler](event::key_release event) {
      event_handler->consume(event);
    };
  }
};
}

#endif //GQL_WINDOW_H
