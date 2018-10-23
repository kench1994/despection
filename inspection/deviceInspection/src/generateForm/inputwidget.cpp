#include "inputwidget.h"
#include <QQuickItem>
#include <QDomElement>
InputWidget::InputWidget(QQmlContext *rootContext, QQuickItem *contentColItem,
                         baseWidgetType widgetType, const QString &formType, QVariantMap formValue):
    BaseWidget(rootContext, contentColItem, widgetType, formType, formValue)
{
    qDebug() << "InputWidget::InputWidget constructor";
}

InputWidget::~InputWidget(){
    qDebug() << "InputWidget::~InputWidget()";
}

int InputWidget::showWidget(){
    qDebug() << "InputWidget::showWidget()";
    if(m_pItem == NULL)
        BaseWidget::showWidget();
    QString formType = this->getFormType();
    if(formType == PAGE_FORM_TYPE_STANDARD)
        m_pItem->setProperty("unit", m_nFormValueMap[SUBMIT_NODE_ATTR_COUNTUNITNAME].toString());
    else if(formType == PAGE_FORM_TYPE_INSPECTION || formType == PAGE_FORM_TYPE_DATAREPORT ||
            formType == PAGE_FORM_TYPE_DAILYTASK || formType == PAGE_FORM_TYPE_MAINTAIN)
        m_pItem->setProperty("inputText", m_nFormValueMap[SUBMIT_NODE_ATTR_DEFAULTVALUE].toString());
    return 0;
}
//"EnglishName,IndicatorID,IndicatorName,Value,IsNormal"
//value, isNormal:手动输入字段
QString InputWidget::getResult(){
    QString elementText = BaseWidget::getResult();
    if(elementText.isEmpty())
        return "";
    QDomDocument document;
    document.setContent(elementText, true);
    QDomElement element = document.documentElement();
    qDebug() << "000000000000000000000000  formtype is  " << getFormType();
    QString inputValue = m_pItem->property("inputText").toString();
    if(getFormType() == PAGE_FORM_TYPE_INSPECTION
            || getFormType() == PAGE_FORM_TYPE_DAILYTASK
            || getFormType() == PAGE_FORM_TYPE_MAINTAIN
            || getFormType() == PAGE_FORM_TYPE_DATAREPORT){
        QString isNormal = checkInputIsNormal();
        element.setAttribute(SUBMIT_NODE_ATTR_VALUE, inputValue);
        element.setAttribute(SUBMIT_NODE_ATTR_ISNORMAL, isNormal);
        setExternalParameters(SUBMIT_NODE_ATTR_ISNORMAL, isNormal);
    } else if(getFormType() == PAGE_FORM_TYPE_STANDARD){
        QString counter = inputValue;
        if(counter.isEmpty())
            counter = "1";
        element.setAttribute(SUBMIT_NODE_ATTR_COUNTER, counter);
        QString standardScore = m_nFormValueMap[SUBMIT_STANDARDSCORE].toString();
        double score = counter.toInt() * standardScore.toDouble();
        element.setAttribute(SUBMIT_NODE_ATTR_SCORE, QString("%1").arg(score, 0, 'f', 2));
        setExternalParameters(SUBMIT_NODE_ATTR_SCORE, score);
    }
    return document.toString();
}

//return  0:合法  1:必填 2:运行时必填 3:输入不合法; errorMap:错误信息;  2 二期不用
int InputWidget::checkInputLegality(const int &runStatus, QVariantMap &errorMap){
    Q_UNUSED(runStatus)
    if(getFormType() != PAGE_FORM_TYPE_INSPECTION
            && getFormType() != PAGE_FORM_TYPE_DAILYTASK
            && getFormType() != PAGE_FORM_TYPE_MAINTAIN
            && getFormType() != PAGE_FORM_TYPE_DATAREPORT)
        return 0;
    int rv = 0;
    QString inputValue = m_pItem->property("inputText").toString();
    if (inputValue != ""){
        if(m_nFormValueMap["DataType"].toString() == "StringType"){
            rv = 0;
        } else if (m_nFormValueMap["DataType"].toString() == "IntType"){
            QRegExp rx("^[1-9][0-9]*$");
            if(rx.exactMatch(inputValue) == false)
                rv = 3;
        } else if (m_nFormValueMap["DataType"].toString() == "DecimalType"){
            QString precision = m_nFormValueMap["Precision"].toString();
            QString regExp = QString("^-?\\d+\\.\\d{0,") +  precision + QString("}$");
            QRegExp rxDouble(regExp);
            QRegExp rxInt("^[1-9][0-9]*$");
            if(rxDouble.exactMatch(inputValue) == false && rxInt.exactMatch(inputValue) == false)
                rv = 3;
        } else
            rv = 3;
    } else {
        rv = 1;
    }
    if(rv != 0){
        errorMap["Label"] = m_nFormValueMap[WIDGET_MAINTITLE];
        errorMap["DataType"] = m_nFormValueMap["DataType"];
        errorMap["Precision"] = m_nFormValueMap["Precision"];
    }
    return rv;
}

QString InputWidget::checkInputIsNormal(){
    if(m_nFormValueMap[WIDGET_DATATYPE].toString() == WIDGET_DATATYPE_STRINGTYPE){
        return "1";
    } else if (m_nFormValueMap[WIDGET_DATATYPE].toString() == WIDGET_DATATYPE_INTTYPE){
        int upperLimit = m_nFormValueMap["UpperLimit"].toInt();
        int lowerLimit = m_nFormValueMap["LowerLimit"].toInt();
        int dataValue = m_pItem->property("inputText").toInt();
        if(dataValue >= lowerLimit && dataValue <= upperLimit){
            return "1";
        }else
            return "0";
    } else if(m_nFormValueMap[WIDGET_DATATYPE].toString() == WIDGET_DATATYPE_DECIMALTYPE){
        double upperLimit = m_nFormValueMap["UpperLimit"].toDouble();
        double lowerLimit = m_nFormValueMap["LowerLimit"].toDouble();
        double dataValue = m_pItem->property("inputText").toDouble();
        if(dataValue >= lowerLimit && dataValue <= upperLimit){
            return "1";
        } else
            return "0";
    }
    return "0";
}

