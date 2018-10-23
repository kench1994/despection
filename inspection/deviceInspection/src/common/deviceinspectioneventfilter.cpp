#include "deviceinspectioneventfilter.h"

DeviceInspectionEventFilter* DeviceInspectionEventFilter::m_pDinseEventfilter = NULL;

DeviceInspectionEventFilter::DeviceInspectionEventFilter(QObject *parent) :
    QObject(parent)
{
}

DeviceInspectionEventFilter *DeviceInspectionEventFilter::getInstance()
{
    if(m_pDinseEventfilter == NULL)
        m_pDinseEventfilter = new DeviceInspectionEventFilter();
    return m_pDinseEventfilter;
}

bool DeviceInspectionEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QString keyType;
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        switch(keyEvent->key()){
            case Qt::Key_Home:{
                qDebug() << "=======DeviceInspectionEventFilter=========Key_Home===========";
                //emit toHomeTotal();
                keyType = "KEY_HOME";
                break;
            }
            case Qt::Key_Camera:{
                qDebug() << "=======DeviceInspectionEventFilter=========Key_Camera===========";
                //emit openCameraTotal();
                keyType = "KEY_CAMERA";
                break;
            }
            case Qt::Key_Launch0:{
               // qDebug() << "=======DeviceInspectionEventFilter=========Key_Launch0===========";
                //emit scanRFIDTotal();
            keyType = "KEY_RFID";
                break;
            }
            case Qt::Key_Launch1:{
                //qDebug() << "=======DeviceInspectionEventFilter=========Key_Launch1===========";
                //emit scanBarCodeTotal();
            keyType = "KEY_BARSCAN";
                break;
            }
            default:
            keyType = "KEY_UNKOWN";
                break;
        }
        if(keyType != "KEY_UNKOWN")
            emit shorCutPressed(keyType);
    }
    return QObject::eventFilter(obj, event);
}
