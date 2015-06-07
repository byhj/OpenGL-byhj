#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;

out VS_OUT {
  vec2 tc;
}vs_out;

uniform mat4 mvp;

void main(void)
{
   gl_Position = mvp * vec4(Position, 1.0);
   vs_out.tc = TexCoords;
}