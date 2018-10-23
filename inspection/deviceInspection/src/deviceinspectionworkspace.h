/*!
* \file deviceinspectionworkspace.h
* \brief 进程工作空间
*
*此类是进程工作空间
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
#ifndef DEVICEINSPECTIONWORKSPACE_H
#define DEVICEINSPECTIONWORKSPACE_H

#include <QQuickView>
#include <cworkspace.h>
#include <SyberosGuiCache>
#include <cgui_application.h>

/*!
  \class DeviceInspectionWorkSpace
  \inModule 进程工作空间
  \brief 进程工作空间
  \ingroup main
  DeviceInspectionWorkSpace 进程工作空间
*/
using namespace SYBEROS;

class DeviceInspectionWorkSpace : public CWorkspace
{
    Q_OBJECT

public:
    DeviceInspectionWorkSpace(QObject* parent = 0);

    /**
     * @brief onLaunchComplete： 点击桌面图标启动程序时framework调用该函数
     * @param option： 启动方式
     * @param params： 启动参数
     */
    void onLaunchComplete(Option option, const QStringList& params);

    /**
     * @brief open： 程序在后台，被别的程序调用
     * @param params： 启动参数
     */
    void open(const QStringList& params);

private slots:
    /**
     * @brief initialize : 程序初始化函数
     */
    void initialize();

    /**
     * @brief setWindowActiveFlag: 设置程序活动状态
     */
    void setWindowActiveFlag();

private:

    /**
     * @brief entryMsgDetail： 点击通知栏系统消息时，进入消息页面
     * @param uuid：消息的uuid
     */
    void entryMsgDetail(QString uuid);

    QQuickView* m_pViewer;   ///< Qquickview指针

    bool m_nWindowIsActive; ///< 窗口是否活动
};

#endif // DEVICEINSPECTIONWORKSPACE_H
