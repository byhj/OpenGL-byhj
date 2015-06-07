#include <common/common.h>


const static GLfloat VertexData[] = {
	// Positions       
	-1.0, -1.0,  0.0, 
	 1.0, -1.0,  0.0, 
	 1.0,  1.0,  0.0, 
	-1.0,  1.0,  0.0
};

const static GLuint ElementData[] = {  // Note that we start from 0!
	0, 1, 2, // First Triangle
	2, 3, 0  // Second Triangle
};  



class TesssellationApp: public byhj::Application
{
public:
	TesssellationApp():TesssellationShader("Tesssellation Shader") {}
	~TesssellationApp() {}

	void init_shader();
	void init_buffer();
	void init_texture();
	void init_vertexArray();
	void init_windowInfo();
	void shutdown();

	void init();
	void render();

	struct	Uniform 
	{
		GLuint mvp_loc;
	}uniform;

private:
	GLuint program, vao, ebo, vbo, texture;
	Shader TesssellationShader;
};

GL_MAIN(TesssellationApp);

void TesssellationApp::init()
{
	init_shader();
	init_buffer();
	init_vertexArray();
	init_texture();
	init_windowInfo();
}

void TesssellationApp::init_shader()
{
	TesssellationShader.init();
	TesssellationShader.attach(GL_VERTEX_SHADER, "tes.vert");
	TesssellationShader.attach(GL_TESS_CONTROL_SHADER, "tes.tcs");
	TesssellationShader.attach(GL_TESS_EVALUATION_SHADER, "tes.tes");
	TesssellationShader.attach(GL_GEOMETRY_SHADER, "tes.geom");
	TesssellationShader.attach(GL_FRAGMENT_SHADER, "tes.frag");
	TesssellationShader.link();
	program = TesssellationShader.GetProgram();
	uniform.mvp_loc = glGetUniformLocation(program, "mvp");
}

void TesssellationApp::init_windowInfo()
{
	windowInfo.title = "Tesssellation Window";
}

void TesssellationApp::render()
{
	static const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f };  //清除指定的缓存
	glClearBufferfv(GL_COLOR, 0, color);
	glUseProgram(program);
	glBindVertexArray(vao);

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) );
	glm::mat4 proj = glm::perspective(45.0f, getAspect(), 0.1f, 1000.0f);
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 mvp = proj * view * model;
	glUniformMatrix4fv(uniform.mvp_loc, 1, GL_FALSE, &mvp[0][0]);

	glEnable( GL_CULL_FACE);
	// We work with 4 points per patch.
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, 0);
}

void TesssellationApp::shutdown()
{
	glDeleteProgram(program);
}

void TesssellationApp::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), ElementData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TesssellationApp::init_texture()
{

}

void TesssellationApp::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
}