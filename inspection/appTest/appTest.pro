TEMPLATE = app
TARGET = appTest
target.path = $$TARGET_DIR/bin
INSTALLS += target
QT += core xml sql  multimedia dbus 

CONFIG += link_pkgconfig debug
PKGCONFIG += syberos-qt-system syberos-qt
PKGCONFIG += syberos-application syberos-application-cache

SOURCES += \
    main.cpp \
    commondbustypes.cpp

HEADERS += \
    commondbustypes.h

