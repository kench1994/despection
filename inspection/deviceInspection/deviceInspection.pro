#
# This file is part of user-guide
#
# Copyright (C) 2014 Beijing Yuan Xin Technology Co.,Ltd. All rights reserved.
#
# Authors:
#       Shou Xiaoyun <shouxiaoyun@syberos.com>
#      
# This software, including documentation, is protected by copyright controlled
# by Beijing Yuan Xin Technology Co.,Ltd. All rights are reserved.
#

TEMPLATE = app
TARGET = deviceInspection
target.path = $$TARGET_DIR/bin
INSTALLS += target
QT += core qml quick gui-private xml sql gui widgets multimedia dbus nfc positioning
LIBS += -lz -lcrypto

CONFIG += link_pkgconfig debug
PKGCONFIG += syberos-qt-system syberos-qt
PKGCONFIG += syberos-application syberos-application-cache
INCLUDEPATH += ./src
HEADERS += \
    src/generateForm/generateForm.h \
    src/generallistmodel/generallistmodel.h \
    src/common/transmitmanager.h \
    src/common/inspectionsyssetting.h \
    src/deviceinspectionworkspace.h \
    src/upgrade/upgradeconfig.h \
    src/upgrade/upgradeThread.h \
    src/theme/themenode.h \
    src/theme/thememanager.h \
    src/theme/themeadaptor.h \
    src/theme/theme.h \
    src/common/nfcmanager.h \
    src/common/gpsmanager.h \
    src/common/deviceinspectioneventfilter.h \
    src/generateForm/baseform.h \
    src/generateForm/inspectionform.h \
    src/generateForm/maintainform.h \
    src/generateForm/basewidget.h \
    src/imageProcess/qtcamera.h \
    src/imageProcess/qandroidmultimediautils.h \
    src/imageProcess/imageproc.h \
    src/imageProcess/decodeworkspace.h \
    src/imageProcess/decodethread.h \
    src/generateForm/inputwidget.h \
    src/generateForm/enumwidget.h \
    src/generateForm/inputenumwidget.h \
    src/generateForm/formConfigType.h \
    src/generateForm/standardform.h \
    src/generateForm/timewidget.h \
    src/generateForm/datareportform.h \
    src/generateForm/multiinputwidget.h \
    src/imageProcess/audiorecorder.h \
    src/imageProcess/multimediautils.h \
    src/imageProcess/videorecorder.h \
    ../framework/common/convertdata.h \
    ../framework/common/functiontype.h \
    ../framework/common/utils.h \
    ../framework/dataManager/datacrypto.h \
    ../framework/common/commondbustypes.h \
    src/common/clientinterface.h \
    src/generateForm/dailyTaskform.h \
    src/common/httprequest.h \
    ../framework/common/systemconfig.h \
    ../framework/common/networkstatus.h \
    src/generallistmodel/commonproxymodel.h \
    src/common/runtimeparam.h

SOURCES += src/main.cpp \
    src/generateForm/generateForm.cpp \
    src/generallistmodel/generallistmodel.cpp \
    src/common/transmitmanager.cpp \
    src/common/inspectionsyssetting.cpp \
    src/deviceinspectionworkspace.cpp \
    src/upgrade/upgradeThread.cpp \
    src/upgrade/upgradeconfig.cpp \
    src/theme/themenode.cpp \
    src/theme/thememanager.cpp \
    src/theme/themeadaptor.cpp \
    src/theme/theme.cpp \
    src/common/nfcmanager.cpp \
    src/common/gpsmanager.cpp \
    src/common/deviceinspectioneventfilter.cpp \
    src/generateForm/baseform.cpp \
    src/generateForm/inspectionform.cpp \
    src/generateForm/maintainform.cpp \
    src/generateForm/basewidget.cpp \
    src/imageProcess/qtcamera.cpp \
    src/imageProcess/qandroidmultimediautils.cpp \
    src/imageProcess/imageproc.cpp \
    src/imageProcess/decodeworkspace.cpp \
    src/imageProcess/decodethread.cpp \
    src/generateForm/inputwidget.cpp \
    src/generateForm/enumwidget.cpp \
    src/generateForm/inputenumwidget.cpp \
    src/generateForm/formConfigType.cpp \
    src/generateForm/standardform.cpp \
    src/generateForm/timewidget.cpp \
    src/generateForm/datareportform.cpp \
    src/generateForm/multiinputwidget.cpp \
    src/imageProcess/audiorecorder.cpp \
    src/imageProcess/multimediautils.cpp \
    src/imageProcess/videorecorder.cpp \
    ../framework/common/convertdata.cpp \
    ../framework/dataManager/datacrypto.cpp \
    ../framework/common/commondbustypes.cpp \
    src/common/clientinterface.cpp \
    src/generateForm/dailyTaskform.cpp \
    src/common/httprequest.cpp \
    ../framework/common/systemconfig.cpp \
    ../framework/common/networkstatus.cpp \
    src/generallistmodel/commonproxymodel.cpp \
    src/common/runtimeparam.cpp

RESOURCES += \
    deviceInspection.qrc

OTHER_FILES += \
    ../sopconfig.xml \
    systemConfig.ini \
    serviceConfig.ini \
    ../deviceInspection.spec \
    deviceInspection.service \
    qml/DataReport/DataReportTaskDelegate.qml \
    qml/DataReport/DataReportForm.qml \
    qml/DataReport/DataReportBaseList.qml \
    qml/Common/HeadTitle.qml \
    qml/Common/ListMultiInputItem.qml

config.files = ../sopconfig.xml
config.path = $$TARGET_DIR/
INSTALLS += config


systemConfig.files = systemConfig.ini
systemConfig.path = $$TARGET_DIR/
INSTALLS += systemConfig

serviceConfig.files = serviceConfig.ini
serviceConfig.path = $$TARGET_DIR/
INSTALLS += serviceConfig

desktopicon.files = res/*
desktopicon.path = $$TARGET_DIR/res/

INSTALLS += desktopicon

INCLUDEPATH +=  theme \
               upgrade \
               common

# translate
lupdate_only {
    SOURCES += qml/*.qml \
               qml/InspectionTask/*.qml \
               qml/InventoryTask/*.qml \
               qml/DeviceOperationTask/*.qml \
               qml/ComprehensiveTask/*.qml \
               qml/RectificationTask/*.qml \
               qml/DeviceMaintainTask/*.qml \
               qml/StandardizedTask/*.qml \
               qml/Common/*.qml \
               qml/SystemMessage/*.qml \
               qml/SystemSettings/*.qml \
               qml/LoginAndHome/*.qml \
               qml/DateTime/*.qml \
               qml/DataReport/*.qml \
               qml/DailyTask/*.qml \
               qml/Multimedia/*.qml \
               qml/ScanTask/*.qml
}

TRANSLATIONS += \
    deviceInspection.zh_CN.ts \
    deviceInspection.zh_HK.ts

qm.path = $$TARGET_DIR/qm
qm.files = *.qm
INSTALLS += qm

themesIcons.path=$$TARGET_DIR/themes/default/res
themesIcons.files=./themes/default/res/*.png
INSTALLS += themesIcons

themesConfig.path=$$TARGET_DIR/themes/default
themesConfig.files=./themes/default/theme.xml
INSTALLS += themesConfig

DISTFILES += \
    qml/DeviceOperationTask/FaultRepairMainPage.qml \
    qml/DeviceOperationTask/FaultAcceptForm.qml \
    qml/DeviceOperationTask/FaultRepairDetail.qml \
    qml/DeviceOperationTask/FaultApproveForm.qml \
    qml/DeviceOperationTask/FaultDispatchForm.qml \
    qml/DeviceOperationTask/FaultMaintainForm.qml \
    qml/DeviceOperationTask/FaultCostForm.qml \
    qml/DeviceOperationTask/FaultAppraiseForm.qml \
    qml/DeviceOperationTask/CheckComponentInfo.qml \
    qml/Multimedia/VolumeLine.qml \
    qml/SystemSettings/RebootItem.qml \
    qml/Common/AddComponents.qml \
    qml/Knowledge/Knowledge.qml \
    qml/Knowledge/KnowledgeTaskListDelegate.qml \
    qml/Knowledge/KnowledgeDetail.qml
