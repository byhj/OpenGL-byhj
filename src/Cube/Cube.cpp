#include <common/common.h>
#include <common/TextureManager.cpp>

static const GLfloat VertexData[] = 
{
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f , -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f , 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f , 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f , -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f , 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f , 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	0.5f , 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f , 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f , -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f , -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f , -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f , 0.5f, 0.5f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f , -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f , -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f , -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	0.5f , 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f , 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f , 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};


class CubeApp: public byhj::Application
{
public:
	CubeApp():CubeShader("Cube Shader") {}
	~CubeApp() {}

	void init_shader();
	void init_buffer();
	void init_texture();
	void init_vertexArray();
	void init_windowInfo();
	void shutdown();

	void init();
	void render();
public:
    struct	Uniform 
	{
		GLuint mvp_loc;
		GLuint tex_loc;
	}uniform;
private:
	GLuint program, vao, ebo, vbo;
	Shader CubeShader;
	GLuint texture;
};

GL_MAIN(CubeApp);

void CubeApp::init()
{
	glEnable(GL_DEPTH_TEST);
	init_shader();
	init_buffer();
	init_vertexArray();
	init_texture();
	init_windowInfo();
}

void CubeApp::init_shader()
{
	CubeShader.init();
	CubeShader.attach(GL_VERTEX_SHADER, "cube.vert");
	CubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	CubeShader.link();
	program = CubeShader.GetProgram();
	uniform.mvp_loc = glGetUniformLocation(program, "mvp");
	uniform.tex_loc = glGetUniformLocation(program, "tex");
	glUniform1i(uniform.tex_loc, 0);
}

void CubeApp::init_windowInfo()
{
	windowInfo.title = "Cube Window";
}

void CubeApp::render()
{
	static const GLfloat color[] = {0.3f, 0.3f, 0.3f, 1.0f };  //清除指定的缓存
	static const GLfloat one[] = {1.0f};
	glClearBufferfv(GL_DEPTH, 0, one);
	glClearBufferfv(GL_COLOR, 0, color);

	float time = glfwGetTime();

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) );
	
	//near and far data is accroding to the camera pos
	glm::mat4 proj = glm::perspective(45.0f, getAspect(), 0.1f, 1000.0f);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
		            * glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 0.0f) );
	glm::mat4 mvp = proj * view * model;
	glUniformMatrix4fv(uniform.mvp_loc, 1, GL_FALSE, &mvp[0][0]);

	glUseProgram(program);
	glBindVertexArray(vao);
	TextureManager::Inst()->BindTexture(texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void CubeApp::shutdown()
{
	glDeleteProgram(program);
}

void CubeApp::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CubeApp::init_texture()
{
	texture = SOIL_load_OGL_texture
		(
		"../../media/texture/crate.tga",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

}

void CubeApp::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, BUFFER_OFFSET(sizeof(GLfloat) * 3) );
	glBindVertexArray(0);
}