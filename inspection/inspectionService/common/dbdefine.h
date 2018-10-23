/*!
* \file comparetool.h
* \brief 数据库表名和字段宏定义类
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2017/2/15
*/
#ifndef DBDEFINE_H
#define DBDEFINE_H

/**
* 设备领用数据库相关常量定义
*/
#define DISTRIBUTION_TABLE_TASK            "EMD_Distribute"
#define DISTRIBUTION_TABLE_EQUIPMENT       "EMD_DistributeEquipment"
#define DISTRIBUTION_TABLE_TASK_SYNC_DATE  "EMD_DistributeTaskSyncState"
#define DISTRIBUTION_TABLE_SYNC_DATE       "EMD_DistributeSyncState"
#define DISTRIBUTION_TABLE_SUBMISSION      "EMD_DistributeSubmit"

/**
* 规范检查表名称
*/
#define EXAMINE_TABLE_TASK               "EXAM_ExamineTask"
#define EXAMINE_TABLE_OBJECT             "EXAM_ExamineTaskObject"
#define EXAMINE_TABLE_ITEM               "EXAM_ExamineTaskItem"
#define EXAMINE_TABLE_AREA               "EXAM_ExamineTaskArea"
#define EXAMINE_TABLE_ORGANIZATION       "EXAM_ExamineOrganiseUnit"
#define EXAMINE_TABLE_TEXT_SUBMISSION    "EXAM_ExamineTextSubmit"
#define EXAMINE_TABLE_BINARY_SUBMISSION  "EXAM_ExamineBinarySubmit"
#define EXAMINE_TABLE_TASK_SYNC_DATE     "EXAM_ExamineTaskSyncState"
#define EXAMINE_TABLE_SYNC_DATE          "EXAM_ExamineSyncState"

/**
* 保养表名称
*/
#define MAINTENANCE_TABLE_USER_TIME          "EMM_Maintenance_Time"
#define MAINTENANCE_TABLE_USER_TASK_TIME     "EMM_Maintenance_Task_Time"
#define MAINTENANCE_TABLE_TASK               "EMM_Maintenance_Task"
#define MAINTENANCE_TABLE_TASK_GROUP         "EMM_Maintenance_Task_Group"
#define MAINTENANCE_TABLE_TASK_OBJECT        "EMM_Maintenance_Task_Object"
#define MAINTENANCE_TABLE_SUBMIT             "EMM_Maintenance_Submit"
#define MAINTENANCE_TABLE_ATTACHMENT_SUBMIT  "EMM_Maintenance_Attachment_Submit"
#define MAINTENANCE_TABLE_PARTS              "EMM_Maintenance_Parts"


/**
* 设备调拨表名称
*/
#define   DEPLOYMENT_TABLE_USER_TIME         "EMA_Allocation_Time"
#define   DEPLOYMENT_TABLE_USER_TASK_TIME    "EMA_Allocation_Task_Time"
#define   DEPLOYMENT_TABLE_TASK              "EMA_Allocation_Task"
#define   DEPLOYMENT_TABLE_EQUIPMENT         "EMA_Allocation_Equipment"
#define   DEPLOYMENT_TABLE_SUBMIT            "EMA_Allocation_Submit"

/**
* 检查整改表名称
*/
#define   RECTIFICATION_TABLE_TASK              "Exam_Examine_Rectify"
#define   RECTIFICATION_TABLE_USER_TIME         "Exam_Examine_Rectify_User_Time"
#define   RECTIFICATION_TABLE_USER_TASK_TIME    "Exam_Examine_Rectify_Task_Time"
#define   RECTIFICATION_TABLE_SUBMIT            "Exam_Examine_Rectify_Submit"
#define   RECTIFICATION_TABLE_ATTACHMENT_SUBMIT "Exam_Examine_Rectify_Attachment_Submit"



/**
* 故障申报表名称
*/
#define   FAULT_REPORT_TABLE_SUBMIT_DETAIL      "EMF_FaultRepair_ImportDetail"
#define   FAULT_REPAIR_TABLE_ATTACHMENT_SUBMIT  "EMF_FaultRepair_BinarySubmit"


/**
* 设备及部件表名称
*/
#define   DEVICE_TABLE                  "EMB_Equipment"
#define   DEVICE_PART_TABLE             "EMB_Equipment_Part"
#define   DEVICE_SUBMIT_TABLE           "EMB_Submit"
#define   DEVICE_SUBMIT_ATTACH_TABLE    "EMB_Attach_Submit"

/**
* 用户表名称
*/
#define   USER_TABLE                    "SDMS_User"
#define   USER_TABLE_POST               "SDMS_User_Post"
#define   USER_TABLE_APPLICATION        "SDMS_Application"
#define   USER_TABLE_MODULE             "SDMS_Module"
#define   HISTORY_USER_TABLE             "SDMS_HistoryUser"


/**
*盘点表名称
*/
#define   INVENTORY_TABLE_TASK            "EMI_InventoryTask"
#define   INVENTORY_TABLE_OBJECT          "EMI_Inventory_Object"
#define   INVENTORY_TABLE_SUBMIT          "EMI_Inventory_submit"
#define   INVENTORY_TABLE_DIC             "EMI_InventoryDic"
#define   INVENTORY_TABLE_USER_TIME       "EMI_Inventory_User_Time"
#define   INVENTORY_USER_TASK_TIME        "EMI_Inventory_User_Task_Time"

/**
* 巡检表名称
*/
#define   INSPECTION_TABLE_TASK                    "ISM_InspectionTask"
#define   INSPECTION_TABLE_TASK_GROUP              "ISM_InspectionTaskGroup"
#define   INSPECTION_TABLE_TASK_OBJECT             "ISM_InspectionTaskObject"
#define   INSPECTION_TABLE_SUBMIT                  "ISM_Inspection_Submit"
#define   INSPECTION_TABLE_ATTACH_SUBMIT           "ISM_Inspection_Attachment_Submit"
#define   INSPECTION_TABLE_DAILY_TASK              "ISM_InspectionPlanTask"
#define   INSPECTION_TABLE_DAILY_TASK_GROUP        "ISM_InspectionPlanGroup"
#define   INSPECTION_TABLE_DAILY_TASK_OBJECT       "ISM_InspectionPlanObject"
#define   INSPECTION_TABLE_USER_TIME               "ISM_Inspection_Time"
#define   INSPECTION_TABLE_USER_TASK_TIME          "ISM_Inspection_Task_Time"
#define   INSPECTION_TABLE_DAILY_USER_TIME         "ISM_Inspection_Daily_Time"
#define   INSPECTION_TABLE_DAILY_USER_TASK_TIME    "ISM_Inspection_Daily_Task_Time"


/**
* Home表名称
*/
#define   HOME_TABLE                   "CMDS_HomeInfo"
#define   HOME_TABLE_MODULE_COUNT      "CMDS_ItemCount"


/**
*字段名称
*/
#define   DB_ID                          "ID"
#define   TASK_DATA_KEY                  "TaskDataKey"
#define   TASK_GROUP_DATA_KEY            "TaskGroupDataKey"
#define   TASK_OBJECT_DATA_KEY           "TaskObjectDataKey"
#define   TASK_ITEM_DATA_KEY             "TaskItemDataKey"
#define   LOCAL_STATUS                   "LocalStatus"
#define   SYNC_STATUS                    "SyncStatus"
#define   EXAM_OBJECT_COUNT              "ExamObjectCount"
#define   EXAM_ITEM_COUNT                "ExamItemCount"
#define   USER_ID                        "UserID"
#define   LAST_TIME                      "LastTime"
#define   PLAN_ID                        "PlanID"
#define   TASK_ID                        "TaskID"
#define   ORDER_CODE                     "OrderCode"
#define   ALLOCATION_ID                  "AllocationID"
#define   EQUIPMENT_ID                   "EquipmentID"
#define   EQUIPMENT_MODIFIED_DATE        "EquipmentModifiedDate"
#define   MODIFIED_DATE                  "ModifiedDate"
#define   RECTIFY_STATUS                 "RectifyStatus"
#define   ORDER_ID                       "OrderID"
#define   TASK_TYPE                      "TaskType"
#define   LAST_DEX_TIME                  "LastDEXTime"
#define   USER_CODE                      "UserCode"
#define   USER_RFID                      "RFIDCode"
#define   POST_CODE                      "PostCode"
#define   SERVER_DATE                    "ServerDate"
#define   RATE                           "Rate"
#define   PLAN_ID_CREATE_TIME            "CreatedTime"
#define   MODULE_ID                      "ModuleID"
#define   MODULE_NAME                    "ModuleName"
#define   ITEM_COUNT                     "ItemCount"
#define   SHORT_NAME                     "ShortName"
#define   ISPRETASK                      "IsPreTask"
#define   DISTRIBUTE_ID                  "DistributeID"

/**
* 任务状态
*/
#define  TASK_STATUS_NO_FINISH        "0"
#define  TASK_STATUS_CACHE            "1"
#define  TASK_STATUS_FINISH           "2"

#define  TASK_STATUS_SYNC_ING         "0"
#define  TASK_STATUS_SYNC_FINISH      "1"
#define  TASK_STATUS_SYNC_FAIL        "2"

#define  DEVICE_NOT_SYNC   0
#define  DEVICE_SYNCED     1

#endif // DBDEFINE_H
