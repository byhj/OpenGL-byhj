#ifndef COMMON_H
#define COMMON_H

#include "model.h"
#include "shader.h"

//gl header
#include <gl/glew.h>
#include <gl/glfw3.h>
#include <gl/freeglut.h>


//texture header
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>

//math header
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/byhj.h"
#include "common/TextureManager.h"

//link the lib
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "freeImage.lib")
#endif //  COMMON_H
