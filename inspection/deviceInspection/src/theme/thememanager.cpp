#include "thememanager.h"
#include "themenode.h"
#include "../../../framework/common/systemconfig.h"
#include <QDir>
#include <QDebug>
#include "../../../framework/common/utils.h"

#define TAG "ThemeManager : "

ThemeManager* ThemeManager::m_pInstance = NULL;

ThemeManager::ThemeManager(QObject *parent) : QObject(parent)
{
    m_nCurrentThemeID = "default";
    initialize("");
}

ThemeManager::~ThemeManager()
{
    m_nThemeList.clear();
}

const QList<Theme>& ThemeManager::getThemeList() {
    return m_nThemeList;
}

bool ThemeManager::getTheme(const QString& id, Theme& ret) {
    if(m_nThemeList.size() <= 0) {
        return false;
    }
    for (int i = 0; i < m_nThemeList.size(); ++i) {
        Theme theme = m_nThemeList.at(i);
        if (theme.getID() == id){
            ret =  theme;
            return true;
        }
    }
    ret = m_nThemeList.at(0);
    return true;
}

bool ThemeManager::getCurrentTheme(Theme& ret) {
    QString id = getCurrentThemeID();
    qDebug() << "getCurrentThemeID: " << id;
    if(m_nThemeList.size() <= 0) {
        return false;
    }
    for (int i = 0; i < m_nThemeList.size(); ++i) {
        Theme theme = m_nThemeList.at(i);
        qDebug() << "theme.getID() " << theme.getID() ;
        if (theme.getID() == id){
            ret =  theme;
            return true;
        }
    }
    /// Return first theme if current theme doesn't existed
    ret = m_nThemeList.at(0);
    return true;
}

bool ThemeManager::setCurrentTheme(const QString& id) {
   Theme out;
    m_nCurrentThemeID = id;
    this->initialize(m_nCurrentThemeID);
    /// save default ID into INI file
    SystemConfig::getInstance()->setCurrentTheme(m_nCurrentThemeID);
    if(getCurrentTheme(out)){
        emit currentThemeChanged(out.getID(), out.getStyle());
    }
    return true;
}

QString ThemeManager::getThemesDir()
{
    /// Get App Directory
    QString ret = SystemConfig::getInstance()->getAppInstallPath();
    qDebug() << "install dir-------" << ret;
    ret += "/";
    ret += THEME_DIR;
    ret += "/";
    return ret;
}
QString ThemeManager::getThemesConfig()
{
    //获取/data/data/com.syberos.deviceInspection/systemConfig.ini文件中主题名称信息
    QString ret = SystemConfig::getInstance()->getCurrentTheme();
    return ret;

}

void ThemeManager::initialize(const QString & theme) {
//    QDir dir;
    QString strDir = getThemesDir();
    if(strDir.length() <= 0)
    {
        qCritical() << "initialize:getThemesDir() failed.";
        return;
    }
    if(theme.isEmpty()) {
            strDir.append(getThemesConfig());
    }else {
            strDir.append(theme);
    }

    qDebug() << "initialize:strDir=" << strDir;

    loadTheme(strDir);
//    dir.setPath(strDir);
//    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
//    QFileInfoList list = dir.entryInfoList();
//    for (int i = 0; i < list.size(); ++i) {
//        QFileInfo fileInfo = list.at(i);
//        qDebug() << "ThemeManager::initialize connt =" << list.size();
//        qDebug() << "ThemeManager::ifileInfo " << fileInfo.absoluteFilePath();
//        loadTheme(fileInfo.absoluteFilePath());
//    }

}

QString ThemeManager::getCurrentThemeID() {
    /// read default ID from INI file
    m_nCurrentThemeID = SystemConfig::getInstance()->getCurrentTheme();
    return m_nCurrentThemeID;
}

QString ThemeManager::getCurrentThemeStyle(){
    QString id = getCurrentThemeID();
    if(m_nThemeList.size() <= 0) {
        return "";
    }
    for (int i = 0; i < m_nThemeList.size(); ++i) {
        Theme theme = m_nThemeList.at(i);
        if (theme.getID() == id){
            return theme.getStyle();
        }
    }
    /// Return first theme if current theme doesn't existed
    Theme theme = m_nThemeList.at(0);
    return theme.getStyle();
}

void ThemeManager::loadTheme(const QString& path)
{
    QString xmlPath = path + "/" + THEME_FILE;
    qDebug() << "ThemeManager::loadTheme: xmlPath=" << xmlPath;
    if(!QFileInfo(xmlPath).exists()) {
        qCritical() << TAG << "Theme isn't existed : " << xmlPath;
        return;
    }
    Theme theme;
    theme.setThemePath(path);
    if(!theme.parse()) {
        qCritical() << TAG << "Parse theme fail : " << xmlPath;
    }
    m_nThemeList.append(theme);
}

ThemeManager* ThemeManager::getInstance(QObject *parent)
{
    if(!m_pInstance) {
        m_pInstance = new ThemeManager(parent);
    }
    return m_pInstance;
}
