#ifndef NETWROKRECEIVER_H
#define NETWROKRECEIVER_H

#include <QThread>
#include "pushlistener.h"
#include "../framework/dataManager/datahandler.h"

class NetworkReceiver:public QThread
{
public:
    NetworkReceiver();
    void run();
    void setListener(PushListener listener);
    bool isInited();
    void setInited(bool val);
    void quitReceiver();
    void receptionProcess();
    void exitReceiver();

protected:
    /**
     * 初始化标记
     */
   bool beInited;


   void startReception();

private:

   /**
    * 退出标记
    */
  bool beExited;

   /**
    * 网络操作对象
    */
   INetworkTool networkTool;

   /**
    * 事件监听器
    */
   PushListener listener;

   /**
    * 协议解析模块
    */
   DataHandler dataHandler;

   /**
    * 清除Looper并退出
    */
   void cleanup();

   /**
    * 接收Push信息
    */
   bool receiveData();
};

#endif // NETWROKRECEIVER_H
