#pragma once
/*
*      Copyright (C) 2012-2013 Team XBMC
*      http://www.xbmc.org
*
*  This Program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2, or (at your option)
*  any later version.
*
*  This Program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with XBMC; see the file COPYING.  If not, see
*  <http://www.gnu.org/licenses/>.
*
*/
#pragma once

#include "XInputDLL.h"
#include "api/JoystickInterface.h"

#include <windows.h> // TODO: Remove me when IsXInputDevice() is moved

namespace JOYSTICK
{
  class CJoystickInterfaceXInput : public CJoystickInterface
  {
  public:
    CJoystickInterfaceXInput(void);
    virtual ~CJoystickInterfaceXInput(void) { Deinitialize(); }

    virtual bool Initialize(void);
    virtual void Deinitialize(void);

    // TODO: Move IsXInputDevice() to JoystickInterfaceXInput.cpp
    static bool IsXInputDevice(const GUID* pGuidProductFromDirectInput);



  protected:
    virtual bool PerformJoystickScan(std::vector<CJoystick*>& joysticks);
  };
}
