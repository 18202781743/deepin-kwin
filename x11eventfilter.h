/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2014 Fredrik Höglund <fredrik@kde.org>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#ifndef X11EVENTFILTER_H
#define X11EVENTFILTER_H

#include <xcb/xcb.h>

#include <kwin_export.h>

namespace KWin
{

class KWIN_EXPORT X11EventFilter
{
public:
    /**
     * Creates an event filter for the given event type.
     */
    X11EventFilter(int eventType, int opcode = 0, int genericEventType = 0);

    /**
     * Destroys the event filter.
     */
    virtual ~X11EventFilter();

    /**
     * Returns the type of events to filter.
     */
    int eventType() const { return m_eventType; }

    /**
     * Returns the major opcode of the extension.
     *
     * Only used when the event type is XCB_GE_GENERIC.
     */
    int extension() const { return m_extension; }

    /**
     * Returns the type of generic events to filter.
     *
     * Only used when the event type is XCB_GE_GENERIC.
     */
    int genericEventType() const { return m_genericEventType; }

    /**
     * This method is called for every event of the filtered type.
     *
     * Return true to accept the event and stop further processing, and false otherwise.
     */
    virtual bool event(xcb_generic_event_t *event) = 0;

private:
    int m_eventType;
    int m_extension;
    int m_genericEventType;
};

} // namespace KWin

#endif
