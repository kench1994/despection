#ifndef RUNTIMEPARAM_H
#define RUNTIMEPARAM_H
#include <csystemdevicemanager.h>
#include <QString>
class RuntimeParam
{
public:
    ~RuntimeParam();
    /**
     * @brief   getInstance:获取单例对象
     * @return  RuntimeParam*, 非NULL 成功, NULL 失败
     */
    static RuntimeParam *getInstance();
    /**
     * @brief  getPadId : 获取设备id
     * @return QString, 设备id
     */
    Q_INVOKABLE QString getPadId();
private:
    RuntimeParam();
    static RuntimeParam *m_pSysParam;///<  本地单例对象
    QString m_nPadId;
};

#endif // RUNTIMEPARAM_H
