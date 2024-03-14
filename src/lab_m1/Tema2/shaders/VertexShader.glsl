#version 330

// Input
layout(location = 0) in vec3 v_position;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 object_color;
uniform float health;

// Output value to fragment shader
out float tankHealth;
out vec3 color;


void main()
{
    color = object_color;
    tankHealth = health;

    float scaleFactor = 1.0 + (tankHealth / 3.0) / 10;
    if(health <= -1)
        gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    else    
        gl_Position = Projection * View * Model * vec4(v_position * scaleFactor, 1.0);
}
