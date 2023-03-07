#include <gql/window.h>

int main() {
  [[maybe_unused]] const auto window_init = gql::window::init(); // To handle the C libraries requiring an "init function"

  auto window = gql::window(1920, 1080, "My Window");

  while (!window.should_close()) {
    const auto tick = window.tick();
    // Insert rendering code here
  }
}