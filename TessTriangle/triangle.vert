#version 430 core                                            
         
layout (location = 0) in vec3 Position;

out vec3 vPosition;
                                                  
void main(void)                                               
{                                                                                                                              
     vPosition =  Position;                        
}                                                                 