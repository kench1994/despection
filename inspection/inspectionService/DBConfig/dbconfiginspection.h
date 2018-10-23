#ifndef DBCONFIGINSPECTION_H
#define DBCONFIGINSPECTION_H
#include "dbconfigbase.h"

class DBConfigInspection:public DBConfigBase
{
public:
    DBConfigInspection();
    ~DBConfigInspection();
    static QString TABLE_INSPECTIONSUBMIT;
    static QString TABLE_INSPECTIONSUBMIT_BINARY;
};

#endif // DBCONFIGINSPECTION_H
