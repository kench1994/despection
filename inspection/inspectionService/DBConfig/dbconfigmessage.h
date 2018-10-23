#ifndef DBCONFIGMESSAGE_H
#define DBCONFIGMESSAGE_H

#include "dbconfigbase.h"
class DBConfigMessage : public DBConfigBase
{
public:
    DBConfigMessage();
    ~DBConfigMessage();
    static QString TABLE_MESSAGE_TOKEN;
    static QString TABLE_MESSAGE_INFO;
};

#endif // DBCONFIGMESSAGE_H
