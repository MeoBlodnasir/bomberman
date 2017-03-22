#version 410 core

#include "LayoutLocation.h.glsl"

LAYOUT_LOCATION_POSITION in vec3 vPosition;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

void main()
{
    gl_Position = mProjection * mView * mModel * vec4(vPosition, 1.0);
}
