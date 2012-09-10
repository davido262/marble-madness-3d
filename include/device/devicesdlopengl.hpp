/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  David Cavazos <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef DEVICESDLOPENGL_HPP
#define DEVICESDLOPENGL_HPP

#include "device.hpp"

struct SDL_Surface;

class DeviceSDLOpenGL : public Device {
public:
    void setTitle(const std::string& title);
    void setFullscreen(const bool useFullscreen);
    void setResolution(const size_t width, const size_t height);
    size_t videoMemKB();
    void processEvents(bool& isRunning);

private:
    static SDL_Surface* m_screen;

    void initialize();
    void shutdown();
};

#endif // DEVICESDLOPENGL_HPP
