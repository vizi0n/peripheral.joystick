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
#pragma once

#include "api/Joystick.h"

#include "kodi/threads/mutex.h"
#include "kodi/threads/threads.h"

namespace JOYSTICK
{
  class CButtonFactory;

  /*
   * Hey! My name is Joe
   * And I work in a button factory
   * I have a husband, three kids, and a family.
   */
  class CJoe : public CJoystick, public PLATFORM::CThread
  {
  public:
    CJoe(CButtonFactory* api);
    virtual ~CJoe(void) { Deinitialize(); }

    virtual bool Initialize(void);
    virtual void Deinitialze(void);

  protected:
    virtual void* Process(void);

    virtual bool ScanEvents(std::vector<ADDON::PeripheralEvent>& events);

  private:
    unsigned int                        m_busyness;
    std::vector<ADDON::PeripheralEvent> m_events;
    PLATFORM::CMutex                    m_eventsMutex;
    PLATFORM::CEvent                    m_sleepEvent;
  };
}
