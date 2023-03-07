#ifndef GQL_EVENT_H
#define GQL_EVENT_H

#include <array>

namespace gql::event {
struct key_press {
  int key;
};

struct key_release {
  int key;
};

struct mouse_move {
  double x;
  double y;
};

struct mouse_button_press {
  int button;
};

struct mouse_button_release {
  int button;
};
}

namespace gql {
class window;

struct keyboard_event_consumer {
private:
  friend window;

  enum class key_state {
    UNKNOWN,
    PRESSED,
    HELD,
    UP,
  };

  std::array<key_state, 348> states {};

  void consume(gql::event::key_press event) {
    states[event.key] = key_state::PRESSED;
  }

  void consume(gql::event::key_release event) {
    states[event.key] = key_state::UP;
  }
public:
  keyboard_event_consumer() {
    for (auto &state : states) {
      state = key_state::UNKNOWN;
    }
  }

  void tick() {
    for (auto &state : states) {
      if (state == key_state::PRESSED) {
        state = key_state::HELD;
      }
    }
  }

  [[nodiscard]] bool is_down(int key) const noexcept {
    return
      states[key] == key_state::PRESSED ||
      states[key] == key_state::HELD;
  }

  [[nodiscard]] bool was_pressed(int key) const noexcept {
    return
      states[key] == key_state::PRESSED;
  }
};

struct mouse_event_listener {
private:
  friend window;

  double x = std::numeric_limits<double>::infinity();
  double y = std::numeric_limits<double>::infinity();

  enum class button_state {
    UNKNOWN,
    PRESSED,
    HELD,
    UP,
  };

  std::array<button_state, 8> states {};


  void consume(gql::event::mouse_move event) {
    x = event.x;
    y = event.y;
  }

  void consume(gql::event::mouse_button_press event) {
    states[event.button] = button_state::PRESSED;
  }

  void consume(gql::event::mouse_button_release event) {
    states[event.button] = button_state::UP;
  }
public:
  void tick() {
    for (auto &state : states) {
      if (state == button_state::PRESSED) {
        state = button_state::HELD;
      }
    }
  }


  [[nodiscard]] std::tuple<double, double> pos() const noexcept {
    return {x, y};
  }

  [[nodiscard]] bool is_down(int key) const noexcept {
    return
      states[key] == button_state::PRESSED ||
      states[key] == button_state::HELD;
  }

  [[nodiscard]] bool was_pressed(int key) const noexcept {
    return
      states[key] == button_state::PRESSED;
  }
};
}

#endif //GQL_EVENT_H
