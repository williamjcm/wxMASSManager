# wxMASSManager
# Copyright (C) 2020 Guillaume Jacquemin
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.5)

project(wxMASSManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(BUILD_STATIC ON CACHE BOOL "" FORCE)
set(WITH_INTERCONNECT OFF CACHE BOOL "" FORCE)
set(WITH_PLUGINMANAGER OFF CACHE BOOL "" FORCE)
set(WITH_TESTSUITE OFF CACHE BOOL "" FORCE)
add_subdirectory(corrade)

find_package(Corrade REQUIRED Containers Utility)

include_directories(SYSTEM "C:/msys64/mingw64/lib/wx/include/msw-unicode-static-3.0")
include_directories(SYSTEM "C:/msys64/mingw64/include/wx-3.0")

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(wxMASSManager WIN32
    main.cpp
    GUI/MainFrame.fbp
    GUI/MainFrame.h
    GUI/MainFrame.cpp
    GUI/EvtMainFrame.h
    GUI/EvtMainFrame.cpp
    GUI/NameChangeDialog.fbp
    GUI/NameChangeDialog.h
    GUI/NameChangeDialog.cpp
    GUI/EvtNameChangeDialog.h
    GUI/EvtNameChangeDialog.cpp
    MassManager/MassManager.h
    MassManager/MassManager.cpp
    resource.rc)

target_compile_options(wxMASSManager PRIVATE -D_FILE_OFFSET_BITS=64 -D__WXMSW__ -fpermissive)

target_link_options(wxMASSManager PRIVATE -static -static-libgcc -static-libstdc++ -pipe -Wl,--subsystem,windows -mwindows)

target_link_libraries(wxMASSManager PRIVATE
    Corrade::Containers
    Corrade::Utility
    wx_mswu_adv-3.0
    wx_mswu_core-3.0
    wx_baseu-3.0
    wxregexu-3.0
    wxexpat-3.0
    wxtiff-3.0
    wxjpeg-3.0
    wxpng-3.0
    wxzlib-3.0
    rpcrt4
    oleaut32
    ole32
    uuid
    lzma
    jbig
    winspool
    winmm
    shell32
    comctl32
    comdlg32
    advapi32
    wsock32
    gdi32
    oleacc
    wtsapi32)
