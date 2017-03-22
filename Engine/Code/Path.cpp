
#include "Path.hpp"

#include "Core.hpp"

#if defined(_WIN32)
#	include <windows.h>
#else
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <unistd.h>
#   include <stdlib.h> 
#   include <linux/limits.h>
#   include <errno.h>
#   include <string.h>
#endif

FT_TODO("Faire tout ça pour le systeme de fichiers UNIX")

namespace ft
{
#if defined(_WIN32)
	const char Path::s_iSeparator		 = '\\';
	const char Path::s_iNotUsedSeparator = '/';
#else
	const char Path::s_iSeparator		 = '/';
	const char Path::s_iNotUsedSeparator = '\\';
#endif

	Path::Path()
	{
	}

	Path::Path(const Path& oPath)
	{
		FT_TEST(Set(oPath.m_sFullPath));
	}

	Path::Path(const std::string& sPath)
	{
		FT_TEST(Set(sPath));
	}

	Path::Path(const char* csPath)
	{
		FT_TEST(Set(csPath));
	}

	Path::~Path()
	{
	}

	Path&	Path::operator = (const Path& oPath)
	{
		FT_TEST(Set(oPath.m_sFullPath));
		return *this;
	}

	std::string		Path::GetFullName() const
	{
		std::size_t iBegin = m_sFullPath.find_last_of(s_iSeparator);
		std::size_t iEnd   = std::string::npos;

		if (iBegin == m_sFullPath.size() - 1)
		{
			iBegin = m_sFullPath.substr(0, m_sFullPath.size() - 1).find_last_of(s_iSeparator);
			iEnd = m_sFullPath.size() - 2 - iBegin;
		}

		if (iBegin != std::string::npos)
			return m_sFullPath.substr(iBegin + 1, iEnd);
		else
			return m_sFullPath;
	}

	std::string		Path::GetDirPath() const
	{
		std::size_t iEnd = m_sFullPath.find_last_of(s_iSeparator);

		if (iEnd == m_sFullPath.size() - 1)
			iEnd = m_sFullPath.substr(0, m_sFullPath.size() - 1).find_last_of(s_iSeparator);

		if (iEnd != std::string::npos)
			return m_sFullPath.substr(0, iEnd + 1);
		else
			return IsEmpty() ? std::string() : (std::string(".") + s_iSeparator);
	}

	std::string		Path::GetName() const
	{
		const std::string sName	 = GetFullName();
		const std::size_t iBegin = sName.find_last_of(s_iSeparator);
		const std::size_t iEnd	 = sName.find_last_of('.');

		if (iBegin != std::string::npos)
			return sName.substr(iBegin, iEnd);
		else
			return sName.substr(0, iEnd);
	}

	std::string		Path::GetExtension() const
	{
		const std::string sName = GetFullName();
		const std::size_t iPos  = sName.find_last_of('.');

		if (iPos != std::string::npos)
			return sName.substr(iPos);
		else
			return std::string();
	}

	uint32	Path::GetLength() const
	{
		return (uint32)m_sFullPath.length();
	}

	bool	Path::Set(const std::string& sPath)
	{
		m_sFullPath = sPath;
		Normalize();
		return IsValid();
	}

	bool	Path::Set(const char* csPath)
	{
		return Set(std::string(csPath));
	}

	bool	Path::IsDirectory() const
	{
#if defined(_WIN32)
		const DWORD iFileAttr = GetFileAttributes(m_sFullPath.c_str());
		return (iFileAttr != INVALID_FILE_ATTRIBUTES) && (iFileAttr & FILE_ATTRIBUTE_DIRECTORY);
#else
        struct stat buf;
        stat(m_sFullPath.c_str(), &buf);
        return (S_ISDIR(buf.st_mode));
#endif
	}

	bool	Path::DoesExist() const
	{
		FT_TODO("Tester Path::DoesExist() mieux que ça");
		return IsValid();
	}

	bool	Path::IsValid() const
	{
#if defined(_WIN32)
		const DWORD iFileAttr = GetFileAttributes(m_sFullPath.c_str());
		return iFileAttr != INVALID_FILE_ATTRIBUTES;
#else
        struct stat buf;
        return (stat(m_sFullPath.c_str(), &buf) < 0) ? false : true;
#endif
	}

	bool	Path::IsEmpty() const
	{
		return m_sFullPath.empty();
	}

	bool	Path::Normalize()
	{
#if defined(_WIN32)
#	define FT_BUFFERSIZE	512
		TCHAR csFullPathBuffer[FT_BUFFERSIZE] = {0};
		DWORD iFullPathLength = 0;
#endif
		// Normaliser les séparateurs
		for (std::string::iterator it = m_sFullPath.begin(), itEnd = m_sFullPath.end(); it != itEnd; ++it)
		{
			if (*it == s_iNotUsedSeparator)
				*it = s_iSeparator;
		}

#if defined(_WIN32)
		iFullPathLength = GetFullPathName(m_sFullPath.c_str(), FT_BUFFERSIZE, csFullPathBuffer, 0);
		FT_ASSERT(iFullPathLength < FT_BUFFERSIZE);
        m_sFullPath = std::string(csFullPathBuffer);
#else
      char *ptr;
      char buf[PATH_MAX];
      ptr = realpath(m_sFullPath.c_str(), buf); 
      FT_ASSERT(ptr == nullptr);
      if (ptr != nullptr)
        m_sFullPath = std::string(ptr);

#endif

      // Ajouter un '/' en fin de dossier
      if (IsDirectory() && m_sFullPath.back() != s_iSeparator)
        m_sFullPath += s_iSeparator;

      return IsValid();
    }

    bool	Path::Add(const std::string& sName)
    {
      return Set(m_sFullPath + sName);
    }
}

std::ostream& operator << (std::ostream& oOs, const ft::Path& oPath)
{
  oOs << oPath.GetFullPath();
  return oOs;
}
