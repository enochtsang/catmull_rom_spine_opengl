#version 410 core

layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec2 tex_pos_in;

uniform mat4 transform;

out vec2 tex_pos;

void main()
{
    gl_Position = transform * vec4(pos_in, 1.0);
    tex_pos = tex_pos_in;
}
