#version 430 core

layout (location = 0) in vec3 Position;

out VS_OUT {
  vec4 color;
}vs_out;

uniform mat4 mvp;

void main(void)
{
   gl_Position = mvp * vec4(Position, 1.0);
   vs_out.color = vec4(Position, 1.0);
}