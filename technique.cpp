#include "technique.h"
#include "util.h"
#include <string>

Technique::Technique()
{
}

Technique::~Technique()
{
    for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
    {
        glDeleteShader(*it);
    }

    if (m_shaderProgram != 0)
    {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
}

bool Technique::init()
{
	m_shaderProgram = glCreateProgram();

	if (m_shaderProgram == 0)
		return false;

	return true;
}

void Technique::enable()
{
    glUseProgram(m_shaderProgram);
}

bool Technique::addShader(GLenum shaderType, const char* pFilename)
{
    GLuint shaderObj = glCreateShader(shaderType);

    string s;
    if (!readFile(pFilename, s))
        return false;

    if (shaderObj == 0)
    {
        exit(0);
    }

    m_shaderObjList.push_back(shaderObj);

    const GLchar* p[1];
    p[0] = s.c_str();

    GLint lengths[1];
    lengths[0] = { (GLint)s.size() };

    glShaderSource(shaderObj, 1, p, lengths);

    glCompileShader(shaderObj);

    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
        fprintf(stderr, "Error compiling shader type %d: %s'\n'", shaderType, infoLog);
        exit(1);
    }

    glAttachShader(m_shaderProgram, shaderObj);
}

bool Technique::finalize()
{
    GLint success = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(m_shaderProgram);
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

    if (success == 0)
    {
        glGetProgramInfoLog(m_shaderProgram, sizeof(errorLog), NULL, errorLog);
        fprintf(stderr, "Invalid program: '\%s'\n", errorLog);
    }

    for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
    {
        glDeleteShader(*it);
    }

    m_shaderObjList.clear();

    return true;
}

GLint Technique::getUniformLocation(const char* pUniformName)
{
    GLint location = glGetUniformLocation(m_shaderProgram, pUniformName);

    return location;
}
