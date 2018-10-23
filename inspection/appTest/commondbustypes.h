#ifndef COMMONDBUSTYPES_H
#define COMMONDBUSTYPES_H
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusObjectPath>
#include "../framework/common/utils.h"

//DataHead head;    ///< head数据
//QMap<QString, QString> para;    ///< para数据
//Summary summary;    ///< summary数据
//QString pagerTotalCount;    ///< Page Count
//QList<DataTable> tables;    ///< 数据表
//QList<Attachment> attachments;    ///< 附件
//QList<Attachment> binaries;    ///< 二进制附件

typedef QMap<QString, QString> StringMap;
Q_DECLARE_METATYPE ( StringMap );
Q_DECLARE_METATYPE (DataHead);
Q_DECLARE_METATYPE (Attachment);
Q_DECLARE_METATYPE (Summary);
Q_DECLARE_METATYPE (DataTable);
Q_DECLARE_METATYPE (DataPackage);

//重载DataHead的赋值预算符
QDBusArgument &operator<<(QDBusArgument &argument, const DataHead &obj);
const QDBusArgument &operator>>(const QDBusArgument &argument, DataHead &obj);
//重载Attachment的赋值预算符
QDBusArgument &operator<<(QDBusArgument &argument, const Attachment &obj);
const QDBusArgument &operator>>(const QDBusArgument &argument, Attachment &obj);
//重载Summary的赋值预算符
QDBusArgument &operator<<(QDBusArgument &argument, const Summary &obj);
const QDBusArgument &operator>>(const QDBusArgument &argument, Summary &obj);
//重载DataTable的赋值预算符
QDBusArgument &operator<<(QDBusArgument &argument, const DataTable &obj);
const QDBusArgument &operator>>(const QDBusArgument &argument, DataTable &obj);
//重载DataPackage的赋值预算符
QDBusArgument &operator<<(QDBusArgument &argument, const DataPackage &obj);
const QDBusArgument &operator>>(const QDBusArgument &argument, DataPackage &obj);

inline void registerCommonDataTypes() {
  qDBusRegisterMetaType<StringMap>();
  qDBusRegisterMetaType<DataHead>();
  qDBusRegisterMetaType<Attachment>();
  qDBusRegisterMetaType<Summary>();
  qDBusRegisterMetaType<DataTable>();
  qDBusRegisterMetaType<DataPackage>();
  qRegisterMetaType<DataPackage>("DataPackage");
}

#endif //COMMONDBUSTYPES_H
