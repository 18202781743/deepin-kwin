# for things that are also used by kwin libraries
configure_file(libkwineffects/kwinconfig.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/libkwineffects/deepin_kwinconfig.h)
configure_file(config-kwin.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kwin.h)

set(kwin_effects_dbus_xml ${CMAKE_CURRENT_SOURCE_DIR}/org.deepin.kwin.Effects.xml)
qt_add_dbus_interface(effects_interface_SRCS ${kwin_effects_dbus_xml} kwineffects_interface)
add_library(KWinEffectsInterface STATIC ${effects_interface_SRCS})
target_link_libraries(KWinEffectsInterface Qt::DBus)

add_subdirectory(effects)
add_subdirectory(scripts)
add_subdirectory(tabbox)
add_subdirectory(scripting)
add_subdirectory(helpers)
add_subdirectory(qml)

add_subdirectory(libkwineffects)
if (KWIN_BUILD_KCMS)
    add_subdirectory(kcmkwin)
endif()

set(kwin_SRCS
    3rdparty/xcursor.c
    splitoutline.cpp
    splitmanage.cpp
    abstract_client.cpp
    abstract_output.cpp
    abstract_wayland_output.cpp
    activation.cpp
    appmenu.cpp
    atoms.cpp
    client_machine.cpp
    composite.cpp
    cursor.cpp
    dbusinterface.cpp
    debug_console.cpp
    decorationitem.cpp
    decorations/decoratedclient.cpp
    decorations/decorationbridge.cpp
    decorations/decorationpalette.cpp
    decorations/decorations_logging.cpp
    decorations/settings.cpp
    deleted.cpp
    dmabuftexture.cpp
    dpmsinputeventfilter.cpp
    effectloader.cpp
    effects.cpp
    events.cpp
    focuschain.cpp
    ftrace.cpp
    gestures.cpp
    globalshortcuts.cpp
    group.cpp
    idle_inhibition.cpp
    input.cpp
    input_event.cpp
    input_event_spy.cpp
    inputbackend.cpp
    inputdevice.cpp
    inputmethod.cpp
    inputpanelv1client.cpp
    inputpanelv1integration.cpp
    internal_client.cpp
    item.cpp
    keyboard_input.cpp
    keyboard_layout.cpp
    keyboard_layout_switching.cpp
    keyboard_repeat.cpp
    killwindow.cpp
    layers.cpp
    layershellv1client.cpp
    layershellv1integration.cpp
    linux_dmabuf.cpp
    main.cpp
    modifier_only_shortcuts.cpp
    moving_client_x11_filter.cpp
    netinfo.cpp
    onscreennotification.cpp
    options.cpp
    osd.cpp
    outline.cpp
    overlaywindow.cpp
    placement.cpp
    platform.cpp
    plugin.cpp
    pluginmanager.cpp
    pointer_input.cpp
    popup_input_filter.cpp
    renderbackend.cpp
    renderjournal.cpp
    renderloop.cpp
    rootinfo_filter.cpp
    rulebooksettings.cpp
    rules.cpp
    recordeventmonitor.cpp
    scene.cpp
    screenedge.cpp
    screenlockerwatcher.cpp
    screens.cpp
    scripting/v2/clientmodel.cpp
    scripting/v3/clientmodel.cpp
    scripting/v3/virtualdesktopmodel.cpp
    scripting/dbuscall.cpp
    scripting/desktopbackgrounditem.cpp
    scripting/screenedgeitem.cpp
    scripting/scriptedeffect.cpp
    scripting/scripting.cpp
    scripting/scripting_logging.cpp
    scripting/scriptingutils.cpp
    scripting/thumbnailitem.cpp
    scripting/workspace_wrapper.cpp
    session.cpp
    session_consolekit.cpp
    session_logind.cpp
    session_noop.cpp
    shadow.cpp
    shadowitem.cpp
    sm.cpp
    surfaceitem.cpp
    surfaceitem_internal.cpp
    surfaceitem_wayland.cpp
    surfaceitem_x11.cpp
    syncalarmx11filter.cpp
    tablet_input.cpp
    toplevel.cpp
    hide_cursor_spy.cpp
    touch_input.cpp
    udev.cpp
    unmanaged.cpp
    useractions.cpp
    virtualdesktops.cpp
    virtualdesktopsdbustypes.cpp
    virtualkeyboard_dbus.cpp
    was_user_interaction_x11_filter.cpp
    wayland_server.cpp
    waylandclient.cpp
    waylandoutput.cpp
    waylandoutputconfig.cpp
    waylandoutputdevicev2.cpp
    waylandshellintegration.cpp
    window_property_notify_x11_filter.cpp
    windowitem.cpp
    workspace.cpp
    x11client.cpp
    x11eventfilter.cpp
    x11syncmanager.cpp
    xcursortheme.cpp
    xdgactivationv1.cpp
    xdgshellclient.cpp
    xdgshellintegration.cpp
    xkb.cpp
    xwaylandclient.cpp
    xwl/xwayland_interface.cpp
)

qt_add_dbus_adaptor(kwin_SRCS scripting/org.kde.kwin.Script.xml scripting/scripting.h KWin::AbstractScript)

kconfig_add_kcfg_files(kwin_SRCS settings.kcfgc)
kconfig_add_kcfg_files(kwin_SRCS rulesettings.kcfgc)
kconfig_add_kcfg_files(kwin_SRCS rulebooksettingsbase.kcfgc)

qt_add_dbus_adaptor(kwin_SRCS org.deepin.KWin.xml dbusinterface.h KWin::DBusInterface)
qt_add_dbus_adaptor(kwin_SRCS org.deepin.kwin.Compositing.xml dbusinterface.h KWin::CompositorDBusInterface)
qt_add_dbus_adaptor(kwin_SRCS ${kwin_effects_dbus_xml} effects.h KWin::EffectsHandlerImpl)
qt_add_dbus_adaptor(kwin_SRCS org.deepin.KWin.VirtualDesktopManager.xml dbusinterface.h KWin::VirtualDesktopManagerDBusInterface)
qt_add_dbus_adaptor(kwin_SRCS org.kde.KWin.Session.xml sm.h KWin::SessionManager)
qt_add_dbus_adaptor(kwin_SRCS org.deepin.KWin.Plugins.xml dbusinterface.h KWin::PluginManagerDBusInterface)
qt_add_dbus_adaptor(kwin_SRCS org.deepin.kwin.Xkb.xml xkb.h KWin::Xkb)

qt_add_dbus_interface(kwin_SRCS ${KSCREENLOCKER_DBUS_INTERFACES_DIR}/kf5_org.freedesktop.ScreenSaver.xml screenlocker_interface)
qt_add_dbus_interface(kwin_SRCS ${KSCREENLOCKER_DBUS_INTERFACES_DIR}/org.kde.screensaver.xml kscreenlocker_interface)
qt_add_dbus_interface(kwin_SRCS org.kde.kappmenu.xml appmenu_interface)

qt5_add_resources( kwin_SRCS ../resources.qrc )

ki18n_wrap_ui(kwin_SRCS
    debug_console.ui
    shortcutdialog.ui
)

add_library(deepin-kwin SHARED ${kwin_SRCS})
set_target_properties(deepin-kwin PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_link_libraries(deepin-kwin
    deepin-kwineffects
    deepin-kwinglutils

    Qt::Concurrent
    Qt::DBus
    Qt::Quick

    KF5::ConfigCore
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::GlobalAccel
    KF5::GlobalAccelPrivate
    KF5::I18n
    KF5::Notifications
    KF5::Package
    KF5::Plasma
    KF5::QuickAddons
    KF5::WindowSystem

    KDecoration2::KDecoration
    KDecoration2::KDecoration2Private

    PW::KScreenLocker
    Wayland::Server
    DWaylandServer

    XCB::COMPOSITE
    XCB::CURSOR
    XCB::DAMAGE
    XCB::GLX
    XCB::ICCCM
    XCB::KEYSYMS
    XCB::RANDR
    XCB::RENDER
    XCB::SHAPE
    XCB::SHM
    XCB::SYNC
    XCB::XCB
    XCB::XFIXES

    UDev::UDev
    XKB::XKB
    Qt5::XkbCommonSupportPrivate
    epoxy::epoxy

    Threads::Threads
    -lXext
    -lXtst
)

add_subdirectory(backends)
add_subdirectory(scenes)
add_subdirectory(utils)

if (KWIN_BUILD_CMS)
    target_sources(deepin-kwin PRIVATE
        colordevice.cpp
        colormanager.cpp
    )
    target_link_libraries(deepin-kwin lcms2::lcms2)
endif()

if (KWIN_BUILD_ACTIVITIES)
    target_sources(deepin-kwin PRIVATE activities.cpp)
    target_link_libraries(deepin-kwin KF5::Activities)
endif()

if (KWIN_BUILD_TABBOX)
    target_sources(deepin-kwin PRIVATE
        tabbox/clientmodel.cpp
        tabbox/desktopchain.cpp
        tabbox/desktopmodel.cpp
        tabbox/switcheritem.cpp
        tabbox/tabbox.cpp
        tabbox/tabbox_logging.cpp
        tabbox/tabboxconfig.cpp
        tabbox/tabboxhandler.cpp
        tabbox/x11_filter.cpp
    )
    target_link_libraries(deepin-kwin Qt::GuiPrivate)
endif()

qt5_generate_dbus_interface(virtualkeyboard_dbus.h org.deepin.kwin.VirtualKeyboard.xml OPTIONS -A)
qt5_generate_dbus_interface(tabletmodemanager.h org.deepin.KWin.TabletModeManager.xml OPTIONS -A)

generate_export_header(deepin-kwin
    BASE_NAME KWIN
    EXPORT_FILE_NAME deepin_kwin_export.h)

add_executable(deepin-kwin_x11 main_x11.cpp)
target_link_libraries(deepin-kwin_x11
    kwin4_effect_builtins
    deepin-kwin
    KF5::Crash
    Qt::X11Extras
    PkgConfig::Systemd
)

install(TARGETS deepin-kwin ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
install(TARGETS deepin-kwin_x11 ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(xwl)

add_executable(deepin-kwin_wayland
    main_wayland.cpp
    tabletmodemanager.cpp
)
target_link_libraries(deepin-kwin_wayland
    kwin4_effect_builtins
    deepin-kwin
    KF5::Crash
    KWinXwaylandServerModule
)
if (HAVE_LIBCAP)
    target_link_libraries(deepin-kwin_wayland ${Libcap_LIBRARIES})
endif()

install(TARGETS deepin-kwin_wayland ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
if (HAVE_LIBCAP)
    install(
    CODE "execute_process(
            COMMAND
                ${SETCAP_EXECUTABLE}
                CAP_SYS_NICE=+ep
                \$ENV{DESTDIR}${CMAKE_INSTALL_FULL_BINDIR}/deepin-kwin_wayland)"
    )
endif()

add_subdirectory(platformsupport)
add_subdirectory(plugins)

target_link_libraries(deepin-kwin_wayland
    KWinQpaPlugin
    KF5GlobalAccelKWinPlugin
    KF5WindowSystemKWinPlugin
    KF5IdleTimeKWinPlugin
)

add_custom_target(
    KWinDBusInterfaces
    ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/org.deepin.kwin.VirtualKeyboard.xml
            ${CMAKE_CURRENT_BINARY_DIR}/org.deepin.KWin.TabletModeManager.xml
)

if (PipeWire_FOUND)
    target_link_libraries(deepin-kwin_wayland KWinScreencastPlugin)
endif()

install(FILES kwin.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR} RENAME ${KWIN_NAME}.kcfg)
install(FILES kwin.notifyrc DESTINATION ${KDE_INSTALL_KNOTIFY5RCDIR} RENAME ${KWIN_NAME}.notifyrc)
install(
    FILES
        org.deepin.KWin.VirtualDesktopManager.xml
        org.deepin.KWin.xml
        org.deepin.kwin.Compositing.xml
        org.deepin.kwin.Effects.xml
        org.deepin.KWin.Plugins.xml
        org.deepin.kwin.Xkb.xml
        ${CMAKE_CURRENT_BINARY_DIR}/org.deepin.kwin.VirtualKeyboard.xml
        ${CMAKE_CURRENT_BINARY_DIR}/org.deepin.KWin.TabletModeManager.xml
    DESTINATION
        ${KDE_INSTALL_DBUSINTERFACEDIR}
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/deepin_kwin_export.h DESTINATION ${KDE_INSTALL_INCLUDEDIR} COMPONENT Devel)

# Install the KWin/Script service type
install(FILES scripting/deepin-kwinscript.desktop DESTINATION ${KDE_INSTALL_KSERVICETYPES5DIR})

# translations
file(GLOB TS LIST_DIRECTORIES false translations/popupmenu/popupmenu*.ts)
foreach(_ts_file ${TS})
    execute_process(
        COMMAND ${LUPDATE_EXECUTABLE} -recursive ${CMAKE_SOURCE_DIR}/translations -ts ${_ts_file})
    execute_process(COMMAND lrelease ${_ts_file})
endforeach()

file(GLOB QM_FILES "translations/popupmenu/*.qm")
install_files("/share/translations/popupmenu" FILES ${QM_FILES})
