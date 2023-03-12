#version 450

layout (location = 0) out vec2 tex_coord;

void main() {
    vec2 pos = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1.0f;
    tex_coord = (pos + 1.0f) * 0.5f;
    gl_Position = vec4(pos, 0.0f, 1.0f);
}