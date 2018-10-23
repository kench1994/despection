/*!
* \file comparetool.h
* \brief 业务接口定义
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2017/2/15
*/
#ifndef BUSSINESSMAP_H
#define BUSSINESSMAP_H
#include <QMap>
#include <QString>
#include "dataoperationtype.h"
/**
 * @brief The BussinessMap class: 业务映射，objects映射到相应的业务模块
 */

//用户
#define User_QueryPhonePrincipal              "User_QueryPhonePrincipal"            ///获取终端用户权限信息
#define User_QueryPhonePrincipalByRFIDCode    "User_QueryPhonePrincipalByRFIDCode"  ///通过RFIDCode获取终端用户权限信息
#define SDMS_User_Change_Pwd                  "SDMS_User_Change_Pwd"                ///更新用户密码

//Home
#define Module_Query_ItemCounts_ForAndroid    "Module_Query_ItemCounts_ForAndroid"  ///获取待操作业务数量

//ScanTask
#define Equipment_ScanTaskListByRFID          "Equipment_ScanTaskListByRFID"        ///通过扫码获取任务项
#define EISM_GetTaskObjectListByGroup_RFID    "EISM_GetTaskObjectListByGroup_RFID"  ///扫码通过组查对象列表

#define User_QueryHistoryUsers_Local          "User_QueryHistoryUsers_Local"   ///获取历史登陆用户信息，本地接口
#define User_SetHistoryUsers_Local            "User_SetHistoryUsers_Local"     ///设置历史登陆用户信息，本地接口

//设备领用
#define EMD_Distribute_PlanList               "EMD_Distribute_PlanList"               ///领用任务列表
#define EMD_Off_GetUserRunningTaskDataKeys    "EMD_Off_GetUserRunningTaskDataKeys"    ///领用任务ID列表
#define EMD_Off_GetSomeRunningTasksPacket     "EMD_Off_GetSomeRunningTasksPacket"     ///根据领用任务ID获取任务详情
#define EMD_DistributeEquipmentList           "EMD_DistributeEquipmentList"           ///领用设备列表
#define EMD_DistributeEquipment_Para          "EMD_DistributeEquipment_Para"          ///获取设备领用确认参数信息
#define EMD_DistributeEquipment_Import        "EMD_DistributeEquipment_Import"        ///设备领用确认

//设备调拨
#define EMA_Allocation_PlanList               "EMA_Allocation_PlanList"            ///调拨任务列表
#define EMA_Off_GetUserRunningTaskDataKeys    "EMA_Off_GetUserRunningTaskDataKeys" ///调拨任务ID列表
#define EMA_Off_GetSomeRunningTasksPacket     "EMA_Off_GetSomeRunningTasksPacket"  ///根据调拨任务ID获取任务详情
#define EMA_AllocationEquipmentList           "EMA_AllocationEquipmentList"        ///调拨设备列表
#define EMA_Allocation_Import                 "EMA_Allocation_Import"              ///调拨确认

//检查整改
#define EXAM_ExamineRectifyProcList                "EXAM_ExamineRectifyProcList"                ///获取待整改实施单列表
#define EXAM_ExamineRectifyProcDetail              "EXAM_ExamineRectifyProcDetail"              ///整改实施明细
#define EXAM_ExamineRectify_Import                 "EXAM_ExamineRectify_Import"                 ///整改实施数据上报
#define EXAMR_Off_GetUserRunningTaskDataKeys       "EXAMR_Off_GetUserRunningTaskDataKeys"       ///整改实施任务ID列表
#define EXAMR_Off_GetSomeRunningTasksPacket        "EXAMR_Off_GetSomeRunningTasksPacket"        ///根据整改实施任务ID获取任务详情

//盘点
#define EMI_InventoryTask_RuningList            "EMI_InventoryTask_RuningList"       ///盘点任务
#define EMI_InventoryEquipmentList "EMI_InventoryEquipmentList"    //盘点设备列表
#define EMI_InventoryEquipment_Para "EMI_InventoryEquipment_Para" //盘点参数
#define EMI_Off_GetUserRunningTaskDataKeys      "EMI_Off_GetUserRunningTaskDataKeys" ///获取当前用户待盘点任务标识拼接串(离线)
#define EMI_Off_GetSomeRunningTasksPacket       "EMI_Off_GetSomeRunningTasksPacket"  ///获取若干(单)待盘点任务包(离线)
#define EMI_InventoryEquipment_Import "EMI_InventoryEquipment_Import"  //盘点设备信息提交

//设备
#define Equipment_QueryOne                    "Equipment_QueryOne"                    ///获取设备
#define Equipment_QueryList_ByName            "Equipment_QueryList_ByName"            ///根据名称查询设备列表
#define Equipment_ConRFID_Query_ByRFIDCode    "Equipment_ConRFID_Query_ByRFIDCode"    ///通过RFID编码等获取设备信息
#define EMB_Equipment_QRBCode_Import          "EMB_Equipment_QRBCode_Import"          ///RFID设备关联提交
#define EMB_Equipment_UpdateSerialKey         "EMB_Equipment_UpdateSerialKey"         ///关联图片

//规范检查
#define EXAM_ExamineTaskList                  "EXAM_ExamineTaskList"                       ///规范检查任务列表
#define EXAM_Off_GetUserRunningTaskDataKeys   "EXAM_Off_GetUserRunningTaskDataKeys"        ///规范检查任务ID列表
#define EXAM_Off_GetSomeRunningTasksPacket    "EXAM_Off_GetSomeRunningTasksPacket"         ///根据规范检查任务ID获取任务详情
#define EXAM_ExamineTaskObjectList            "EXAM_ExamineTaskObjectList"                 ///规范检查组列表
#define EXAM_ExamineTaskItemList              "EXAM_ExamineTaskItemList"                   ///规范检查项列表
#define EXAM_ExamineTaskItemDetail            "EXAM_ExamineTaskItemDetail"                 ///规范检查项详情
#define EXAM_ExamineTaskItem_Import           "EXAM_ExamineTaskItem_Import"                ///规范检查提交
#define EXAM_Off_GetPrePlanIDsByUserID        "EXAM_Off_GetPrePlanIDsByUserID"             ///规范检查PlanID列表
#define EXAM_Off_GetPrePacketByPlanID         "EXAM_Off_GetPrePacketByPlanID"              ///根据规范检查PlanID获取任务详情


//巡检
#define EISM_GetUserRunningTask    "EISM_GetUserRunningTask"   //巡检任务
#define EISM_GetTaskGroupList    "EISM_GetTaskGroupList"   //巡检任务
#define EISM_GetTaskObjectListByTask    "EISM_GetTaskObjectListByTask"   //通过任务查对象列表
#define EISM_GetTaskObjectListByGroup    "EISM_GetTaskObjectListByGroup"   //通过组查对象列表
#define EISM_GetTaskObjectDetail        "EISM_GetTaskObjectDetail"   //获取明细

#define EISM_GetDailyRunningTask "EISM_GetDailyRunningTask" //获取用户待巡检日常任务列表
#define EISM_GetDailyTaskGroupList "EISM_GetDailyTaskGroupList" //获取日常待巡对象组表
#define EISM_GetDailyObjectListByGroup "EISM_GetDailyObjectListByGroup" //根据对象组获取日常对象列表
#define EISM_GetDailyObjectListByTask "EISM_GetDailyObjectListByTask" //根据任务获取日常对象列表
#define EISM_GetDailyObjectDetail "EISM_GetDailyObjectDetail" //根据PlanObjectID获取日常任务对象详情

#define EISM_Off_GetUserRunningTaskDataKeys "EISM_Off_GetUserRunningTaskDataKeys" //获取当前用户待巡检任务标识拼接串(离线)
#define EISM_Off_GetSomeRunningTasksPacket "EISM_Off_GetSomeRunningTasksPacket" //获取若干(单)待巡检任务包(离线)
#define EISM_Off_GetUserDailyTaskDataKeys "EISM_Off_GetUserDailyTaskDataKeys"  // 	 	获取当前用户待巡检日常任务标识拼接串(离线)
#define EISM_Off_GetSomeDailyTasksPacket "EISM_Off_GetSomeDailyTasksPacket"  // 	 	 	获取若干(单)日常巡检任务包(离线)
#define EISM_Off_GetPrePlanIDsByUserID "EISM_Off_GetPrePlanIDsByUserID" //获取预生成任务计划ID列表
#define EISM_Off_GetPrePacketByPlanID "EISM_Off_GetPrePacketByPlanID" //通过任务ID获取预生成巡检任务包(离线)

#define EISM_TaskObject_UpdateOne  "EISM_TaskObject_UpdateOne" //提交
#define EISM_TaskGroupObject_UpdateOne "EISM_TaskGroupObject_UpdateOne" //组对象提交
#define EISM_PRE_TaskGroupObject_UpdateOne "EISM_PRE_TaskGroupObject_UpdateOne"  //预生成设备巡检组对象数据更新
#define EISM_PRE_TaskObject_UpdateOne "EISM_PRE_TaskObject_UpdateOne"  //预生成设备巡检数据更新
#define EISM_TaskDailyObject_ADDOne "EISM_TaskDailyObject_ADDOne"  //设备巡检日常数据提交

//保养
#define EMM_GetUserRunningTask                 "EMM_GetUserRunningTask"                ///保养任务
#define EMM_GetTaskGroupList                   "EMM_GetTaskGroupList"                  ///获取待保养对象组表(在线)
#define EMM_GetTaskObjectListByGroup           "EMM_GetTaskObjectListByGroup"          ///根据对象组获取待保养对象列表(在线)
#define EMM_GetTaskObjectListByTask            "EMM_GetTaskObjectListByTask"           ///根据任务获取待保养对象表(在线)
#define EMM_GetTaskObjectDetail                "EMM_GetTaskObjectDetail"               ///根据TaskObjectDataKey获取对象详情(在线)
#define EMM_TaskObject_UpdateOne               "EMM_TaskObject_UpdateOne"              ///保养设备信息提交
#define EMM_TaskObject_Parts_Import            "EMM_TaskObject_Parts_Import"           ///保养设备信息提交
#define EMM_MaintenanceParts_List              "EMM_MaintenanceParts_List"             ///获取保养零部件列表
#define EMM_MaintenanceParts_Local_Add         "EMM_MaintenanceParts_Local_Add"        ///新增保养零部件
#define EMM_MaintenanceParts_Local_Update      "EMM_MaintenanceParts_Local_Update"     ///修改保养零部件
#define EMM_MaintenanceParts_Local_Delete      "EMM_MaintenanceParts_Local_Delete"     ///删除保养零部件
#define EMM_Off_GetUserRunningTaskDataKeys     "EMM_Off_GetUserRunningTaskDataKeys"    ///保养任务ID列表
#define EMM_Off_GetSomeRunningTasksPacket      "EMM_Off_GetSomeRunningTasksPacket"     ///根据保养任务ID获取任务详情
#define EMM_Off_GetPrePlanIDsByUserID          "EMM_Off_GetPrePlanIDsByUserID"         ///保养PlanID列表
#define EMM_Off_GetPrePacketByPlanID           "EMM_Off_GetPrePacketByPlanID"          ///根据保养PlanID获取任务详情

//运维
#define EMF_FaultRepair_Para "EMF_FaultRepair_Para"       ///获取运维参数
#define EMF_FaultRepair_Query_List "EMF_FaultRepair_Query_List"    ///设备运维查询列表
#define EMF_FaultRepair_Query_One "EMF_FaultRepair_Query_One"    ///查询运维明细数据
#define EMF_FaultRepair_Query_Parts "EMF_FaultRepair_Query_Parts"     ///获取维修部件

#define EMF_FaultAct_List "EMF_FaultAct_List"     ///获取用户维修流程
#define EMF_FaultRepair_TaskAllInfo "EMF_FaultRepair_TaskAllInfo"            ///获取指定运维任务详细信息和受理任务信息
#define EMF_FaultRepair_Query_Task "EMF_FaultRepair_Query_Task"          ///获取指定运维任务列表
#define EMF_FaultRepair_ModelCount "EMF_FaultRepair_ModelCount"      ///获取模块对应的数量
#define EMF_FaultRepair_Import_Sync "EMF_FaultRepair_Import_Sync"     ///设备故障申报
#define EMF_FaultRepair_Handle_By_Device_Import "EMF_FaultRepair_Handle_By_Device_Import"       ///运维受理
#define EMF_FaultRepair_Audit_By_Device_Import   "EMF_FaultRepair_Audit_By_Device_Import"  ///运维审批
#define EMF_FaultRepair_Dispatch_By_Device_Import   "EMF_FaultRepair_Dispatch_By_Device_Import"    ///运维派工
#define EMF_FaultRepair_Repair_Part_Import    "EMF_FaultRepair_Repair_Part_Import"      ///运维维修
#define EMF_FaultRepair_Cost_By_Device_Import      "EMF_FaultRepair_Cost_By_Device_Import"    ///运维费用
#define EMF_FaultRepair_Appraise_By_Device_Import    "EMF_FaultRepair_Appraise_By_Device_Import" ///运维评价

//知识库
#define EMK_knowledge_all "EMK_knowledge_all" ///获取全部知识库列表
#define EMK_knowledge_query "EMK_knowledge_query" ///查询特定知识库列表
#define EMK_knowledge_One "EMK_knowledge_One" ///获取知识库明细


//同步设备
#define EMB_Off_GetEquipmentAndPartsListByJoinIDs "EMB_Off_GetEquipmentAndPartsListByJoinIDs" //同步设备
class BussinessMap
{
public:
    enum MuduleType{
        MODULE_TYPE_SYNC,
        MODULE_TYPE_HANDLER,
        MODULE_TYPE_LOCALCACHE
    };

    ~BussinessMap();

    /**
     * @brief getBussinessType : 获取objects映射的业务模块
     * @param object
     * @param moduleType : 模块
     * @return : 业务模块
     */
    DataOperationType getBussinessType(const QString &object, const MuduleType& moduleType);

    /**
     * @brief getInstance : 获取单例
     * @return : 单例指针
     */
    static BussinessMap *getInstance();
private:
     BussinessMap();
private:
    QMap<QString, int> m_nSyncManagerMap; ///< 映射的map
    QMap<QString, int> m_nHandlerManagerMap; ///< 映射的map
    QMap<QString, int> m_nLocalCacheManagerMap; ///< 映射的map
    static BussinessMap *m_pBussinessMap; ///< 单例指针
};
#endif // BUSSINESSMAP_H
