#ifndef __APPSERVICE_WORKSPACE__
#define __APPSERVICE_WORKSPACE__

#include <SyberosServiceCache>
#include <cservice_application.h>
#include <cworkspace.h>
#include "pushinterface.h"

using namespace SYBEROS;

class PushService_Workspace : public CWorkspace
{
    Q_OBJECT

public:
    /**
     * 构造函数
     */
    PushService_Workspace();

    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);

    void open(const QStringList& params);

private:

    /**
     * dbus句柄
     */
    PushInterface *interface;
};
#endif //__APPSERVICE_WORKSPACE__

