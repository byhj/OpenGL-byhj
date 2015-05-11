#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT {
  vec2 tc;
  vec3 light;
  vec3 eye;
}fs_in;

uniform sampler2D tex_color;
uniform sampler2D tex_normal;

void main(void)
{
	vec4 textureColor = texture(tex_color, fs_in.tc);
	vec4 color = 0.3 * textureColor;
	// Convert the color information to a normal.

	vec3 normalDX = normalize( texture(tex_normal, fs_in.tc).xyz * 2.0 - 1.0 );
	// DirectX: Origin of uv is upper left. OpenGL: Origin of st is lower left. Normal vector points "towards" us in both cases.
	// As normal maps usually are created for DirectX, we have to convert from DirectX to OpenGL tangent space.
	vec3 normal;
	normal.x = dot(vec3(1.0, 0.0, 0.0), normalDX);
	normal.y = dot(vec3(0.0, -1.0, 0.0), normalDX);
	normal.z = dot(vec3(0.0, 0.0, 1.0), normalDX);

	vec3 light = normalize(fs_in.light);
	float nDotL = max(dot(light, normal), 0.0);

	if (nDotL > 0.0)
	{
		vec3 eye = normalize(fs_in.eye);
		// Incident vector is opposite light direction vector.
		vec3 reflection = reflect(-light, normal);
		float eDotR = max(dot(eye, reflection), 0.0);
		color += textureColor * nDotL;
		// Fixed 0.1 specular color factor and fixed 20.0 specular exponent.
		color += vec4(0.1, 0.1, 0.1, 0.1) * pow(eDotR, 20.0);
	}

	fragColor = vec4(1.0);	
}