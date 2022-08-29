QT += quick bluetooth widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
        bluetoothcontroller.cpp \
        devicemodel.cpp \
        main.cpp

RESOURCES += qml.qrc

INCLUDEPATH  += $$PWD/3rd-party/win-bluetooth/include

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/3rd-party/win-bluetooth/lib/x64/vs2022/Debug win-bluetooth.lib User32.lib
}

CONFIG(release, debug|release) {
    LIBS += -L$$PWD/3rd-party/win-bluetooth/lib/x64/vs2022/Release win-bluetooth.lib User32.lib
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    bluetoothcontroller.h \
    devicemodel.h \
    globals.h

DISTFILES +=
