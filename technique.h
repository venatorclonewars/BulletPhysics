#pragma once
#include <list>
#include <GL/glew.h>

class Technique
{
public:
	Technique();

	virtual ~Technique();

	virtual bool init();

	void enable();

protected:

	bool addShader(GLenum shaderType, const char* pFilename);

	bool finalize();

	GLint getUniformLocation(const char* pUniformName);

	GLint m_shaderProgram;

private:
	typedef std::list<GLint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};

