#version 410 core

in vec4 vColor_vs;

out vec4 vColor_fs;

void main()
{
    vColor_fs = vColor_vs;
}
