// wxMASSManager
// Copyright (C) 2020 Guillaume Jacquemin
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <Corrade/version.h>

#if !(CORRADE_VERSION_YEAR * 100 + CORRADE_VERSION_MONTH >= 202006)
    #error This application requires Corrade 2020.06 or later to build.
#endif

#include <shlobj.h>
#include <wtsapi32.h>

#include <Corrade/Containers/ScopeGuard.h>
#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/Unicode.h>

#include "MassBuilderManager.h"

using namespace Corrade;

MassBuilderManager::MassBuilderManager() {
    _ready = findSaveDirectory();
}

auto MassBuilderManager::ready() -> bool {
    return _ready;
}

auto MassBuilderManager::lastError() -> std::string const& {
    return _lastError;
}

auto MassBuilderManager::saveDirectory() -> std::string const& {
    return _saveDirectory;
}

auto MassBuilderManager::checkGameState() -> GameState {
    WTS_PROCESS_INFOW* process_infos = nullptr;
    unsigned long process_count = 0;
    _gameState = GameState::Unknown;

    if(WTSEnumerateProcessesW(WTS_CURRENT_SERVER_HANDLE, 0, 1, &process_infos, &process_count)) {
        Containers::ScopeGuard guard{process_infos, WTSFreeMemory};

        for(unsigned long i = 0; i < process_count; ++i) {
            if(std::wcscmp(process_infos[i].pProcessName, L"MASS_Builder-Win64-Shipping.exe") == 0) {
                _gameState = GameState::Running;
                break;
            }
            else {
                _gameState = GameState::NotRunning;
            }
        }
    }

    return _gameState;
}

auto MassBuilderManager::gameState() -> GameState {
    return _gameState;
}

auto MassBuilderManager::findSaveDirectory() -> bool {
    wchar_t h[MAX_PATH];
    if(!SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, h))) {
        _lastError = "SHGetFolderPathW() failed in MassBuilderManager::findSaveDirectory()";
        return false;
    }

    _saveDirectory = Utility::Directory::join(Utility::Directory::fromNativeSeparators(Utility::Unicode::narrow(h)), "MASS_Builder");

    if(!Utility::Directory::exists(_saveDirectory)) {
        _lastError = _saveDirectory + " wasn't found.";
        return false;
    }

    return true;
}
