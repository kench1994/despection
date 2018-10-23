#ifndef PUSHAUTHORITY_H
#define PUSHAUTHORITY_H

#include <QObject>
#include <QMap>

/**
 * Push注册用的数据信息类
 * 对应以下协议信息
 * <Para UserAccount="administrator" TopicIds="Login,ChartMessage,Logout"/>
 */
class PushAuthority : public QObject
{
    Q_OBJECT
public:

    /**
     * 构造函数
     */
    explicit PushAuthority(QObject *parent = 0);

    /**
     * 拷贝构造函数
     */
    PushAuthority(const PushAuthority& others);

    /**
     * 重载＝
     */
    PushAuthority& operator =(const PushAuthority &others);

    /**
     * 获取用户&注册信息
     * @return 用户&注册信息
     */
    QMap<QString, QString> getAuthority();

    /**
     * 添加一个用户&注册信息
     * @param uid 用户id
     * @param authority push权限
     */
    void setAuthority(const QString &uid, const QString &authority);

signals:

public slots:

private:
    /**
     * 注册的用户&权限信息
     */
    QMap<QString, QString> authority;
};
#endif // PUSHAUTHORITY_H
