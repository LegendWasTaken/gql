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
}

#endif //GQL_EVENT_H
