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
#include "debug_console.h"
#include "client.h"
#include "main.h"
#include "shell_client.h"
#include "unmanaged.h"
#include "wayland_server.h"
#include "workspace.h"

#include "ui_debug_console.h"

// KWayland
#include <KWayland/Server/buffer_interface.h>
#include <KWayland/Server/clientconnection.h>
#include <KWayland/Server/subcompositor_interface.h>
#include <KWayland/Server/surface_interface.h>
// frameworks
#include <KLocalizedString>
#include <NETWM>
// Qt
#include <QMouseEvent>
#include <QMetaProperty>
#include <QMetaType>

namespace KWin
{


static QString tableHeaderRow(const QString &title)
{
    return QStringLiteral("<tr><th colspan=\"2\">%1</th></tr>").arg(title);
}

template<typename T>
static
QString tableRow(const QString &title, const T &argument)
{
    return QStringLiteral("<tr><td>%1</td><td>%2</td></tr>").arg(title).arg(argument);
}

static QString timestampRow(quint32 timestamp)
{
    return tableRow(i18n("Timestamp"), timestamp);
}

static QString buttonToString(Qt::MouseButton button)
{
    switch (button) {
        case Qt::LeftButton:
            return i18nc("A mouse button", "Left");
        case Qt::RightButton:
            return i18nc("A mouse button", "Right");
        case Qt::MiddleButton:
            return i18nc("A mouse button", "Middle");
        case Qt::BackButton:
            return i18nc("A mouse button", "Back");
        case Qt::ForwardButton:
            return i18nc("A mouse button", "Forward");
        case Qt::TaskButton:
            return i18nc("A mouse button", "Task");
        case Qt::ExtraButton4:
            return i18nc("A mouse button", "Extra Button 4");
        case Qt::ExtraButton5:
            return i18nc("A mouse button", "Extra Button 5");
        case Qt::ExtraButton6:
            return i18nc("A mouse button", "Extra Button 6");
        case Qt::ExtraButton7:
            return i18nc("A mouse button", "Extra Button 7");
        case Qt::ExtraButton8:
            return i18nc("A mouse button", "Extra Button 8");
        case Qt::ExtraButton9:
            return i18nc("A mouse button", "Extra Button 9");
        case Qt::ExtraButton10:
            return i18nc("A mouse button", "Extra Button 10");
        case Qt::ExtraButton11:
            return i18nc("A mouse button", "Extra Button 11");
        case Qt::ExtraButton12:
            return i18nc("A mouse button", "Extra Button 12");
        case Qt::ExtraButton13:
            return i18nc("A mouse button", "Extra Button 13");
        case Qt::ExtraButton14:
            return i18nc("A mouse button", "Extra Button 14");
        case Qt::ExtraButton15:
            return i18nc("A mouse button", "Extra Button 15");
        case Qt::ExtraButton16:
            return i18nc("A mouse button", "Extra Button 16");
        case Qt::ExtraButton17:
            return i18nc("A mouse button", "Extra Button 17");
        case Qt::ExtraButton18:
            return i18nc("A mouse button", "Extra Button 18");
        case Qt::ExtraButton19:
            return i18nc("A mouse button", "Extra Button 19");
        case Qt::ExtraButton20:
            return i18nc("A mouse button", "Extra Button 20");
        case Qt::ExtraButton21:
            return i18nc("A mouse button", "Extra Button 21");
        case Qt::ExtraButton22:
            return i18nc("A mouse button", "Extra Button 22");
        case Qt::ExtraButton23:
            return i18nc("A mouse button", "Extra Button 23");
        case Qt::ExtraButton24:
            return i18nc("A mouse button", "Extra Button 24");
    default:
        return QString();
    }
}

static QString buttonsToString(Qt::MouseButtons buttons)
{
    QString ret;
    for (uint i = 1; i < Qt::ExtraButton24; i = i << 1) {
        if (buttons & i) {
            ret.append(buttonToString(Qt::MouseButton(uint(buttons) & i)));
            ret.append(QStringLiteral(" "));
        }
    };
    return ret;
}

static const QString s_hr = QStringLiteral("<hr/>");
static const QString s_tableStart = QStringLiteral("<table>");
static const QString s_tableEnd = QStringLiteral("</table>");

DebugConsoleFilter::DebugConsoleFilter(QTextEdit *textEdit)
    : InputEventFilter()
    , m_textEdit(textEdit)
{
}

DebugConsoleFilter::~DebugConsoleFilter() = default;

bool DebugConsoleFilter::pointerEvent(QMouseEvent *event, quint32 nativeButton)
{
    QString text = s_hr;
    const QString timestamp = timestampRow(event->timestamp());

    text.append(s_tableStart);
    switch (event->type()) {
    case QEvent::MouseMove:
        text.append(tableHeaderRow(i18nc("A mouse pointer motion event", "Pointer Motion")));
        text.append(timestamp);
        text.append(tableRow(i18nc("The global mouse pointer position", "Global Position"), QStringLiteral("%1/%2").arg(event->pos().x()).arg(event->pos().y())));
        break;
    case QEvent::MouseButtonPress:
        text.append(tableHeaderRow(i18nc("A mouse pointer button press event", "Pointer Button Press")));
        text.append(timestamp);
        text.append(tableRow(i18nc("A button in a mouse press/release event", "Button"), buttonToString(event->button())));
        text.append(tableRow(i18nc("A button in a mouse press/release event",  "Native Button code"), nativeButton));
        text.append(tableRow(i18nc("All currently pressed buttons in a mouse press/release event", "Pressed Buttons"), buttonsToString(event->buttons())));
        break;
    case QEvent::MouseButtonRelease:
        text.append(tableHeaderRow(i18nc("A mouse pointer button release event", "Pointer Button Release")));
        text.append(timestamp);
        text.append(tableRow(i18nc("A button in a mouse press/release event", "Button"), buttonToString(event->button())));
        text.append(tableRow(i18nc("A button in a mouse press/release event", "Native Button code"), nativeButton));
        text.append(tableRow(i18nc("All currently pressed buttons in a mouse press/release event", "Pressed Buttons"), buttonsToString(event->buttons())));
        break;
    default:
        break;
    }
    text.append(s_tableEnd);

    m_textEdit->insertHtml(text);
    m_textEdit->ensureCursorVisible();
    return false;
}

bool DebugConsoleFilter::wheelEvent(QWheelEvent *event)
{
    QString text = s_hr;
    text.append(s_tableStart);
    text.append(tableHeaderRow(i18nc("A mouse pointer axis (wheel) event", "Pointer Axis")));
    text.append(timestampRow(event->timestamp()));
    const Qt::Orientation orientation = event->angleDelta().x() == 0 ? Qt::Vertical : Qt::Horizontal;
    text.append(tableRow(i18nc("The orientation of a pointer axis event", "Orientation"),
                         orientation == Qt::Horizontal ? i18nc("An orientation of a pointer axis event", "Horizontal")
                                                       : i18nc("An orientation of a pointer axis event", "Vertical")));
    text.append(tableRow(QStringLiteral("Delta"), orientation == Qt::Horizontal ? event->angleDelta().x() : event->angleDelta().y()));
    text.append(s_tableEnd);

    m_textEdit->insertHtml(text);
    m_textEdit->ensureCursorVisible();
    return false;
}

bool DebugConsoleFilter::keyEvent(QKeyEvent *event)
{
    QString text = s_hr;
    text.append(s_tableStart);

    switch (event->type()) {
    case QEvent::KeyPress:
        text.append(tableHeaderRow(i18nc("A key press event", "Key Press")));
        break;
    case QEvent::KeyRelease:
        text.append(tableHeaderRow(i18nc("A key release event", "Key Release")));
        break;
    default:
        break;
    }
    auto modifiersToString = [event] {
        QString ret;
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            ret.append(i18nc("A keyboard modifier", "Shift"));
            ret.append(QStringLiteral(" "));
        }
        if (event->modifiers().testFlag(Qt::ControlModifier)) {
            ret.append(i18nc("A keyboard modifier", "Control"));
            ret.append(QStringLiteral(" "));
        }
        if (event->modifiers().testFlag(Qt::AltModifier)) {
            ret.append(i18nc("A keyboard modifier", "Alt"));
            ret.append(QStringLiteral(" "));
        }
        if (event->modifiers().testFlag(Qt::MetaModifier)) {
            ret.append(i18nc("A keyboard modifier", "Meta"));
            ret.append(QStringLiteral(" "));
        }
        if (event->modifiers().testFlag(Qt::KeypadModifier)) {
            ret.append(i18nc("A keyboard modifier", "Keypad"));
            ret.append(QStringLiteral(" "));
        }
        if (event->modifiers().testFlag(Qt::GroupSwitchModifier)) {
            ret.append(i18nc("A keyboard modifier", "Group-switch"));
            ret.append(QStringLiteral(" "));
        }
        return ret;
    };
    text.append(timestampRow(event->timestamp()));
    text.append(tableRow(i18nc("Whether the event is an automatic key repeat", "Repeat"), event->isAutoRepeat()));
    text.append(tableRow(i18nc("The code as read from the input device", "Scan code"), event->nativeScanCode()));
    text.append(tableRow(i18nc("The translated code to an Xkb symbol", "Xkb symbol"), event->nativeVirtualKey()));
    text.append(tableRow(i18nc("The translated code interpreted as text", "Utf8"), event->text()));
    text.append(tableRow(i18nc("The currently active modifiers", "Modifiers"), modifiersToString()));

    text.append(s_tableEnd);

    m_textEdit->insertHtml(text);
    m_textEdit->ensureCursorVisible();
    return false;
}

bool DebugConsoleFilter::touchDown(quint32 id, const QPointF &pos, quint32 time)
{
    QString text = s_hr;
    text.append(s_tableStart);
    text.append(tableHeaderRow(i18nc("A touch down event", "Touch down")));
    text.append(timestampRow(time));
    text.append(tableRow(i18nc("The id of the touch point in the touch event", "Point identifier"), id));
    text.append(tableRow(i18nc("The global position of the touch point", "Global position"),
                         QStringLiteral("%1/%2").arg(pos.x()).arg(pos.y())));
    text.append(s_tableEnd);

    m_textEdit->insertHtml(text);
    m_textEdit->ensureCursorVisible();
    return false;
}

bool DebugConsoleFilter::touchMotion(quint32 id, const QPointF &pos, quint32 time)
{
    QString text = s_hr;
    text.append(s_tableStart);
    text.append(tableHeaderRow(i18nc("A touch motion event", "Touch Motion")));
    text.append(timestampRow(time));
    text.append(tableRow(i18nc("The id of the touch point in the touch event", "Point identifier"), id));
    text.append(tableRow(i18nc("The global position of the touch point", "Global position"),
                         QStringLiteral("%1/%2").arg(pos.x()).arg(pos.y())));
    text.append(s_tableEnd);

    m_textEdit->insertHtml(text);
    m_textEdit->ensureCursorVisible();
    return false;
}

bool DebugConsoleFilter::touchUp(quint32 id, quint32 time)
{
    QString text = s_hr;
    text.append(s_tableStart);
    text.append(tableHeaderRow(i18nc("A touch up event", "Touch Up")));
    text.append(timestampRow(time));
    text.append(tableRow(i18nc("The id of the touch point in the touch event", "Point identifier"), id));
    text.append(s_tableEnd);

    m_textEdit->insertHtml(text);
    m_textEdit->ensureCursorVisible();
    return false;
}

DebugConsole::DebugConsole()
    : QWidget()
    , m_ui(new Ui::DebugConsole)
{
    m_ui->setupUi(this);
    m_ui->treeView->setItemDelegate(new DebugConsoleDelegate(this));
    m_ui->treeView->setModel(new DebugConsoleModel(this));
    m_ui->quitButton->setIcon(QIcon::fromTheme(QStringLiteral("application-exit")));
    m_ui->windowsButton->setIcon(QIcon::fromTheme(QStringLiteral("view-list-tree")));
    m_ui->surfacesButton->setIcon(QIcon::fromTheme(QStringLiteral("view-list-tree")));

    if (kwinApp()->operationMode() == Application::OperationMode::OperationModeX11) {
        m_ui->surfacesButton->setDisabled(true);
        m_ui->inputEventsButton->setDisabled(true);
    }

    connect(m_ui->quitButton, &QAbstractButton::clicked, this, &DebugConsole::deleteLater);
    connect(m_ui->windowsButton, &QAbstractButton::toggled, this,
        [this] (bool toggled) {
            if (!toggled) {
                return;
            }
            m_ui->surfacesButton->setChecked(false);
            m_ui->inputEventsButton->setChecked(false);
            m_ui->treeView->model()->deleteLater();
            m_ui->treeView->setModel(new DebugConsoleModel(this));
            m_ui->treeView->setVisible(true);
            m_ui->inputTextEdit->setVisible(false);
        }
    );
    connect(m_ui->surfacesButton, &QAbstractButton::toggled, this,
        [this] (bool toggled) {
            if (!toggled) {
                return;
            }
            m_ui->windowsButton->setChecked(false);
            m_ui->inputEventsButton->setChecked(false);
            m_ui->treeView->model()->deleteLater();
            m_ui->treeView->setModel(new SurfaceTreeModel(this));
            m_ui->treeView->setVisible(true);
            m_ui->inputTextEdit->setVisible(false);
        }
    );
    connect(m_ui->inputEventsButton, &QAbstractButton::toggled, this,
        [this] (bool toggled) {
            if (!toggled) {
                return;
            }
            m_ui->windowsButton->setChecked(false);
            m_ui->surfacesButton->setChecked(false);
            m_ui->treeView->setVisible(false);
            m_ui->inputTextEdit->setVisible(true);
            if (m_inputFilter.isNull()) {
                m_inputFilter.reset(new DebugConsoleFilter(m_ui->inputTextEdit));
                input()->prepandInputEventFilter(m_inputFilter.data());
            }
        }
    );

    m_ui->inputTextEdit->setVisible(false);

    // for X11
    setWindowFlags(Qt::X11BypassWindowManagerHint);
}

DebugConsole::~DebugConsole() = default;

DebugConsoleDelegate::DebugConsoleDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

DebugConsoleDelegate::~DebugConsoleDelegate() = default;

QString DebugConsoleDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    switch (value.type()) {
    case QMetaType::QPoint: {
        const QPoint p = value.toPoint();
        return QStringLiteral("%1,%2").arg(p.x()).arg(p.y());
    }
    case QMetaType::QPointF: {
        const QPointF p = value.toPointF();
        return QStringLiteral("%1,%2").arg(p.x()).arg(p.y());
    }
    case QMetaType::QSize: {
        const QSize s = value.toSize();
        return QStringLiteral("%1x%2").arg(s.width()).arg(s.height());
    }
    case QMetaType::QSizeF: {
        const QSizeF s = value.toSizeF();
        return QStringLiteral("%1x%2").arg(s.width()).arg(s.height());
    }
    case QMetaType::QRect: {
        const QRect r = value.toRect();
        return QStringLiteral("%1,%2 %3x%4").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
    }
    default:
        if (value.userType() == qMetaTypeId<KWayland::Server::SurfaceInterface*>()) {
            if (auto s = value.value<KWayland::Server::SurfaceInterface*>()) {
                return QStringLiteral("KWayland::Server::SurfaceInterface(0x%1)").arg(qulonglong(s), 0, 16);
            } else {
                return QStringLiteral("nullptr");
            }
        }
        break;
    }
    return QStyledItemDelegate::displayText(value, locale);
}

static const int s_x11ClientId = 1;
static const int s_x11UnmanagedId = 2;
static const int s_waylandClientId = 3;
static const int s_waylandInternalId = 4;
static const quint32 s_propertyBitMask = 0xFFFF0000;
static const quint32 s_clientBitMask   = 0x0000FFFF;
static const quint32 s_idDistance = 10000;

template <class T>
void DebugConsoleModel::add(int parentRow, QVector<T*> &clients, T *client)
{
    beginInsertRows(index(parentRow, 0, QModelIndex()), clients.count(), clients.count());
    clients.append(client);
    endInsertRows();
}

template <class T>
void DebugConsoleModel::remove(int parentRow, QVector<T*> &clients, T *client)
{
    const int remove = clients.indexOf(client);
    if (remove == -1) {
        return;
    }
    beginRemoveRows(index(parentRow, 0, QModelIndex()), remove, remove);
    clients.removeAt(remove);
    endRemoveRows();
}

DebugConsoleModel::DebugConsoleModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    if (waylandServer()) {
        const auto clients = waylandServer()->clients();
        for (auto c : clients) {
            m_shellClients.append(c);
        }
        const auto internals = waylandServer()->internalClients();
        for (auto c : internals) {
            m_internalClients.append(c);
        }
        // TODO: that only includes windows getting shown, not those which are only created
        connect(waylandServer(), &WaylandServer::shellClientAdded, this,
            [this] (ShellClient *c) {
                if (c->isInternal()) {
                    add(s_waylandInternalId -1, m_internalClients, c);
                } else {
                    add(s_waylandClientId -1, m_shellClients, c);
                }
            }
        );
        connect(waylandServer(), &WaylandServer::shellClientRemoved, this,
            [this] (ShellClient *c) {
                remove(s_waylandInternalId -1, m_internalClients, c);
                remove(s_waylandClientId -1, m_shellClients, c);
            }
        );
    }
    const auto x11Clients = workspace()->clientList();
    for (auto c : x11Clients) {
        m_x11Clients.append(c);
    }
    const auto x11DesktopClients = workspace()->desktopList();
    for (auto c : x11DesktopClients) {
        m_x11Clients.append(c);
    }
    connect(workspace(), &Workspace::clientAdded, this,
        [this] (Client *c) {
            add(s_x11ClientId -1, m_x11Clients, c);
        }
    );
    connect(workspace(), &Workspace::clientRemoved, this,
        [this] (AbstractClient *ac) {
            Client *c = qobject_cast<Client*>(ac);
            if (!c) {
                return;
            }
            remove(s_x11ClientId -1, m_x11Clients, c);
        }
    );

    const auto unmangeds = workspace()->unmanagedList();
    for (auto u : unmangeds) {
        m_unmanageds.append(u);
    }
    connect(workspace(), &Workspace::unmanagedAdded, this,
        [this] (Unmanaged *u) {
            add(s_x11UnmanagedId -1, m_unmanageds, u);
        }
    );
    connect(workspace(), &Workspace::unmanagedRemoved, this,
        [this] (Unmanaged *u) {
            remove(s_x11UnmanagedId -1, m_unmanageds, u);
        }
    );
}

DebugConsoleModel::~DebugConsoleModel() = default;

int DebugConsoleModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

int DebugConsoleModel::topLevelRowCount() const
{
    return kwinApp()->shouldUseWaylandForCompositing() ? 4 : 2;
}

template <class T>
int DebugConsoleModel::propertyCount(const QModelIndex &parent, T *(DebugConsoleModel::*filter)(const QModelIndex&) const) const
{
    if (T *t = (this->*filter)(parent)) {
        return t->metaObject()->propertyCount();
    }
    return 0;
}

int DebugConsoleModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return topLevelRowCount();
    }

    switch (parent.internalId()) {
    case s_x11ClientId:
        return m_x11Clients.count();
    case s_x11UnmanagedId:
        return m_unmanageds.count();
    case s_waylandClientId:
        return m_shellClients.count();
    case s_waylandInternalId:
        return m_internalClients.count();
    default:
        break;
    }

    if (parent.internalId() & s_propertyBitMask) {
        // properties do not have children
        return 0;
    }

    if (parent.internalId() < s_idDistance * (s_x11ClientId + 1)) {
        return propertyCount(parent, &DebugConsoleModel::x11Client);
    } else if (parent.internalId() < s_idDistance * (s_x11UnmanagedId + 1)) {
        return propertyCount(parent, &DebugConsoleModel::unmanaged);
    } else if (parent.internalId() < s_idDistance * (s_waylandClientId + 1)) {
        return propertyCount(parent, &DebugConsoleModel::shellClient);
    } else if (parent.internalId() < s_idDistance * (s_waylandInternalId + 1)) {
        return propertyCount(parent, &DebugConsoleModel::internalClient);
    }

    return 0;
}

template <class T>
QModelIndex DebugConsoleModel::indexForClient(int row, int column, const QVector<T*> &clients, int id) const
{
    if (column != 0) {
        return QModelIndex();
    }
    if (row >= clients.count()) {
        return QModelIndex();
    }
    return createIndex(row, column, s_idDistance * id + row);
}

template <class T>
QModelIndex DebugConsoleModel::indexForProperty(int row, int column, const QModelIndex &parent, T *(DebugConsoleModel::*filter)(const QModelIndex&) const) const
{
    if (T *t = (this->*filter)(parent)) {
        if (row >= t->metaObject()->propertyCount()) {
            return QModelIndex();
        }
        return createIndex(row, column, quint32(row + 1) << 16 | parent.internalId());
    }
    return QModelIndex();
}

QModelIndex DebugConsoleModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        // index for a top level item
        if (column != 0 || row >= topLevelRowCount()) {
            return QModelIndex();
        }
        return createIndex(row, column, row + 1);
    }
    if (column >= 2) {
        // max of 2 columns
        return QModelIndex();
    }
    // index for a client (second level)
    switch (parent.internalId()) {
    case s_x11ClientId:
        return indexForClient(row, column, m_x11Clients, s_x11ClientId);
    case s_x11UnmanagedId:
        return indexForClient(row, column, m_unmanageds, s_x11UnmanagedId);
    case s_waylandClientId:
        return indexForClient(row, column, m_shellClients, s_waylandClientId);
    case s_waylandInternalId:
        return indexForClient(row, column, m_internalClients, s_waylandInternalId);
    default:
        break;
    }

    // index for a property (third level)
    if (parent.internalId() < s_idDistance * (s_x11ClientId + 1)) {
        return indexForProperty(row, column, parent, &DebugConsoleModel::x11Client);
    } else if (parent.internalId() < s_idDistance * (s_x11UnmanagedId + 1)) {
        return indexForProperty(row, column, parent, &DebugConsoleModel::unmanaged);
    } else if (parent.internalId() < s_idDistance * (s_waylandClientId + 1)) {
        return indexForProperty(row, column, parent, &DebugConsoleModel::shellClient);
    } else if (parent.internalId() < s_idDistance * (s_waylandInternalId + 1)) {
        return indexForProperty(row, column, parent, &DebugConsoleModel::internalClient);
    }

    return QModelIndex();
}

QModelIndex DebugConsoleModel::parent(const QModelIndex &child) const
{
    if (child.internalId() <= s_waylandInternalId) {
        return QModelIndex();
    }
    if (child.internalId() & s_propertyBitMask) {
        // a property
        const quint32 parentId = child.internalId() & s_clientBitMask;
        if (parentId < s_idDistance * (s_x11ClientId + 1)) {
            return createIndex(parentId - (s_idDistance * s_x11ClientId), 0, parentId);
        } else if (parentId < s_idDistance * (s_x11UnmanagedId + 1)) {
            return createIndex(parentId - (s_idDistance * s_x11UnmanagedId), 0, parentId);
        } else if (parentId < s_idDistance * (s_waylandClientId + 1)) {
            return createIndex(parentId - (s_idDistance * s_waylandClientId), 0, parentId);
        } else if (parentId < s_idDistance * (s_waylandInternalId + 1)) {
            return createIndex(parentId - (s_idDistance * s_waylandInternalId), 0, parentId);
        }
        return QModelIndex();
    }
    if (child.internalId() < s_idDistance * (s_x11ClientId + 1)) {
        return createIndex(s_x11ClientId -1, 0, s_x11ClientId);
    } else if (child.internalId() < s_idDistance * (s_x11UnmanagedId + 1)) {
        return createIndex(s_x11UnmanagedId -1, 0, s_x11UnmanagedId);
    } else if (child.internalId() < s_idDistance * (s_waylandClientId + 1)) {
        return createIndex(s_waylandClientId -1, 0, s_waylandClientId);
    } else if (child.internalId() < s_idDistance * (s_waylandInternalId + 1)) {
        return createIndex(s_waylandInternalId -1, 0, s_waylandInternalId);
    }
    return QModelIndex();
}

QVariant DebugConsoleModel::propertyData(QObject *object, const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    const auto property = object->metaObject()->property(index.row());
    if (index.column() == 0) {
        return property.name();
    } else {
        const QVariant value = property.read(object);
        if (qstrcmp(property.name(), "windowType") == 0) {
            switch (value.toInt()) {
            case NET::Normal:
                return QStringLiteral("NET::Normal");
            case NET::Desktop:
                return QStringLiteral("NET::Desktop");
            case NET::Dock:
                return QStringLiteral("NET::Dock");
            case NET::Toolbar:
                return QStringLiteral("NET::Toolbar");
            case NET::Menu:
                return QStringLiteral("NET::Menu");
            case NET::Dialog:
                return QStringLiteral("NET::Dialog");
            case NET::Override:
                return QStringLiteral("NET::Override");
            case NET::TopMenu:
                return QStringLiteral("NET::TopMenu");
            case NET::Utility:
                return QStringLiteral("NET::Utility");
            case NET::Splash:
                return QStringLiteral("NET::Splash");
            case NET::DropdownMenu:
                return QStringLiteral("NET::DropdownMenu");
            case NET::PopupMenu:
                return QStringLiteral("NET::PopupMenu");
            case NET::Tooltip:
                return QStringLiteral("NET::Tooltip");
            case NET::Notification:
                return QStringLiteral("NET::Notification");
            case NET::ComboBox:
                return QStringLiteral("NET::ComboBox");
            case NET::DNDIcon:
                return QStringLiteral("NET::DNDIcon");
            case NET::OnScreenDisplay:
                return QStringLiteral("NET::OnScreenDisplay");
            case NET::Unknown:
            default:
                return QStringLiteral("NET::Unknown");
            }
        }
        return value;
    }
    return QVariant();
}

template <class T>
QVariant DebugConsoleModel::clientData(const QModelIndex &index, int role, const QVector<T*> clients) const
{
    if (index.row() >= clients.count()) {
        return QVariant();
    }
    auto c = clients.at(index.row());
    if (role == Qt::DisplayRole) {
        return QStringLiteral("%1: %2").arg(c->window()).arg(c->caption());
    } else if (role == Qt::DecorationRole) {
        return c->icon();
    }
    return QVariant();
}

QVariant DebugConsoleModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (!index.parent().isValid()) {
        // one of the top levels
        if (index.column() != 0 || role != Qt::DisplayRole) {
            return QVariant();
        }
        switch (index.internalId()) {
        case s_x11ClientId:
            return i18n("X11 Client Windows");
        case s_x11UnmanagedId:
            return i18n("X11 Unmanaged Windows");
        case s_waylandClientId:
            return i18n("Wayland Windows");
        case s_waylandInternalId:
            return i18n("Internal Windows");
        default:
            return QVariant();
        }
    }
    if (index.internalId() & s_propertyBitMask) {
        if (index.column() >= 2 || role != Qt::DisplayRole) {
            return QVariant();
        }
        if (ShellClient *c = shellClient(index)) {
            return propertyData(c, index, role);
        } else if (ShellClient *c = internalClient(index)) {
            return propertyData(c, index, role);
        } else if (Client *c = x11Client(index)) {
            return propertyData(c, index, role);
        } else if (Unmanaged *u = unmanaged(index)) {
            return propertyData(u, index, role);
        }
    } else {
        if (index.column() != 0) {
            return QVariant();
        }
        switch (index.parent().internalId()) {
        case s_x11ClientId:
            return clientData(index, role, m_x11Clients);
        case s_x11UnmanagedId: {
            if (index.row() >= m_unmanageds.count()) {
                return QVariant();
            }
            auto u = m_unmanageds.at(index.row());
            if (role == Qt::DisplayRole) {
                return u->window();
            }
            break;
        }
        case s_waylandClientId:
            return clientData(index, role, m_shellClients);
        case s_waylandInternalId:
            return clientData(index, role, m_internalClients);
        default:
            break;
        }
    }

    return QVariant();
}

template<class T>
static T *clientForIndex(const QModelIndex &index, const QVector<T*> &clients, int id)
{
    const qint32 row = (index.internalId() & s_clientBitMask) - (s_idDistance * id);
    if (row < 0 || row >= clients.count()) {
        return nullptr;
    }
    return clients.at(row);
}

ShellClient *DebugConsoleModel::shellClient(const QModelIndex &index) const
{
    return clientForIndex(index, m_shellClients, s_waylandClientId);
}

ShellClient *DebugConsoleModel::internalClient(const QModelIndex &index) const
{
    return clientForIndex(index, m_internalClients, s_waylandInternalId);
}

Client *DebugConsoleModel::x11Client(const QModelIndex &index) const
{
    return clientForIndex(index, m_x11Clients, s_x11ClientId);
}

Unmanaged *DebugConsoleModel::unmanaged(const QModelIndex &index) const
{
    return clientForIndex(index, m_unmanageds, s_x11UnmanagedId);
}

/////////////////////////////////////// SurfaceTreeModel
SurfaceTreeModel::SurfaceTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    // TODO: it would be nice to not have to reset the model on each change
    auto reset = [this] {
        beginResetModel();
        endResetModel();
    };
    using namespace KWayland::Server;

    const auto unmangeds = workspace()->unmanagedList();
    for (auto u : unmangeds) {
        connect(u->surface(), &SurfaceInterface::subSurfaceTreeChanged, this, reset);
    }
    for (auto c : workspace()->allClientList()) {
        connect(c->surface(), &SurfaceInterface::subSurfaceTreeChanged, this, reset);
    }
    for (auto c : workspace()->desktopList()) {
        connect(c->surface(), &SurfaceInterface::subSurfaceTreeChanged, this, reset);
    }
    for (auto c : waylandServer()->internalClients()) {
        connect(c->surface(), &SurfaceInterface::subSurfaceTreeChanged, this, reset);
    }
    connect(waylandServer(), &WaylandServer::shellClientAdded, this,
        [this, reset] (ShellClient *c) {
            connect(c->surface(), &SurfaceInterface::subSurfaceTreeChanged, this, reset);
            reset();
        }
    );
    connect(workspace(), &Workspace::clientAdded, this,
        [this, reset] (AbstractClient *c) {
            connect(c->surface(), &SurfaceInterface::subSurfaceTreeChanged, this, reset);
            reset();
        }
    );
    connect(workspace(), &Workspace::clientRemoved, this, reset);
    connect(workspace(), &Workspace::unmanagedAdded, this,
        [this, reset] (Unmanaged *u) {
            connect(u->surface(), &SurfaceInterface::subSurfaceTreeChanged, this, reset);
            reset();
        }
    );
    connect(workspace(), &Workspace::unmanagedRemoved, this, reset);
}

SurfaceTreeModel::~SurfaceTreeModel() = default;

int SurfaceTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

int SurfaceTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        using namespace KWayland::Server;
        if (SurfaceInterface *surface = static_cast<SurfaceInterface*>(parent.internalPointer())) {
            const auto &children = surface->childSubSurfaces();
            return children.count();
        }
        return 0;
    }
    // toplevel are all windows
    return workspace()->allClientList().count() +
           workspace()->desktopList().count() +
           workspace()->unmanagedList().count() +
           waylandServer()->internalClients().count();
}

QModelIndex SurfaceTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (column != 0) {
        // invalid column
        return QModelIndex();
    }

    if (parent.isValid()) {
        using namespace KWayland::Server;
        if (SurfaceInterface *surface = static_cast<SurfaceInterface*>(parent.internalPointer())) {
            const auto &children = surface->childSubSurfaces();
            if (row < children.count()) {
                return createIndex(row, column, children.at(row)->surface().data());
            }
        }
        return QModelIndex();
    }
    // a window
    const auto &allClients = workspace()->allClientList();
    if (row < allClients.count()) {
        // references a client
        return createIndex(row, column, allClients.at(row)->surface());
    }
    int reference = allClients.count();
    const auto &desktopClients = workspace()->desktopList();
    if (row < reference + desktopClients.count()) {
        return createIndex(row, column, desktopClients.at(row-reference)->surface());
    }
    reference += desktopClients.count();
    const auto &unmanaged = workspace()->unmanagedList();
    if (row < reference + unmanaged.count()) {
        return createIndex(row, column, unmanaged.at(row-reference)->surface());
    }
    reference += unmanaged.count();
    const auto &internal = waylandServer()->internalClients();
    if (row < reference + internal.count()) {
        return createIndex(row, column, internal.at(row-reference)->surface());
    }
    // not found
    return QModelIndex();
}

QModelIndex SurfaceTreeModel::parent(const QModelIndex &child) const
{
    using namespace KWayland::Server;
    if (SurfaceInterface *surface = static_cast<SurfaceInterface*>(child.internalPointer())) {
        const auto &subsurface = surface->subSurface();
        if (subsurface.isNull()) {
            // doesn't reference a subsurface, this is a top-level window
            return QModelIndex();
        }
        SurfaceInterface *parent = subsurface->parentSurface().data();
        if (!parent) {
            // something is wrong
            return QModelIndex();
        }
        // is the parent a subsurface itself?
        if (parent->subSurface()) {
            auto grandParent = parent->subSurface()->parentSurface();
            if (grandParent.isNull()) {
                // something is wrong
                return QModelIndex();
            }
            const auto &children = grandParent->childSubSurfaces();
            for (int row = 0; row < children.count(); row++) {
                if (children.at(row).data() == parent->subSurface().data()) {
                    return createIndex(row, 0, parent);
                }
            }
            return QModelIndex();
        }
        // not a subsurface, thus it's a true window
        int row = 0;
        const auto &allClients = workspace()->allClientList();
        for (; row < allClients.count(); row++) {
            if (allClients.at(row)->surface() == parent) {
                return createIndex(row, 0, parent);
            }
        }
        row = allClients.count();
        const auto &desktopClients = workspace()->desktopList();
        for (int i = 0; i < desktopClients.count(); i++) {
            if (desktopClients.at(i)->surface() == parent) {
                return createIndex(row + i, 0, parent);
            }
        }
        row += desktopClients.count();
        const auto &unmanaged = workspace()->unmanagedList();
        for (int i = 0; i < unmanaged.count(); i++) {
            if (unmanaged.at(i)->surface() == parent) {
                return createIndex(row + i, 0, parent);
            }
        }
        row += unmanaged.count();
        const auto &internal = waylandServer()->internalClients();
        for (int i = 0; i < internal.count(); i++) {
            if (internal.at(i)->surface() == parent) {
                return createIndex(row + i, 0, parent);
            }
        }
    }
    return QModelIndex();
}

QVariant SurfaceTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    using namespace KWayland::Server;
    if (SurfaceInterface *surface = static_cast<SurfaceInterface*>(index.internalPointer())) {
        if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
            return QStringLiteral("%1 (%2) - %3").arg(surface->client()->executablePath())
                                                .arg(surface->client()->processId())
                                                .arg(surface->id());
        } else if (role == Qt::DecorationRole) {
            if (auto buffer = surface->buffer()) {
                if (buffer->shmBuffer()) {
                    return buffer->data().scaled(QSize(64, 64), Qt::KeepAspectRatio);
                }
            }
        }
    }
    return QVariant();
}

}
