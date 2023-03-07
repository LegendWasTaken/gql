#include <gql/window.h>

#include <iostream>

struct keyboard_event_consumer { // Note: there is a default `gql::keyboard_event_consumer` that can be used, and will be enough for most use cases.
public:
  void consume(gql::event::key_press event) {
    std::cout << "Key: " << event.key << " was pressed" << std::endl;
  }

  void consume(gql::event::key_release event) {
    std::cout << "Key: " << event.key << " was released" << std::endl;
  }
};

int main() {
  [[maybe_unused]] const auto window_init = gql::window::init(); // To handle the C libraries requiring an "init function"

  auto keyboard_events = keyboard_event_consumer();
  auto window = gql::window(1920, 1080, "My Window");
  window.keyboard_events(&keyboard_events);

  while (!window.should_close()) {
    const auto tick = window.tick();

    // Insert rendering code here
  }
}