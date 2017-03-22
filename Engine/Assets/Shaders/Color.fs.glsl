#version 410 core

in vec3 vColor_vs;

out vec4 vColor_fs;

void main()
{
    vColor_fs = vec4(vColor_vs, 1.0f);
}
