# wxMASSManager
# Copyright (C) 2020-2021 Guillaume Jacquemin
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

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_CORRADE_SUBMODULE "Use Corrade from the Git submodule. If set to OFF, make sure you have Corrade 2020.06 or later installed to a path CMake can search in." ON)

if(USE_CORRADE_SUBMODULE)
    set(BUILD_STATIC ON CACHE BOOL "" FORCE)
    set(BUILD_STATIC_PIC ON CACHE BOOL "" FORCE)
    set(BUILD_STATIC_UNIQUE_GLOBALS OFF CACHE BOOL "" FORCE)

    set(WITH_INTERCONNECT OFF CACHE BOOL "" FORCE)
    set(WITH_PLUGINMANAGER OFF CACHE BOOL "" FORCE)
    set(WITH_TESTSUITE OFF CACHE BOOL "" FORCE)
    add_subdirectory(corrade EXCLUDE_FROM_ALL)
endif()

add_subdirectory(src)
