#include "theme.h"
#include "../../../framework/common/utils.h"
#include <QDomDocument>
#include <QFile>
#include <QDebug>

#define TAG "THEME_CPP : "
Theme::Theme()
{
    initialize();
}

Theme::~Theme()
{
    m_nIconNodeList.clear();
    m_nStaticNodeList.clear();
}

QString Theme::getID()
{
    return m_nID;
}

QString Theme::getNameEN()
{
    return m_nNameEN;
}

QString Theme::getNameCN() {
    return m_nNameCN;
}

QString Theme::getImage()
{
    return m_nImage;
}

QString Theme::getStyle()
{
    return m_nStyle;
}

void Theme::setID(const QString& value) {
    m_nID = value;
}

void Theme::setNameEN(const QString& value) {
    m_nNameEN = value;
}

void Theme::setNameCN(const QString& value)
{
    m_nNameCN = value;
}

void Theme::setImage(const QString& value)
{
    m_nImage = value;
}

void Theme::setStyle(const QString& value)
{
    m_nStyle = value;
}

QList<ThemeNode> Theme::getStaticNodes()
{
    return m_nStaticNodeList;
}

QList<ThemeNode> Theme::getIconNodes()
{
    return m_nIconNodeList;
}

int Theme::getStaticNodeCount()
{
    return m_nStaticNodeList.count();
}

int Theme::getIconNodeCount()
{
    return m_nIconNodeList.count();
}

void Theme::addStaticNode(ThemeNode node)
{
    m_nStaticNodeList.append(node);
}

void Theme::addIconNode(ThemeNode node)
{
    m_nIconNodeList.append(node);
}

void Theme::initialize()
{
    m_nIconNodeList.clear();
    m_nStaticNodeList.clear();
}

bool Theme::parse()
{
    QString xmlPath = m_nThemePath + "/" + THEME_FILE;
    QString content;
    QString errorMsg;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    QFile file(xmlPath);
    if (!file.open(QIODevice::ReadOnly)){
        qCritical() << TAG << xmlPath <<" isn't existed";
        return false;
    }
    content = QString::fromUtf8(file.readAll());
    if (!doc.setContent(content, &errorMsg, &errorLine, &errorColumn)) {
        qCritical() << TAG << xmlPath <<" format error";
        qCritical() << TAG << errorMsg;
        qCritical() << TAG << errorLine;
        qCritical() << TAG << errorColumn;
        file.close();
        return false;
    }
    QDomElement rootTag = doc.documentElement();
    setStyle(rootTag.attribute(THEME_STYLE));
    setNameCN(rootTag.attribute(THEME_NAME_CN));
    setNameEN(rootTag.attribute(THEME_NAME_EN));
    setImage(rootTag.attribute(THEME_IMAGE));
    setID(rootTag.tagName());
    QDomNode nodeTable = rootTag.firstChild();
    while(!nodeTable.isNull()){
        if(nodeTable.isElement()){
            QDomElement childNode = nodeTable.toElement();
            ThemeNode node;
            if(!parseNode(childNode, node)){
                qCritical() << TAG << "Parse sub-node fail";
                continue;
            }
            if(node.getType() == "static"){
                m_nStaticNodeList.append(node);
            }else{
                m_nIconNodeList.append(node);
            }
            nodeTable = nodeTable.nextSibling();
        }
    }
    orderNode();
    file.close();
    return true;
}

bool Theme::parseNode(QDomElement node, ThemeNode& out)
{
    QMap<QString, QVariant> para;
    out.setID(node.tagName());
    out.setType(node.attribute(THEME_NODE_TYPE));
    out.setIndex(node.attribute(THEME_NODE_INDEX, QString("0")).toInt());
    para.insert(THEME_NODE_ID, node.tagName());
    QDomNamedNodeMap nodeMap = node.attributes();
    for(int i = 0; i < nodeMap.count(); i++){
        QDomNode attributeNode = nodeMap.item(i);
        if(attributeNode.isAttr()){
            QDomAttr attribute = attributeNode.toAttr();
            QString key = attribute.name();
            QString value = attribute.value();
            para.insert(key, value);
        }
    }
    out.setPara(para);
    QDomNode nodeTable = node.firstChild();
    while(!nodeTable.isNull()){
        if(nodeTable.isElement()){
            QDomElement childNode = nodeTable.toElement();
            ThemeNode node;
            if(!parseNode(childNode, node)){
                qCritical() << TAG << "Parse sub-node fail";
                continue;
            }
            if(node.getType() == "static"){
                out.addStaticNode(node);
            }else{
                out.addIconNode(node);
            }
            nodeTable = nodeTable.nextSibling();
        }
    }
    out.orderNode();
    return true;
}

QString Theme::getThemeResPath()
{
    return m_nThemePath + "/" + THEME_RES_DIR;
}

QString Theme::getThemePath()
{
    return m_nThemePath;
}

void Theme::setThemePath(const QString& value)
{
    m_nThemePath = value;
}

bool caseInsensitiveLessThanTheme(const ThemeNode &s1, const ThemeNode &s2)
{
    return s1.getIndex() < s2.getIndex();
}

void Theme::orderNode()
{
    qStableSort(m_nIconNodeList.begin(), m_nIconNodeList.end(), caseInsensitiveLessThanTheme);
}
