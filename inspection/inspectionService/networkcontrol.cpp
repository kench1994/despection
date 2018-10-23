#include "networkcontrol.h"
#include <QDebug>
NetworkControl *NetworkControl::m_pNetworkControl = NULL;
NetworkControl::NetworkControl()
{
    m_pNetworkManager = NetworkManager::getInstance(NULL);
    m_pNetworkStatus = NetworkStatus::getInstance();
    connect(m_pNetworkManager, SIGNAL(cmdError(QString,QString,int, int, QString)),
            this, SLOT(errorOccured(QString,QString,int, int, QString)));
    connect(m_pNetworkManager, SIGNAL(cmdResponse(QString,QString,QString,DataPackage)),
            this, SLOT(response(QString,QString,QString,DataPackage)));
    connect(m_pNetworkManager, SIGNAL(cmdResult(QString,QString,QString,QString)),
            this, SLOT(result(QString,QString,QString,QString)));

    //二进制相关信号
    connect(m_pNetworkManager,SIGNAL(binaryProgress(QString,int,int,int)), this, SLOT(binaryProgress(QString,int,int,int)));
    connect(m_pNetworkManager, SIGNAL(binaryResponse(QString,QString)), this, SLOT(binaryResponse(QString,QString)));
    connect(m_pNetworkManager,SIGNAL(binaryError(QString,int,QString)), this, SLOT(binaryError(QString,int,QString)));

    connect(m_pNetworkStatus,SIGNAL(networkConnectChanged(QString)),
            this,  SIGNAL(networkConnectChanged(QString)));
}

NetworkControl* NetworkControl::getInstance(){
    if(m_pNetworkControl == NULL)
        m_pNetworkControl = new NetworkControl();
    return m_pNetworkControl;
}

NetworkControl::~NetworkControl()
{
    disconnect(m_pNetworkManager, SIGNAL(cmdError(QString,QString,int, int, QString)),
               this, SLOT(errorOccured(QString,QString,int, int, QString)));
    disconnect(m_pNetworkManager, SIGNAL(cmdResponse(QString,QString,QString,DataPackage)),
               this, SLOT(response(QString,QString,QString,DataPackage)));
    disconnect(m_pNetworkManager, SIGNAL(cmdResult(QString,QString,QString,QString)),
               this, SLOT(result(QString,QString,QString,QString)));

    disconnect(m_pNetworkManager,SIGNAL(binaryProgress(QString,QString,QString,int,QString,QString,uint,uint,int,int)),
            this, SLOT(binaryProgress(QString,QString,QString,int,QString,QString,uint,uint,int,int)));
    disconnect(m_pNetworkManager, SIGNAL(binaryResponse(QString,QString)),
            this, SLOT(binaryResponse(QString,QString)));

    disconnect(m_pNetworkManager,SIGNAL(binaryError(QString,int,QString)),
            this, SLOT(binaryError(QString,int,QString)));

}


void NetworkControl::sendData(const DataPackage &dataPackage, const QString &action, bool binaryType)
{
    qDebug() << Q_FUNC_INFO << " action is " << action << "  binaryType is " << binaryType;
    bool sendBinary = binaryType;
    if(dataPackage.head.type == TYPE_BINARY){
        sendBinary = true;
    }
    NetworkManager *manager = NetworkManager::getInstance(NULL);
    if(!sendBinary){
        manager->sendCommandRequest(dataPackage, action);
    } else {
        if(dataPackage.head.type == TYPE_BINARY){
            //请求更新客户端
            QString remotePath = dataPackage.para["FileUrl"];
            QString localPath = dataPackage.para["FilePath"];
            qDebug() << Q_FUNC_INFO << "request client download  remotePath " << remotePath << "  localPath " << localPath;
            manager->sendBinaryRequest(remotePath, localPath, action, "", "", "", "", true);
        } else {
            Attachment attch = dataPackage.attachments.at(0);
            qDebug() << "sendBinaryRequest path is " << attch.path << " name is " << attch.name << "  title " << attch.title
                     << "  description " << attch.description << "  serialKey " << attch.serialKey;
            manager->sendBinaryRequest("", attch.path, action, attch.name, attch.title, attch.description, attch.serialKey, false);
        }
    }
}

//网络信号
void NetworkControl::errorOccured(const QString &objects, const QString &action, const int &error,
                                     const int &serverCode, const QString &description)
{
    qDebug() <<"------NetworkControl::errorOccured---objects:" << objects << ", action:"<<action <<
               ",error:"<< error << ",serverCode:"<<serverCode <<",description:"<< description;
    DbusPackage map;
    map.otherMap.insert("objects", objects);
    map.otherMap.insert("serverCode", QString::number(serverCode));
    map.otherMap.insert("description", description);
    emit sendNetworkResultSignal(objects, action, false, error, map);
}

void NetworkControl::response(const QString &objects, const QString &type,
                               const QString &action, const DataPackage &package)
{
    Q_UNUSED(type)
    qDebug() <<"------NetworkControl::11111111111response---objects:" << objects;
    DbusPackage map;
    map.dataPackage = package;
    emit sendNetworkResultSignal(objects, action, true, NETWORK_NO_ERROR, map);
}

void NetworkControl::result(const QString &objects, const QString &type, const QString &action, const QString &result)
{
    qDebug() <<"------NetworkControl::result---objects:" << objects << "  result is  " << result;
    Q_UNUSED(type);
    bool bResult = false;
    int error = -1;
    if(result == "1" || result == "100"){
        bResult = true;
        error = 0;
    }
    DbusPackage map;
    map.otherMap["objects"] = objects;
    emit sendNetworkResultSignal(objects, action, bResult, error, map);
}

void NetworkControl::binaryProgress(const QString action, const int length,  const int progress, const int status)
{
    qDebug() << Q_FUNC_INFO << action << progress;
    DbusPackage map;
    map.otherMap.insert("binaryType", "binaryProgress");
    map.otherMap.insert("length", QString::number(length));
    map.otherMap.insert("progress", QString::number(progress));
    map.otherMap.insert("status", QString::number(status));
    emit sendNetworkResultSignal("binary", action, true, NETWORK_NO_ERROR, map);
}

void NetworkControl::binaryResponse(const QString action, const QString target)
{
    qDebug() << Q_FUNC_INFO << " action " << action << " target " << target;
    DbusPackage map;
    map.otherMap.insert("binaryType", "binaryResponse");
    map.otherMap.insert("target", target);
    emit sendNetworkResultSignal("binary", action, true, NETWORK_NO_ERROR, map);
}

void NetworkControl::binaryError(const QString action, const int error, const QString description)
{
    qDebug() << Q_FUNC_INFO << " action " << action << " error " << error << "   description " << description;
    DbusPackage map;
    map.otherMap.insert("binaryType", "binaryError");
    map.otherMap.insert("description", description);
    emit sendNetworkResultSignal("binary", action, false, error, map);
}

QString NetworkControl::getNetworkConnectStatus()
{
    return m_pNetworkStatus->getNetworkConnectStatus();
}
