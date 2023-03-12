#include <gql/window.h>
#include <gql/imgui.h>

struct basic_widget {
  gql::slider<float> slider = { 0.0f, 10.0f, "Sliders", 5.0f };
  gql::input<int> input = { "Input", 0};
  gql::radio radio = { "Radio", false };

  [[nodiscard]] std::string_view name() const noexcept {
    return "Hello ImGui!";
  }

  void render() {
    gql::component_block(slider, input, radio);
    gql::collapsable_block("extra settings", slider, input, radio);
  }
};

int main() {
  [[maybe_unused]] const auto window_init = gql::window::init(); // To handle the C libraries requiring an "init function"

  auto window = gql::window(1920, 1080, "My Window");

  auto widget = basic_widget();

  while (!window.should_close()) {
    const auto tick = window.tick();

    gql::display_widgets(widget);
  }
}