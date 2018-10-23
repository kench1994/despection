#ifndef DBCONFIGDAILY_H
#define DBCONFIGDAILY_H

#include "dbconfigbase.h"
class DBConfigDaily : public DBConfigBase
{
public:
    DBConfigDaily();
    ~DBConfigDaily();
    static QString TABLE_DAILYSUBMIT;
    static QString TABLE_DAILYSUBMIT_BINARY;
};

#endif // DBCONFIGDAILY_H
