//
// Word size dependent definitions
// DAL 1/03
//

#ifndef ARCHTYPES_H
#define ARCHTYPES_H

#include <cstddef>

#if defined(__cplusplus) && (201103L <= __cplusplus)
#include <cstdint>
#else
#include "stdint.h"
#endif


typedef ::int16_t int16;
typedef ::uint16_t uint16;
typedef ::int32_t int32;
typedef ::uint32_t uint32;
typedef ::int64_t int64;
typedef ::uint64_t uint64;

typedef ::std::ptrdiff_t    intp;   // intp is an integer that can accomodate a pointer
typedef ::size_t            uintp;  // (ie, sizeof(intp) >= sizeof(int) && sizeof(intp) >= sizeof(void *)

#endif /* ARCHTYPES_H */
