#pragma once

#include <memory>

// Macros pratiques
#if defined(Sptr)
#	undef	Sptr
#endif
#define		Sptr	std::shared_ptr

#if defined(Wptr)
#	undef	Wptr
#endif
#define		Wptr	std::weak_ptr

#if defined(Uptr)
#	undef	Uptr
#endif
#define		Uptr	std::unique_ptr
