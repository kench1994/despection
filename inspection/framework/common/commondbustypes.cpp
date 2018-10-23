#include "commondbustypes.h"
// Marshall the ConnmanObject data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const DataHead &obj){
    argument.beginStructure();
    argument << obj.key << obj.type << obj.objects << obj.date;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, DataHead &obj){
    argument.beginStructure();
    argument >> obj.key >> obj.type >> obj.objects >> obj.date;
    argument.endStructure();
    return argument;
}

//重载Attachment的赋值预算符
QDBusArgument &operator<<(QDBusArgument &argument, const Attachment &obj){
    argument.beginStructure();
    argument << obj.name << obj.mime << obj.path << obj.serialKey << obj.title << obj.description << obj.length;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, Attachment &obj){
    argument.beginStructure();
    argument >> obj.name >> obj.mime >> obj.path >> obj.serialKey >> obj.title >> obj.description >> obj.length;
    argument.endStructure();
    return argument;
}

//重载Summary的赋值预算符
QDBusArgument &operator<<(QDBusArgument &argument, const Summary &obj){
    argument.beginStructure();
    argument << obj.code << obj.result << obj.description;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, Summary &obj){
    argument.beginStructure();
    argument >> obj.code >> obj.result >> obj.description;
    argument.endStructure();
    return argument;
}

//重载DataTable的赋值预算符
QDBusArgument &operator<<(QDBusArgument &argument, const DataTable &obj){
    argument.beginStructure();
    argument << obj.name << obj.key << obj.value;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, DataTable &obj){
    argument.beginStructure();
    argument >> obj.name >> obj.key >> obj.value;
    argument.endStructure();
    return argument;
}

//重载DataPackage的赋值预算符
QDBusArgument &operator<<(QDBusArgument &argument, const DataPackage &obj){
    argument.beginStructure();
    argument << obj.head << obj.para << obj.summary << obj.pagerTotalCount << obj.tables << obj.attachments << obj.binaries;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, DataPackage &obj){
    argument.beginStructure();
    argument >> obj.head >> obj.para >> obj.summary >> obj.pagerTotalCount >> obj.tables >> obj.attachments >> obj.binaries;
    argument.endStructure();
    return argument;
}


//重载DbusPackage的赋值预算符
QDBusArgument &operator<<(QDBusArgument &argument, const DbusPackage &obj){
    argument.beginStructure();
    argument << obj.dataPackage << obj.otherMap;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, DbusPackage &obj){
    argument.beginStructure();
    argument >> obj.dataPackage >> obj.otherMap;
    argument.endStructure();
    return argument;
}
