#version 410 core

uniform vec4 vColor;

out vec4 vColor_fs;

void main()
{
    vColor_fs = vColor;
}
