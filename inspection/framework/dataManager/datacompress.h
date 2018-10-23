/*!
* \file datacompress.h
* \brief 数据的压缩和解压缩类头文件
*
*此类提供了对于数据的压缩和解压缩功能，采用的压缩算法是GZip
*
*\author 陈冬 chendong@syberos.com
*\version 1.0
*\date 2015/9/6
*/
#ifndef DATACOMPRESS_H
#define DATACOMPRESS_H

#include <QObject>
#include <zlib.h>
#include <QByteArray>

#define GZIP_WINDOWS_BIT 15 + 16
#define GZIP_CHUNK_SIZE 32 * 1024

/*!
  \class DataCompress
  \inModule 数据处理模块
  \brief 负责对数据流进行压缩和解压缩的模块,采用的压缩算法是GZip
  \ingroup dataManager
  DataCompress类为上层提供了压缩和解压缩数据流的接口
*/
class DataCompress : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     *
     * @param parent : 父对象
     */
    explicit DataCompress(QObject *parent = 0);

   /**
     * @brief gzipCompress:压缩数据
     * @param input    : QByteArray  需要压缩的数据流
     * @param output    : QByteArray&  压缩结果
     * @param level  : 压缩等级，默认为-1（默认等级）
     * @return  bool, true 成功, false 失败
     */
    static bool gzipCompress(QByteArray input, QByteArray &output, int level = -1);

   /**
     * @brief gzipDecompress:解压缩数据
     * @param input    : QByteArray  需要解压缩的数据流
     * @param output    : QByteArray&  解压缩结果
     * @return  bool, true 成功, false 失败
     */
    static bool gzipDecompress(QByteArray input, QByteArray &output);
    
signals:
    
public slots:
    
};

#endif // DATACOMPRESS_H
