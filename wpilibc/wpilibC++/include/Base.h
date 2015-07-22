/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/
#pragma once

#if (__cplusplus < 201103L)
	#if !defined(_MSC_VER)
		#define nullptr NULL
	#endif
	#define constexpr const
#endif

#if defined(_MSC_VER)
  #define noexcept throw()
#endif

// A struct to use as a deleter when a std::shared_ptr must wrap a raw pointer
// that is being deleted by someone else.
// This should only be called in deprecated functions; using it anywhere else
// will throw warnings.
template<class T>
struct
#if !defined(_MSC_VER)
[[deprecated]]
#endif
NullDeleter {
  void operator()(T *) const noexcept {};
};
