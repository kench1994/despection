#include "usbtool.h"
#include "../../common/utils.h"

USBTool::USBTool()
{
    iTimeout = NETWORK_TIMEOUT_ASYN;
}

USBTool::~USBTool(){
    if(isConnected()) {
        disconnect();
    }
}

bool USBTool::isConnected()
{
    return bConnected;
}

int USBTool::connectToServer()
{
    return NETWORK_NO_ERROR;
}

int USBTool::connectToServer(QString address, int port)
{
    QMutexLocker sockLocker(&mMutex);
    Q_UNUSED(address);
    Q_UNUSED(port);
    return NETWORK_NO_ERROR;
}

int USBTool::sendData(QByteArray& data)
{
    Q_UNUSED(data);
    QMutexLocker sockLocker(&mMutex);
    return 0;
}

int USBTool::sendData(char* data, int len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);
    QMutexLocker sockLocker(&mMutex);
    return 0;
}

int USBTool::recvData(char* data, const int& len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);
    QMutexLocker sockLocker(&mMutex);
    return 0;
}

int USBTool::recvData(QByteArray& data, const int& len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);
    QMutexLocker sockLocker(&mMutex);
    return 0;
}

void USBTool::disconnect()
{
    QMutexLocker sockLocker(&mMutex);
}

int USBTool::getTimeout()
{
    return iTimeout;
}

void USBTool::setTimeout(int value)
{
    iTimeout = value;
}

bool USBTool::hasDataIncome()
{
    return false;
}
