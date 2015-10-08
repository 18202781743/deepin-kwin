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
#ifndef KWIN_SCENE_QPAINTER_X11_BACKEND_H
#define KWIN_SCENE_QPAINTER_X11_BACKEND_H

#include "scene_qpainter.h"

#include <QObject>

namespace KWin
{

class VirtualBackend;

class VirtualQPainterBackend : public QObject, public QPainterBackend
{
    Q_OBJECT
public:
    VirtualQPainterBackend(VirtualBackend *backend);
    virtual ~VirtualQPainterBackend();

    QImage *buffer() override;
    bool needsFullRepaint() const override;
    bool usesOverlayWindow() const override;
    void prepareRenderingFrame() override;
    void present(int mask, const QRegion &damage) override;
    void screenGeometryChanged(const QSize &size) override;
    void renderCursor(QPainter *painter) override;

private:
    QImage m_backBuffer;
    VirtualBackend *m_backend;
    int m_frameCounter = 0;
};

}

#endif
