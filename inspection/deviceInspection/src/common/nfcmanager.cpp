#include "nfcmanager.h"
#include <QDebug>

#define DBG_TAG "NfcManager : "

NfcManager* NfcManager::m_pInstance = NULL;

NfcManager::NfcManager(QObject *parent) :
    QObject(parent)
{
    m_pNearFieldManager = new QNearFieldManager();
    connect(m_pNearFieldManager, SIGNAL(targetDetected(QNearFieldTarget*)),
            this, SLOT(targetDetected(QNearFieldTarget*)));
    connect(m_pNearFieldManager, SIGNAL(targetLost(QNearFieldTarget*)),
            this, SLOT(targetLost(QNearFieldTarget*)));
}

NfcManager::~NfcManager()
{
    m_pNearFieldManager->stopTargetDetection();
    delete m_pNearFieldManager;
    m_pNearFieldManager = NULL;
    m_pInstance = NULL;
}

NfcManager* NfcManager::getInstance()
{
    if(m_pInstance == NULL){
        m_pInstance = new NfcManager();
    }
    return m_pInstance;
}

void NfcManager::targetDetected(QNearFieldTarget *target)
{
    qDebug() << DBG_TAG << "target detected uid:[" << target->uid().toHex() << "]";
    QString uid;
    uid = target->uid().toHex();
    emit done(uid);
}

void NfcManager::targetLost(QNearFieldTarget *target)
{
    //Q_UNUSED(target);
    target->deleteLater();
    qDebug() << DBG_TAG << "Target Lost";
}

void NfcManager::startDetection()
{
    QString str;
    if(m_pNearFieldManager){
        bool ret = m_pNearFieldManager->startTargetDetection();
        if(!ret){
            str = "Couldn't start detecting";
            emit error(str);
        }
    }else{
        str = "Object doesn't existed";
        emit error(str);
    }
}

void NfcManager::stopDetection()
{
    if(m_pNearFieldManager){
        m_pNearFieldManager->stopTargetDetection();
    }
}
