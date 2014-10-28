/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2014 Martin Gräßlin <mgraesslin@kde.org>

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
#ifndef KWIN_DECORATION_BRIDGE_H
#define KWIN_DECORATION_BRIDGE_H

#include <KDecoration2/Private/DecorationBridge>

#include <QObject>

class KPluginFactory;

namespace KWin
{

class Client;

namespace Decoration
{

class DecorationBridge : public QObject, public KDecoration2::DecorationBridge
{
    Q_OBJECT
public:
    explicit DecorationBridge(QObject *parent);
    virtual ~DecorationBridge();

    void init();
    KDecoration2::Decoration *createDecoration(Client *client);

    std::unique_ptr<KDecoration2::DecoratedClientPrivate> createClient(KDecoration2::DecoratedClient *client, KDecoration2::Decoration *decoration) override;
    std::unique_ptr<KDecoration2::DecorationSettingsPrivate> settings(KDecoration2::DecorationSettings *parent) override;
    void update(KDecoration2::Decoration *decoration, const QRect &geometry) override;

    bool needsBlur() const {
        return m_blur;
    }

    void reconfigure();

    static DecorationBridge *self();
private:
    void loadMetaData(const QJsonObject &object);
    void findTheme(const QVariantMap &map);
    void initPlugin();
    QString readTheme() const;
    KPluginFactory *m_factory;
    bool m_blur;
    QString m_plugin;
    QString m_defaultTheme;
    QString m_theme;
};
} // Decoration
} // KWin

#endif
