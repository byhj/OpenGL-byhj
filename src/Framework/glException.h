#ifndef GLEXCEPTION_H
#define GLEXCEPTION_H

#include <string>
#include <exception>

namespace byhj
{

	class Exception: public std::exception
	{
	protected:
	      long line;
		  int number;
		  std::string typeName;
		  std::string description;
		  std::string source;
		  std::string file;
		  mutable std::string fullDesc;
	public:
    
		Exception(int number, const std::string &description, const std::string &source);
		Exception(int number, const std::string &description,  const std::string& sourece, 
			      const char *type, const char*file, long line);

		//copy constructor
		Exception(const Exception &rhs);
		~Exception() throw() {}
		 Exception & operator = (const Exception& rhs);

        virtual const std::string& getFullDescription(void) const;
        virtual int getNumber(void) const throw();
        virtual const std::string &getSource() const { return source; }
        virtual const std::string &getFile() const { return file; }

        virtual long getLine() const { return line; }
        virtual const std::string &getDescription(void) const { return description; }
        const char* what() const throw() { return getFullDescription().c_str(); }

	};

}

#endif
