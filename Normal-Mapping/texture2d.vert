#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 BiTangent;


out VS_OUT {
  vec2 tc;
  vec3 light;
  vec3 eye;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 lightDir = vec3(0.5, 0.5, 0.5);

void main(void)
{
   vs_out.tc = TexCoords;
   	// Bring tangent, bitangent and normal to camera space.
	mat3 normalMatrix = mat3(view * model);

	vec3 tangent = normalMatrix * Tangent;
	vec3 bitangent = normalMatrix * BiTangent;
	vec3 normal = normalMatrix * Normal;

	vs_out.light.x = dot(tangent, lightDir);
	vs_out.light.y = dot(bitangent, lightDir);
	vs_out.light.z = dot(normal, lightDir);

	// Bring vertex in camera space.
	vec4 vertex = view * model * vec4(Position, 1.0);
	vec3 eye = -vec3(vertex);
	// Like the light, this brings the eye vector to texture space.
	vs_out.eye.x = dot(eye, tangent);
	vs_out.eye.y = dot(eye, bitangent);
	vs_out.eye.z = dot(eye, normal);

   gl_Position = proj * vertex;

}