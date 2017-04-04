#pragma once

#include "Core/Types.hpp"

#include <string>

namespace ft
{
	class Path
	{
	public:

		static const char	s_iSeparator;
		static const char	s_iNotUsedSeparator;

		Path();
		Path(const Path& oPath);
		Path(const std::string& sPath);
		Path(const char* csPath);
		~Path();

		Path&			operator = (const Path& oPath);

		inline const std::string&	GetFullPath() const		{ return m_sFullPath; }

		std::string		GetFullName() const;
		std::string		GetDirPath() const;
		std::string		GetName() const;
		std::string		GetExtension() const;
		uint32			GetLength() const;

		bool			Set(const std::string& sPath);
		bool			Set(const char* csPath);

		bool			IsDirectory() const;
		bool			DoesExist() const;
		bool			IsValid() const;
		bool			IsEmpty() const;

		bool			Normalize();
		bool			Add(const std::string& s);

	protected:

		std::string		m_sFullPath;
	};

	std::ostream& operator << (std::ostream& oOs, const ft::Path& oPath);
}
