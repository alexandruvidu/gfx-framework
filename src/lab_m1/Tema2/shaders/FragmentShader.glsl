#version 330

// Input
in vec3 color;
in float tankHealth;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 obj_col;
    if(tankHealth <= -1)
        obj_col = color;
    else
        obj_col = mix(color, vec3(0, 0, 0), 1.0 - tankHealth / 3);

    out_color = vec4(obj_col, 1);

}
