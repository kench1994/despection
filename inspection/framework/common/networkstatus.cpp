#include "networkstatus.h"
#include <QDebug>

NetworkStatus *NetworkStatus::m_pNetworkStatus = NULL;
NetworkStatus::NetworkStatus(QObject *parent):QObject(parent)
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

NetworkStatus* NetworkStatus::getInstance(){
    if(m_pNetworkStatus == NULL)
        m_pNetworkStatus = new NetworkStatus();
    return m_pNetworkStatus;
}

NetworkStatus::~NetworkStatus()
{
}

void NetworkStatus::slotNetworkStatusChanged(bool flag, CNetworkManager::NetworkType type)
{
    qDebug() << "------------NetworkStatus::slotNetworkStatusChanged-------flag:" << flag << "CNetworkManager::NetworkType:" << type;
    if(type ==  CNetworkManager::CellularNetwork)
        m_nConnectNetworkType = "cellular";
    else if(type == CNetworkManager::WifiNetwork)
        m_nConnectNetworkType = "wifi";
    else
        m_nConnectNetworkType = "";
   qDebug() << "------------NetworkStatus::networkConnectChanged-------";
   emit networkConnectChanged(m_nConnectNetworkType);
}

QString NetworkStatus::getNetworkConnectStatus()
{
    qDebug() << "------------NetworkStatus::getNetworkConnectStatus-------net type:" << m_nConnectNetworkType;
    return m_nConnectNetworkType;
}
