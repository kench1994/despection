TEMPLATE = app
TARGET = inspectionService

target.path = $$TARGET_DIR/bin
INSTALLS += target

QT += core xml sql  multimedia dbus gui quick
LIBS += -lcrypto -lz

CONFIG += link_pkgconfig debug
PKGCONFIG += syberos-qt-system syberos-qt openssl
PKGCONFIG += syberos-application syberos-application-cache

HEADERS += \
    service_workspace.h \
    inspectioninterface.h \
    dataprovider.h \
    common/dataoperationtype.h \
    common/bussinessmap.h \
    localcache/localcachemanager.h \
    localcache/datalocalcachebase.h \
    localcache/devicedatalocalcache.h \
    localcache/devicedeploymentdatalocalcache.h \
    localcache/devicereceivedatalocalcache.h \
    localcache/inspectiondatalocalcache.h \
    localcache/inventorydatalocalcache.h \
    localcache/operationdatalocalcache.h \
    localcache/standardizationdatalocalcache.h \
    localcache/userinfodatalocalcache.h \
    DBConfig/dbconfigbase.h \
    DBConfig/dbconfigmanager.h \
    DBConfig/dbconfigfactory.h \
    DBConfig/dbconfigdevice.h \
    DBConfig/dbconfigdevicedeployment.h \
    DBConfig/dbconfigdevicereceive.h \
    DBConfig/dbconfiginventory.h \
    DBConfig/dbconfiginspection.h \
    DBConfig/dbconfigoperation.h \
    DBConfig/dbconfigmaintenance.h \
    DBConfig/dbconfigstandardization.h \
    DBConfig/dbconfiguserinfo.h \
    common/tableconfig.h \
    common/tablefield.h \
    common/dbhelper.h \
    ../framework/dataManager/datacompress.h \
    ../framework/dataManager/datacrypto.h \
    ../framework/dataManager/datadecoder.h \
    ../framework/dataManager/dataencoder.h \
    ../framework/dataManager/datahandler.h \
    ../framework/network/networkmanager.h \
    ../framework/network/networkthreadbase.h \
    ../framework/common/utils.h \
    localcache/localcachefactory.h \
    localcache/maintenancedatalocalcache.h \
    ../framework/common/functiontype.h \
    ../framework/network/networkheadtool.h \
    ../framework/network/tools/inetworktool.h \
    ../framework/network/tools/networktoolfactory.h \
    ../framework/network/tools/sockettool.h \
    ../framework/network/tools/usbtool.h \
    ../framework/network/tools/sslsockettool.h \
    ../framework/network/command/commandmanager.h \
    ../framework/network/command/commandtaskqueue.h \
    ../framework/network/command/networkcommandthread.h \
    ../framework/network/binary/binarymanager.h \
    ../framework/network/binary/binarytaskqueue.h \
    ../framework/network/binary/networkbinarythread.h \
    ../framework/config/sdisruntimecontext.h \
    ../framework/utils/sdisfiletool.h \
    ../framework/common/convertdata.h \
    ../framework/common/commondbustypes.h \
    dataHandler/datahandlermanager.h \
    dataHandler/datahandlerbase.h \
    dataHandler/devicehandler.h \
    dataHandler/devicedeploymenthandler.h \
    dataHandler/devicereceivehandler.h \
    dataHandler/inspectionhandler.h \
    dataHandler/inventoryhandler.h \
    dataHandler/maintenancehandler.h \
    dataHandler/operationhandler.h \
    dataHandler/standardizationhandler.h \
    dataHandler/userinfohandler.h \
    dataHandler/datahandlerfactory.h \
    sync/syncmanager.h \
    sync/syncfactory.h \
    sync/devicedeploymentsynchronizer.h \
    sync/devicedistributionsynchronizer.h \
    sync/devicesynchronizer.h \
    sync/examinesynchronizer.h \
    sync/homesynchronizer.h \
    sync/inspectionsynchronizer.h \
    sync/inventorysynchronizer.h \
    sync/maintenancesynchronizer.h \
    sync/operationsynchronizer.h \
    sync/userinfosynchronizer.h \
    sync/synchronizerbase.h \
    common/comparetool.h \
    networkcontrol.h \
    localcache/rectificationlocalcache.h \
    DBConfig/dbconfigrectification.h \
    localcache/messagelocalcache.h \
    DBConfig/dbconfigmessage.h \
    dataHandler/messagehandler.h \
    ../framework/network/tools/networkencryptionutil.h \
    ../framework/common/networkstatus.h \
    dataHandler/updatehandler.h \
    dataHandler/homehandler.h \
    dataHandler/rectificationhandler.h \
    ../framework/common/systemconfig.h \
    DBConfig/dbconfighomeinfo.h \
    common/dbdefine.h \
    workthread.h \
    dataHandler/dailytaskhandler.h \
    DBConfig/dbconfigdaily.h \
    sync/dailysynchronizer.h \
    localcache/dailylocalcache.h \
    sync/rectificationsynchronizer.h \
    common/runtimeparam.h \
    dataHandler/scantaskhandler.h \
    dataHandler/knowledge.h

SOURCES += \
    main.cpp \
    service_workspace.cpp \
    inspectioninterface.cpp \
    dataprovider.cpp \
    common/bussinessmap.cpp \
    localcache/localcachemanager.cpp \
    localcache/datalocalcachebase.cpp \
    localcache/devicedatalocalcache.cpp \
    localcache/devicedeploymentdatalocalcache.cpp \
    localcache/devicereceivedatalocalcache.cpp \
    localcache/inspectiondatalocalcache.cpp \
    localcache/inventorydatalocalcache.cpp \
    localcache/operationdatalocalcache.cpp \
    localcache/standardizationdatalocalcache.cpp \
    localcache/userinfodatalocalcache.cpp \
    DBConfig/dbconfigbase.cpp \
    DBConfig/dbconfigmanager.cpp \
    DBConfig/dbconfigfactory.cpp \
    DBConfig/dbconfigdevice.cpp \
    DBConfig/dbconfigdevicedeployment.cpp \
    DBConfig/dbconfigdevicereceive.cpp \
    DBConfig/dbconfiginventory.cpp \
    DBConfig/dbconfiginspection.cpp \
    DBConfig/dbconfigoperation.cpp \
    DBConfig/dbconfigmaintenance.cpp \
    DBConfig/dbconfigstandardization.cpp \
    DBConfig/dbconfiguserinfo.cpp \
    common/tableconfig.cpp \
    common/tablefield.cpp \
    common/dbhelper.cpp \
    ../framework/dataManager/datacompress.cpp \
    ../framework/dataManager/datacrypto.cpp \
    ../framework/dataManager/datadecoder.cpp \
    ../framework/dataManager/dataencoder.cpp \
    ../framework/dataManager/datahandler.cpp \
    ../framework/network/networkmanager.cpp \
    ../framework/network/networkthreadbase.cpp \
    localcache/localcachefactory.cpp \
    localcache/maintenancedatalocalcache.cpp \
    ../framework/network/networkheadtool.cpp \
    ../framework/network/tools/networktoolfactory.cpp \
    ../framework/network/tools/sockettool.cpp \
    ../framework/network/tools/usbtool.cpp \
    ../framework/network/tools/sslsockettool.cpp \
    ../framework/network/command/commandmanager.cpp \
    ../framework/network/command/commandtaskqueue.cpp \
    ../framework/network/command/networkcommandthread.cpp \
    ../framework/network/binary/binarymanager.cpp \
    ../framework/network/binary/binarytaskqueue.cpp \
    ../framework/network/binary/networkbinarythread.cpp \
    ../framework/config/sdisruntimecontext.cpp \
    ../framework/utils/sdisfiletool.cpp \
    ../framework/common/convertdata.cpp \
    ../framework/common/commondbustypes.cpp \
    dataHandler/datahandlermanager.cpp \
    dataHandler/datahandlerbase.cpp \
    dataHandler/devicehandler.cpp \
    dataHandler/devicedeploymenthandler.cpp \
    dataHandler/devicereceivehandler.cpp \
    dataHandler/inspectionhandler.cpp \
    dataHandler/inventoryhandler.cpp \
    dataHandler/maintenancehandler.cpp \
    dataHandler/operationhandler.cpp \
    dataHandler/standardizationhandler.cpp \
    dataHandler/userinfohandler.cpp \
    dataHandler/datahandlerfactory.cpp \
    sync/syncmanager.cpp \
    sync/syncfactory.cpp \
    sync/devicedeploymentsynchronizer.cpp \
    sync/devicedistributionsynchronizer.cpp \
    sync/devicesynchronizer.cpp \
    sync/examinesynchronizer.cpp \
    sync/homesynchronizer.cpp \
    sync/inspectionsynchronizer.cpp \
    sync/inventorysynchronizer.cpp \
    sync/maintenancesynchronizer.cpp \
    sync/operationsynchronizer.cpp \
    sync/userinfosynchronizer.cpp \
    sync/synchronizerbase.cpp \
    common/comparetool.cpp \
    networkcontrol.cpp \
    localcache/rectificationlocalcache.cpp \
    DBConfig/dbconfigrectification.cpp \
    localcache/messagelocalcache.cpp \
    DBConfig/dbconfigmessage.cpp \
    dataHandler/messagehandler.cpp \
    ../framework/network/tools/networkencryptionutil.cpp \
    ../framework/common/networkstatus.cpp \
    dataHandler/updatehandler.cpp \
    dataHandler/homehandler.cpp \
    dataHandler/rectificationhandler.cpp \
    ../framework/common/systemconfig.cpp \
    DBConfig/dbconfighomeinfo.cpp \
    workthread.cpp \
    dataHandler/dailytaskhandler.cpp \
    DBConfig/dbconfigdaily.cpp \
    sync/dailysynchronizer.cpp \
    localcache/dailylocalcache.cpp \
    sync/rectificationsynchronizer.cpp \
    common/runtimeparam.cpp \
    dataHandler/scantaskhandler.cpp \
    dataHandler/knowledge.cpp

tableXml.path=$$TARGET_DIR/DBTableXml/
tableXml.files=./DBTableXml/*.xml
INSTALLS += tableXml

DISTFILES += \
    DBTableXml/device_deployment.xml \
    DBTableXml/devicereceive_table_info.xml \
    DBTableXml/device_table_info.xml \
    DBTableXml/inspection_table_info.xml \
    DBTableXml/inventory_table_info.xml \
    DBTableXml/maintenance_table_info.xml \
    DBTableXml/operation_table_info.xml \
    DBTableXml/rectification_table_info.xml \
    DBTableXml/standardization_table_info.xml \
    DBTableXml/userinfo.xml \
    DBTableXml/message_table_info.xml \
    DBTableXml/homeinfo.xml \
    DBTableXml/daily_table_info.xml

cert.path=$$TARGET_DIR/
cert.files=certificate.pem

INSTALLS += cert

OTHER_FILES += certificate.pem
