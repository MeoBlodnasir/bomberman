
#include "Graphics/OpenGL.hpp"

#include "Core/Output.hpp"

namespace ft
{
	// Faire un enum -> string pour le log? Chiant à définir, même glewErrorToString est un peu naze...
	void	PrintOpenGLError(GLuint iError)
	{
		FT_CERR << "########## ASSERTION OPENGL ECHOUEE ###########" << std::endl;
		FT_CERR << "Erreur OpenGL: 0x" << std::hex << iError << '\n';
		FT_CERR << "###############################################" << std::endl;
	}
}
