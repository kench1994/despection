#ifndef COMMONDBUSTYPES_H
#define COMMONDBUSTYPES_H
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusObjectPath>
#include "utils.h"

//typedef struct _DBUS_PACKAGE_{
//    DataPackage dataPackage;
//    QMap<QString, QString> otherMap;
//}DbusPackage;

typedef QMap<QString, QString> StringMap;
Q_DECLARE_METATYPE ( StringMap );
Q_DECLARE_METATYPE (DataHead);
Q_DECLARE_METATYPE (Attachment);
Q_DECLARE_METATYPE (Summary);
Q_DECLARE_METATYPE (DataTable);
Q_DECLARE_METATYPE (DataPackage);
Q_DECLARE_METATYPE (DbusPackage);


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
//重载DataPackage的赋值预算符
QDBusArgument &operator<<(QDBusArgument &argument, const DbusPackage &obj);
const QDBusArgument &operator>>(const QDBusArgument &argument, DbusPackage &obj);

inline void registerCommonDataTypes() {
  qDBusRegisterMetaType<StringMap>();
  qDBusRegisterMetaType<DataHead>();
  qDBusRegisterMetaType<Attachment>();
  qDBusRegisterMetaType<Summary>();
  qDBusRegisterMetaType<DataTable>();
  qDBusRegisterMetaType<DataPackage>();
  qDBusRegisterMetaType<DbusPackage>();
  qRegisterMetaType<DataPackage>("DataPackage");
  qRegisterMetaType<DbusPackage>("DbusPackage");
}

#endif //COMMONDBUSTYPES_H
