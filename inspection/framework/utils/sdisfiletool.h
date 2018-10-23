#ifndef SDISFILETOOL_H
#define SDISFILETOOL_H

#include <QString>

class SDISFileTool
{
public:
    static int getSize(QString path);
    static QString getMimeType(QString path);
    static QString getDirPath(QString path);
    static QString getFileName(QString path);
    static bool mkdir(QString path);
    static void move(QString src, QString target);
};

#endif // SDISFILETOOL_H
