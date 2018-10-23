#include <QTimer>
#include <QProcess>
#include <QDir>
#include <QByteArray>
#include <QFile>
#include <QCryptographicHash>
#include <QDebug>
#include <QUrl>
#include <errno.h>
#include "../../../framework/common/systemconfig.h"
#include "upgradeconfig.h"
#include "upgradeThread.h"

#define   TAG  "UpgradeThread: "
#define DEVICEINSPECTION_SOP_NAME "deviceInspection.sop"
const char * sopid = "com.syberos.deviceInspection";

UpgradeThread* UpgradeThread::m_pUpgrade = NULL;

UpgradeThread::UpgradeThread(QObject *parent) :QThread(parent){
    initialize();
}
UpgradeThread *UpgradeThread::getInstance(){
    if(m_pUpgrade == NULL)
        m_pUpgrade = new UpgradeThread();
    return m_pUpgrade;
}

UpgradeThread::~UpgradeThread(){
    qDebug() << TAG << "UpgradeThread--------------- quit.";
    quit();
}

void UpgradeThread::initialize(){
  // this->moveToThread(this);
    connect(&m_mng, SIGNAL(installStatusChanged(const QString&, const QString&, CPackageInfo::PackageStatus,CPackageInfo::PackageError, int)),
           this,    SLOT(recvInstallProcess(QString,QString,CPackageInfo::PackageStatus,CPackageInfo::PackageError,int)),Qt::DirectConnection) ;

    QDir dir(BINARY_TMP_PATH);
    if(dir.exists() == false) {
        if (dir.mkdir(BINARY_TMP_PATH) == false){
            qWarning() << TAG << "mkdir " << BINARY_TMP_PATH << "failed.";
            return ;
        }
    }
    qDebug() << TAG << "package install start.";
}

void UpgradeThread::run(){
    qDebug() << TAG  << "UpgradeThread run - Thread ID : " << QThread::currentThreadId();
    doTask();
  //  exec();
}


bool UpgradeThread::addTask(const QString &pkgFullPath,const QString &pkgMd5, const QString &pkgSha1){
    qDebug() << TAG << "Add task - Thread ID : " << QThread::currentThreadId() << "  thread running  " << this->isRunning();
    if(this->isRunning())
        return false;
    QMutexLocker locker(&m_nTaskListMutex);
    qDebug() << TAG << "lock mutex success";
    InstallTask task;
    task.pkgFullPath  = pkgFullPath;
    task.pkgMd5 = pkgMd5;
    task.pkgSHA1 = pkgSha1;
    m_nTaskList.append(task);
    qDebug() << Q_FUNC_INFO << "pkgFullPath = " << task.pkgFullPath << "   pkgmd5 " << task.pkgMd5 << "   pkgsha1  " << task.pkgSHA1;
    start();
    return true;
}

void UpgradeThread::doTask(){
    qDebug() << TAG << "do task - Thread ID : " << QThread::currentThreadId();
    if(getTaskCount() <= 0)
        return ;
    InstallTask task = getTask(0);
    if(task.pkgFullPath == "" || (task.pkgMd5 == "" && task.pkgSHA1 == "")){
        removeTask(0);
        sendErrSig(-1);
        return;
    }
    m_nPkgFullPath = task.pkgFullPath;
    m_nPkgMd5 = task.pkgMd5;
    m_nPkgSha1 = task.pkgSHA1;
    doTaskOnce();
}

void UpgradeThread::doTaskOnce(){
    qDebug() << Q_FUNC_INFO << "  Thread ID : " << QThread::currentThreadId();
    qDebug() << TAG << "emit process2222  " << m_nPkgFullPath << "  " << INSTALL_PROGRESS_BINARY_BEGIN << "  0";
    emit process(m_nPkgFullPath, INSTALL_PROGRESS_BINARY_BEGIN, 0);
    int res = checkPkg();
    if(res != 0) {
        removeTask(0);
        sendErrSig(-2);
        return;
    }

    res = upgradePkg();
    if(res != 0){
        removeTask(0);
        sendErrSig(-3);
        return;
    }

    removeTask(0);
    if(getTaskCount() > 0) {
        doTask();
    }
    return;
}

InstallTask UpgradeThread::getTask(int index)
{
    QMutexLocker locker(&m_nTaskListMutex);
    InstallTask task;
    task.pkgFullPath = "";
    task.pkgMd5 = "";
    task.pkgSHA1 = "";
    if(index >= 0 && m_nTaskList.size() > index) {
        task = m_nTaskList.at(index);
        qDebug() << TAG << "getTask: task" << task.pkgFullPath;
    }
    return task;
}

void UpgradeThread::removeTask(int index)
{
    QMutexLocker loker(&m_nTaskListMutex);
    if(m_nTaskList.size() <= 0 ){
        qCritical() << TAG << "getTask: task is empty.";
        return;
    }
    if(m_nTaskList.size() < index + 1) {
        qCritical() << TAG << "getTask: index out of range";
        return;
    }
    return m_nTaskList.removeAt(index);
}

int UpgradeThread::getTaskCount()
{
    QMutexLocker locker(&m_nTaskListMutex);
    return m_nTaskList.size();
}

void UpgradeThread::clearTasks()
{
    QMutexLocker locker(&m_nTaskListMutex);
    return m_nTaskList.clear();
}

int UpgradeThread::checkPkg()
{
    qDebug() << TAG << "check package... ";
    // 检查md5是否一致
    QString md5Str = getFileMD5(m_nPkgFullPath);
    if(md5Str != m_nPkgMd5) {
        qDebug() << Q_FUNC_INFO << "MD5 check failed." << "after download file md5 is " << md5Str << "  get md5 is " << m_nPkgMd5;
        return -1;
    }
    return 0;
}

int UpgradeThread::upgradePkg(){
    qDebug() << Q_FUNC_INFO << "Install app:" << m_nPkgFullPath;
    if(m_mng.isPackageManagerdReady() == true) {
        qDebug() << TAG << "package manager has been started.";
        m_mng.install(m_nPkgFullPath, true);
    } else {
        qWarning() << TAG << "package manager is not ready.";
        return -1;
    }
    return 0;
}

QString UpgradeThread::getFileMD5(const QString &fileFullPath){
    QString filePath = fileFullPath;
    QFile theFile(filePath);
    if(!(theFile.open(QIODevice::ReadOnly))){
        qDebug() << filePath << "  open fail " << errno;
        return QString("");
    }
    QByteArray ba = QCryptographicHash::hash(theFile.readAll(),QCryptographicHash::Md5);
    theFile.close();
    return QString(ba.toHex());
}

void UpgradeThread::sendErrSig(int errnoCode){

    QString reason = "";
    switch(errnoCode) {
        case -1:
            reason = "获取参数错误";
            break;
        case -2:
            reason = "md5校验失败";
            break;
        case -3:
            reason = "调用升级接口失败";
            break;
        case 1001:
            reason = "下载失败";
            break;
        case 1002:
            reason = "下载超时";
            break;
        case 1003:
            reason = "下载文件不存在";
            break;
        case 1004:
            reason = "未知原因，下载失败";
            break;
        case 1005:
            reason = "安装失败";
            break;
        case 1006:
            reason = "未知安装错误";
            break;
        case 1007:
            reason = "取消安装失败";
            break;
        case 1008:
            reason = "应用程序未安装";
            break;
        case 1009:
            reason = "预装系统应用无法卸载";
            break;
        case 1010:
            reason = "安装程序异常";
            break;
        case 1011:
            reason = "安装包不存在";
            break;
        case 1012:
            reason = "无法识别的包类型";
            break;
        case 1013:
            reason = "signed fail";
            break;
        case 1014:
            reason = "包配置文件不存在";
            break;
        case 1015:
            reason = "安装包有非法文件";
            break;
        case 1016:
            reason = "安装失败";
            break;
        case 1017:
            reason = "应用程序身份标识长度检查失败";
            break;
        case 1018:
            reason = "磁盘空间不足";
            break;
        case 1019:
            reason = "Release签名认证失败";
            break;
        case 1020:
            reason = "安装超时";
            break;
        case 1021:
            reason = "应用程序不在白名单";
            break;
        default:
            reason = "未知错误";
            break;
    }
    qDebug() << "emit sendInstallError  reason " << reason << "  errnoCOde is  " << errnoCode;
    emit sendInstallError(reason, errnoCode);
}

void UpgradeThread::recvInstallProcess(const QString &sopId, const QString &pkgPath,
                                       CPackageInfo::PackageStatus status,
                                       CPackageInfo::PackageError error, int percent){
    qDebug() << Q_FUNC_INFO << "sopId:  " << sopId << "   packPath: " << pkgPath<< "     status:" << status<< "      error:" << error << "    percent:" << percent;
    QString sopPath = pkgPath;
    sopPath = sopPath.right(sopPath.length() - sopPath.lastIndexOf("/") - 1);
    if(sopId == sopid || sopPath == DEVICEINSPECTION_SOP_NAME) {
        qDebug() << TAG << "(sopId == sopid)  ";
        if(status == 10 || status == 8) {
            emit process(m_nPkgFullPath, INSTALL_PROGRESS_BINARY_FINISH, 0);
            qDebug() << Q_FUNC_INFO << "  updade ok";
            //emit restartApp();
            return;
        } else if (error > 1000){
            qDebug() << TAG << "error > 1000 sendErrSig ";
            sendErrSig(error);
        }
    }
    return;
}
