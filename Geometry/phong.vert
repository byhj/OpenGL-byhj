#version 410 core

uniform mat4 mv;

in vec3 a_vertex;
in vec3 a_normal;

out vec3 g_normal;

void main(void)
{
	g_normal = mat3(mv) * a_normal;
	gl_Position = vec4(a_vertex, 1.0);
}
