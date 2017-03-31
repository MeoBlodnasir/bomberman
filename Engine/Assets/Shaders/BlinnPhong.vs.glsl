#version 410 core

#include "LayoutLocation.h.glsl"

LAYOUT_LOCATION_POSITION	in vec3 vPosition;
LAYOUT_LOCATION_NORMAL		in vec3 vNormal;
LAYOUT_LOCATION_UV			in vec2 vUV;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

out vec3 vFragPos_vs;
out vec3 vNormal_vs;
out vec2 vUV_vs;

void main()
{
    gl_Position = mProjection * mView * mModel * vec4(vPosition, 1.0);
	vFragPos_vs = vec3(mModel * vec4(vPosition, 1.0));
	vNormal_vs = mat3(transpose(inverse(mModel))) * vNormal;
	vUV_vs = vUV;
}
