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

#include <wx/app.h>

#include "GUI/EvtMainFrame.h"

class MyApp: public wxApp {
    public:
        bool OnInit() {
            SetAppName("wxMASSManager");
            SetAppDisplayName("wxMASSManager");

            wxImage::AddHandler(new wxPNGHandler);

            EvtMainFrame* main_frame = new EvtMainFrame(nullptr);

            if(!main_frame->ready()) {
                return false;
            }

            main_frame->Show();
            return true;
        }
};

wxIMPLEMENT_APP(MyApp);
