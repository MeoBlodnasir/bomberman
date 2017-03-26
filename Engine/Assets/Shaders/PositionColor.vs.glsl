#version 410 core

#include "LayoutLocation.h.glsl"

LAYOUT_LOCATION_POSITION in vec3 vPosition;
LAYOUT_LOCATION_COLOR	 in vec4 vColor;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

out vec4 vColor_vs;

void main()
{
	vColor_vs	= vColor;
    gl_Position = mProjection * mView * mModel * vec4(vPosition, 1.0);
}
