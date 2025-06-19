#include <string>


inline static bool linker_should_failed_if_string_constructor_are_bogus_() {
    // this constructor is bogus in some versions of mingw
    // we can get `multiple definition` with `interprocedural optimization` and `static_libstdc++`
    ::std::string data_("", 0);
    return data_.empty();
}

int main(int, char **) {
    linker_should_failed_if_string_constructor_are_bogus_();
    return 0;
}
