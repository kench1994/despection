#include "generateForm.h"
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include "common/transmitmanager.h"
#include "../common/runtimeparam.h"

static QString actionSequence;
GenerateForm::GenerateForm(QObject *parent):QObject(parent){
    qDebug() << Q_FUNC_INFO;
    m_pViewer = RuntimeParam::getInstance()->getViewer();
    m_pTransMng = TransmitManager::getInstance();
    m_pRootContext = NULL;
    m_pContentColItem = NULL;
    m_pChoiceDialogItem = NULL;
    m_pChoiceDialogItem = NULL;
    m_pBaseForm = NULL;
}

GenerateForm::~GenerateForm(){
    qDebug() <<"GenerateForm::~GenerateForm() ";
    for(int i = 0; i < m_pBaseFormList.count(); i++){
        BaseForm *pBaseForm;
        pBaseForm = m_pBaseFormList.at(i);
        if(pBaseForm != NULL){
            QObject::disconnect(pBaseForm, SIGNAL(errorOccurredWhenCheckLegality(QString,int,QString,QString)),
                                this, SIGNAL(checkLegalityErrorOccurred(QString,int,QString,QString)));
            delete pBaseForm;
            pBaseForm = NULL;
            qDebug() << "parseFormXml   delete pBaseForm ";
        }
    }
    m_pBaseFormList.clear();
}

void GenerateForm::setRootObjName(const QString &rootObjName){
    m_nRootObjName = rootObjName;
}

QString GenerateForm::getRootObjName() const{
    return m_nRootObjName;
}

void GenerateForm::setContentColObjName(const QString & contentColObjName){
    m_nContentColObjName = contentColObjName;
}

QString GenerateForm::getContentColObjName() const{
    return m_nContentColObjName;
}

void GenerateForm::setFormType(const QString &formType){
    qDebug() << "void GenerateForm::setFormType   " << formType;
    m_nFormType = formType;
}

QString GenerateForm::getFormType() const{
    return m_nFormType;
}

void GenerateForm::setChoiceDialogObjName(const QString &choiceDialogObjName){
    m_nChoiceDialogObjName = choiceDialogObjName;
}

QString GenerateForm::getChoiceDialogObjName() const{
    return m_nChoiceDialogObjName;
}

void GenerateForm::setDeviceValue(QVariantMap deviceValue){
    m_nDeviceValue = deviceValue;
    qDebug() << "=======================================";
    qDebug() <<m_nDeviceValue;
    qDebug() << "=======================================";
}

QVariantMap GenerateForm::getDeviceValue(){
    return m_nDeviceValue;
}

void GenerateForm::setDataPackage(QVariantMap dataPackage){
    m_nDataPackage = dataPackage;
}

QVariantMap GenerateForm::getDataPackage(){
    return m_nDataPackage;
}

void GenerateForm::getItemPtr(){
    if(m_pRootContext == NULL){
        m_pRootContext = m_pViewer->rootContext();
        QQuickItem *rootItem = m_pViewer->rootObject();
        m_pContentColItem = rootItem->findChild<QQuickItem *>(m_nContentColObjName);
        m_pChoiceDialogItem = rootItem->findChild<QQuickItem *>(m_nChoiceDialogObjName);
    }
}

void GenerateForm::initializeForm(){
    getItemPtr();
    parseFormXml();
}

void GenerateForm::parseFormXml(){
    //qDebug() << "void GenerateForm::parseFormXml:  " << m_nDataPackage;
    if(m_nDataPackage["tables"].toList().length() == 0){
        // throw warining data error
        qDebug() << "m_nDataPackage[tables].toList().length() == 0";
        return ;
    }
    QVariantMap table = m_nDataPackage["tables"].toList().at(0).toMap();
    if(table["value"].toList().length() == 0){
        //throw warining data error
        qDebug() << "table[value].toList().length() == 0";
        return ;
    }
    for(int i = 0; i < m_pBaseFormList.count(); i++){
        BaseForm *pBaseForm;
        pBaseForm = m_pBaseFormList.at(i);
        if(pBaseForm != NULL){
            QObject::disconnect(pBaseForm, SIGNAL(errorOccurredWhenCheckLegality(QString,int,QString,QString)),
                                this, SIGNAL(checkLegalityErrorOccurred(QString,int,QString,QString)));
            delete pBaseForm;
            pBaseForm = NULL;
            qDebug() << "parseFormXml   delete pBaseForm ";
        }
    }
    m_pBaseFormList.clear();

    if(m_nFormType == PAGE_FORM_TYPE_INSPECTION){
        BaseForm *pBaseForm;
        pBaseForm = new InspectionForm(m_nDeviceValue, m_nDataPackage, m_pRootContext,
                                   m_pContentColItem, m_pChoiceDialogItem);
        m_pBaseFormList.append(pBaseForm);
        QObject::connect(pBaseForm, SIGNAL(errorOccurredWhenCheckLegality(QString,int,QString,QString)),
                this, SIGNAL(checkLegalityErrorOccurred(QString,int,QString,QString)));
    } else if(m_nFormType == PAGE_FORM_TYPE_MAINTAIN){
        BaseForm *pBaseForm;
        pBaseForm = new MaintainForm(m_nDeviceValue, m_nDataPackage, m_pRootContext,
                                m_pContentColItem, m_pChoiceDialogItem);
        m_pBaseFormList.append(pBaseForm);
        QObject::connect(pBaseForm, SIGNAL(errorOccurredWhenCheckLegality(QString,int,QString,QString)),
                this, SIGNAL(checkLegalityErrorOccurred(QString,int,QString,QString)));
    } else if(m_nFormType == PAGE_FORM_TYPE_STANDARD){
        BaseForm *pBaseForm;
        pBaseForm = new StandardForm(m_nDeviceValue, m_nDataPackage, m_pRootContext,
                                m_pContentColItem, m_pChoiceDialogItem);
        m_pBaseFormList.append(pBaseForm);
        QObject::connect(pBaseForm, SIGNAL(errorOccurredWhenCheckLegality(QString,int,QString,QString)),
                this, SIGNAL(checkLegalityErrorOccurred(QString,int,QString,QString)));
    } else if(m_nFormType == PAGE_FORM_TYPE_DAILYTASK){
        BaseForm *pBaseForm;
        pBaseForm = new DailyTaskForm(m_nDeviceValue, m_nDataPackage, m_pRootContext,
                                   m_pContentColItem, m_pChoiceDialogItem);
        m_pBaseFormList.append(pBaseForm);
        QObject::connect(pBaseForm, SIGNAL(errorOccurredWhenCheckLegality(QString,int,QString,QString)),
                this, SIGNAL(checkLegalityErrorOccurred(QString,int,QString,QString)));
    } else if(m_nFormType == PAGE_FORM_TYPE_DATAREPORT){
        int i = 0;
        qDebug() << "value list length is  " << table["value"].toList().length();
        for(; i < table["value"].toList().length(); i++){
            QVariantMap tmpMap;
            tmpMap["head"] = m_nDataPackage["head"];
            QList<QVariant> tablesmap;
            QVariantMap tabmap;
            QList<QVariant> valuemap;
            tabmap.insert("name", m_nDataPackage["tables"].toList()[0].toMap()["name"]);
            tabmap.insert("key", m_nDataPackage["tables"].toList()[0].toMap()["key"]);

            valuemap.append(m_nDataPackage["tables"].toList()[0].toMap()["value"].toList().at(i));
            tabmap.insert("value", QVariant(valuemap));

            tablesmap.append(QVariant(tabmap));
            tmpMap.insert("tables", QVariant(tablesmap));
            qDebug() << "tmpMap is  " << tmpMap;
            BaseForm *pBaseForm;
            pBaseForm = new DataReportForm(m_nDeviceValue, tmpMap, m_pRootContext,
                                             m_pContentColItem, m_pChoiceDialogItem);
            m_pBaseFormList.append(pBaseForm);
            QObject::connect(pBaseForm, SIGNAL(errorOccurredWhenCheckLegality(QString,int,QString,QString)),
                    this, SIGNAL(checkLegalityErrorOccurred(QString,int,QString,QString)));
        }
    }
    emit createComponentFinished();
}

QVariantMap GenerateForm::getItemData(const QVariantMap &otherParam)
{
    QMap<QString, QString> dataRow;
    BaseForm *pBaseForm;
    pBaseForm = m_pBaseFormList.at(0);
    pBaseForm->getDataRow(dataRow, otherParam);
    QVariantMap dataRowMap;
    QMap<QString, QString>::const_iterator iter;
    for (iter = dataRow.constBegin(); iter != dataRow.constEnd(); ++iter)
        dataRowMap[iter.key()] = iter.value();
    return  dataRowMap;
}

int GenerateForm::checkFormInputLegality()
{
    for(int i = 0; i < m_pBaseFormList.count(); i++){
        //检查输入型控件输入值的格式
        int rv = m_pBaseFormList.at(i)->checkInputLegality();
        if(rv != 0){
            qDebug() << "checkInputLegality  != 0 return;";
            return rv;
        }
    }
    return 0;
}
