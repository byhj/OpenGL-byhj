#version 430 core

layout (location = 0) out vec4 fragColor;

uniform sampler2D tex;

in VS_OUT {
  vec2 tc;
}fs_in;

void main(void)
{
   fragColor = texture(tex, fs_in.tc);
}