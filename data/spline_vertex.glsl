#version 410 core

layout (location = 0) in vec3 pos_in;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(pos_in, 1.0);
}
