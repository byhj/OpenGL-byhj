#ifndef TEXTFILEREAD_H
#define TEXTFILEREAD_H

#include <cstdio>
#include <iostream>
#include <exception>

char *textFileRead(const char *fn)
{    
	//read the text to content
	FILE *fp;  
	char *content = NULL;  
	int count = 0;  

	if (fn != NULL) 
	{  
		  if ( fopen_s(&fp , fn, "rt") )
			  throw std::runtime_error("Can not open the shader file!");

			 fseek(fp, 0, SEEK_END);  
			 count = ftell(fp);  
			 rewind(fp);  

			 if (count > 0) 
			 {  
				content = (char *)malloc(sizeof(char) * (count+1));  
				count = fread(content,sizeof(char),count,fp);  
				content[count] = '\0';  
			 }  
			fclose(fp);  
	}  
	return content;  
}  

#endif //textFileRead