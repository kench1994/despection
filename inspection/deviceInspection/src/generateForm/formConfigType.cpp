#include "formConfigType.h"
#include <QDebug>
FormConfigType* FormConfigType::m_pFormConfigType = NULL;
FormConfigType* FormConfigType::getInstance(){
    if(m_pFormConfigType == NULL)
        m_pFormConfigType = new FormConfigType();
    return m_pFormConfigType;
}

FormConfigType::FormConfigType()
{
    qDebug() << "FormConfigType::FormConfigType()0000000000000000000000000";
    widgetPagePath_map.insert(INSPECTION_WIDGET_INPUT, "qrc:/qml/Common/ListInputItem.qml");
    widgetPagePath_map.insert(INSPECTION_WIDGET_ENUM, "qrc:/qml/Common/ListItem.qml");
    widgetPagePath_map.insert(INSPECTION_WIDGET_DATETIME, "qrc:/qml/Common/ListItem.qml");
    widgetPagePath_map.insert(INSPECTION_WIDGET_DATE, "qrc:/qml/Common/ListItem.qml");
    widgetPagePath_map.insert(INSPECTION_WIDGET_TIME, "qrc:/qml/Common/ListItem.qml");


    widgetPagePath_map.insert(MAINTAIN_WIDGET_INPUT, "qrc:/qml/Common/ListInputItem.qml");
    widgetPagePath_map.insert(MAINTAIN_WIDGET_ENUM, "qrc:/qml/Common/ListItem.qml");
    widgetPagePath_map.insert(MAINTAIN_WIDGET_DATETIME, "qrc:/qml/Common/ListItem.qml");
    widgetPagePath_map.insert(MAINTAIN_WIDGET_DATE, "qrc:/qml/Common/ListItem.qml");
    widgetPagePath_map.insert(MAINTAIN_WIDGET_TIME, "qrc:/qml/Common/ListItem.qml");

    widgetPagePath_map.insert(STANDARD_WIDGET_INPUT, "qrc:/qml/StandardizedTask/DeviceStdCheckFormInputItem.qml");
    widgetPagePath_map.insert(STANDARD_WIDGET_ENUM, "qrc:/qml/StandardizedTask/DeviceStdCheckFormOnlyEnum.qml");
    widgetPagePath_map.insert(STANDARD_WIDGET_INPUT_ENUM, "qrc:/qml/StandardizedTask/DeviceStdCheckFormListItem.qml");
    widgetPagePath_map.insert(STANDARD_WIDGET_CHECK, "qrc:/qml/StandardizedTask/DeviceStdCheckFormOnlyCheck.qml");

    widgetPagePath_map.insert(DATAREPORT_WIDGET_INPUT, "qrc:/qml/Common/ListInputItem.qml");
    widgetPagePath_map.insert(DATAREPORT_WIDGET_MULTIINPUT, "qrc:/qml/Common/ListMultiInputItem.qml");
    widgetPagePath_map.insert(DATAREPORT_WIDGET_ENUM, "qrc:/qml/Common/ListItem.qml");
    widgetPagePath_map.insert(DATAREPORT_WIDGET_DATETIME, "qrc:/qml/Common/ListItem.qml");
    widgetPagePath_map.insert(DATAREPORT_WIDGET_DATE, "qrc:/qml/Common/ListItem.qml");
    widgetPagePath_map.insert(DATAREPORT_WIDGET_TIME, "qrc:/qml/Common/ListItem.qml");
}

QMap<int , QString> FormConfigType::getWidgetPagePathMap(){
    return widgetPagePath_map;
}

QStringList FormConfigType::getWidgetSubmitKeyList(baseWidgetType widgetType){
    QStringList submitKeyList;
    switch (widgetType) {
    case INSPECTION_WIDGET_INPUT:
    case INSPECTION_WIDGET_ENUM:
    case INSPECTION_WIDGET_DATETIME:
    case INSPECTION_WIDGET_DATE:
    case INSPECTION_WIDGET_TIME:
        submitKeyList.append(QString(SUBMIT_NODE_INDICATORNAME));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_INDICATORCODE));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_DISPLAYNAME));
        submitKeyList.append(QString(SUBMIT_NODE_PARENTINDICATORID));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_INDICATORID));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_MODELITEMID));
        break;
    case MAINTAIN_WIDGET_INPUT:
    case MAINTAIN_WIDGET_ENUM:
    case MAINTAIN_WIDGET_DATETIME:
    case MAINTAIN_WIDGET_DATE:
    case MAINTAIN_WIDGET_TIME:
        submitKeyList.append(QString(SUBMIT_NODE_INDICATORNAME));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_INDICATORID));
        submitKeyList.append(QString(SUBMIT_NODE_PARENTINDICATORID));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_INDICATORCODE));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_DISPLAYNAME));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_MODELITEMID));
        break;

    case DATAREPORT_WIDGET_INPUT:
    case DATAREPORT_WIDGET_ENUM:
    case DATAREPORT_WIDGET_DATETIME:
    case DATAREPORT_WIDGET_DATE:
    case DATAREPORT_WIDGET_TIME:
    case DATAREPORT_WIDGET_MULTIINPUT:
        submitKeyList.append(QString(SUBMIT_NODE_ENGLISHNAME));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_INDICATORID));
        submitKeyList.append(QString(SUBMIT_NODE_INDICATORNAME));
        break;
    case STANDARD_WIDGET_CHECK:
        submitKeyList.append(QString(SUBMIT_NODE_INDICATORNAME));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_INDICATORID));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_DISPLAYNAME));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_ISCOUNTER));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_INDICATORTYPE));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_SCORE));
        break;
    case STANDARD_WIDGET_ENUM:
    case STANDARD_WIDGET_INPUT_ENUM:
    case STANDARD_WIDGET_INPUT:
        submitKeyList.append(QString(SUBMIT_NODE_INDICATORNAME));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_INDICATORID));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_DISPLAYNAME));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_ISCOUNTER));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_INDICATORTYPE));
        submitKeyList.append(QString(SUBMIT_NODE_ATTR_COUNTUNITNAME));
        break;
    default:
        break;
    }
    return submitKeyList;
}
