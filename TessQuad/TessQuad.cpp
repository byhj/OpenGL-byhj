#include "common.h"
#include "gumbo.h"

struct Uniform_loc {
	GLuint Projection;
	GLuint ModelView;
	GLuint NormalMatrix;
	GLuint LightPosition;

	GLuint AmbientMaterial;
	GLuint DiffuseMaterial;
	GLuint SpecularMaterial;
	GLuint Shininess;

	GLuint PatchMatrix;
	GLuint TransposedPatchMatrix;
	GLuint TessLevelInner;
	GLuint TessLevelOuter;
};

class TessQuadApp: public byhj::Application
{
public:
	TessQuadApp():TessQuadShader("Triangle Shader") {}
	~TessQuadApp() {}

	void init_shader();
	void init_buffer();
	void init_texture();
	void init_vertexArray();
	void init_windowInfo();
	void shutdown();

	void init();
	void render();

private:
	GLuint program, vao, ibo, vbo;
	Shader TessQuadShader;
	Uniform_loc uniform_loc;
};

GL_MAIN(TessQuadApp);

static GLsizei VertCount;
static const GLuint PositionSlot = 0;

static float TessLevelInner;
static float TessLevelOuter;

void TessQuadApp::init()
{
	TessLevelInner = 6;
	TessLevelOuter = 6;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	init_shader();
	init_buffer();
	init_vertexArray();
	init_texture();
	init_windowInfo();
}

void TessQuadApp::init_shader()
{
	TessQuadShader.init();
	TessQuadShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TessQuadShader.attach(GL_TESS_CONTROL_SHADER, "triangle.tcs");
	TessQuadShader.attach(GL_TESS_EVALUATION_SHADER, "triangle.tes");
	TessQuadShader.attach(GL_GEOMETRY_SHADER, "triangle.geom");
	TessQuadShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TessQuadShader.link();
    TessQuadShader.use();
	program = TessQuadShader.get_program();

	uniform_loc.Projection       = glGetUniformLocation(program, "Projection");
	uniform_loc.ModelView        = glGetUniformLocation(program, "ModelView");
	uniform_loc.LightPosition    = glGetUniformLocation(program, "LightPosition");
	uniform_loc.AmbientMaterial  = glGetUniformLocation(program, "AmbientMaterial");
	uniform_loc.DiffuseMaterial  = glGetUniformLocation(program, "DiffuseMaterial");
	uniform_loc.SpecularMaterial = glGetUniformLocation(program, "SpecularMaterial");
	uniform_loc.Shininess        = glGetUniformLocation(program, "Shininess");
	uniform_loc.TessLevelInner   = glGetUniformLocation(program, "TessLevelInner");
	uniform_loc.TessLevelOuter   = glGetUniformLocation(program, "TessLevelOuter");
	uniform_loc.PatchMatrix = glGetUniformLocation(program, "B");
	uniform_loc.TransposedPatchMatrix = glGetUniformLocation(program, "BT");
	glm::mat4 bezier = glm::mat4
		(
		glm::vec4(-1, 3, -3, 1),
		glm::vec4(3, -6, 3, 0),
		glm::vec4(-3, 3, 0, 0),
		glm::vec4(1, 0, 0, 0) 
		);
	glUniformMatrix4fv(uniform_loc.PatchMatrix, 1, GL_FALSE, &bezier[0][0]);
	glUniformMatrix4fv(uniform_loc.TransposedPatchMatrix, 1, GL_TRUE, &bezier[0][0]);
	
	glm::vec4 lightPosition = glm::vec4(0.25, 0.25, 1, 0);
	glUniform3f(uniform_loc.DiffuseMaterial, 0, 0.75, 0.75);
	glUniform3f(uniform_loc.AmbientMaterial, 0.04f, 0.04f, 0.04f);
	glUniform3f(uniform_loc.SpecularMaterial, 0.5f, 0.5f, 0.5f);
	glUniform1f(uniform_loc.Shininess, 50);
	glUniform3fv(uniform_loc.LightPosition, 1, &lightPosition[0]);

}

void TessQuadApp::init_windowInfo()
{
	windowInfo.title = "Triangle Window";
}

void TessQuadApp::render()
{
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glUseProgram(program);
	glBindVertexArray(vao);

	glUseProgram(program);
	glBindVertexArray(vao);

	/*
	glm::vec3 minBound = glm::vec3(100, 100, 100);
	glm::vec3 maxBound = glm::vec3(-100, -100, -100);
	int vert;
	for (vert = 0; vert < VertCount; vert++) {
		float x = PatchData[vert][0];
		float y = PatchData[vert][1];
		float z = PatchData[vert][2];
		glm::vec3 p = glm::vec3(x, y, z);
		minBound = glm::vec3(minBound, p);
		maxBound = glm::vec3(maxBound, p);
	}
	glm::vec3 CenterPoint = (maxBound, minBound, 0.5f);

	*/

	glm::mat4 view = glm::lookAt(glm::vec3(-5.0f, -50.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) );
	glm::mat4 proj = glm::perspective(45.0f, getAspect(), 0.1f, 1000.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mv = view * model;

	glUniform1f(uniform_loc.TessLevelInner, TessLevelInner);
	glUniform1f(uniform_loc.TessLevelOuter, TessLevelOuter);
	glUniformMatrix4fv(uniform_loc.Projection, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(uniform_loc.ModelView, 1, GL_FALSE, &mv[0][0]);

    glPatchParameteri(GL_PATCH_VERTICES, 16);
	glDrawArrays(GL_PATCHES, 0, VertCount);

}

void TessQuadApp::shutdown()
{
	glDeleteProgram(program);
}

void TessQuadApp::init_buffer()
{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

	    VertCount = sizeof(PatchData);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, VertCount, PatchData, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

}



void TessQuadApp::init_texture()
{

}

void TessQuadApp::init_vertexArray()
{
}