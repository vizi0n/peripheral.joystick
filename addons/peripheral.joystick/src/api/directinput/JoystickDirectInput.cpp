/*
 *      Copyright (C) 2014 Garrett Brown
 *      Copyright (C) 2014 Team XBMC
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

#include "JoystickDirectInput.h"
#include "JoystickInterfaceDirectInput.h"
#include "log/Log.h"

using namespace JOYSTICK;

#define AXIS_MIN       -32768  // minimum value for axis coordinate
#define AXIS_MAX        32767  // maximum value for axis coordinate

/* TODO
#define MAX_AXISAMOUNT  32768

#define JOY_POV_360  JOY_POVBACKWARD * 2
#define JOY_POV_NE   (JOY_POVFORWARD + JOY_POVRIGHT) / 2
#define JOY_POV_SE   (JOY_POVRIGHT + JOY_POVBACKWARD) / 2
#define JOY_POV_SW   (JOY_POVBACKWARD + JOY_POVLEFT) / 2
#define JOY_POV_NW   (JOY_POVLEFT + JOY_POV_360) / 2
*/

CJoystickDirectInput::CJoystickDirectInput(LPDIRECTINPUTDEVICE8 joystickDevice, CJoystickInterfaceDirectInput* api)
 : CJoystick(api),
   m_joystickDevice(joystickDevice)
{
}

bool CJoystickDirectInput::Initialize(void)
{
  // Initialize axes
  // Enumerate the joystick objects. The callback function enabled user
  // interface elements for objects that are found, and sets the min/max
  // values properly for discovered axes.
  HRESULT hr = m_joystickDevice->EnumObjects(EnumObjectsCallback, m_joystickDevice, DIDFT_ALL);
  if (FAILED(hr))
  {
    esyslog("%s: Failed to enumerate objects", __FUNCTION__);
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------
// Name: EnumObjectsCallback()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a
//       joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
//-----------------------------------------------------------------------------
BOOL CALLBACK CJoystickDirectInput::EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext)
{
  LPDIRECTINPUTDEVICE8 pJoy = static_cast<LPDIRECTINPUTDEVICE8>(pContext);

  // For axes that are returned, set the DIPROP_RANGE property for the
  // enumerated axis in order to scale min/max values.
  if (pdidoi->dwType & DIDFT_AXIS)
  {
    DIPROPRANGE diprg;
    diprg.diph.dwSize = sizeof(DIPROPRANGE);
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    diprg.diph.dwHow = DIPH_BYID;
    diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
    diprg.lMin = AXIS_MIN;
    diprg.lMax = AXIS_MAX;

    // Set the range for the axis
    HRESULT hr = pJoy->SetProperty(DIPROP_RANGE, &diprg.diph);
    if (FAILED(hr))
      esyslog("%s : Failed to set property on %s", __FUNCTION__, pdidoi->tszName);
  }
  return DIENUM_CONTINUE;
}

bool CJoystickDirectInput::GetEvents(std::vector<ADDON::PeripheralEvent>& events)
{
  HRESULT     hr;
  DIJOYSTATE2 js; // DInput joystick state

  hr = m_joystickDevice->Poll();

  if (FAILED(hr))
  {
    int i = 0;
    // DInput is telling us that the input stream has been interrupted. We
    // aren't tracking any state between polls, so we don't have any special
    // reset that needs to be done. We just re-acquire and try again 10 times.
    do
    {
      hr = m_joystickDevice->Acquire();
    } while (hr == DIERR_INPUTLOST && i++ < 10);

    // hr may be DIERR_OTHERAPPHASPRIO or other errors. This may occur when the
    // app is minimized or in the process of switching, so just try again later.
    return false;
  }

  // Get the input's device state
  hr = m_joystickDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
  if (FAILED(hr))
    return false; // The device should have been acquired during the Poll()

  /* TODO

  // Gamepad buttons
  for (unsigned int b = 0; b < state.buttons.size(); b++)
    state.buttons[b] = ((js.rgbButtons[b] & 0x80) ? 1 : 0);

  // Gamepad hats
  for (unsigned int h = 0; h < state.hats.size(); h++)
  {
    state.hats[h].Center();
    bool bCentered = ((js.rgdwPOV[h] & 0xFFFF) == 0xFFFF);
    if (!bCentered)
    {
      if ((JOY_POV_NW <= js.rgdwPOV[h] && js.rgdwPOV[h] <= JOY_POV_360) || js.rgdwPOV[h] <= JOY_POV_NE)
        state.hats[h][CJoystickHat::UP] = true;
      else if (JOY_POV_SE <= js.rgdwPOV[h] && js.rgdwPOV[h] <= JOY_POV_SW)
        state.hats[h][CJoystickHat::DOWN] = true;

      if (JOY_POV_NE <= js.rgdwPOV[h] && js.rgdwPOV[h] <= JOY_POV_SE)
        state.hats[h][CJoystickHat::RIGHT] = true;
      else if (JOY_POV_SW <= js.rgdwPOV[h] && js.rgdwPOV[h] <= JOY_POV_NW)
        state.hats[h][CJoystickHat::LEFT] = true;
    }
  }

  // Gamepad axes
  long amounts[] = { js.lX, js.lY, js.lZ, js.lRx, js.lRy, js.lRz };
  for (unsigned int a = 0; a < std::min(state.axes.size(), 6U); a++)
    state.SetAxis(a, amounts[a], MAX_AXISAMOUNT);

  */

  return false;
}