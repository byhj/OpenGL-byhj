#version 410 core

// A triangle with three points come in
layout(triangles) in;

// Out of three triangle points, we create a triangle strip with two triangles
layout(triangle_strip, max_vertices = 6) out;

uniform mat4 proj;
uniform mat4 mv;
uniform vec4 u_lightPosition = vec4(0.0f, 0.5f, 2.0f, 1.0);

in vec3 g_normal[3];

out vec3 v_normal;
out vec3 v_eye;
out vec4 v_color;
out vec3 v_lightDirection;

void main(void)
{
    int i;

	mat4 translateMatrix = mat4(1.0);
	
	vec4 position;

	translateMatrix[3][0] = -1.5;
	
    for (i=0; i < gl_in.length(); i++)
    {
		gl_Position = proj * mv * gl_in[i].gl_Position;

		// Normal is already in camera space
		v_normal = g_normal[i];
		v_eye = -vec3(position);
		v_color = vec4(1.0, 0.0, 0.0, 1.0);

		// Light position is in camera space
		v_lightDirection = vec3(u_lightPosition - position);

        EmitVertex();
    }
    
    EndPrimitive();    

	translateMatrix[3][0] = +1.5;
	
    for (i=0; i < gl_in.length(); i++)
    {
        gl_Position = proj * mv * gl_in[i].gl_Position;

		v_normal = g_normal[i];
		v_eye = -vec3(position);
		v_color = vec4(0.0, 0.0, 1.0, 1.0);

		v_lightDirection = vec3(u_lightPosition - position);

        EmitVertex();
    }
    
    EndPrimitive();    
}
