#ifndef SB6_H
#define SB6_H

#include <GL/glfw3.h>
#include <iostream>
#include <string>
#include <functional>
#include <windows.h>

namespace byhj 
{
	class Application 
	{
	public:
		Application() {}
		virtual ~Application() {}

		virtual void run(byhj::Application *the_app);
		virtual void init_windowInfo() = 0;
		virtual void init() = 0;
		virtual void render() = 0;
		virtual void shotdown() {};

	    virtual float getAspect()
		{
			return static_cast<GLfloat>(windowInfo.Width) / static_cast<GLfloat>(windowInfo.Height);
		}

		virtual void keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);
		}
	protected:
		struct WindowInfo
		{
			WindowInfo():title("byhj--OpenGL Example"), Width(1200), Height(800), posX(400), posY(100){}
			std::string title;
			int Width;
			int Height;
			int posX, posY;
		}windowInfo;

	protected:
		static byhj::Application *app;
		static void glfw_key(GLFWwindow * window, int key, int scancode, int action, int mode) 
		{
			app->keyboard(window,  key,  scancode, action,  mode);
		}

	};  //class

void Application::run(byhj::Application *the_app)
{
	app = the_app;
	std::cout << "Starting GLFW context" << std::endl;
	if (!glfwInit()) 
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return;
	}
	init_windowInfo();
	GLFWwindow *window = glfwCreateWindow(windowInfo.Width, windowInfo.Height, windowInfo.title.c_str(), nullptr, nullptr);
	glfwSetWindowPos(window, windowInfo.posX, windowInfo.posY);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, glfw_key);

	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return ;
	}	
	glewExperimental = GL_TRUE;

	//1���鿴GLSL��OpenGL�İ汾  
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return ;
	}
	const GLubyte *renderer = glGetString( GL_RENDERER );  
	const GLubyte *vendor = glGetString( GL_VENDOR );  
	const GLubyte *version = glGetString( GL_VERSION );  
	const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );  
	GLint major, minor;  
	glGetIntegerv(GL_MAJOR_VERSION, &major);  
	glGetIntegerv(GL_MINOR_VERSION, &minor);  
	std::cout << "GL Vendor    :" << vendor << std::endl;  
	std::cout << "GL Renderer  : " << renderer << std::endl;  
	std::cout << "GL Version (std::string)  : " << version << std::endl;  
	std::cout << "GL Version (integer) : " << major << "." << minor << std::endl;  
	std::cout << "GLSL Version : " << glslVersion << std::endl;    
	std::cout << "------------------------------------------------------------------------------" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major); //opengl 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //using opengl core file
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	// Create a GLFWwindow object that we can use for GLFW's functions

	init();
	glViewport(0, 0, windowInfo.Width, windowInfo.Height);

	while (!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();
		render();
		glfwSwapBuffers(window);
	}
	  shotdown();
	  glfwTerminate();
   }//run
}  //namespace 

byhj::Application * byhj::Application::app; 
#define GL_MAIN(a)                                        \
int main(int argc, const char **argv)                     \
{                                                         \
	a *app = new a;                                       \
	try {                                                 \
      app->run(app);                                      \
    } catch(std::exception& e) {                          \
      MessageBox(NULL, e.what(),                          \
             "An exception has occurred!",                \
             MB_OK | MB_ICONERROR | MB_TASKMODAL);        \
   }                                                      \
   delete app;                                            \
   return 0;                                              \
}

#endif  //byhj_h


