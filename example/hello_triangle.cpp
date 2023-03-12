#include <gql/window.h>
#include <gql/gl.h>

int main() {
  [[maybe_unused]] const auto window_init = gql::window::init(); // To handle the C libraries requiring an "init function"

  auto window = gql::window(1920, 1080, "My Window");

  auto program = gql::create_program(
    "example/data/triangle.frag",
    "example/data/triangle.vert");

  gql::use(program);
  auto dummy_vao = GLuint();
  glGenVertexArrays(1, &dummy_vao);

  while (!window.should_close()) {
    const auto tick = window.tick();

    glBindVertexArray(dummy_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
}