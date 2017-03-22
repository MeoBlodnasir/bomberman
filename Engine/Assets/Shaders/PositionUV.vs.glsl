#version 410 core

#include "LayoutLocation.h.glsl"

LAYOUT_LOCATION_POSITION in vec3 vPosition;
LAYOUT_LOCATION_UV		 in vec2 vUV;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

out vec2 vUV_vs;

void main()
{
	vUV_vs		= vUV;
    gl_Position = mProjection * mView * mModel * vec4(vPosition, 1.0);
}
