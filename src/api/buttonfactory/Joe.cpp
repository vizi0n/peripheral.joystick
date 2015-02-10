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

#include "Joe.h"
#include "ButtonFactory.h"

using namespace JOYSTICK;

#define EVENT_PERIOD_MS   1000

CJoe::CJoe(CButtonFactory* api)
 : CJoystick(api),
   m_busyness(0)
{
  SetName("Joe");
  SetButtonCount(1);
  SetHatCount(1);
  SetAxisCount(1);

  Features().push_back(new ADDON::DriverButton(JOYSTICK_FEATURE_BUTTON_RIGHT, 0));
  Features().push_back(new ADDON::DriverHat(JOYSTICK_FEATURE_BUTTON_LEFT, 0, JOYSTICK_DRIVER_HAT_LEFT));
  Features().push_back(new ADDON::DriverSemiAxis(JOYSTICK_FEATURE_BUTTON_UP,   0, JOYSTICK_DRIVER_SEMIAXIS_DIRECTION_POSITIVE));
  Features().push_back(new ADDON::DriverSemiAxis(JOYSTICK_FEATURE_BUTTON_DOWN, 0, JOYSTICK_DRIVER_SEMIAXIS_DIRECTION_NEGATIVE));
}

bool CJoe::Initialize(void)
{
  CreateThread(false);
  return CJoystick::Initialize();
}

void CJoe::Deinitialze(void)
{
  StopThread(-1);
  m_sleepEvent.Broadcast();
}

void* CJoe::Process(void)
{
  while (!IsStopped())
  {
    {
      PLATFORM::CLockObject lock(m_eventsMutex);

      if (m_busyness == 0)
      {
        /*
         * Hey! My name is Joe
         * And I work in a button factory.
         * I have a husband, three kids, and a family.
         * One day my boss came up to me.
         * He said "Joe, are you busy?"
         * I said, "No"
         * He said to me "push this button with your right hand"
         */
        m_events.push_back(ADDON::PeripheralEvent(Index(), 0, JOYSTICK_STATE_BUTTON_PRESSED));
        m_busyness++;
      }
      else if (m_busyness == 1)
      {
        /*
         * Hey! My name is Joe
         * And I work in a button factory.
         * I have a husband, three kids, and a family.
         * One day my boss came up to me.
         * He said "Joe, are you busy?"
         * I said, "No"
         * He said to me "push this button with your left hand"
         */
        m_events.push_back(ADDON::PeripheralEvent(Index(), 0, JOYSTICK_STATE_HAT_LEFT));
        m_busyness++;
      }
      else if (m_busyness == 2)
      {
        /*
         * Hey! My name is Joe
         * And I work in a button factory.
         * I have a husband, three kids, and a family.
         * One day my boss came up to me.
         * He said "Joe, are you busy?"
         * I said, "No"
         * He said to me "push this button with your right knee"
         */
        m_events.push_back(ADDON::PeripheralEvent(Index(), 0, 1.0f));
        m_busyness++;
      }
      else if (m_busyness == 3)
      {
        /*
         * Hey! My name is Joe
         * And I work in a button factory.
         * I have a husband, three kids, and a family.
         * One day my boss came up to me.
         * He said "Joe, are you busy?"
         * I said, "No"
         * He said to me "push this button with your tongue"
         */
        m_events.push_back(ADDON::PeripheralEvent(Index(), 0, -1.0f));
        m_busyness++;
      }
      else
      {
        /*
         * Hey! My name is Joe
         * And I work in a button factory.
         * I have a husband, three kids, and a family.
         * One day my boss came up to me.
         * He said "Joe, are you busy?"
         * I said, "YES!
         */
        m_events.push_back(ADDON::PeripheralEvent(Index(), 0, JOYSTICK_STATE_BUTTON_UNPRESSED));
        m_events.push_back(ADDON::PeripheralEvent(Index(), 0, JOYSTICK_STATE_HAT_UNPRESSED));
        m_events.push_back(ADDON::PeripheralEvent(Index(), 0, 0.0f));
        m_busyness = 0;
      }
    }

    m_sleepEvent.Wait(EVENT_PERIOD_MS);
  }
  return NULL;
}

bool CJoe::ScanEvents(std::vector<ADDON::PeripheralEvent>& events)
{
  PLATFORM::CLockObject lock(m_eventsMutex);

  if (!m_events.empty())
  {
    events.insert(events.end(), m_events.begin(), m_events.end());
    m_events.clear();
  }

  return true;
}
