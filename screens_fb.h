/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2015 Martin Gräßlin <mgraesslin@kde.org>

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
#ifndef KWIN_SCREENS_FRAMEBUFFER_H
#define KWIN_SCREENS_FRAMEBUFFER_H
#include "screens.h"

namespace KWin
{
class FramebufferBackend;

class FramebufferScreens : public Screens
{
    Q_OBJECT
public:
    FramebufferScreens(FramebufferBackend *backend, QObject *parent = nullptr);
    virtual ~FramebufferScreens();
    void init() override;
    QRect geometry(int screen) const override;
    int number(const QPoint &pos) const override;
    QSize size(int screen) const override;
    void updateCount() override;

private:
    FramebufferBackend *m_backend;
};

}

#endif
