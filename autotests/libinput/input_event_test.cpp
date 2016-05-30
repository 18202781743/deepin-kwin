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
#include "mock_libinput.h"
#include "../../libinput/device.h"
#include "../input_event.h"

#include <QtTest/QtTest>

using namespace KWin;
using namespace KWin::LibInput;

class InputEventsTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testInitMouseEvent_data();
    void testInitMouseEvent();
    void testInitKeyEvent_data();
    void testInitKeyEvent();
    void testInitWheelEvent_data();
    void testInitWheelEvent();
};

void InputEventsTest::testInitMouseEvent_data()
{
    QTest::addColumn<QEvent::Type>("type");

    QTest::newRow("Press") << QEvent::MouseButtonPress;
    QTest::newRow("Release") << QEvent::MouseButtonRelease;
    QTest::newRow("Move") << QEvent::MouseMove;
}

void InputEventsTest::testInitMouseEvent()
{
    // this test verifies that a MouseEvent is constructed correctly

    // first create the test LibInput::Device
    libinput_device device;
    Device d(&device);

    QFETCH(QEvent::Type, type);
    // now create our own event
    MouseEvent event(type, QPointF(100, 200), Qt::LeftButton, Qt::LeftButton | Qt::RightButton,
                     Qt::ShiftModifier | Qt::ControlModifier, 300, &d);
    // and verify the contract of QMouseEvent
    QCOMPARE(event.type(), type);
    QCOMPARE(event.globalPos(), QPoint(100, 200));
    QCOMPARE(event.screenPos(), QPointF(100, 200));
    QCOMPARE(event.localPos(), QPointF(100, 200));
    QCOMPARE(event.button(), Qt::LeftButton);
    QCOMPARE(event.buttons(), Qt::LeftButton | Qt::RightButton);
    QCOMPARE(event.modifiers(), Qt::ShiftModifier | Qt::ControlModifier);
    QCOMPARE(event.timestamp(), 300ul);
    // and our custom argument
    QCOMPARE(event.device(), &d);
}

void InputEventsTest::testInitKeyEvent_data()
{
    QTest::addColumn<QEvent::Type>("type");
    QTest::addColumn<bool>("autorepeat");

    QTest::newRow("Press") << QEvent::KeyPress << false;
    QTest::newRow("Repeat") << QEvent::KeyPress << true;
    QTest::newRow("Release") << QEvent::KeyRelease << false;
}

void InputEventsTest::testInitKeyEvent()
{
    // this test verifies that a KeyEvent is constructed correctly

    // first create the test LibInput::Device
    libinput_device device;
    Device d(&device);

    // setup event
    QFETCH(QEvent::Type, type);
    QFETCH(bool, autorepeat);
    KeyEvent event(type, Qt::Key_Space, Qt::ShiftModifier | Qt::ControlModifier, 200, 300,
                   QStringLiteral(" "), autorepeat, 400, &d);
    // and verify the contract of QKeyEvent
    QCOMPARE(event.type(), type);
    QCOMPARE(event.isAutoRepeat(), autorepeat);
    QCOMPARE(event.key(), int(Qt::Key_Space));
    QCOMPARE(event.nativeScanCode(), 200u);
    QCOMPARE(event.nativeVirtualKey(), 300u);
    QCOMPARE(event.text(), QStringLiteral(" "));
    QCOMPARE(event.count(), 1);
    QCOMPARE(event.nativeModifiers(), 0u);
    QCOMPARE(event.modifiers(), Qt::ShiftModifier | Qt::ControlModifier);
    QCOMPARE(event.timestamp(), 400ul);
    // and our custom argument
    QCOMPARE(event.device(), &d);
}

void InputEventsTest::testInitWheelEvent_data()
{
    QTest::addColumn<Qt::Orientation>("orientation");
    QTest::addColumn<qreal>("delta");
    QTest::addColumn<QPoint>("expectedAngleDelta");

    QTest::newRow("horiz") << Qt::Horizontal << 3.0 << QPoint(3, 0);
    QTest::newRow("vert") << Qt::Vertical << 2.0 << QPoint(0, 2);
}

void InputEventsTest::testInitWheelEvent()
{
    // this test verifies that a WheelEvent is constructed correctly

    // first create the test LibInput::Device
    libinput_device device;
    Device d(&device);

    // setup event
    QFETCH(Qt::Orientation, orientation);
    QFETCH(qreal, delta);
    WheelEvent event(QPointF(100, 200), delta, orientation, Qt::LeftButton | Qt::RightButton,
                     Qt::ShiftModifier | Qt::ControlModifier, 300, &d);
    // compare QWheelEvent contract
    QCOMPARE(event.type(), QEvent::Wheel);
    QCOMPARE(event.posF(), QPointF(100, 200));
    QCOMPARE(event.globalPosF(), QPointF(100, 200));
    QCOMPARE(event.buttons(), Qt::LeftButton | Qt::RightButton);
    QCOMPARE(event.modifiers(), Qt::ShiftModifier | Qt::ControlModifier);
    QCOMPARE(event.timestamp(), 300ul);
    QTEST(event.angleDelta(), "expectedAngleDelta");
    // and our custom argument
    QCOMPARE(event.device(), &d);

}

QTEST_GUILESS_MAIN(InputEventsTest)
#include "input_event_test.moc"
