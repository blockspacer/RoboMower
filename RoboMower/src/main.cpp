//==============================================================================
// RoboMower - Copyright (C) Matt Marchant; All Rights Reserved
// Unauthorized copying of this file via any medium is strictly prohibited
// Proprietary and confidential
// Written by Matt Marchant (matty_styles@hotmail.com) 2015
//==============================================================================

#include <App.hpp>

#ifdef __linux
#include <X11/Xlib.h>
#endif // __linux

int main()
{
#ifdef __linux
    XInitThreads();
#endif //__linux

    App app;
    app.run();

    return 0;
}