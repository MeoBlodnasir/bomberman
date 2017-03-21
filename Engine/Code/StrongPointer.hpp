#pragma once

#include "Countable.hpp"

namespace ft
{
	// Macro pratique (à utiliser au sein du namespace ft)
#if defined(SPtr)
#	undef	SPtr
#endif
#define		SPtr	ft::StrongPtr

	// A mettre autre part...
	template <typename T>
	inline void		Swap(T* const p1, T* const p2)
	{
		FT_ASSERT(p1 != nullptr);
		FT_ASSERT(p2 != nullptr);

		const T oTmp(*p1);
		*p1 = *p2;
		*p2 = oTmp;
	}

	// Référence dénombrable autodestructrice pour StrongPointer
	// Allocation DYANMIQUE requise! (voir comment le forcer au moment de la compilation)
	class CountableSPtr : public Countable
	{
	public:

		inline virtual void ReleaseReference() override
		{
			Countable::ReleaseReference();
			if ((int32)GetReferenceCount() <= 0)
				delete this;
		}
	};

	// Pointeur à référence dénombrable
	template <class T>
	class StrongPtr
	{
	public:
	
		StrongPtr() :							m_pData(nullptr)							{}
		StrongPtr(T* p) :						m_pData(p)									{ AddReference_(m_pData); }
		StrongPtr(const StrongPtr<T>& xOther) :	m_pData(xOther.m_pData)						{ AddReference_(m_pData); }
		template <class U>
		StrongPtr(const StrongPtr<U>& xOther) :	m_pData(dynamic_cast<T*>(xOther.m_pData))	{ AddReference_(m_pData); }

		~StrongPtr()																		{ Release_(m_pData); }

		StrongPtr&	operator = (const StrongPtr<T>& xRhs)		{ StrongPtr<T> xTemp(xRhs); Swap(&m_pData, &xTemp.m_pData); return *this; }
		StrongPtr&	operator = (T* pRhs)						{ StrongPtr<T> xTemp(pRhs); Swap(&m_pData, &xTemp.m_pData); return *this; }
		template <class U> 
		StrongPtr&	operator = (const StrongPtr<U>& xRhs)		{ StrongPtr<T> xTemp(xRhs); Swap(&m_pData, &xTemp.m_pData); return *this; }

		inline		operator T* () const						{ return m_pData; }
		inline T&	operator *  () const						{ FT_ASSERT(m_pData != nullptr); return *m_pData; }
		inline T*	operator -> () const						{ FT_ASSERT(m_pData != nullptr); return  m_pData; }
		inline bool	operator !  () const						{ return m_pData == nullptr; }

		inline T*	Ptr() const									{ return m_pData; }
		inline void	Release()									{ *this = nullptr; }

	protected:

		T*	m_pData;

	private:

		static inline void		AddReference_(T* p)
		{
			if (p != nullptr)
			{
				FT_ASSERT(dynamic_cast<const CountableSPtr*>(p) != nullptr);
				((CountableSPtr*)p)->AddReference();
			}
		}

		static inline void		Release_(T* p)
		{
			if (p != nullptr)
			{
				FT_ASSERT(dynamic_cast<const CountableSPtr*>(p) != nullptr);
				((CountableSPtr*)p)->ReleaseReference();
			}
		}
	};
}

template <class T, class U> inline bool operator == (const SPtr<T>& a, const SPtr<U>& b )		{ return a.Ptr() == b.Ptr(); }
template <class T, class U> inline bool operator != (const SPtr<T>& a, const SPtr<U>& b )		{ return a.Ptr() != b.Ptr(); }

template <class T, class U> inline bool operator == (const SPtr<T>& a, U* b )					{ return a.Ptr() == b; }
template <class T, class U> inline bool operator != (const SPtr<T>& a, U* b )					{ return a.Ptr() != b; }

template <class T, class U> inline bool operator == (T* a, const SPtr<U>& b )					{ return a == b.Ptr(); }
template <class T, class U> inline bool operator != (T* a, const SPtr<U>& b )					{ return a != b.Ptr(); }

template <class T>			inline bool operator <  (const SPtr<T>& a, const SPtr<T>& b)		{ return a.Ptr() < b.Ptr(); }