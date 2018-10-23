#include "transmitmanager.h"
#include <QtQml>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include "./runtimeparam.h"
#include "../../../framework/common/utils.h"
#include "../../../framework/common/functiontype.h"
#include "../../../framework/common/networkstatus.h"
#include "../../../framework/common/convertdata.h"
#include <stdlib.h>
#include <QDBusConnection>
#include <QDBusError>
TransmitManager* TransmitManager::m_pTransMng = NULL;
TransmitManager *TransmitManager::getInstance()
{
    if(m_pTransMng == NULL)
        m_pTransMng = new TransmitManager();
    return m_pTransMng;
}

TransmitManager::TransmitManager(QObject *parent) :
    QObject(parent), m_pViewer(NULL),m_pLoading(NULL)
{
    isRunnig = false;
    m_ActionList.clear();
    qDebug() << "set ip and deviceID8888888888888888888888888";
    m_pToast = new CToastManager(parent);
    m_pViewer = RuntimeParam::getInstance()->getViewer();
    m_pClientInterface = new ClientInterface();
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.registerService(CLIENT_DBUS_SERVICE)) {
        qDebug() << "dbus registerService error" <<bus.lastError().message();
        exit(1);
    }
    bus.registerObject(CLIENT_DBUS_PATH, m_pClientInterface, QDBusConnection::ExportAllSignals);

    connect(m_pClientInterface, SIGNAL(response(QString,QString,bool,int,DbusPackage)),
            this, SLOT(serviceResponse(QString,QString,bool,int,DbusPackage)));

    connect(m_pClientInterface, SIGNAL(messageUpdated(QString)),
            this, SLOT(messageUpdate(QString)));
}

TransmitManager::~TransmitManager()
{
    disconnect(m_pClientInterface, SIGNAL(response(QString,QString,bool,int,DbusPackage)),
            this, SLOT(serviceResponse(QString,QString,bool,int,DbusPackage)));

    disconnect(m_pClientInterface, SIGNAL(messageUpdated(const QString &)),
            this, SLOT(messageUpdate(QString)));
}

void TransmitManager::setStatus(const QString &statusType, const QVariantMap &map)
{
    m_pClientInterface->setStatus(statusType, map);
}

int TransmitManager::requestData(const QVariantMap &head, const QVariantMap &para,
                                 const QString &action, const QString &errorText, bool runInBackground , bool needShowLoading)
{
    DataPackage package;
    if(head["Key"].toString() == "")
        package.head.key = PACKAGE_HEAD_KEY;
    else
        package.head.key = head["Key"].toString();
    if(head["Type"].toString() == "")
        package.head.type = "1";
    else
        package.head.type = head["Type"].toString();

    if(head["Objects"].toString() == ""){
        qDebug() << "head[Objects] == empty  return -1";
        return -1;
    }
    package.head.objects = head["Objects"].toString();
    QMap<QString, QString> tmpPara;
    QMap<QString, QVariant>::const_iterator iter = para.constBegin();
    while (iter != para.constEnd()) {
        tmpPara[iter.key()] = iter.value().toString();
        ++iter;
    }
    package.para = tmpPara;
    sendData(package, action, errorText , runInBackground, needShowLoading, FUNCTION_TYPE_REQUEST);
    return 0;
}

int TransmitManager::submitData(const QVariantMap &head, const QVariantMap &table, QVariant dataRowList,
                                QVariant attachementes, const QString &serialKey, const QString &action,
                                const QString &errorText, bool runInBackground, bool needShowLoading)
{
    QList<QVariant> dataList = dataRowList.toList();
    qDebug() <<Q_FUNC_INFO   << "   \ndataRowList   " << dataList.count();
    DataPackage package;
    DataTable importTable;
    if(head["Key"].toString() == "")
        package.head.key = PACKAGE_HEAD_KEY;
    else
        package.head.key = head["Key"].toString();

    package.head.type = "2";
    if(head["Objects"].toString() == ""){
        qDebug() << "head[Objects] == empty  return -1";
        return -1;
    }
    package.head.objects = head["Objects"].toString();
    if(table.count() > 0){
        importTable.key = table["Key"].toString();
        importTable.name = table["Name"].toString();
        for(int i = 0; i < dataList.count(); i++){
            QVariantMap dataRow = dataList.at(i).toMap();
            if(dataRow.count() > 0){
                QMap<QString, QString> tmpDataRow;
                QMap<QString, QVariant>::const_iterator dataIter = dataRow.constBegin();
                while (dataIter != dataRow.constEnd()) {
                    tmpDataRow[dataIter.key()] = dataIter.value().toString();
                    ++dataIter;
                }
                importTable.value.append(tmpDataRow);
            }
        }
        package.tables.append(importTable);
    }

    QString uuid = serialKey;
    if(uuid == "")
        uuid =  QUuid::createUuid().toString().replace("{","").replace("}","");
    QList<QVariant> attachList = attachementes.toList();
    int i = 0;
    for(i = 0; i < attachList.length(); i++){
        QVariantMap tmpAttach = attachList.at(i).toMap();
        Attachment attmnt;
        attmnt.name = tmpAttach["Name"].toString();
        attmnt.title = tmpAttach["Title"].toString();
        attmnt.description = tmpAttach["Description"].toString();
        attmnt.path = tmpAttach["Path"].toString();
        if(tmpAttach["SerialKey"].toString() == ""){
            attmnt.serialKey = uuid;
        } else {
            attmnt.serialKey = tmpAttach["SerialKey"].toString();
        }
        package.binaries.append(attmnt);
    }
    sendData(package, action, errorText, runInBackground, needShowLoading, FUNCTION_TYPE_SUBMIT);
    return 0;
}

int TransmitManager::submitDataWithPart(const QVariantMap &head, const QVariantMap &table, QVariant dataRowList,
                                         QVariant attachementes, const QVariantMap &partTable, QVariant partDataRowList, const QString &serialKey,
                                         const QString &action, const QString &errorText, bool runInBackground, bool needShowLoading)
{
    QList<QVariant> dataList = dataRowList.toList();
    QList<QVariant> partDataList = partDataRowList.toList();
    qDebug() <<Q_FUNC_INFO   << "   \ndataRowList   " << dataList.count();
    DataPackage package;
    DataTable importTable;
    DataTable importPartTable;
    if(head["Key"].toString() == "")
        package.head.key = PACKAGE_HEAD_KEY;
    else
        package.head.key = head["Key"].toString();

    package.head.type = "2";
    if(head["Objects"].toString() == ""){
        qDebug() << "head[Objects] == empty  return -1";
        return -1;
    }
    package.head.objects = head["Objects"].toString();
    if(table.count() > 0){
        importTable.key = table["Key"].toString();
        importTable.name = table["Name"].toString();
        for(int i = 0; i < dataList.count(); i++){
            QVariantMap dataRow = dataList.at(i).toMap();
            if(dataRow.count() > 0){
                QMap<QString, QString> tmpDataRow;
                QMap<QString, QVariant>::const_iterator dataIter = dataRow.constBegin();
                while (dataIter != dataRow.constEnd()) {
                    tmpDataRow[dataIter.key()] = dataIter.value().toString();
                    ++dataIter;
                }
                importTable.value.append(tmpDataRow);
            }
        }
        package.tables.append(importTable);
    }

    if(partTable.count() > 0){
        importPartTable.key = partTable["Key"].toString();
        importPartTable.name = partTable["Name"].toString();
        for(int i = 0; i < partDataList.count(); i++){
            QVariantMap dataRow = partDataList.at(i).toMap();
            if(dataRow.count() > 0){
                QMap<QString, QString> tmpDataRow;
                QMap<QString, QVariant>::const_iterator dataIter = dataRow.constBegin();
                while (dataIter != dataRow.constEnd()) {
                    tmpDataRow[dataIter.key()] = dataIter.value().toString();
                    ++dataIter;
                }
                importPartTable.value.append(tmpDataRow);
            }
        }
        package.tables.append(importPartTable);
    }

    QString uuid = serialKey;
    if(uuid == "")
        uuid =  QUuid::createUuid().toString().replace("{","").replace("}","");
    QList<QVariant> attachList = attachementes.toList();

    for(int i = 0; i < attachList.length(); i++){
        QVariantMap tmpAttach = attachList.at(i).toMap();
        Attachment attmnt;
        attmnt.name = tmpAttach["Name"].toString();
        attmnt.title = tmpAttach["Title"].toString();
        attmnt.description = tmpAttach["Description"].toString();
        attmnt.path = tmpAttach["Path"].toString();
        if(tmpAttach["SerialKey"].toString() == ""){
            attmnt.serialKey = uuid;
        } else {
            attmnt.serialKey = tmpAttach["SerialKey"].toString();
        }
        package.binaries.append(attmnt);
    }
    sendData(package, action, errorText, runInBackground, needShowLoading, FUNCTION_TYPE_SUBMIT);
    return 0;
}

void TransmitManager::sendOriginalForm(const DataPackage &dataPkg, const QString &action, const QString &errorText){
    sendData(dataPkg, action, errorText, false, true, FUNCTION_TYPE_SUBMIT);
}

int TransmitManager::syncPreTask(const QVariantMap &head, const QVariantMap &para, const QString &action, const QString &errorText, bool runInBackground, bool needShowLoading)
{
    DataPackage package;
    if(head["Key"].toString() == "")
        package.head.key = PACKAGE_HEAD_KEY;
    else
        package.head.key = head["Key"].toString();
    if(head["Type"].toString() == "")
        package.head.type = "1";
    else
        package.head.type = head["Type"].toString();

    if(head["Objects"].toString() == ""){
        qDebug() << "head[Objects] == empty  return -1";
        return -1;
    }
    package.head.objects = head["Objects"].toString();
    QMap<QString, QString> tmpPara;
    QMap<QString, QVariant>::const_iterator iter = para.constBegin();
    while (iter != para.constEnd()) {
        tmpPara[iter.key()] = iter.value().toString();
        ++iter;
    }
    package.para = tmpPara;
    sendData(package, action, errorText , runInBackground, needShowLoading, FUNCTION_TYPE_SYNC);
    return 0;
}

int TransmitManager::requestTaskData(const QVariantMap &head, const QVariantMap &para, const QString &action, const QString &errorText, bool runInBackground, bool needShowLoading)
{
    DataPackage package;
    if(head["Key"].toString() == "")
        package.head.key = PACKAGE_HEAD_KEY;
    else
        package.head.key = head["Key"].toString();
    if(head["Type"].toString() == "")
        package.head.type = "1";
    else
        package.head.type = head["Type"].toString();

    if(head["Objects"].toString() == ""){
        qDebug() << "head[Objects] == empty  return -1";
        return -1;
    }
    package.head.objects = head["Objects"].toString();
    QMap<QString, QString> tmpPara;
    QMap<QString, QVariant>::const_iterator iter = para.constBegin();
    while (iter != para.constEnd()) {
        tmpPara[iter.key()] = iter.value().toString();
        ++iter;
    }
    package.para = tmpPara;
    sendData(package, action, errorText , runInBackground, needShowLoading, FUNCTION_TYPE_REQUEST_TASK);
    return 0;
}

void TransmitManager::requestFaultRepairImageList(const QStringList &fileUrlList, const QString &action,
                                                  const QString &errorText){
    Q_UNUSED(errorText)
    downLoadFile(fileUrlList, action);
}

void TransmitManager::serviceResponse(QString objects, QString action, bool result, int error, DbusPackage map)
{
    QVariantMap tmpMap;
    if(map.otherMap.size() > 0){
        QMap<QString, QString>::const_iterator iter = map.otherMap.constBegin();
        while (iter != map.otherMap.constEnd()) {
            tmpMap[iter.key()] = iter.value();
            ++iter;
        }
    } else
        tmpMap = ConvertData::getInstance()->convert(map.dataPackage);
    qDebug() << Q_FUNC_INFO << "objects " << objects << "  action " << action << "  result " << result << "  error " << error;
//    if(error == NETWORK_ERROR_NONETWORK && m_ActionList.at(0).runInBackground == false){
//        m_pToast->notify("无网络链接");
//        QMetaObject::invokeMethod(m_pViewer->rootObject(), "popToFirstPage" , Qt::DirectConnection);
//    }

    if(objects == "binary" && tmpMap["binaryType"] == "binaryProgress"){
        //qDebug() << Q_FUNC_INFO << "resultOfResponse";
        emit resultOfResponse(result, error, action, tmpMap);
    } else
        sendResult(result, error, action, tmpMap);
}

void TransmitManager::messageUpdate(QString messageType)
{
    qDebug() << "-------TransmitManager::messageUpdate----------messageType: " << messageType;
    emit messageUpdated(messageType);
}

void TransmitManager::showLoading(bool runInBackground, bool needShowLoading)
{
    if(runInBackground){
        qDebug() << Q_FUNC_INFO <<"  run in background nothing todo, return";
        return;
    }
    if(!m_pLoading){
        QQmlEngine *engine = m_pViewer->engine();
        QQmlContext *rootContext = m_pViewer->rootContext();
        QQmlComponent component(engine, QUrl("qrc:/qml/LoginAndHome/Loading.qml"), QQmlComponent::PreferSynchronous);
        QObject *obj = component.create(rootContext);
        QQuickItem *m_rootItem = m_pViewer->rootObject();
        m_pLoading = qobject_cast<QQuickItem *>(obj);
        m_pLoading->setParent(m_rootItem);
        m_pLoading->setParentItem(m_rootItem);
        qDebug() << "ransmitManager::showLoading   m_pViewer->rootObject() is  " << m_rootItem
                 <<"  engine " << engine << "  rootContext is " << rootContext << " obj is " << obj;
    }
    m_pLoading->setProperty("visible", true);
    if(needShowLoading){
        m_pLoading->setProperty("running", true);
    }
}

void TransmitManager::hideLoading()
{
    qDebug()<<"TransmitManager::hide loading";
    if(m_pLoading){
        m_pLoading->setProperty("running", false);
        m_pLoading->setProperty("visible", false);
    }
}

void TransmitManager::sendData(const DataPackage &package, const QString &action,  QString errorText,
                                bool runInBackground, bool needShowLoading, QString functionType)
{
    if(!runInBackground){
       hideLoading();
       showLoading(runInBackground,needShowLoading);
    }
    InspectionActions actiontask;
    actiontask.package = package;
    actiontask.actionName = action;
    actiontask.functionType = functionType;
    actiontask.errorString = errorText;
    actiontask.runInBackground = runInBackground;
    qDebug()<<  Q_FUNC_INFO <<"TransmitManager::append task"<<actiontask.actionName  << "  isRunning  " << isRunnig;
    m_ActionList.append(actiontask);
    if(!isRunnig && !m_ActionList.isEmpty()){
         doTask(m_ActionList.at(0));
    }
}

void TransmitManager::downLoadFile(const QStringList &fileUrlList, const QString &action,
                                   const QString errorText, bool runInBackground, bool needShowLoading){
    QString networkType = NetworkStatus::getInstance()->getNetworkConnectStatus();
    qDebug() << "sendData  network Connected    " << networkType;
    if(networkType.isEmpty()){
        QVariantMap tmp;
        emit resultOfResponse(false, NETWORK_ERROR_NONETWORK, action, tmp);
        if(runInBackground == false){
            QMetaObject::invokeMethod(m_pViewer->rootObject(), "popToFirstPage" , Qt::DirectConnection);
        }
        return;
    }

    if(!runInBackground){
       hideLoading();
       showLoading(runInBackground,needShowLoading);
    }
    DataPackage package;
    InspectionActions actiontask;
    actiontask.downLoadFileList = fileUrlList;
    actiontask.package = package;
    actiontask.actionName = action;
    actiontask.errorString = errorText;
    actiontask.runInBackground = runInBackground;
    qDebug()<< Q_FUNC_INFO <<"TransmitManager::append task"<<actiontask.actionName;
    m_ActionList.append(actiontask);
    if(!isRunnig && !m_ActionList.isEmpty()){
         doTask(m_ActionList.at(0));
    }
}

void TransmitManager::doTask(InspectionActions actiontask)
{
    bool sendPkgRv = true;
    qDebug()<<"doTask TransmitManager::action is"<<actiontask.actionName<<"error is"<<actiontask.errorString;
    sendPkgRv = m_pClientInterface->sendData(actiontask.package, actiontask.actionName, actiontask.functionType);
    if(sendPkgRv == false){
        qDebug() << "TransmitManager::doTask  m_pClientInterface->sendData return false";
        m_ActionList.removeAt(0);
        if(m_ActionList.isEmpty()){
            isRunnig = false;
        }else{
            doTask(m_ActionList.at(0));
        }
    } else
        isRunnig = true;
}

void TransmitManager::sendResult(const bool &result, const int &error, const QString &action, const QVariantMap &map)
{
    //发送列表为空
    if(m_ActionList.isEmpty()){
        qDebug()<<"TransmitManager::done";
        hideLoading();
        emit resultOfResponse(result, error, action, map);
        return;
    }

    InspectionActions actiontask = m_ActionList.at(0);
    qDebug() << "TransmitManager::sendResult  action  " << action << "  actiontask.actionName is " << actiontask.actionName;
    bool hasAction = true;
    //找到网络序列号或者返回service没有注册DBus
    if(actiontask.actionName == action || error == DBUS_ERROR_SERVICE_UNKNOWN){
        if(!actiontask.runInBackground)
            hideLoading();
        m_ActionList.removeAt(0);
    } else {
        hasAction = false;
    }

    QString rvAction = action;
    if(rvAction.isEmpty())
        rvAction = actiontask.actionName;
    emit resultOfResponse(result, error, rvAction, map);

    if(!hasAction)
        return;

    if(error != NETWORK_NO_ERROR && !actiontask.runInBackground){
        if(error == NETWORK_ERROR_NONETWORK){
            m_pToast->notify("无网络链接");
        }else{
            qDebug()<< "TransmitManager::toast" << actiontask.errorString;
            if(!actiontask.errorString.isEmpty()){
                m_pToast->notify(actiontask.errorString);
            }
        }
    }

    if(m_ActionList.isEmpty()){
        isRunnig = false;
    }else{
        doTask(m_ActionList.at(0));
    }
}
