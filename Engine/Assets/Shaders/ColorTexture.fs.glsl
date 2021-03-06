#version 410 core

in vec4 vColor_vs;
in vec2 vUV_vs;

out vec4 vColor_fs;

uniform sampler2D oDiffuseTexture;

void main()
{
    vColor_fs = texture(oDiffuseTexture, vUV_vs) * vColor_vs;
}
