#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 Pos;
	glm::vec3 Color;
};

namespace Color 
{
   glm::vec4 White  (1.0f, 1.0f, 1.0f, 1.0f); 
   glm::vec4 Black  (0.0f, 0.0f, 0.0f, 1.0f);
   glm::vec4 Red    (1.0f, 0.0f, 0.0f, 1.0f);
   glm::vec4 Green  (0.0f, 1.0f, 0.0f, 1.0f);
   glm::vec4 Blue   (0.0f, 0.0f, 1.0f, 1.0f);
   glm::vec4 Yellow (1.0f, 1.0f, 0.0f, 1.0f);
   glm::vec4 Silver (0.75f, 0.75f, 0.75f, 1.0f);

}

#endif // !VERTEX_H
