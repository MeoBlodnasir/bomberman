
#include "Core/Path.hpp"

#include "Core/Core.hpp"

#if defined(_WIN32)
#	include <windows.h>
#else
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <unistd.h>
#   include <stdlib.h> 
#   include <linux/limits.h>
#endif

#include <algorithm>

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
		return IsEmpty() || IsValid();
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

		struct stat oStats;
		stat(m_sFullPath.c_str(), &oStats);
		return (S_ISDIR(oStats.st_mode));

#endif
	}

	bool	Path::DoesExist() const
	{
		FT_TODO("Tester Path::DoesExist() mieux que �a");
		return IsValid();
	}

	bool	Path::IsValid() const
	{
#if defined(_WIN32)

		const DWORD iFileAttr = GetFileAttributes(m_sFullPath.c_str());
		return iFileAttr != INVALID_FILE_ATTRIBUTES;

#else

		struct stat oStats;
		return (stat(m_sFullPath.c_str(), &oStats) < 0) ? false : true;

#endif
	}

	bool	Path::IsEmpty() const
	{
		return m_sFullPath.empty();
	}

	bool	Path::Normalize()
	{
		// Normaliser les s�parateurs
		std::replace(m_sFullPath.begin(), m_sFullPath.end(), s_iNotUsedSeparator, s_iSeparator);

		// R�cup�rer le chemin complet absolu
#if defined(_WIN32)

		TCHAR csFullPathBuffer[MAX_PATH] = {0};
		DWORD iFullPathLength = 0;
		iFullPathLength = GetFullPathName(m_sFullPath.c_str(), MAX_PATH, csFullPathBuffer, 0);
		FT_ASSERT(iFullPathLength < MAX_PATH);
		m_sFullPath = std::string(csFullPathBuffer);

#else

		char *csFullPathPtr;
		char csFullPathBuffer[PATH_MAX];
		csFullPathPtr = realpath(m_sFullPath.c_str(), csFullPathBuffer); 
		FT_ASSERT(csFullPathPtr != nullptr);
		m_sFullPath = std::string(csFullPathPtr);

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

	std::ostream& operator << (std::ostream& oOs, const ft::Path& oPath)
	{
		oOs << oPath.GetFullPath();
		return oOs;
	}
}
