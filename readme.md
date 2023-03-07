# GQL - OpenGL Quick Layer

A bad name, I'm aware. But it's what I came up with so it's staying.

What problem does this solve? Good question - I write a lot of OpenGL applications that use a decent bit of different things. ImGui, ENTT, GLFW. These generally use mouse inputs, keyboard inputs, etc.. So instead of spending 5 minutes every project to add it. I've decided to dedicate even more time to writing a quick library I can use.

Do I expect everyone to be able to use this? No, it's very opinionated and my own personal taste. 

Do you want to use this, but can't because of aforementioned opinions? Feel free to open an issue with your concerns / wants, and it can be discussed.

Anyways, to the features / usage 
## Features
1) Easy to integrate (uses FetchContent from CMake)
2) Abstractions over
   1) Windowing
   2) Keyboard input
   3) Mouse input
   4) ImGui (Docking)
   5) OpenGL
3) Nice to use (I think...)

And more! For the one time payment price of $0.00!

## Integration

blah blah - todo write cmake stuff here

## Usage

Opening a window
```cpp
#include <gql/window.h>

int main() {
  [[maybe_unused]] const auto window_init = gql::window::init(); // To handle the C libraries requiring an "init function"
  
  auto window = gql::window(1920, 1080, "My Window");
  
  while (!window.should_close()) {
    const auto tick = window.tick();
    // Insert rendering code here
  }
}
```

Reading keyboard input
```cpp
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
```

Reading mouse input 
```cpp
#include <gql/window.h>

#include <iostream>

struct mouse_event_consumer { // Note: there is a default `gql::mouse_event_consumer` that can be used, and will be enough for most use cases.
public:
  void consume(gql::event::mouse_move event) {
    std::cout << "Mouse move to: " << event.x << ", " << event.y << std::endl;
  }
  
  void consume(gql::event::mouse_key_press event) {
    std::cout << "Mouse Key: " << event.key << " was pressed" << std::endl;
  }
  
  void consume(gql::event::mouse_key_release event) {
    std::cout << "Mouse Key: " << event.key << " was released" << std::endl;
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
```

ImGui (Non-docking)
```cpp
#include <gql/window.h>
#include <gql/imgui.h>

struct basic_widget {
  gql::slider<0.0f, 10.0f, "slider"> slider;
  gql::input<int, "input"> input;
  gql::radio<"radio"> radio;
  
  [[nodiscard]] std::string_view name() const noexcept {
    return "Hello ImGui!";
  }
  
  void render() {
    gql::component_block(slider, input, gql::separator(), radio);
    // Same as
    /*
     * ImGui::SliderFloat("sliders", &float, 0.0f, 10.0f);
     * ImGui::InputInt("input", &int);
     * ImGui::RadioButton("radio", &radio);
     */
    gql::collapsable_block("extra settings", slider, input, radio);
    // Same as above, but allows for collapsing
  }
};

int main() {
  [[maybe_unused]] const auto window_init = gql::window::init(); // To handle the C libraries requiring an "init function"
  
  auto keyboard_events = keyboard_event_consumer();
  const auto window = gql::window(1920, 1080, "My Window");
  
  auto widget = basic_widget();
  
  while (!window.should_close()) {
    const auto tick = window.tick();
    
    gql::display_widgets(widget);
  }
}
```
