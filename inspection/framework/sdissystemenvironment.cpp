#include "sdissystemenvironment.h"
#include <QDebug>
#include "csystemdevicemanager.h"

QString SDISSystemEnvironment::deviceID = "";
QString SDISSystemEnvironment::getDeviceID()
{
    if(deviceID == ""){
        CSystemDeviceManager sysDeviceMng;
        QString imei  = sysDeviceMng.imei(0);
        qDebug() << "--------SDISSystemEnvironment::getDeviceID------------imei: " << imei;
        if(!imei.isEmpty()){
            deviceID = imei;
        }else{
            deviceID = "1111111111";
        }
    }
    return deviceID;
}
