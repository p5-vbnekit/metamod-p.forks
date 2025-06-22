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
- This file ([README.md](README.md)).

## Major changes

- Added `CMake` build system.
- Added [`github actions`](https://github.com/p5-vbnekit/forks.metamod-p/actions) [workflow](.github/workflows/main.yml).
  - automated multiplatform builds.
  - automated packages generation (watch artifacts in workflow run).
  - automated uploading release assets (watch assets in [demo release](https://github.com/p5-vbnekit/forks.metamod-p/releases/tag/v1.21p38-demo)).

## TODO

- Kill `c++` + `c` Frankenstein. It's better to use `c++`. Migrate as carefully as possible (preferably lossless).
  - Find `c` code and migrate to `c++` as possible.
  - Fix code and remove `-Wno-write-strings` option (split `char *` and `const char *` as possible).
- Fix for old versions of `mingw` compilers.
  - Find alternative of compile definitions for `windres`.
  - Find alternative for `-Wl,--add-stdcall-alias`.
- Fix for `x86_64` compiler.
- Rebase/cherry-pick & push into [upstream](https://github.com/Bots-United/metamod-p).
