#pragma once

#   include <GL/glew.h>
#if defined(_WIN32)

#	include <GL/wglew.h>

#else


#endif

namespace ft
{
	void	PrintOpenGLError(GLuint iError);
}

#if defined (__FT_DEBUG__)

#	define FT_GL_FLUSH_ERROR()					glGetError()

#	define FT_GL_ASSERT(expr)					\
	{											\
		FT_GL_FLUSH_ERROR();					\
		expr;									\
		GLuint iError__ = glGetError();			\
		if (iError__ != GL_NO_ERROR)			\
		{										\
			PrintOpenGLError(iError__);			\
			FT_FAILED_ASSERTION(expr);			\
		}										\
	}

#else

#	define FT_GL_FLUSH_ERROR()
#	define FT_GL_ASSERT(expr)		expr

#endif

#define FT_GL_INVALID_UNIFORM_LOCATION		((GLint)0xFFFFFFFF)
