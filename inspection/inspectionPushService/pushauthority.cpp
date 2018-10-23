#include "pushauthority.h"

/**
 * 构造函数
 */
PushAuthority::PushAuthority(QObject *parent) : QObject(parent)
{

}

/**
 * 拷贝构造函数
 */
PushAuthority::PushAuthority(const PushAuthority &others)
{
    this->authority = others.authority;
}

/**
 * 重载＝
 */
PushAuthority &PushAuthority::operator =(const PushAuthority &others)
{
    this->authority = others.authority;
    return *this;
}

/**
 * 获取用户&注册信息
 * @return 用户&注册信息
 */
QMap<QString, QString> PushAuthority::getAuthority() {
    return authority;
}

/**
 * 添加一个用户&注册信息
 * @param uid 用户id
 * @param authority push权限
 */
void PushAuthority::setAuthority(const QString &uid, const QString &authority) {
    this->authority.insert(uid, authority);
}
