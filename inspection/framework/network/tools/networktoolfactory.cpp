#include "networktoolfactory.h"
#include "sockettool.h"
#include "usbtool.h"
#include "sslsockettool.h"

INetworkTool* NetworkToolFactory::createTool(NetworkType type)
{
    INetworkTool* tool = NULL;
    switch (type) {
    case SOCKET:
        tool = new SocketTool();
        break;
    case SSLSOCKET:
        tool = new SSLSocketTool();
        break;
    case USB:
        tool = new USBTool();
        break;
    }
    return tool;
}
