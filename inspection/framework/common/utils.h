/*!
* \file utils.h
* \brief 公共类型和定义头文件
*
*公共用结构体，宏等定义的集合文件
*
*\author 陈冬 chendong@syberos.com
*\version 1.0
*\date 2015/9/6
*/
#ifndef __INSPECTION_UTIL_H__
#define __INSPECTION_UTIL_H__

#include <QStringList>
#include <QList>
#include <QString>
#include <QMap>
#include <QByteArray>
#include <QDateTime>

//dbus begin
#define SERVICE_DBUS_SERVICE              "com.syberos.inspectionService"
#define SERVICE_DBUS_INTERFACE            "com.syberos.inspectionService.interface"
#define SERVICE_DBUS_PATH                 "/"

#define CLIENT_DBUS_SERVICE               "com.syberos.inspectionClient"
#define CLIENT_DBUS_INTERFACE             "com.syberos.inspectionClient.interface"
#define CLIENT_DBUS_PATH                  "/"

#define PUSH_SERVICE_DBUS_SERVICE         "com.syberos.inspectionPushService"
#define PUSH_SERVICE_DBUS_INTERFACE       "com.syberos.inspectionPushService.interface"
#define PUSH_SERVICE_DBUS_PATH            "/"

//dbus end

/* MIMT Type
超文本标记语言文本 .html text/html
xml文档 .xml text/xml
XHTML文档 .xhtml application/xhtml+xml
普通文本 .txt text/plain
RTF文本 .rtf application/rtf
PDF文档 .pdf application/pdf
Microsoft Word文件 .word application/msword
PNG图像 .png image/png
GIF图形 .gif image/gif
JPEG图形 .jpeg,.jpg image/jpeg
au声音文件 .au audio/basic
MIDI音乐文件 mid,.midi audio/midi,audio/x-midi
RealAudio音乐文件 .ra, .ram audio/x-pn-realaudio
MPEG文件 .mpg,.mpeg video/mpeg
AVI文件 .avi video/x-msvideo
GZIP文件 .gz application/x-gzip
TAR文件 .tar application/x-tar
任意的二进制数据 application/octet-stream
*/

// DEXML 协议的节点定义
/*!
  \def DEXML_TAG_ROOT
  XML根节点.
*/
#define DEXML_TAG_ROOT "DataExchange"

/*!
  \def DEXML_TAG_HEAD
  XML Head节点.
*/
#define DEXML_TAG_HEAD "Head"

/*!
  \def DEXML_TAG_PARA
  XML PARA节点.
*/
#define DEXML_TAG_PARA "Para"

/*!
  \def DEXML_TAG_SUMMARY
  XML Summary节点.
*/
#define DEXML_TAG_SUMMARY "Summary"

/*!
  \def DEXML_TAG_SUMMARY
  XML Data Source节点.
*/
#define DEXML_TAG_DATA "DataSource"

/*!
  \def DEXML_TAG_ATTACHMENTS
  XML Attachments 节点.
*/
#define DEXML_TAG_ATTACHMENTS "Attachments"

/*!
  \def DEXML_TAG_ATTACHMENTS_ATTACHMENT
  XML Attachment 节点.
*/
#define DEXML_TAG_ATTACHMENTS_ATTACHMENT "Attachment"

/*!
  \def DEXML_TAG_DATA_TABLE
  XML Table 节点.
*/
#define DEXML_TAG_DATA_TABLE "Table"

/*!
  \def DEXML_TAG_DATA_TABLE_FIELD
  XML Field 节点.
*/
#define DEXML_TAG_DATA_TABLE_FIELD "Field"

/*!
  \def DEXML_TAG_DATA_TABLE_VALUE
  XML Table value 节点.
*/
#define DEXML_TAG_DATA_TABLE_VALUE "D"

/*!
  \def DEXML_ATT_HEAD_KEY
  XML DataKey 属性.
*/
#define DEXML_ATT_HEAD_KEY "DataKey"

/*!
  \def DEXML_ATT_HEAD_TYPE
  XML DEXType 属性.
*/
#define DEXML_ATT_HEAD_TYPE "DEXType"

/*!
  \def DEXML_ATT_HEAD_OBJECTS
  XML DataObjects 属性.
*/
#define DEXML_ATT_HEAD_OBJECTS "DataObjects"

/*!
  \def DEXML_ATT_HEAD_DEX_VERSION
  XML version 属性.
*/
#define DEXML_ATT_HEAD_DEX_VERSION "DEXVersion"

/*!
  \def DEXML_ATT_HEAD_VERSION
  XML head version 属性.
*/
#define DEXML_ATT_HEAD_VERSION "Version"

/*!
  \def DEXML_ATT_HEAD_DEXTIME
  XML DEXTime 属性.
*/
#define DEXML_ATT_HEAD_DEXTIME "DEXTime"

/*!
  \def DEXML_ATT_DATA_NAME
  XML Data Name 属性.
*/
#define DEXML_ATT_DATA_NAME "Name"

/*!
  \def DEXML_ATT_DATA_KEY
  XML Data Key 属性.
*/
#define DEXML_ATT_DATA_KEY "Key"

/*!
  \def DEXML_ATT_SUMMARY_CODE
  XML Status code 属性.
*/
#define DEXML_ATT_SUMMARY_CODE "StatusCode"

/*!
  \def DEXML_ATT_SUMMARY_RESULT
  XML DEXResult 属性.
*/
#define DEXML_ATT_SUMMARY_RESULT "DEXResult"

/*!
  \def DEXML_ATT_ATTACHMENT_NAME
  XML Attachment Name 属性.
*/
#define DEXML_ATT_ATTACHMENT_NAME "Name"

/*!
  \def DEXML_ATT_ATTACHMENT_MIME
  XML Attachment MIME 属性.
*/
#define DEXML_ATT_ATTACHMENT_MIME "MIMEType"

/*!
  \def DEXML_ATT_ATTACHMENT_TITLE
  XML Attachment Title 属性.
*/
#define DEXML_ATT_ATTACHMENT_TITLE "Title"

/*!
  \def DEXML_ATT_ATTACHMENT_DESCRIPT
  XML Attachment Descript 属性.
*/
#define DEXML_ATT_ATTACHMENT_DESCRIPT "Descript"

/*!
  \def DEXML_ATT_ATTACHMENT_LENGTH
  XML Attachment Length 属性.
*/
#define DEXML_ATT_ATTACHMENT_LENGTH "Length"

/*!
  \def DEXML_ATT_ATTACHMENT_SERIAL
  XML Attachment Serial Key 属性.
*/
#define DEXML_ATT_ATTACHMENT_SERIAL "SerialKey"

/*!
  \def DEXML_ATT_DATA_TABLE_FIDLES_VALUE
  XML Data's table fidles 属性.
*/
#define DEXML_ATT_DATA_TABLE_FIDLES_VALUE "Value"

/*!
  \def DEXML_ATT_DATA_TABLE_VALUE_VALUE
  XML Data's value 属性.
*/
#define DEXML_ATT_DATA_TABLE_VALUE_VALUE "V"

/*!
  \def DEXML_ATT_DATA_SOURCE_PAGE_TOTAL_COUNT
  XML Page's count 属性.
*/
#define DEXML_ATT_DATA_SOURCE_PAGE_TOTAL_COUNT "PagerTotalCount"

/*!
  \def DEXML_SEPARATOR
  XML 分隔符.
*/
#define DEXML_SEPARATOR "や"  // DEXML协议的分隔符

/*!
  \def MESSAGE_DB_PATH
  定义临时消息数据库路径.
*/
#define MESSAGE_DB_PATH "/data/data/com.syberos.deviceInspection/abc.db"

/*!
  \def USER_HISTORY_CACHE
  定义临时登陆历史缓存路径
*/
#define USER_HISTORY_CACHE "/data/data/com.syberos.deviceInspection/testUser.txt"

/// 本地消息数据库的字段定义
//ID  MessageTitle  Publisher  PublishDate  MessageContent  OrganiseUnitID  DepartmentID  OrganiseUnitName  DepartmentName
//MessageType   OperType
#define MESSAGE_TABLE_NAME          "UserMessage"
#define MESSAGE_ID                  "MessageID"
#define MESSAGE_MESSAGETITLE        "MessageTitle"
#define MESSAGE_PUBLISHER           "Publisher"
#define MESSAGE_PUBLISHDATE         "PublishDate"
#define MESSAGE_MESSAGECONTENT      "MessageContent"
#define MESSAGE_ORGANISEUNITID      "OrganiseUnitID"
#define MESSAGE_DEPARTMENTID        "DepartmentID"
#define MESSAGE_ORGANISEUNITNAME    "OrganiseUnitName"
#define MESSAGE_DEPARTMENTNAME      "DepartmentName"
#define MESSAGE_MESSAGETYPE         "MessageType"
#define MESSAGE_OPERTYPE            "OperType"

#define MESSAGE_NOTIFY_KEY  "NotifyKey"
#define MESSAGE_READ_STATUS "Status"

#define MESSAGE_DATE_FORMAT_DATABASE "yyyy/MM/dd hh:mm:ss"

#define MESSAGE_DATE_FORMAT_QMLSHOW "yyyy-MM-dd hh:mm:ss"

/*!
  \def NETWORK_INTERVAL_BASE
  网络收发间隔
*/
#define NETWORK_INTERVAL_BASE 30
#define NETWORK_TIMEOUT_ASYN 30   ///< 手动超时时间

#define NETWORK_NO_ERROR 0
#define NETWORK_ERROR_SOCKET -1

/*!
  \def NETWORK_ERROR_SOCKET_CONNECTION
  Socket连接错误
*/
#define NETWORK_ERROR_SOCKET_CONNECTION -2

/*!
  \def NETWORK_ERROR_SEND_FAIL
  发送数据失败
*/
#define NETWORK_ERROR_SEND_FAIL -3

/*!
  \def NETWORK_ERROR_RECV_FAIL
  接收数据失败
*/
#define NETWORK_ERROR_RECV_FAIL -4

/*!
  \def NETWORK_ERROR_STORE_ATTACHMENT
  存储数据失败
*/
#define NETWORK_ERROR_STORE_ATTACHMENT -5

/*!
  \def NETWORK_ERROR_NO_ATTACHMENT
  附件缺失错误
*/
#define NETWORK_ERROR_NO_ATTACHMENT -6

/*!
  \def NETWORK_ERROR_DNS
  DNS解析错误
*/
#define NETWORK_ERROR_DNS -7

/*!
  \def NETWORK_ERROR_NONETWORK
  无可用网络错误
*/
#define NETWORK_ERROR_NONETWORK -8
#define NETWORK_ERROR_SERVER_HANDLE -9
#define NETWORK_ERROR_EMPTY_ADDRESS -10
#define NETWORK_ERROR_READ_FILE -11
#define NETWORK_ERROR_ENCODE_FAIL -12
#define NETWORK_ERROR_PROTOCOL -13
#define NETWORK_ERROR_DECODE_FAIL -14

#define DATABASE_ERROR_OPERATE_FAIL -100
#define DATABASE_ERROR_INSERT_FAIL -101
#define DATABASE_ERROR_DELETE_FAIL -102
#define DATABASE_ERROR_SELECT_FAIL -103
#define DATABASE_ERROR_UPDATE_FAIL -104
/**
  \def service没有注册dbus
  */
#define DBUS_ERROR_SERVICE_UNKNOWN -105

/*!
  \def NETWORK_ERROR_TOKEN_INVALID_OR_TIMEOUT
  令牌无效或者过期
*/
#define NETWORK_ERROR_TOKEN_INVALID_OR_TIMEOUT -15

/*!
  \def NETWORK_SERVER_COMMAND_TIMEOUT
  请求命令过期
*/
#define NETWORK_SERVER_COMMAND_TIMEOUT -1000

/*!
  \def NETWORK_SERVER_TOKEN_INVALID
  访问令牌无效
*/
#define NETWORK_SERVER_TOKEN_INVALID -1011

/*!
  \def NETWORK_SERVER_TOKEN_TIMEOUT
  令牌过期
*/
#define NETWORK_SERVER_TOKEN_TIMEOUT -1012


/*!
  \def NETWORK_SERVER_CODE_INVALID
  授权码无效
*/
#define NETWORK_SERVER_CODE_INVALID -1020

/*!
  \def NETWORK_SERVER_RESULT_COMPLETE
  服务器操作成功
*/
#define NETWORK_SERVER_RESULT_COMPLETE 100

/*!
  \def NETWORK_SERVER_RESULT_HANDLING
  服务器操作中
*/
#define NETWORK_SERVER_RESULT_HANDLING 1

#define BUSINESS_ERROR_NO_BUSINESS  -100

///< 网络配置参数
#define NETWORK_DATA_TIMEOUT_SEC 4  ///< 网络超时秒数
#define NETWORK_DATA_TIMEOUT_MSEC 500 ///< 网络超时毫秒数
#define NETWORK_TIMEOUT_LIMIT 3 ///< 网络超时重试次数

/*!
  \def NETWORK_BINARY_BLOCK_LENGTH_4K
  二进制收发块大小定义
*/
#define NETWORK_BINARY_BLOCK_LENGTH_4K 4*1024

/*!
  \def NETWORK_BINARY_BLOCK_LENGTH_8K
  二进制收发块大小定义
*/
#define NETWORK_BINARY_BLOCK_LENGTH_8K 8*1024

/*!
  \def ATTACHMENT_TMP_PATH
  附件临时存放目录
*/
#define ATTACHMENT_TMP_PATH "/tmp/"

/*!
  \def BINARY_CACHE_TMP_PATH
  二进制大文件缓存存放目录
*/
#define BINARY_CACHE_TMP_PATH "/data/home/user/cache/"

/*!
  \def BINARY_TMP_PATH
  二进制大文件临时存放目录
*/
#define BINARY_TMP_PATH "/data/home/user/appUpgrade/"

/*!
  \def UNCOMPRESS_DIR
  升级文件包解压缩目录
*/
#define UNCOMPRESS_DIR "/data/home/user/appUpgrade/Package/"

/*!
  \def NETWORK_BINARY_STAUS_BEGIN
  二进制大文件接收进度信息 - 开始
*/
#define NETWORK_BINARY_STAUS_BEGIN 0

/*!
  \def NETWORK_BINARY_STAUS_REQUEST
  二进制大文件接收进度信息 - 发送请求
*/
#define NETWORK_BINARY_STAUS_REQUEST 1
#define NETWORK_BINARY_STAUS_INFORMATION 2
#define NETWORK_BINARY_STAUS_DOWNLOADING 3
#define NETWORK_BINARY_STAUS_UPLOADING 4

/*!
  \def NETWORK_BINARY_STAUS_FINISHED
  二进制大文件接收进度信息 - 接收完成
*/
#define NETWORK_BINARY_STAUS_FINISHED 5

#define NETWORK_BINARY_THREAD_LIMIT 3
#define NETWORK_COMMAND_THREAD_LIMIT 3

#define INSTALL_PROGRESS_BINARY_BEGIN 0
#define INSTALL_PROGRESS_BINARY_UNCOMPRESS 1
#define INSTALL_PROGRESS_BINARY_CHECK 2
#define INSTALL_PROGRESS_BINARY_FINISH 3
#define INSTALL_PROGRESS_BINARY_UNINSTALL 4

#define SERVER_DATE_FORMAT "yyyy/MM/dd hh:mm:ss.zzz"

#define NETWORK_PROTOCOL_MARK "FDEP"   ///< 协议头标识
#define NETWORK_PROTOCOL_MAIN_VERSION 1    ///< 协议主版本号
#define NETWORK_PROTOCOL_SUB_VERSION 0    ///< 协议副版本号

#define CONFIG_FILE "systemConfig.ini"  ///<配置文件路径
#define SERVICE_CONFIG_FILE "serviceConfig.ini"  ///<服务程序配置文件，主要用来确定是否创建数据库
#define DATABASE_PATH "/home/user/inspection.db" ///<数据库路径
// THEME DEFINITIONS

/*!
  \def THEME_FILE
  皮肤脚本文件
*/
#define THEME_FILE "theme.xml"

/*!
  \def THEME_FILE
  皮肤皮肤资源目录
*/
#define THEME_RES_DIR "res"

/*!
  \def THEME_FILE
  皮肤根目录
*/
#define THEME_DIR "themes"

///< THEME TAG DEFINITIONS
#define THEME_STYLE "Style"
#define THEME_NAME_CN "NameCN"
#define THEME_NAME_EN "NameEN"
#define THEME_IMAGE "Image"
#define THEME_NODE_TYPE "Type"
#define THEME_NODE_INDEX "Index"
#define THEME_NODE_ID "ID"

/**
 * @brief 数据表中的行数据单位
 *
 */
typedef QMap<QString, QString> DataRow;

typedef QMap<QString, QString> RawMessage;

typedef enum __DEXDataFormat__
{
    SingleAction = 0, ///< 单指令(心跳包或文件块)
    DEXML = 1, ///< DEXML格式
    JSON = 2 ///< Json格式
}DEXDataFormat;

///@brief DEXCharset枚举
typedef enum __DEXCharset__
{
    ASCII = 0, ///< ASCII编码
    UTF7 = 1,  ///< UTF16或Unicode编码
    UTF8 = 2,  ///< UTF8编码
    UTF16 = 3, ///< UTF16或Unicode编码
    UTF32 = 4, ///< UTF32编码
    GB2312 = 5 ///< GB2312编码
}DEXCharset;

typedef enum __NetworkType__ {
    SOCKET,
    SSLSOCKET,
    USB
}NetworkType;

typedef struct __protocal_head__ {
    char mark[4]; ///< 前4字节为协议名(FDEP)
    unsigned char mainVersion; ///< 第5字节为主版本号(1)
    unsigned char subVersion; ///< 第6字节为次版本号(0)
    unsigned char type; ///< 第7字节为交换类型
    DEXDataFormat format; ///< 第8字节为数据格式(1)
    DEXCharset charset; ///< 第9字节为字符编码
    unsigned char result; ///< 第10字节为交换结果(1|0)
    short statusCode; ///< 第11、12字节为状态码
    int length; ///< 第13~16字节为内容长度
}ProtocolHead;

#define PACKAGE_HEAD_KEY "41aa80ea-ad6a-4fbe-b5b7-e23f47bd6a93"
#define EMPTY_DEVICE_ID "00000000-0000-0000-0000-000000000000"

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
    QList<Attachment> binaries;    ///< 二进制附件
}DataPackage;

//service和client通过dbus交流的数据结构体
typedef struct _DBUS_PACKAGE_{
    DataPackage dataPackage;
    QMap<QString, QString> otherMap;
}DbusPackage;

typedef struct __NETWORK_TASK__{
    QString objects;   ///< 任务Object
    QString type;    ///< 任务类型
    QString action;    ///< 任务Action
    QByteArray data;    ///< 任务数据
}NetworkTask;
//ID  MessageTitle  Publisher  PublishDate  MessageContent  OrganiseUnitID  DepartmentID  OrganiseUnitName  DepartmentName
//OperType  MessageType

typedef struct __COMMAND_TASK__{
    QString id;
    QString objects;
    QString type;
    QString action;
    QByteArray head;
    QByteArray data;
    bool disposing;
}CommandTask;

typedef struct __BINARY_TASK__{
    QString uuid;
    QString action;
    QString localPath;
    QString remotePath;
    QString name;
    QString title;
    QString description;
    QString seriesKey;
    bool download;
    bool disposing;
}BinaryTask;



typedef struct __MESSAGE__{
    QString uuid;   ///< 消息UUID
    QString messageTitle;    ///< 消息标题
    QString publisher;    ///< 发布人
    QString publishDate;    ///< 发布时间
    QString messageContent;    ///< 消息内容
    QString organiseUnitID;    ///< 组织机构ID
    QString departmentID;    ///< 部门ID
    QString organiseUnitName;    ///< 组织机构名称
    QString departmentName;    ///< 部门名称
    QString operType;    ///< 业务消息类型
    QString messageType;    ///< 消息类型
    int     status;    ///< 消息状态
    QString notifyKey;    ///< 通知栏上对应的UUID
}Message;

///@brief 安装任务结构体
typedef struct __INSTALL_TASK__ {
    QString pkgFullPath;    ///< 压缩包的完整路径
    QString pkgMd5;         ///<压缩包的md5
    QString pkgSHA1;       ///<压缩包的sha1
    //QString pkgType;    ///< 压缩包类型
} InstallTask;

#define BINARY_TASK_CONFIG_ROOT "Tasks"
#define BINARY_TASK_CONFIG_BASE_INFO "BaseInfo"
#define BINARY_TASK_CONFIG_SUBTASK_INFO "TasksInfo"
#define BINARY_TASK_CONFIG_SUBTASK_TAG "SubTask"
#define BINARY_TASK_CONFIG_BASE_INFO_UUID "uuid"
#define BINARY_TASK_CONFIG_BASE_INFO_URL "url"
#define BINARY_TASK_CONFIG_BASE_INFO_LENGTH "length"

//typedef struct _INSPECTION_ACTION{
//  DataPackage package;
//  QString actionName;
//  QString errorString;
//  bool runInBackground;
//  QStringList downLoadFileList;
//}InspectionActions;


/**
 * APP KEY
 */
//#define APP_KEY "2167124871485836"
#define APP_KEY "8538475135874231"

/**
 * APP SECRET KEY
 */
//#define APP_SECRET_KEY "1367281653841285"
#define APP_SECRET_KEY "2364716325841252"

/**
 * APP SYSTEM KEY
 */
#define APP_SYSTEM_KEY "9999999999999999"

/**
 * APP SECRET IV
 */
//#define APP_SECRET_IV "5212731681283172"
#define APP_SECRET_IV "5158658142414726"

/**
 * APP SYSTEM IV
 */
#define APP_SYSTEM_IV "9999999999999999"

/**
 * APP ID
 */
//#define APP_ID "96854572286849129"
#define APP_ID "96895878849106056"

/**
 * APP PUBLIC KEY
 */
#define APP_PUBLIC_KEY ""

/**
 * APP PRIVATE KEY
 */
#define APP_PRIVATE_KEY ""

/**
 * 包体头在数据段中的偏移量
 */
#define BODY_HEAD_OFFSET 0

/**
 * 包体头在数据段中的偏移量
 */
#define BODY_HEAD_OFFSET_APP_ID 8

/**
 * 包体头中的start Index在数据段中的偏移量
 */
#define BODY_HEAD_OFFSET_START_INDEX 16

/**
 * 包体头中的Count偏移量
 */
#define BODY_HEAD_OFFSET_COUNT 20

/**
 * 包体头默认长度
 */
#define BODY_HEAD_LENGTH 32

/**
 * 包体头中信息段默认长度
 */
#define BODY_HEAD_INFO_LENGTH 16

/**
 * 包体中app ID段长度
 */
#define BODY_HEAD_APP_ID_LENGTH 16

/**
 * 获取System Token的用户名
 */
#define SYSTEM_USER_ACCOUNT "dexuser"

/**
 * 获取System Token的密码
 */
#define SYSTEM_USER_PASSWORD "iipplat123"

/**
 * XML 版本
 */
#define DEX_VERSION  "1.7"

/**
 * 传输协议版本
 */
#define PROTOCOL_VERSION  "1.6"

/**
 * 获取 User Access Token的Action
 */
#define ACTION_FETCH_USER_ACCESS_TOKEN "ActionFetchUserAccessToken"

/**
 * 获取 System Access Token的Action
 */
#define ACTION_FETCH_SYSTEM_ACCESS_TOKEN "ActionFetchSystemAccessToken"

/**
 * 获取App Para的Action
 */
#define ACTION_FETCH_APP_PARA "ActionFetchAppPara"

/**
 * 二进制数据请求和提交的类型
 */
#define TYPE_BINARY  "0"

/**
* 请求类型-用于获取数据
*/
#define TYPE_REQUEST "1"

/**
* 提交数据类型 - 用于提交数据表单
*/
#define TYPE_COMMIT "2"

/**
* 提交数据类型 - 用于提交特殊数据
*/
#define TYPE_COMMIT_OTHER "4"

/**
* 提交数据类型 - 用于发送心跳包
*/
#define TYPE_HEART "8"

/**
 * Push请求类型
 */
#define TYPE_PUSH "9"

/**
 * Token请求的类型
 */
#define TYPE_SECURITY "10"

/**
 * 重置安全参数请求的类型
 */
#define TYPE_APP_PARA "100"

/**
 * 更新令牌的Object
 */
#define OBJECT_FETCH_USER_ACCESS_TOKEN "GetAccessToken"

/**
 * 重置安全参数的Object
 */
#define OBJECT_FETCH_APPLICATION_PARAMETER "GetAppPara"

///@brief Token结构体
typedef struct __ACCESS_TOKEN__ {
    long long accessKey;   ///< 传输协议 - AccessKey
    long long accessToken; ///< 传输协议 - AccessToken
    long long deadLine;    ///< 传输协议 - 时间戳
    char account[24];       ///< 传输协议 - 当前用户ID
} AccessToken;

/**
 * 服务用户
 */
#define AUTH_USER_TYPE_SERVICE "0"

/**
 * 业务用户
 */
#define AUTH_USER_TYPE_BUSINESS "1"

#endif

