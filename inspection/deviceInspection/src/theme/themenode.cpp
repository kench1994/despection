#include "themenode.h"

ThemeNode::ThemeNode()
{
    m_nIndex = 0;
}

ThemeNode::~ThemeNode()
{
    m_nIconNodeList.clear();
    m_nStaticNodeList.clear();
}

/**
 * @return const QString&
 */
QString ThemeNode::getID()
{
    return m_nID;
}

QString ThemeNode::getType()
{
    return m_nType;
}

int ThemeNode::getStaticNodeCount()
{
    return m_nStaticNodeList.count();
}

int ThemeNode::getIconNodeCount()
{
    return m_nIconNodeList.count();
}

QList<ThemeNode> ThemeNode::getStaticNodes()
{
    return m_nStaticNodeList;
}

QList<ThemeNode> ThemeNode::getIconNodes()
{
    return m_nIconNodeList;
}

void ThemeNode::setID(const QString& value)
{
    m_nID = value;
}

void ThemeNode::setType(const QString& value)
{
    m_nType = value;
}

void ThemeNode::addStaticNode(ThemeNode node)
{
    m_nStaticNodeList.append(node);
}

void ThemeNode::addIconNode(ThemeNode node)
{
    m_nIconNodeList.append(node);
}

void ThemeNode::setPara(const QMap<QString, QVariant> value)
{
    m_nPara = value;
}

QMap<QString, QVariant> ThemeNode::getPara()
{
    return m_nPara;
}

void ThemeNode::setIndex(const int& value)
{
    m_nIndex = value;
}

int ThemeNode::getIndex() const
{
    return m_nIndex;
}

bool caseInsensitiveLessThanThemeNode(const ThemeNode &s1, const ThemeNode &s2)
{
    return s1.getIndex() < s2.getIndex();
}

void ThemeNode::orderNode()
{
    qStableSort(m_nIconNodeList.begin(), m_nIconNodeList.end(), caseInsensitiveLessThanThemeNode);
}
