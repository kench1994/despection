/*!
* \file datahandler.h
* \brief 数据的处理类头文件
*
*此类提供了对于网络数据处理以及对于上层接口的封装
*
*\author 陈冬 chendong@syberos.com
*\version 1.0
*\date 2015/9/6
*/
#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>
#include "../common/utils.h"
#include "dataencoder.h"
#include "datadecoder.h"
#include "datacompress.h"

/*!
  \class DataHandler
  \inModule 数据处理模块
  \brief 负责对数据流进行XML协议化的模块
  \ingroup dataManager
  DataHandler类为上层提供从程序的结构体到发送的网络流的互相转换接口
*/
class DataHandler : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     *
     * @param parent : 父对象
     */
    explicit DataHandler(QObject *parent = 0);
   /**
     * @brief encodeData:将程序的数据结构体转换到二进制的数据流
     * @param package    : DataPackage& 原始的结构体
     * @param output    : QByteArray& 输出对象
     * @return  bool, true 成功, false 失败
     */
    bool encodeData(const DataPackage& package, QByteArray& output);
   /**
     * @brief decodeData:将二进制的数据流转义到程序的结构体
     * @param data    : const QByteArray& 原始数据
     * @param output    : DataPackage& 返回的结果
     * @return  bool, true 成功, false 失败
     */
    bool decodeData(const QByteArray& data, DataPackage& output, DEXDataFormat format, DEXCharset charset);
   /**
     * @brief setDeviceID: 设置设备ID
     * @param value    : const Qstring& 设备ID
     */
    void setDeviceID(const QString& value);
signals:
    
public slots:

private:
    DataEncoder m_nEncoder;  ///< 数据编码对象
    DataDecoder m_nDecoder;  ///< 数据解码对象
    DataCompress m_nCompress;  ///< 数据压缩对象
    QString m_nDevideID;  ///< 设备ID
};

#endif // DATAHANDLER_H
