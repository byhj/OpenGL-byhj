#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

out VS_OUT {
  vec4 color;
}vs_out;

void main(void)
{
   gl_Position = vec4(Position, 1.0);
   vs_out.color = vec4(Color, 1.0);
}