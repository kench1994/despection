/*!
* \file upgradeThread.h
* \brief  软件升级线程
*
*此类是对安装升级包的操作的封装
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/10
*/

#ifndef UPGRADETHREAD_H
#define UPGRADETHREAD_H

#include "../../../framework/common/utils.h"
#include <QObject>
#include <QThread>
#include <QMutex>
#include <framework/csystempackagemanager.h>
#include <cpackageinfo.h>


/*!
  \class UpgradeThread
  \inModule 升级模块
  \brief 软件升级线程
  \ingroup upgrade
  UpgradeThread类是对安装升级包的操作的封装
*/

class UpgradeThread: public QThread
{
    Q_OBJECT
public:
    /**
     * @brief   getInstance:获取单例对象
     * @return  UpgradeThread*, 非NULL 成功, NULL 失败
     */
    static UpgradeThread *getInstance();
    /**
     * @brief 析构函数
     *
     */
    virtual ~UpgradeThread();
    /**
     * @brief addTask:添加安装软件包任务
     *
     * @param pkgFullPath: 安装包所在完整路径
     * @param pkgType: 安装包类型
     * @return bool: true 成功, false 失败
     */
    //Q_INVOKABLE bool addTask(const QString &pkgFullPath, const QString &pkgType);
    Q_INVOKABLE bool addTask(const QString &pkgFullPath, const QString &pkgMd5, const QString &pkgSha1);

protected:
    /**
     * @brief initialize:升级线程初始化
     */
    void initialize();
    /**
     * @brief run:线程运行函数
     */
    void run();
    /**
     * @brief getTask:获取任务队列中指定位置的任务
     * @return InstallTask; 如果无任务，结构体成员变量值为空字符串;
     */
    InstallTask getTask(int index);
    /**
     * @brief removeTask:删除任务队列中的指定任务
     * @param index: 要删除的任务索引;
     */
    void removeTask(int index);
    /**
     * @brief getTaskCount:获取任务队列中未执行的任务个数
     * @return 任务个数;
     */
    int getTaskCount();
    /**
     * @brief clearTasks:清除任务队列中的所有任务
     */
    void clearTasks();


private:
    /**
     * @brief 构造函数
     *
     * @param parent : 父对象
     */
    UpgradeThread(QObject *parent = 0);

    /**
     * @brief checkPkg:校验解压后的文件是否正确
     * @return <0 --失败; 0--成功
     */
    int checkPkg();
    /**
     * @brief upgradePkg:安装sop包
     * @return -1 --失败; 0--成功
     */
    int upgradePkg();

    /**
     * @brief getFileMD5: 获取文件的MD5值
     * @param fileFullPath: 安装包所在完整路径
     * @return MD5值--成功; “”--失败
     */
    QString getFileMD5(const QString &fileFullPath);

    /**
     * @brief sendErrSig: 发送安装失败信息信号
     * @param errnoCode: 错误码
     */
    void sendErrSig(int errnoCode);

private:
    static UpgradeThread* m_pUpgrade; ///<  本地单例对象
    CSystemPackageManager m_mng; ///< 安装sop包对象
    QMutex m_nTaskListMutex;  ///< 任务队列锁
    QList <InstallTask> m_nTaskList;  ///< 任务队列
    QString m_nPkgFullPath;  ///< 下载的安装包存储的完整路径
    QString m_nPkgMd5;  ///< 压缩包md5值
    QString m_nPkgSha1;  ///< 压缩包sha1值
    QString m_nPackageType;  ///< 压缩包压缩类型
    int m_nUpgradeType;          ///< 升级模式; 0--app升级; 1--系统升级
    bool     m_bIsRemoveData;    ///< 是否清理数据


signals:
    /**
     * @brief process : 发送开始安装信号
     * @param pkgFullPath  : QString  sop包安装路径
     * @param installStatus  : int 0--开始安装
     * @param ret  : int 安装操作是否成功
     */
    void process(const QString pkgFullPath, int installStatus, int ret);
    /**
     * @brief response : 发送安装结果信号
     * @param pkgFullPath  : QString  sop包安装路径
     */
    void response(const QString pkgFullPath);
    /**
     * @brief occuredError : 发送安装失败信息信号
     * @param objects  : QString  下载的安装包存储的完整路径
     * @param errMsg  : QString  错误信息
     * @param ret  : int  错误码
     */
    void occuredError(const QString & objects, const QString &errMsg, int ret);
    /**
     * @brief restartApp : 发送重启APP信号
     */
    void restartApp();
    /**
     * @brief sendInstallError : 发送安装失败信息信号
     * @param reason  : QString  出错原因
     * @param errCode  : int  错误码
     */
    void sendInstallError(const QString reason, int errCode);
private slots:
    /**
     * @brief doTask : 执行任务
     */
    void doTask();
    /**
     * @brief   recvInstallProcess  :   接收csystempackagemanager的安装过程信号
     * @param   sopid : QString com.syberos.deviceInspection
     *@param    pkgPath :QString   安装包路径
     * @param   status : CPackageInfo::PackageStatus   安装包安装阶段user name is not exist.
     * @param   error :CPackageInfo::PackageError   安装包安装各阶段执行结果
     * @param   percent :percent  安装包下载进度
     * @return  QString : rfidtype的当前值
     */
    void recvInstallProcess(const QString &sopid,
                            const QString &pkgPath,
                            CPackageInfo::PackageStatus status,
                            CPackageInfo::PackageError error,
                            int percent);

protected slots:
    /**
     * @brief doTaskOnce:执行一次任务的槽函数
     */
    void doTaskOnce();

};




#endif // UPGRADETHREAD_H
