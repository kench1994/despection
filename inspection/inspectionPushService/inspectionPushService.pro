TEMPLATE = app
TARGET = inspectionPushService

target.path = $$TARGET_DIR/bin
INSTALLS += target

QT += core xml dbus network quick multimedia

LIBS += -lcrypto -lz

CONFIG += link_pkgconfig debug
PKGCONFIG += syberos-qt-system syberos-qt openssl
PKGCONFIG += syberos-application syberos-application-cache

SOURCES += \
    main.cpp \
    push_service_workspace.cpp \
    pushmanager.cpp \
    pushinterface.cpp \
    pushauthority.cpp \
    networkreceiver.cpp \
    networkcontrol.cpp \
    connectionthread.cpp \
    heartbeatmanager.cpp \
    networksender.cpp \
    ../framework/dataManager/datacompress.cpp \
    ../framework/dataManager/datacrypto.cpp \
    ../framework/dataManager/datadecoder.cpp \
    ../framework/dataManager/dataencoder.cpp \
    ../framework/dataManager/datahandler.cpp \
    ../framework/config/sdisruntimecontext.cpp \
    ../framework/network/tools/networktoolfactory.cpp \
    ../framework/network/tools/sockettool.cpp \
    ../framework/network/tools/usbtool.cpp \
    ../framework/network/networkheadtool.cpp \
    ../framework/sdissystemenvironment.cpp \
    ../framework/common/commondbustypes.cpp \
    ../framework/common/networkstatus.cpp \
    ../framework/common/systemconfig.cpp \
    ../framework/network/tools/networkencryptionutil.cpp \
    ../framework/network/tools/sslsockettool.cpp \
    runtimeparam.cpp

HEADERS += \
    push_service_workspace.h \
    pushmanager.h \
    push_define.h \
    pushinterface.h \
    pushauthority.h \
    networkreceiver.h \
    networkcontrol.h \
    connectionthread.h \
    heartbeatmanager.h \
    networksender.h \
    ../framework/dataManager/datacompress.h \
    ../framework/dataManager/datacrypto.h \
    ../framework/dataManager/datadecoder.h \
    ../framework/dataManager/dataencoder.h \
    ../framework/dataManager/datahandler.h \
    ../framework/config/sdisruntimecontext.h \
    ../framework/network/tools/inetworktool.h \
    ../framework/network/tools/networktoolfactory.h \
    ../framework/network/tools/sockettool.h \
    ../framework/network/tools/usbtool.h \
    ../framework/network/networkheadtool.h \
    ../framework/common/utils.h \
    ../framework/common/commondbustypes.h \
    ../framework/common/networkstatus.h \
    ../framework/common/systemconfig.h \
   ../framework/network/tools/networkencryptionutil.h \
    ../framework/network/tools/sslsockettool.h \
    runtimeparam.h
