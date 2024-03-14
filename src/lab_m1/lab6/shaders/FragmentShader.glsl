#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_pos;
in vec3 frag_norm;
in vec3 frag_texture;
in vec3 frag_color;

uniform float time;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(frag_norm * sin(time), 1);

}
