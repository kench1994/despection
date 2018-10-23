#include "dbconfigmessage.h"
#include "../../framework/common/systemconfig.h"
#include <QDebug>
QString DBConfigMessage::TABLE_MESSAGE_TOKEN = "Message_Token_Update";
QString DBConfigMessage::TABLE_MESSAGE_INFO = "Message_Info";
DBConfigMessage::DBConfigMessage()
{
    m_nDataOperationType = BUSINESS_TYPE_MESSAGE;
    QString installPath = SystemConfig::getInstance()->getAppInstallPath();
    m_nTableConfigList = parseXml2TableConfig(installPath + "/DBTableXml/message_table_info.xml");
}

DBConfigMessage::~DBConfigMessage()
{

}
