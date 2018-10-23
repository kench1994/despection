#ifndef BINARYRECEIVER_H
#define BINARYRECEIVER_H
#include "../networkthreadbase.h"
#include "../../common/utils.h"
#include "../../dataManager/datahandler.h"

/*!
  \class NetworkBinaryThread
  \inModule 网络模块
  \brief 底层负责数据收发二进制数据的线程模块
  \ingroup network
  NetworkBinaryThread类实现了发送队列以及关于数据传输的相关操作
*/

class NetworkBinaryThread : public NetworkThreadBase
{
    Q_OBJECT
public:
    /**
     * @brief NetworkBinaryThread:类初始化函数
     * @param parent : 父类对象
     */
    explicit NetworkBinaryThread(QObject *parent = 0);

    ~NetworkBinaryThread();

    QString getUuid();

    void setTask(BinaryTask task);

private:

    QString getTmpFilePath();

    bool generateData(QByteArray& data);

    int recvBinary(int totalLen);

    int sendBinary(int pos);

signals:
    void response(const QString uuid, const QString action, const QString localPath);

    void error(const QString uuid, const QString action, const int error, const QString errMsg);

    void progress(const QString uuid,
                  const QString action,
                  const int totalLength,
                  const int progress,
                  const int status);

protected:
    void run();
    void cleanup();

private:
    QString mAction;
    QString mRemotePath;
    QString mLocalPath;
    QString mId;
    QString mName;
    QString mTitle;
    QString mDescription;
    QString mSeriesKey;
    bool bDownload;
    DataHandler mDataHandler;   ///< 数据处理实例
    QString getProtocolVersion();
};

#endif // BINARYRECEIVER_H
