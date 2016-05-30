/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2016 Martin Gräßlin <mgraesslin@kde.org>

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
#ifndef KWIN_INPUT_EVENT_H
#define KWIN_INPUT_EVENT_H
#include <QInputEvent>

namespace KWin
{

namespace LibInput
{
class Device;
}

class MouseEvent : public QMouseEvent
{
public:
    explicit MouseEvent(QEvent::Type type, const QPointF &pos, Qt::MouseButton button, Qt::MouseButtons buttons,
                        Qt::KeyboardModifiers modifiers, quint32 timestamp, LibInput::Device *device);

    LibInput::Device *device() const {
        return m_device;
    }

private:
    LibInput::Device *m_device;
};

class WheelEvent : public QWheelEvent
{
public:
    explicit WheelEvent(const QPointF &pos, qreal delta, Qt::Orientation orientation, Qt::MouseButtons buttons,
                        Qt::KeyboardModifiers modifiers, quint32 timestamp, LibInput::Device *device);

    LibInput::Device *device() const {
        return m_device;
    }

private:
    LibInput::Device *m_device;
};

class KeyEvent : public QKeyEvent
{
public:
    explicit KeyEvent(QEvent::Type type, Qt::Key key, Qt::KeyboardModifiers modifiers, quint32 code, quint32 keysym,
                      const QString &text, bool autorepeat, quint32 timestamp, LibInput::Device *device);

    LibInput::Device *device() const {
        return m_device;
    }

private:
    LibInput::Device *m_device;
};

}

#endif
