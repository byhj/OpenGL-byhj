#include "common.h"


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
		GLuint mv_loc;
		GLuint proj_loc;
	}uniform;
private:
	GLuint program, vao, ebo, vbo;
	Shader CubeShader;
	Model  Sphere;
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
	CubeShader.attach(GL_VERTEX_SHADER, "phong.vert");
	CubeShader.attach(GL_FRAGMENT_SHADER, "phong.frag");
	CubeShader.link();
	program = CubeShader.get_program();
	uniform.mv_loc = glGetUniformLocation(program, "mv");
	uniform.proj_loc = glGetUniformLocation(program, "proj");
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
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f))
		            * glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 0.0f) );
	glm::mat4 mv = view * model;
	glUniformMatrix4fv(uniform.mv_loc, 1, GL_FALSE, &mv[0][0]);
	glUniformMatrix4fv(uniform.proj_loc, 1, GL_FALSE, &proj[0][0]);
	glUseProgram(program);
	Sphere.Render(program);
}

void CubeApp::shutdown()
{
	glDeleteProgram(program);
}

void CubeApp::init_buffer()
{
	Sphere.loadModel("../media/object/sphere.obj");
}

void CubeApp::init_texture()
{

}

void CubeApp::init_vertexArray()
{
}