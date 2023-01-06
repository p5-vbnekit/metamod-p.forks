# Metamod Half-Life utility mod

See the files under "doc/txt" or "doc/html" for more information.

Will Day \
willday@metamod.org \
http://www.metamod.org

## $${\color{red}WARNING}$$

### **There is a high chance that this branch will be _rebased (commit history may be changed)_!**
The commit history is not final as it is not in perfect condition. \
I'm still working on this, and I apologize in advance for any problems with merge conflicts. :P

#### Some reasons for periodic rebase:
- This is a fork and I'm waiting for a response from the [upstream (Bots-United/metamod-p)](https://github.com/Bots-United/metamod-p) maintainers.
If they responds to [my initial PR](https://github.com/Bots-United/metamod-p/pull/25), I will coordinate further changes with them for the correct phased meging into the upstream. If I am asked to edit, split or remove some commits, then it is quite possible that I will do it.
- "Linuxification" of many files (line endings, extra/missing/trailing spaces, etc.). \
In my opinion, the basic things that can make it more difficult to code review in various utilities should be eliminated. But I'm not sure yet that the maintainers will accept it.
- This file ([README.md](README.md)).

## Major changes

- Added `CMake` build system.
- Added [`github actions`](https://github.com/p5-vbnekit/forks.metamod-p/actions) [workflow](.github/workflows/main.yml).
  - automated multiplatform builds.
  - automated packages generation (watch artifacts in workflow run).
  - automated uploading release assets (watch assets in [demo release](https://github.com/p5-vbnekit/forks.metamod-p/releases/tag/v1.21p38-demo)).
- Fixed for modern compilers.
- Fixed for `x86_64`.
- Fixed lot of warnings.
- Fixed some dangerous/dirty code.

## TODO

- Temporarily add a `__cxa_throw` handler while the `metamod` is being compiled with `-fno-exceptions`. \
  To stay in legacy mode for now (don't apply `libstdc++` and exceptions), there is a workaround - add `__cxa_throw` handler into `metamod.dll`|`so` that will call `::std::terminate` (and `::std::abort`). \
  A long time ago in a galaxy far, far away.... `-fno-exceptions` made sense. \
  Now there will be no overhead for exception handling. In any case, `-fno-exceptions` is correct to use when code only uses `noexcept` calls. There are lot of places in the `metamod` where exceptions can still be thrown (for example, allocation over `new` instead `malloc`), but `-fno-exceptions` will cause them to be ignored. There is no guarantee that the engine or some `mod/gamedll` will not catch and process it without taking into account the specifics of the `metamod`. There is no guarantee that the engine or some mod/gamedll will not throw exception. Which can break "exception-safe" policy.
- Kill `c++` + `c` Frankenstein. It's better to use `c++`. Migrate as carefully as possible (preferably lossless). \
  In today's world, the correct use of `libstdc++` and exceptions will not give overhead. \
  `libstdc++` can be linked as a whole static archive (`-static-libstdc++`).
  - Find `c` code and migrate to `c++` as possible.
  - Fix code and remove `-fno-exceptions` option and `__cxa_throw` workaround.
  - Fix code and remove `-Wno-write-strings` option (split `char *` and `const char *` as possible).
- Fix for old versions of `mingw` compilers.
  - Find alternative of compile definitions for `windres`.
  - Find alternative alternative for `-Wl,--add-stdcall-alias`.
- Fix for `MSVC` (`i686` & `x86_64`) compiler.
- Fix for `CLang` and `intel` compilers... \
  Not critical. Do not rush. It will be boring and sad... =(
- Rebase & push into [upstream](https://github.com/Bots-United/metamod-p).
