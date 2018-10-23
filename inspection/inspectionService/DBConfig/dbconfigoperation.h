#ifndef DBCONFIGOPERATION_H
#define DBCONFIGOPERATION_H

#include "dbconfigbase.h"
class DBConfigOperation:public DBConfigBase
{
public:
    DBConfigOperation();
    ~DBConfigOperation();
    static QString TABLE_FAULTREPORTSUBMIT;
    static QString TABLE_FAULTREPORTSUBMIT_BINARY;
};

#endif // DBCONFIGOPERATION_H
