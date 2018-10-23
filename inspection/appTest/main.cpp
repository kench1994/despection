#include <QCoreApplication>
#include <QDebug>
#include <QDBusInterface>
#include <QDBusReply>
#include "commondbustypes.h"
//receiveData(const DataPackage &dataPackage, const QString &action, const QString &functionType)
Q_DECL_EXPORT int main(int argc, char* argv[]){
    QCoreApplication app(argc, argv);
// void response(QString objects, QString action, bool result, int error, QVariantMap map);
#if 0
    QDBusMessage msg =QDBusMessage::createSignal("/",  "com.syberos.inspectionClient.interface", "responseToClient");
    QVariantMap map;
    map.insert("key", "value");
    msg<<"1111" << "action"<<true<<1<< map;
    bool rv =  QDBusConnection::sessionBus().send(msg);
    qDebug() << "QDBusConnection::sessionBus().send  return " << rv;
#else

        registerCommonDataTypes();
        // 创建QDBusInterface
        QDBusInterface iface( "com.syberos.inspectionService", "/",
                              "com.syberos.inspectionService.interface", QDBusConnection::sessionBus());
        if (!iface.isValid()) {
                qDebug() << qPrintable(QDBusConnection::sessionBus().lastError().message());
                exit(1);
        }
        // 呼叫远程的checkIn，参数为num_room
        DataPackage dataPackage;
        dataPackage.head.objects = "EMF_FaultRepair_Para";
        dataPackage.head.key = "headKey";
        dataPackage.head.type = "2";
        dataPackage.head.date = "2016/01/02 12:20:00";
        DataTable table;
        table.name = "faultRepair";
        table.key = "111111111";
        DataRow data;
        data.insert("EquipmentID", "123456789");
        data.insert("EquipmentName", "youji");
        data.insert("EquipmentFactory", "yuanxin");
        data.insert("FaultType", "1");
        data.insert("serialKey", "b6d529f2-68f2-11e6-bebf-02004c4f4f50");
        table.value.append(data);
        dataPackage.tables.append(table);


        Attachment attach;
        attach.description = "descriptior";
        attach.mime = "mp4";
        attach.name = "testvideo.mp4";
        attach.title = "testvideo.mp4";
        attach.path = "/data/developer/testvideo.mp4";
        attach.serialKey = "b6d529f2-68f2-11e6-bebf-02004c4f4f50";

        dataPackage.binaries.append(attach);
        QVariant v1, v2,  v3;
        v1.setValue(dataPackage);
        v2.setValue(QString("111111111111"));
        v3.setValue(QString("submit"));
        QDBusReply<void> reply = iface.call("receiveData", v1, v2, v3);
        if (reply.isValid()) {
            qDebug() << "reply.isValid()";
        } else {
            qDebug() << "reply. not isValid()";
        }


#endif
        app.exec();
}
