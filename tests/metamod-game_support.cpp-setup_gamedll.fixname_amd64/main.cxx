#include <cstdlib>
#include <cstring>
#include <iostream>


#define TEST_ASSERT_(condition) ::test_::assert(condition, #condition, __LINE__)


namespace test_ {

    void assert(bool value, char const *text, int line) {
        if (value) return;
        ::std::cerr << "assertion failed at " << line << ": " << text << ::std::endl << ::std::flush;
        ::std::abort();
    }

    inline static char const * new_logic(char const *name) {
        char const *knownfn_ = name;

        if (true) {
            struct Helper_ {
                inline static char const * routine(char const *src) {
                    if (! src) return src;
                    ::std::string name_ = src;
                    ::std::size_t const name_size_ = name_.size();
                    static ::std::size_t const pattern_size_ = sizeof("_i386.so") - 1;
                    if (! (name_size_ > pattern_size_)) return src;
                    if (true) {
                        char &char_ = name_.at(name_size_ - sizeof("86.so"));
                        if ('3' != char_) {
                            if (('3' < char_) && ('7' > char_)) char_ = '3';
                            else return src;
                        }
                    }
                    ::std::size_t const offset_ = name_size_ - pattern_size_;
                    if (0 != ::std::strcmp("_i386.so", name_.data() + offset_)) return src;
                    name_.resize(offset_);
                    // pointer is given outside function
                    static ::std::string fixed_;
                    fixed_ = name_ + "_amd64.so";
                    return fixed_.data();
                }
            };
            knownfn_ = Helper_::routine(knownfn_);
        }

        return knownfn_;
    }

} // namespace test_

int main(int, char **) {
    TEST_ASSERT_(NULL == ::test_::new_logic(NULL));

    static char const * const empty_ = "";
    TEST_ASSERT_(empty_ == ::test_::new_logic(empty_));

    TEST_ASSERT_(0 == ::std::strcmp("_i386.so", ::test_::new_logic("_i386.so")));
    TEST_ASSERT_(0 == ::std::strcmp("a_ia86.so", ::test_::new_logic("a_ia86.so")));
    TEST_ASSERT_(0 == ::std::strcmp("a_i086.so", ::test_::new_logic("a_i086.so")));
    TEST_ASSERT_(0 == ::std::strcmp("a_i186.so", ::test_::new_logic("a_i186.so")));
    TEST_ASSERT_(0 == ::std::strcmp("a_i286.so", ::test_::new_logic("a_i286.so")));
    TEST_ASSERT_(0 == ::std::strcmp("a_amd64.so", ::test_::new_logic("a_i386.so")));
    TEST_ASSERT_(0 == ::std::strcmp("a_amd64.so", ::test_::new_logic("a_i486.so")));
    TEST_ASSERT_(0 == ::std::strcmp("a_amd64.so", ::test_::new_logic("a_i586.so")));
    TEST_ASSERT_(0 == ::std::strcmp("a_amd64.so", ::test_::new_logic("a_i686.so")));
    TEST_ASSERT_(0 == ::std::strcmp("a_i786.so", ::test_::new_logic("a_i786.so")));
    TEST_ASSERT_(0 == ::std::strcmp("a_i886.so", ::test_::new_logic("a_i886.so")));
    TEST_ASSERT_(0 == ::std::strcmp("a_i986.so", ::test_::new_logic("a_i986.so")));

    return 0;
}
