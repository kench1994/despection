#include "baseform.h"
#include "QDomDocument"
#include "formConfigType.h"
#include "../common/runtimeparam.h"
#include <QDateTime>
#include <QDebug>
BaseForm::BaseForm(const QVariantMap &deviceValue, const QVariantMap &dataPackage, const QString &pageFormType,
                   QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem)
    :m_pChoiceDialogItem(choiceDialogItem),
      m_pContentColItem(contentColItem),
      m_pRootContext(rootContext),
      m_nDeviceValue(deviceValue),
      m_nDataPackage(dataPackage),
      m_nPageFormType(pageFormType)

{
    parseXml();
}

BaseForm::~BaseForm(){
    qDebug() << "BaseForm::~BaseForm()";
    for(int i = 0; i < m_pBaseWidgetList.length(); i++){
        delete m_pBaseWidgetList.at(i);
    }
    m_pBaseWidgetList.clear();
    qDebug() << "BaseForm::~BaseForm() after delete ,m_pBaseWidgetList.length is " << m_pBaseWidgetList.length();
}

int BaseForm::parseXml(){
    QVariantMap table = m_nDataPackage["tables"].toList().at(0).toMap();
    QVariantMap dataRow = table["value"].toList().at(0).toMap();
    QString itemSchema = dataRow["ItemSchema"].toString();

    itemSchema = itemSchema.replace("&amp;", "&");
    itemSchema = itemSchema.replace("&quot;", "\"");
    itemSchema = itemSchema.replace("&apos;", "'");
    itemSchema = itemSchema.replace("&lt;", "<");
    itemSchema = itemSchema.replace("&gt;", ">");

    QDomDocument doc("ItemSchemaDocument");
    QString errorMsg;
    if(!doc.setContent(itemSchema, true, &errorMsg)){
       qDebug()<<"load file error setcontent  "<<errorMsg;
    }
    QDomNode indicatorEnums = doc.elementsByTagName("IndicatorEnums").at(0);
    QDomNodeList indicatorEnumList = indicatorEnums.toElement().elementsByTagName("IndicatorEnum");

    QDomNode indicatorsNode = doc.elementsByTagName("Indicators").at(0);
    QDomNodeList indicatorNodeList = indicatorsNode.toElement().elementsByTagName("Indicator");

    for(int i = 0; i < indicatorNodeList.count(); i++){
        QDomNode tmpNode = indicatorNodeList.at(i);
        QDomNamedNodeMap domMap = tmpNode.attributes();
        qDebug() <<"indicatorNodeList ------------  size is " <<domMap.size();
        QVariantMap formValue;
        for(int i = 0; i < domMap.size(); i++){
            QString key = domMap.item(i).nodeName();
            QString value = domMap.item(i).nodeValue();
            formValue.insert(key, value);
        }
        m_nFormValueList.append(formValue);
    }

    for(int i = 0; i < indicatorEnumList.count(); i++){
        QDomNode tmpNode = indicatorEnumList.at(i);
        QDomNamedNodeMap domMap = tmpNode.attributes();
        qDebug() <<"indicatorEnumList ------------  size is " <<domMap.size();
        QVariantMap enumValue;
        for(int i = 0; i < domMap.size(); i++){
            QString key = domMap.item(i).nodeName();
            QString value = domMap.item(i).nodeValue();
            enumValue.insert(key, value);
        }
        m_nFormEnumList.append(enumValue);
    }
    return 0;
}

QString BaseForm::getPageFormType() const{
    return m_nPageFormType;
}

int BaseForm::checkInputLegality(const int &runStatus){
    int rv = 0;
    QVariantMap errorMap;
    for(int i = 0; i < m_pBaseWidgetList.count(); i++){
        rv = m_pBaseWidgetList.at(i)->checkInputLegality(runStatus, errorMap);
        if(rv != 0){
            QString label = errorMap["Label"].toString();
            QString dataType = errorMap["DataType"].toString();
            QString percision = errorMap["Precision"].toString();
            emit errorOccurredWhenCheckLegality(label, rv, dataType, percision);
            break;
        }
    }
    return rv;
}

QString BaseForm::getResult(){
    RuntimeParam *runParam = RuntimeParam::getInstance();
    QDomDocument document;
    QDomElement itemDatas = document.createElement("ItemDatas");

    QDomElement itemdata = document.createElement("ItemData");
    itemdata.setAttribute("CreatedBy", runParam->getUserId());
    itemdata.setAttribute("CreatedDate", QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
    for(int i = 0; i < m_pBaseWidgetList.count(); i++){
        QDomElement data;
        QString elementText = m_pBaseWidgetList.at(i)->getResult();
        if(elementText.isEmpty())
            continue;
        QDomDocument tmpDoc;
        tmpDoc.setContent(elementText, true);
        data = tmpDoc.documentElement();
        itemdata.appendChild(data);
    }
    itemDatas.appendChild(itemdata);
    document.appendChild(itemDatas);
    QString itemDatasStr = document.toString();
    itemDatasStr = itemDatasStr.replace("\n", "");
    qDebug() << "itemDatasStr is " << itemDatasStr << "  : ok\n\n";
    return itemDatasStr;
}

void BaseForm::getDataRow(QMap<QString, QString> &dataRow, const QVariantMap &otherParam)
{
    Q_UNUSED(otherParam)
    qDebug() << dataRow;
}


