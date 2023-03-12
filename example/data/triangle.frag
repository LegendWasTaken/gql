#version 450

out vec4 FragColor;

layout (location = 0) in vec2 tex_coord;

void main() {
    FragColor = vec4(0.2f, 0.2, 0.8f, 1.0f);
}
