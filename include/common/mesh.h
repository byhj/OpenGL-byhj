#ifndef MESH_H
#define MESH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

#define BUFFER_OFFSET(offset) ( (GLvoid *)(NULL + offset) )

struct Vertex
{
  glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh {

public:
	//mesh data
	std::vector<Vertex> VertexData;
	std::vector<GLuint> ElementData;
	std::vector<Texture> Textures;

	//constructor
	Mesh(const std::vector<Vertex> &V, const std::vector<GLuint> &E, const std::vector<Texture> &T) 
		 :VertexData(V), ElementData(E), Textures(T)
	{
		   init_mesh_buffer();
		   init_mesh_vertexArrray();
	}

	void Render(GLuint program)
	{
	
	   GLuint diffuseNr = 1;
	   GLuint specularNr = 1;
	   for (GLuint i = 0; i < Textures.size(); i++)
	   {
	   		// Retrieve texture number (the N in diffuse_textureN)
		   glActiveTexture(GL_TEXTURE0 + i); 
		   std::stringstream ss;
		   std::string number;
	 	   std::string name = Textures[i].type;
	  	   if (name == "texture_diffuse")
			  ss << diffuseNr++; 
		   else if (name == "texture_specular")
			  ss << specularNr++; 
		   number = ss.str();

	       //为当前纹理单元的TEXTURE_2D绑定纹理
		   glUniform1f(glGetUniformLocation(program, (name + number).c_str()), i);
		   glBindTexture(GL_TEXTURE_2D, Textures[i].id);
	   }

	   // Also set each mesh's shininess property to a default value 
	   glUniform1f(glGetUniformLocation(program, "material.shininess"), 16.0f);

	   // Draw mesh
	   glUseProgram(program);
	   glBindVertexArray(vao);
	   glDrawElements(GL_TRIANGLES, ElementData.size(), GL_UNSIGNED_INT, 0);
	   glBindVertexArray(0);

	  // Always good practice to set everything back to defaults once configured.
	     for (GLuint i = 0; i < Textures.size(); i++)
	    {
		    glActiveTexture(GL_TEXTURE0 + i);
	      glBindTexture(GL_TEXTURE_2D, 0);
	    }
	}
public:
	GLuint vao, vbo, ebo;
	void init_mesh_buffer()
	{
	  //set the Vertex Data to buffer
	  glGenBuffers(1, &vbo);
	  glBindBuffer(GL_ARRAY_BUFFER, vbo);
	  glBufferData(GL_ARRAY_BUFFER, VertexData.size() *  sizeof(Vertex), &VertexData[0], GL_STATIC_DRAW);
	  glBindBuffer(GL_ARRAY_BUFFER, 0);

     //set the Element Data to buffer
	  glGenBuffers(1, &ebo);
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ElementData.size() * sizeof(GLuint), &ElementData[0], GL_STATIC_DRAW);
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void init_mesh_vertexArrray()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//1-->Position       2-->Normal       3-->TexCoords
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(NULL));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3)) );
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(2 * sizeof(glm::vec3)) );

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
};
#endif
