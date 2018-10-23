#include "bussinessmap.h"
#include <QDebug>
BussinessMap * BussinessMap::m_pBussinessMap = NULL;
BussinessMap::BussinessMap(){
    //用户信息
    m_nHandlerManagerMap.insert(User_QueryPhonePrincipal,           BUSINESS_TYPE_USER_INFO);
    m_nHandlerManagerMap.insert(User_QueryPhonePrincipalByRFIDCode, BUSINESS_TYPE_USER_INFO);
    m_nHandlerManagerMap.insert(SDMS_User_Change_Pwd,               BUSINESS_TYPE_USER_INFO);
    m_nHandlerManagerMap.insert(User_QueryHistoryUsers_Local,               BUSINESS_TYPE_USER_INFO);///获取历史登陆用户信息，本地接口
    m_nHandlerManagerMap.insert(User_SetHistoryUsers_Local,               BUSINESS_TYPE_USER_INFO);///设置历史登陆用户信息，本地接口

    //主功能页面
    m_nHandlerManagerMap.insert(Module_Query_ItemCounts_ForAndroid, BUSINESS_TYPE_HOME);

    //扫码
    m_nHandlerManagerMap.insert(Equipment_ScanTaskListByRFID, BUSINESS_TYPE_SCAN);
    m_nHandlerManagerMap.insert(EISM_GetTaskObjectListByGroup_RFID, BUSINESS_TYPE_SCAN);

    //设备运维
    m_nHandlerManagerMap.insert(EMF_FaultRepair_Para, BUSINESS_TYPE_OPERATION); //获取运维参数
    m_nHandlerManagerMap.insert(EMF_FaultRepair_Query_List, BUSINESS_TYPE_OPERATION); //设备运维查询列表
    m_nHandlerManagerMap.insert(EMF_FaultRepair_Query_One, BUSINESS_TYPE_OPERATION); //查询运维明细数据
    m_nHandlerManagerMap.insert(EMF_FaultRepair_Query_Parts, BUSINESS_TYPE_OPERATION); //获取维修部件

    m_nHandlerManagerMap.insert(EMF_FaultAct_List, BUSINESS_TYPE_OPERATION);     //获取用户维修流程
    m_nHandlerManagerMap.insert(EMF_FaultRepair_TaskAllInfo, BUSINESS_TYPE_OPERATION);     //获取指定运维任务详细信息和受理任务信息
    m_nHandlerManagerMap.insert(EMF_FaultRepair_Query_Task, BUSINESS_TYPE_OPERATION); //获取指定运维任务列表
    m_nHandlerManagerMap.insert(EMF_FaultRepair_ModelCount, BUSINESS_TYPE_OPERATION); //获取模块对应的数量
    m_nLocalCacheManagerMap.insert(EMF_FaultRepair_Import_Sync, BUSINESS_TYPE_OPERATION); //设备故障申报
    m_nLocalCacheManagerMap.insert(EMF_FaultRepair_Handle_By_Device_Import, BUSINESS_TYPE_OPERATION);//运维受理
    m_nLocalCacheManagerMap.insert(EMF_FaultRepair_Audit_By_Device_Import, BUSINESS_TYPE_OPERATION);//运维审批
    m_nLocalCacheManagerMap.insert(EMF_FaultRepair_Dispatch_By_Device_Import, BUSINESS_TYPE_OPERATION);//运维派工
    m_nLocalCacheManagerMap.insert(EMF_FaultRepair_Repair_Part_Import, BUSINESS_TYPE_OPERATION);//运维维修
    m_nLocalCacheManagerMap.insert(EMF_FaultRepair_Cost_By_Device_Import, BUSINESS_TYPE_OPERATION);//运维费用
    m_nLocalCacheManagerMap.insert(EMF_FaultRepair_Appraise_By_Device_Import, BUSINESS_TYPE_OPERATION);//运维评价

    //巡检
    m_nHandlerManagerMap.insert(EISM_GetUserRunningTask, BUSINESS_TYPE_INSPECTION);//巡检任务
    m_nHandlerManagerMap.insert(EISM_GetTaskGroupList, BUSINESS_TYPE_INSPECTION);//待巡检套件
    m_nHandlerManagerMap.insert(EISM_GetTaskObjectListByTask, BUSINESS_TYPE_INSPECTION); //通过任务查对象列表
    m_nHandlerManagerMap.insert(EISM_GetTaskObjectListByGroup, BUSINESS_TYPE_INSPECTION); //通过组查对象列表
    m_nHandlerManagerMap.insert(EISM_GetTaskObjectDetail, BUSINESS_TYPE_INSPECTION);   //获取明细


    m_nSyncManagerMap.insert(EISM_Off_GetPrePlanIDsByUserID, BUSINESS_TYPE_INSPECTION); //获取预生成任务计划ID列表
    m_nSyncManagerMap.insert(EISM_Off_GetPrePacketByPlanID, BUSINESS_TYPE_INSPECTION); //通过任务ID获取预生成巡检任务包(离线)
    m_nHandlerManagerMap.insert(EISM_Off_GetUserRunningTaskDataKeys, BUSINESS_TYPE_INSPECTION); // 	获取当前用户待巡检任务标识拼接串(离线)
    m_nSyncManagerMap.insert(EISM_Off_GetSomeRunningTasksPacket, BUSINESS_TYPE_INSPECTION); // 	获取若干(单)待巡检任务包(离线)

    m_nLocalCacheManagerMap.insert(EISM_TaskObject_UpdateOne, BUSINESS_TYPE_INSPECTION);// 提交
    m_nLocalCacheManagerMap.insert(EISM_TaskGroupObject_UpdateOne, BUSINESS_TYPE_INSPECTION); //组对象提交
    m_nLocalCacheManagerMap.insert(EISM_PRE_TaskGroupObject_UpdateOne, BUSINESS_TYPE_INSPECTION);  //预生成设备巡检组对象数据更新
    m_nLocalCacheManagerMap.insert(EISM_PRE_TaskObject_UpdateOne, BUSINESS_TYPE_INSPECTION);  //预生成设备巡检数据更新

    //日常任务
    m_nHandlerManagerMap.insert(EISM_GetDailyRunningTask, BUSINESS_TYPE_DAILY); //获取用户待巡检日常任务列表
    m_nHandlerManagerMap.insert(EISM_GetDailyObjectDetail, BUSINESS_TYPE_DAILY); //根据PlanObjectID获取日常任务对象详情
    m_nHandlerManagerMap.insert(EISM_GetDailyObjectListByGroup, BUSINESS_TYPE_DAILY); //根据对象组获取日常对象列表
    m_nHandlerManagerMap.insert(EISM_GetDailyObjectListByTask, BUSINESS_TYPE_DAILY); //根据任务获取日常对象列表
    m_nHandlerManagerMap.insert(EISM_GetDailyTaskGroupList, BUSINESS_TYPE_DAILY); //获取日常待巡对象组表
    m_nHandlerManagerMap.insert(EISM_Off_GetUserDailyTaskDataKeys, BUSINESS_TYPE_DAILY); // 	 	获取当前用户待巡检日常任务标识拼接串(离线)
    m_nSyncManagerMap.insert(EISM_Off_GetSomeDailyTasksPacket, BUSINESS_TYPE_DAILY); // 	 	 	获取若干(单)日常巡检任务包(离线)
    m_nLocalCacheManagerMap.insert(EISM_TaskDailyObject_ADDOne, BUSINESS_TYPE_DAILY);  //设备巡检日常数据提交

    //规范检查
    m_nHandlerManagerMap.insert(EXAM_ExamineTaskList, BUSINESS_TYPE_STANDARDIZATION);
    m_nHandlerManagerMap.insert(EXAM_ExamineTaskObjectList, BUSINESS_TYPE_STANDARDIZATION);
    m_nHandlerManagerMap.insert(EXAM_ExamineTaskItemList, BUSINESS_TYPE_STANDARDIZATION);
    m_nHandlerManagerMap.insert(EXAM_ExamineTaskItemDetail, BUSINESS_TYPE_STANDARDIZATION);
    m_nHandlerManagerMap.insert(EXAM_Off_GetUserRunningTaskDataKeys,BUSINESS_TYPE_STANDARDIZATION);
    m_nSyncManagerMap.insert(EXAM_Off_GetSomeRunningTasksPacket, BUSINESS_TYPE_STANDARDIZATION);
    m_nLocalCacheManagerMap.insert(EXAM_ExamineTaskItem_Import, BUSINESS_TYPE_STANDARDIZATION);
    m_nSyncManagerMap.insert(EXAM_Off_GetPrePlanIDsByUserID, BUSINESS_TYPE_STANDARDIZATION);
    m_nSyncManagerMap.insert(EXAM_Off_GetPrePacketByPlanID, BUSINESS_TYPE_STANDARDIZATION);

    //盘点
    m_nHandlerManagerMap.insert(EMI_InventoryTask_RuningList, BUSINESS_TYPE_INVENTORY);
    m_nHandlerManagerMap.insert(EMI_InventoryEquipmentList, BUSINESS_TYPE_INVENTORY);    //盘点设备列表
    m_nHandlerManagerMap.insert(EMI_InventoryEquipment_Para, BUSINESS_TYPE_INVENTORY); //盘点参数
    m_nHandlerManagerMap.insert(EMI_Off_GetUserRunningTaskDataKeys, BUSINESS_TYPE_INVENTORY); //获取当前用户待盘点任务标识拼接串(离线)
    m_nSyncManagerMap.insert(EMI_Off_GetSomeRunningTasksPacket, BUSINESS_TYPE_INVENTORY); //获取若干(单)待盘点任务包(离线)
    m_nLocalCacheManagerMap.insert(EMI_InventoryEquipment_Import, BUSINESS_TYPE_INVENTORY);  //盘点设备信息提交

    //保养
    m_nHandlerManagerMap.insert(EMM_GetUserRunningTask, BUSINESS_TYPE_MAINTENANCE);
    m_nHandlerManagerMap.insert(EMM_GetTaskGroupList, BUSINESS_TYPE_MAINTENANCE);
    m_nHandlerManagerMap.insert(EMM_GetTaskObjectListByGroup, BUSINESS_TYPE_MAINTENANCE);
    m_nHandlerManagerMap.insert(EMM_GetTaskObjectListByTask, BUSINESS_TYPE_MAINTENANCE);
    m_nHandlerManagerMap.insert(EMM_GetTaskObjectDetail, BUSINESS_TYPE_MAINTENANCE);
    m_nLocalCacheManagerMap.insert(EMM_TaskObject_UpdateOne, BUSINESS_TYPE_MAINTENANCE);
    m_nLocalCacheManagerMap.insert(EMM_TaskObject_Parts_Import, BUSINESS_TYPE_MAINTENANCE);
    m_nHandlerManagerMap.insert(EMM_Off_GetUserRunningTaskDataKeys, BUSINESS_TYPE_MAINTENANCE);
    m_nSyncManagerMap.insert(EMM_Off_GetSomeRunningTasksPacket, BUSINESS_TYPE_MAINTENANCE);
    m_nSyncManagerMap.insert(EMM_Off_GetPrePlanIDsByUserID, BUSINESS_TYPE_MAINTENANCE);
    m_nSyncManagerMap.insert(EMM_Off_GetPrePacketByPlanID, BUSINESS_TYPE_MAINTENANCE);

   //设备领用
   m_nHandlerManagerMap.insert(EMD_Off_GetUserRunningTaskDataKeys, BUSINESS_TYPE_DEVICE_RECEIVE);
   m_nSyncManagerMap.insert(EMD_Off_GetSomeRunningTasksPacket, BUSINESS_TYPE_DEVICE_RECEIVE);
   m_nHandlerManagerMap.insert(EMD_Distribute_PlanList, BUSINESS_TYPE_DEVICE_RECEIVE);
   m_nHandlerManagerMap.insert(EMD_DistributeEquipmentList, BUSINESS_TYPE_DEVICE_RECEIVE);
   m_nHandlerManagerMap.insert(EMD_DistributeEquipment_Para, BUSINESS_TYPE_DEVICE_RECEIVE);
   m_nLocalCacheManagerMap.insert(EMD_DistributeEquipment_Import, BUSINESS_TYPE_DEVICE_RECEIVE);

   //设备调拨
   m_nHandlerManagerMap.insert(EMA_Off_GetUserRunningTaskDataKeys, BUSINESS_TYPE_DEVICE_DEPLOYMENT);
   m_nSyncManagerMap.insert(EMA_Off_GetSomeRunningTasksPacket, BUSINESS_TYPE_DEVICE_DEPLOYMENT);
   m_nHandlerManagerMap.insert(EMA_Allocation_PlanList, BUSINESS_TYPE_DEVICE_DEPLOYMENT);
   m_nHandlerManagerMap.insert(EMA_AllocationEquipmentList, BUSINESS_TYPE_DEVICE_DEPLOYMENT);
   m_nLocalCacheManagerMap.insert(EMA_Allocation_Import, BUSINESS_TYPE_DEVICE_DEPLOYMENT);

   //检查整改
   m_nHandlerManagerMap.insert(EXAM_ExamineRectifyProcList, BUSINESS_TYPE_RECTIFICATION);
   m_nHandlerManagerMap.insert(EXAM_ExamineRectifyProcDetail, BUSINESS_TYPE_RECTIFICATION);
   m_nLocalCacheManagerMap.insert(EXAM_ExamineRectify_Import, BUSINESS_TYPE_RECTIFICATION);
   m_nHandlerManagerMap.insert(EXAMR_Off_GetUserRunningTaskDataKeys, BUSINESS_TYPE_RECTIFICATION);
   m_nSyncManagerMap.insert(EXAMR_Off_GetSomeRunningTasksPacket, BUSINESS_TYPE_RECTIFICATION);

   //设备
   m_nHandlerManagerMap.insert(Equipment_QueryList_ByName, BUSINESS_TYPE_DEVICES);
   m_nHandlerManagerMap.insert(Equipment_QueryOne, BUSINESS_TYPE_DEVICES);
   m_nHandlerManagerMap.insert(Equipment_ConRFID_Query_ByRFIDCode, BUSINESS_TYPE_DEVICES);
   m_nLocalCacheManagerMap.insert(EMB_Equipment_QRBCode_Import, BUSINESS_TYPE_DEVICES);
   m_nLocalCacheManagerMap.insert(EMB_Equipment_UpdateSerialKey, BUSINESS_TYPE_DEVICES);

   //系统消息
   m_nLocalCacheManagerMap.insert("User_AppToken_Update", BUSINESS_TYPE_MESSAGE);      //更新Token
   m_nHandlerManagerMap.insert("CMDS_MessageList", BUSINESS_TYPE_MESSAGE);          //获取消息
   m_nHandlerManagerMap.insert("CMDS_MessageList_Push", BUSINESS_TYPE_MESSAGE); //由pushserver传递的消息，交由messageHandler处理
   m_nHandlerManagerMap.insert("CMDS_Message_Delete", BUSINESS_TYPE_MESSAGE);       //删除消息
   m_nHandlerManagerMap.insert("CMDS_Message_Update", BUSINESS_TYPE_MESSAGE);       //更新系统消息
   m_nHandlerManagerMap.insert("Module_Query_MessageCounts_ForAndroid", BUSINESS_TYPE_MESSAGE);       //读取系统消息角标

   //升级更新
   m_nHandlerManagerMap.insert("DEX_GetApp_One", BUSINESS_TYPE_UPDATE);          //获取客户端版本
   m_nHandlerManagerMap.insert("DownloadFileStream", BUSINESS_TYPE_UPDATE);          //下载客户端版本

   //知识库
   m_nHandlerManagerMap.insert(EMK_knowledge_all, BUSINESS_TYPE_KNOWLEDGE);          //获取全部知识库列表
   m_nHandlerManagerMap.insert(EMK_knowledge_query, BUSINESS_TYPE_KNOWLEDGE);          //查询特定知识库列表
   m_nHandlerManagerMap.insert(EMK_knowledge_One, BUSINESS_TYPE_KNOWLEDGE);          //获取知识库明细
}

BussinessMap::~BussinessMap(){
}

BussinessMap* BussinessMap::getInstance(){
    if(m_pBussinessMap == NULL)
        m_pBussinessMap = new BussinessMap();
    return m_pBussinessMap;
}

DataOperationType BussinessMap::getBussinessType(const QString &object, const BussinessMap::MuduleType &moduleType)
{
    qDebug() << Q_FUNC_INFO << " object: " << object;
    DataOperationType type = BUSINESS_TYPE_UNKOWN;
    switch (moduleType) {
    case MODULE_TYPE_SYNC:
        if(m_nSyncManagerMap.contains(object))
            type = static_cast<DataOperationType>(m_nSyncManagerMap[object]);
        break;
    case MODULE_TYPE_HANDLER:
        if(m_nHandlerManagerMap.contains(object))
            type = static_cast<DataOperationType>(m_nHandlerManagerMap[object]);
        break;
    case MODULE_TYPE_LOCALCACHE:
        if(m_nLocalCacheManagerMap.contains(object))
            type = static_cast<DataOperationType>(m_nLocalCacheManagerMap[object]);
        break;
    default:
        break;
    }
    return type;
}
