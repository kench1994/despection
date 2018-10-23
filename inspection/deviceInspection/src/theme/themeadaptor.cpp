#include "themeadaptor.h"
#include "thememanager.h"
#include <QDebug>

#define TAG "ThemeAdaptor : "
ThemeAdaptor::ThemeAdaptor(QObject *parent) : QObject(parent)
{
    reflesh("");
}

ThemeAdaptor::~ThemeAdaptor()
{

}

void ThemeAdaptor::reflesh(const QString &theme)
{
    bool ret = true;

    if( !theme.isEmpty() ) {
        qDebug() << TAG << "theme is not empty";
        ret = ThemeManager::getInstance()->setCurrentTheme(theme);
        if(!ret) {
            qCritical() << TAG << "Set current theme failed.";
            return;
        }
    }

    ret = ThemeManager::getInstance()->getCurrentTheme(m_nTheme);
    if(!ret) {
        qCritical() << TAG << "Couldn't find current theme";
        return;
    }
    m_nIconNodes = m_nTheme.getIconNodes();
    m_nStaticNodes = m_nTheme.getStaticNodes();
}

QString ThemeAdaptor::getThemePath()
{
    return m_nTheme.getThemePath();
}

QString ThemeAdaptor::getThemeResPath()
{
    return m_nTheme.getThemeResPath();
}

QString ThemeAdaptor::getID()
{
    return m_nTheme.getID();
}

QString ThemeAdaptor::getNameCN()
{
    return m_nTheme.getNameCN();
}

QString ThemeAdaptor::getNameEN()
{
    return m_nTheme.getNameEN();
}

QString ThemeAdaptor::getImagePath()
{
    QString ret = getThemeResPath();
    ret += "/";
    ret += m_nTheme.getImage();
    return ret;
}

QString ThemeAdaptor::getStyle()
{
    return m_nTheme.getStyle();
}

int ThemeAdaptor::getIconCount()
{
    return m_nIconNodes.size();
}

int ThemeAdaptor::getStaticCount()
{
    return m_nStaticNodes.size();
}

QMap<QString, QVariant> ThemeAdaptor::getNodeParas(const QString& id, const QString& parentId)
{
    QMap<QString, QVariant> ret;
    if(parentId.isEmpty()){
        for(int i = 0; i < m_nIconNodes.count(); i++){
            ThemeNode node = m_nIconNodes.at(i);
            if(node.getID() == id) {
                ret = node.getPara();
                break;
            }
        }
    }else{
        int i;
        QList<ThemeNode> parentNodes;
        bool found = false;
        for(i = 0; i < m_nIconNodes.count(); i++){
            ThemeNode node = m_nIconNodes.at(i);
            if(node.getID() == parentId) {
                parentNodes = node.getIconNodes();
                found = true;
                break;
            }
        }
        if(found){
            for(i = 0; i < parentNodes.count(); i++){
                ThemeNode node = parentNodes.at(i);
                if(node.getID() == id) {
                    ret = node.getPara();
                    break;
                }
            }
        }
    }
    return ret;
}

QMap<QString, QVariant> ThemeAdaptor::getNodeParas(const int& index, const QString& parentId)
{
    QMap<QString, QVariant> ret;
    if(parentId.isEmpty()){
        if(index > m_nIconNodes.size() - 1){
            return ret;
        }
        ThemeNode tmp = m_nIconNodes.at(index);
        ret = tmp.getPara();
    }else{
        int i;
        QList<ThemeNode> parentNodes;
        bool found = false;
        for(i = 0; i < m_nIconNodes.count(); i++){
            ThemeNode node = m_nIconNodes.at(i);
            if(node.getID() == parentId) {
                parentNodes = node.getIconNodes();
                found = true;
                break;
            }
        }
        if(found){
            if(index > parentNodes.size() - 1) {
                return ret;
            }
            ThemeNode tmp = parentNodes.at(index);
            ret = tmp.getPara();
        }
    }
    return ret;
}

QMap<QString, QVariant> ThemeAdaptor::getStaticParas(const QString& id)
{
    QMap<QString, QVariant> ret;
    for(int i = 0; i < m_nStaticNodes.count(); i++){
        ThemeNode node = m_nStaticNodes.at(i);
        if(node.getID() == id) {
            ret = node.getPara();
            break;
        }
    }
    return ret;
}

