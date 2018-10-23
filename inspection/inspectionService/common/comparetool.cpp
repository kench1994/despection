#include "comparetool.h"
#include <QDebug>
CompareTool::CompareTool()
{
}

CompareTool::~CompareTool()
{
}

QStringList CompareTool::compare(const QList<QMap<QString, QString> > &localValues,
                                    const QList<QMap<QString, QString> > &netValues, const QString &keywords)
{
    QStringList deleteList;
    qDebug() << Q_FUNC_INFO<< "locaValues size " << localValues.size() << "  netValues size  " << netValues.size();
    if (!keywords.isEmpty()) {
        for(int i = 0; i < localValues.count(); i++){
            QMap<QString, QString> localValue = localValues.at(i);
            if(!localValue.contains(keywords)){
                qDebug() << "!localValue.contains(keywords) " << keywords;
                return deleteList;
            }
            bool find = false;
            QString localKeyValue = localValue[keywords];
            for(int j = 0; j < netValues.count(); j++){
                QMap<QString, QString> netValue = netValues.at(j);
                QString netKeyValue = netValue[keywords];
                if(netKeyValue == localKeyValue){
                    find = true;
                    break;
                }
            }
            if(!find){
                deleteList.append(localKeyValue);
            }
        }
    } else {
        qDebug() << "keywords.isEmpty() ";
    }
    return deleteList;
}

void CompareTool::compare(const DataPackage &data, const QString &keyWords)
{
    QString strStatus;
    QList<QMap<QString, QString> > values = data.tables.at(0).value;
    for(int i = 0; i < values.count(); i++){
        strStatus = values.at(i)["Result"];
        m_nTimeList.append(values.at(i)["ServerDate"]);
        if(strStatus == "1")
            m_nAddList.append(values.at(i)[keyWords]);
        else if(strStatus == "2")
            m_nUpdateList.append(values.at(i)[keyWords]);
        else if(strStatus == "3")
            m_nDeleteList.append(values.at(i)[keyWords]);
    }
}

const QStringList & CompareTool::getAddList()
{
    return m_nAddList;
}

const QStringList & CompareTool::getUpdateList()
{
    return m_nUpdateList;
}

const QStringList & CompareTool::getDeleteList()
{
    return m_nDeleteList;
}

const QStringList &CompareTool::getTimeList()
{
    return m_nTimeList;
}
