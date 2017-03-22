#version 410 core

in vec3 vColor_vs;
in vec2 vUV_vs;

out vec4 vColor_fs;

uniform sampler2D oTexture0;

void main()
{
    vColor_fs = texture(oTexture0, vUV_vs) * vec4(vColor_vs, 1.0f);
}
