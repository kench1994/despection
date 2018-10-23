#include "networkcontrol.h"
#include <QDebug>

NetworkControl *NetworkControl::m_pNetworkControl = NULL;

/**
 * 构造函数
 */
NetworkControl::NetworkControl(QObject *parent):QObject(parent)
{
    connect(&m_nNetWorkManager,SIGNAL(networkStatusChanged(bool, CNetworkManager::NetworkType)),
            this,  SLOT(slotNetworkStatusChanged(bool, CNetworkManager::NetworkType)));
    CNetworkManager::NetworkType type = m_nNetWorkManager.currNetworkType();
    if(type == CNetworkManager::CellularNetwork)
        m_nConnectNetworkType = "cellular";
    else if (type == CNetworkManager::WifiNetwork)
        m_nConnectNetworkType = "wifi";
    else
        m_nConnectNetworkType = "";
}

/**
 * 单例函数
 */
NetworkControl* NetworkControl::getInstance(QObject *parent){
    if(m_pNetworkControl == NULL)
        m_pNetworkControl = new NetworkControl(parent);
    return m_pNetworkControl;
}

/**
 * 析构函数
 */
NetworkControl::~NetworkControl()
{

}

/**
 * @brief slotNetworkStatusChanged : 网络链接状态变更槽函数
 * @param flag
 * @param type : 网络类型
 */
void NetworkControl::slotNetworkStatusChanged(bool flag, CNetworkManager::NetworkType type)
{
    qDebug() << "------------NetworkControl::slotNetworkStatusChanged-------flag:" << flag << "CNetworkManager::NetworkType:" << type;
    if(type ==  CNetworkManager::CellularNetwork)
        m_nConnectNetworkType = "cellular";
    else if(type == CNetworkManager::WifiNetwork)
        m_nConnectNetworkType = "wifi";
    else
        m_nConnectNetworkType = "";
   qDebug() << "------------NetworkControl::networkConnectChanged-------";
   emit networkConnectChanged(m_nConnectNetworkType);

}

/**
 * @brief getNetworkConnectStatus : 网络链接状态
 * @return : "wifi":wlan； "cellular"：移动网络；"":无网络
 */
QString NetworkControl::getNetworkConnectStatus()
{
    qDebug() << "------------NetworkControl::getNetworkConnectStatus-------net type:" << m_nConnectNetworkType;
    return m_nConnectNetworkType;
}
