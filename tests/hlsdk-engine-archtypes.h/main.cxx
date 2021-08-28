#include <archtypes.h>

#include <cstdlib>
#include <iostream>

#if defined(__cplusplus) && (201103L <= __cplusplus)
#include <cstdint>
#include <type_traits>
#else
#include "stdint.h"
#endif


#define TEST_ASSERT_(condition) ::test_::assert(condition, #condition, __LINE__)


namespace test_ {

    void assert(bool value, char const *text, int line) {
        if (value) return;
        ::std::cerr << "assertion failed at " << line << ": " << text << ::std::endl << ::std::flush;
        ::std::abort();
    }

#if defined(__cplusplus) && (201103L <= __cplusplus)
    template <class T> inline static bool is_signed() { return ::std::is_signed<T>::value; }
#else
    template <class T> inline static bool is_signed() { return static_cast<T>(0) > static_cast<T>(-1); }
#endif

} // namespace test_

int main(int, char **) {
    TEST_ASSERT_(2 == sizeof(::int16));
    TEST_ASSERT_(::test_::is_signed< ::int16>());
    TEST_ASSERT_(2 == sizeof(::uint16));
    TEST_ASSERT_(! ::test_::is_signed< ::uint16>());

    TEST_ASSERT_(4 == sizeof(::int32));
    TEST_ASSERT_(::test_::is_signed< ::int32>());
    TEST_ASSERT_(4 == sizeof(::uint32));
    TEST_ASSERT_(! ::test_::is_signed< ::uint32>());

    TEST_ASSERT_(8 == sizeof(::int64));
    TEST_ASSERT_(::test_::is_signed< ::int64>());
    TEST_ASSERT_(8 == sizeof(::uint64));
    TEST_ASSERT_(! ::test_::is_signed< ::uint64>());

    TEST_ASSERT_(sizeof(void *) == sizeof(::intp));
    TEST_ASSERT_(::test_::is_signed< ::intp>());
    TEST_ASSERT_(sizeof(void *) == sizeof(::uintp));
    TEST_ASSERT_(! ::test_::is_signed< ::uintp>());

    return 0;
}
