#ifndef NETWORKHEADTOOL_H
#define NETWORKHEADTOOL_H

#include "../common/utils.h"

class NetworkHeadTool
{
public:
    NetworkHeadTool();
    static int generateProtocolHead(ProtocolHead& head, bool encrypt, bool emptyToken, char* out);
    static void decodeProtocolHead(ProtocolHead& head, char* data);
};

#endif // NETWORKHEADTOOL_H
