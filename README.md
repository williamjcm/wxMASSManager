# wxMASSManager

A manager for M.A.S.S. save files, to facilitate sharing of M.A.S.S.es between users.

## Installing

Get the `wxMASSManager-win64.zip` file from the Releases page, and extract it somewhere. Then, open the `wxMASSManager` folder and launch `wxMASSManager.exe`.

## Building on MSYS2

1. Install the 64-bit (`x86_64`) version of [MSYS2](https://www.msys2.org/) in its default path (`C:\msys64`), and update it fully.
2. Run `pacman -S git mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw64/mingw-w64-x86_64-wxWidgets`.
3. In a `MINGW64` shell, type `git clone https://github.com/williamjcm/wxMASSManager`.
4. Type `cd wxMASSManager && git submodule init && git submodule update && mkdir build && cd build`.
5. Type `cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..`
6. Type `ninja`
7. ...
8. Profit!

You'll be able to find the executable in `build/Release/bin`.
