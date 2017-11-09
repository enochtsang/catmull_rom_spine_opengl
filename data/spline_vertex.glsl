#version 410 core

layout (location = 0) in vec3 pos_in;

void main()
{
    gl_Position = vec4(pos_in, 1.0);
}
