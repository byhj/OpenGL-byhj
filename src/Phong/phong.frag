#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT
{
  vec3 L;
  vec3 N;
  vec3 V;
}fs_in;

uniform vec3 diffuse_material = vec3(0.5, 0.2, 0.7);
uniform float power = 64.0f;
uniform vec3 specular_material = vec3(0.7);

void main()
{    
    vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);

	vec3 R = reflect(-L, N);
	vec3 diffuse = max(dot(N, L), 0.0) * diffuse_material;
	vec3 specular = pow(max(dot(R, V), 0.0), power) * specular_material;

    // calc the all light attribute
    vec3 result = (specular + diffuse);
	fragColor = vec4(result, 1.0);
}
