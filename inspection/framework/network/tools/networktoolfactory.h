#ifndef NETWORKTOOLFACTORY_H
#define NETWORKTOOLFACTORY_H

#include "inetworktool.h"
#include "../../common/utils.h"
class NetworkToolFactory
{
public:
    static INetworkTool* createTool(NetworkType type);
};

#endif // NETWORKTOOLFACTORY_H
