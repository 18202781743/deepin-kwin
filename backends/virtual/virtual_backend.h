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
#ifndef KWIN_VIRTUAL_BACKEND_H
#define KWIN_VIRTUAL_BACKEND_H
#include "abstract_backend.h"

#include <kwin_export.h>

#include <QObject>
#include <QSize>

class QTemporaryDir;

namespace KWin
{

class KWIN_EXPORT VirtualBackend : public Platform
{
    Q_OBJECT
    Q_INTERFACES(KWin::Platform)
    Q_PLUGIN_METADATA(IID "org.kde.kwin.Platform" FILE "virtual.json")
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
public:
    VirtualBackend(QObject *parent = nullptr);
    virtual ~VirtualBackend();
    void init() override;

    QSize size() const {
        return m_size;
    }
    int outputCount() const {
        return m_outputCount;
    }

    bool saveFrames() const {
        return !m_screenshotDir.isNull();
    }
    QString screenshotDirPath() const;

    Screens *createScreens(QObject *parent = nullptr) override;
    QPainterBackend* createQPainterBackend() override;
    OpenGLBackend *createOpenGLBackend() override;

    Q_INVOKABLE void setOutputCount(int count) {
        m_outputCount = count;
    }

Q_SIGNALS:
    void sizeChanged();

private:
    QSize m_size;
    int m_outputCount = 1;
    QScopedPointer<QTemporaryDir> m_screenshotDir;
};

}

#endif
