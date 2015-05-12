#include <AntTweakBar.h>

#include <GL/glew.h>
#include <gl/freeglut.h>
#include <iostream>
#include <vector> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>



using namespace std;

#include <shader.h>

bool    wireframe_mode; //线框模式
GLuint  vao, vbo, ibo;

typedef struct {
	GLuint Projection;
	GLuint ModelView;
	GLuint NormalMatrix;
	GLuint LightPosition;
	GLuint AmbientMaterial;
	GLuint DiffuseMaterial;
	GLuint TessLevelInner;
	GLuint TessLevelOuter;
} Shaderuniform_loc;

static Shaderuniform_loc uniform_loc;  //uniform elements
static int TessLevelInner;
static int TessLevelOuter;
static GLuint Program;
static double fps = 0.0;
static float aspect = 1.0f;

Shader TessellationShader("Triangle Tessellation Shader");

static  const GLuint ElementData[] = {
        2, 1, 0,
        3, 2, 0,
        4, 3, 0,
        5, 4, 0,
        1, 5, 0,

        11, 6,  7,
        11, 7,  8,
        11, 8,  9,
        11, 9,  10,
        11, 10, 6,

        1, 2, 6,
        2, 3, 7,
        3, 4, 8,
        4, 5, 9,
        5, 1, 10,

        2,  7, 6,
        3,  8, 7,
        4,  9, 8,
        5, 10, 9,
        1, 6, 10 
};

static const float VertexData[] = {
         0.000f,  0.000f,  1.000f,
         0.894f,  0.000f,  0.447f,
         0.276f,  0.851f,  0.447f,
        -0.724f,  0.526f,  0.447f,
        -0.724f, -0.526f,  0.447f,
         0.276f, -0.851f,  0.447f,
         0.724f,  0.526f, -0.447f,
        -0.276f,  0.851f, -0.447f,
        -0.894f,  0.000f, -0.447f,
        -0.276f, -0.851f, -0.447f,
         0.724f, -0.526f, -0.447f,
         0.000f,  0.000f, -1.000f 
};

void load_shader()
{
	TessellationShader.init();
	TessellationShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TessellationShader.attach(GL_TESS_CONTROL_SHADER, "triangle.tcs");
	TessellationShader.attach(GL_TESS_EVALUATION_SHADER, "triangle.tes");
	TessellationShader.attach(GL_GEOMETRY_SHADER, "triangle.geom");
	TessellationShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TessellationShader.link();
	TessellationShader.use();
	Program = TessellationShader.get_program();

    uniform_loc.Projection = glGetUniformLocation(Program, "proj_matrix");
    uniform_loc.ModelView = glGetUniformLocation(Program, "mv_matrix");

    uniform_loc.LightPosition = glGetUniformLocation(Program, "LightPosition");
    uniform_loc.AmbientMaterial = glGetUniformLocation(Program, "AmbientMaterial");
    uniform_loc.DiffuseMaterial = glGetUniformLocation(Program, "DiffuseMaterial");
    uniform_loc.TessLevelInner = glGetUniformLocation(Program, "TessLevelInner");
    uniform_loc.TessLevelOuter = glGetUniformLocation(Program, "TessLevelOuter");
    
	TessLevelInner = 3;
    TessLevelOuter = 2;
	glm::vec4 lightPosition = glm::vec4(0.25, 0.25, 1.0, 0.0);
	glUniform3f(uniform_loc.AmbientMaterial, 0.04f, 0.04f, 0.04f);
    glUniform3f(uniform_loc.DiffuseMaterial, 0, 0.75, 0.75);
	glUniform4fv(uniform_loc.LightPosition, 1, glm::value_ptr(lightPosition));

}

void init_buffer()
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), ElementData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void init_vertexArray()
{
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_VERTEX_ARRAY, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void init(void)
{
	init_buffer();
	init_vertexArray();
	load_shader();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void display(void)
{

    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;
    glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glUseProgram(Program);
	glBindVertexArray(vao);

	const float RadiansPerMicrosecond = 0.0005f;
    static float elapsedMicroseconds ;
	elapsedMicroseconds = glutGet(GLUT_ELAPSED_TIME);
    static float Theta = 0;
    Theta = elapsedMicroseconds * RadiansPerMicrosecond;

	glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
	glm::mat4 mv_matrix = glm::lookAt(glm::vec3(0.0, 0.0, 3.0) , glm::vec3(0.0f, 0.0f, 0.0), glm::vec3(0.0, 1.0, 0.0)) 
                          * glm::rotate(glm::mat4(1.0), Theta, glm::vec3(1.0f, 0.0f, 0.0f)) ;

	glUniformMatrix4fv(uniform_loc.Projection, 1, 0, glm::value_ptr(proj_matrix));
	glUniformMatrix4fv(uniform_loc.ModelView, 1, 0, glm::value_ptr(mv_matrix));
	glUniform1i(uniform_loc.TessLevelInner, TessLevelInner);
	glUniform1i(uniform_loc.TessLevelOuter, TessLevelOuter);
  //  glPolygonMode(GL_FRONT_AND_BACK, wireframe_mode ? GL_LINE : GL_FILL);

    glPatchParameteri(GL_PATCH_VERTICES, 3);  //每三个点一个patch
    glDrawElements(GL_PATCHES, sizeof(ElementData) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	static int frame = 0;
	frame++;
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	static float timebase = 0.0;
	if (time - timebase > 1.0) {
		fps = frame;
		timebase = time;		
		frame = 0;
	}
	glBindVertexArray(0);
	glUseProgram(0);

	// Draw tweak bars
	TwDraw();


	glutSwapBuffers(); //强制绘图，去掉会变白色

}

void reshape(int w, int h) 
{
	glViewport(0, 0, w, h);  //视口调整
	aspect = (float)w / h;
	TwWindowSize(w, h);
}

void idle(void)
{
	glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y) 
{
   switch (key) {
        case GLUT_KEY_F1:   
			wireframe_mode = !wireframe_mode; break;
		case GLUT_KEY_RIGHT: 
			TessLevelInner++; break;
        case GLUT_KEY_LEFT: 
			TessLevelInner = TessLevelInner > 1 ? TessLevelInner - 1 : 1; break;
		case GLUT_KEY_UP:  
			TessLevelOuter++; break;
		case GLUT_KEY_DOWN:  
			TessLevelOuter = TessLevelOuter > 1 ? TessLevelOuter - 1 : 1; break;
   }
}

int main(int argc, char ** argv)
{

	TwBar *bar; // Pointer to the tweak bar

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //显示模式，重要
	glutInitWindowPosition(200, 0);
	glutInitWindowSize(720, 640);
	glutCreateWindow("Tessellation2");
    glewInit();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	TwInit(TW_OPENGL, NULL);
	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);

	// Create a tweak bar
	bar = TwNewBar("TessellationBar");
	TwDefine("Options for Tesslation."); // Message added to the help bar.
	TwDefine(" TweakBar size='200 400' color='96 216 224' "); // change default tweak bar size and color
	
	TwAddVarRW(bar, "TessLevelInner", TW_TYPE_INT32, &TessLevelInner, 
		" min=1 max=10 step=1 keyIncr=z keyDecr=Z help='Scale the object (1=original size).' ");
	
	TwAddVarRW(bar, "TessLevelOuter", TW_TYPE_INT32, &TessLevelOuter, 
		" min=1 max=10 step=1 keyIncr=z keyDecr=Z help='Scale the object (1=original size).' ");
	
	TwAddVarRO(bar, "time", TW_TYPE_DOUBLE, &fps, " label='FPS:' precision=1 help='Time (in seconds).' ");         


	glutMainLoop(); //循环调用注册函数display

	return 0;
}