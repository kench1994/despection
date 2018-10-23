#ifndef DEVICEINSPECTIONEVENTFILTER_H
#define DEVICEINSPECTIONEVENTFILTER_H

#include <QObject>
#include <QKeyEvent>
#include <QDebug>

class DeviceInspectionEventFilter : public QObject
{
    Q_OBJECT
      //Q_PROPERTY(KeyboardType keyBoardType READ keyType WRITE setkeyType)
 Q_ENUMS(KeyboardType)
public:
    explicit DeviceInspectionEventFilter(QObject *parent = 0);
    static DeviceInspectionEventFilter* getInstance();
//    enum KeyboardType {
//        KEY_CAMERA = 0,
//        KEY_BARSCAN,
//        KEY_RFID,
//        KEY_HMOE,
//        KEY_UNKOWN
//    };
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private :
    static DeviceInspectionEventFilter *m_pDinseEventfilter;///<  本地单例对象


signals:
    void shorCutPressed(const QString &keyType);
//    void openCameraTotal();
//    void scanBarCodeTotal();
//    void scanRFIDTotal();
//    void toHomeTotal();

public slots:
//private:
//    KeyboardType keyType;

};

#endif // DEVICEINSPECTIONEVENTFILTER_H
