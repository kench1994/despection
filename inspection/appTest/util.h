#ifndef UTIL_H
#define UTIL_H
#include <QMap>
typedef QMap<QString, QString> DataRow;
///@brief Head数据结构
typedef struct __DATA_HEAD__{
    QString key;     ///< key数据
    QString type;    ///< type数据
    QString objects;    ///< objects数据
    QString date;    ///< 当前时间
}DataHead;

///@brief Table数据结构
typedef struct __DATA_TABLE__{
    QString name;    ///< name数据
    QString key;    ///< key数据
    QList<DataRow> value;  ///< 表中的数据
}DataTable;

///@brief Summary数据结构
typedef struct __SUMMARY__{
    int code;    ///< 服务器返回的处理编码
    int result;    ///< 服务器返回的处理结果
    QString description;    ///< 服务器返回的处理明细
}Summary;

///@brief Attachment数据结构
typedef struct __ATTACHMENT__{
    QString name;    ///< 附件名
    QString mime;    ///< 附件类型（MIME）
    QString path;    ///< 附件地址
    QString serialKey;    ///< 附件序列号
    QString title;    ///< 附件标题
    QString description;    ///< 附件描述
    unsigned int length;    ///< 附件长度
}Attachment;

///@brief DataPackage数据包
typedef struct __DATA_PACKAGE__{
    DataHead head;    ///< head数据
    QMap<QString, QString> para;    ///< para数据
    Summary summary;    ///< summary数据
    QString pagerTotalCount;    ///< Page Count
    QList<DataTable> tables;    ///< 数据表
    QList<Attachment> attachments;    ///< 附件
}DataPackage;

#endif // UTIL_H
