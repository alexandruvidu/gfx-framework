#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec3 texture;
layout(location = 3) in vec3 color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_pos;
out vec3 frag_norm;
out vec3 frag_texture;
out vec3 frag_color;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_pos = pos;
    frag_norm = norm;
    frag_texture = texture;
    frag_color = color;
    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(pos * sin(time), 1.0);
}
