#include <gql/window.h>

#include <iostream>

struct mouse_event_consumer { // Note: there is a default `gql::mouse_event_consumer` that can be used, and will be enough for most use cases.
public:
  void consume(gql::event::mouse_move event) {
    std::cout << "Mouse move to: " << event.x << ", " << event.y << std::endl;
  }

  void consume(gql::event::mouse_button_press event) {
    std::cout << "Mouse Key: " << event.button << " was pressed" << std::endl;
  }

  void consume(gql::event::mouse_button_release event) {
    std::cout << "Mouse Key: " << event.button << " was released" << std::endl;
  }
};

int main() {
  [[maybe_unused]] const auto window_init = gql::window::init(); // To handle the C libraries requiring an "init function"

  auto mouse_events = mouse_event_consumer();
  auto window = gql::window(1920, 1080, "My Window");
  window.mouse_events(&mouse_events);

  while (!window.should_close()) {
    const auto tick = window.tick();

    // Insert rendering code here
  }
}