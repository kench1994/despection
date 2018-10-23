#ifndef PUSH_DEFINE_H
#define PUSH_DEFINE_H

#endif // PUSH_DEFINE_H

#define PULL_MESSAGE_ACTION "pull_message_action"

/**
 * Handler处理标识
 */
#define HANDLE_QUIT  -1
#define HANDLE_HEARTBEAT  0
#define HANDLE_SWITCH_ADDRESS  1
#define HANDLE_MODULE_INIT  2
#define HANDLE_PUSH_DATA  3
#define HANDLE_START_SERVICE  4
#define HANDLE_SEVER_CONNECTED  5
#define HANDLE_UNREGISTER  6
#define HANDLE_INIT_PUSH_CONVERSATION  7
#define HANDLE_HEARTBEAT_RESULT  8
#define HANDLE_STOP_SERVICE 9
#define HANDLE_ERROR 10

/**
 * 错误
 */
#define ERROR_DATA_SEND 2
#define ERROR_DATA_RECV 3
#define ERROR_DATA_ENCODE 4
#define ERROR_DATA_PROTOCOL 5

/**
 * Objects 定义
 */
#define OBJECT_CREATE_PUSH  "CreatePush"
#define OBJECT_REGISTER_PUSH "RegisterPush"
#define OBJECT_SUBSCRIBE_TOPIC "SubscribeTopic"
#define OBJECT_UNREGISTER_PUSH "UnregisterPush"
#define OBJECT_UNSUBSCRIBE_ALL "UnsubscribeAll"
#define OBJECT_PUSH_MESSAGE "ReceiveMessage"

/**
 * 常量
 */
#define HEARTBEA_INTERVAL_MIN 2
#define HEARTBEA_INTERVAL_MAX 8
#define PUSH_DEX_TYPE "9"


/**
 * Intent 定义
 */
#define SET_REGISTER_STATUS "setRegisterStatus"

/**
 * Intent Action定义
 */
#define HEARTBEA_ACTION                 "com.syberos.service.PushService.heartbeat"
#define ADDRESS_ACTION                  "com.syberos.service.PushService.address"
#define REGISTER_PUSH_AUTHORITY_ACTION  "com.syberos.service.PushService.register.authority.action"
#define REGISTER_PUSH_ACTION            "com.syberos.service.PushService.register.action"
#define UNREGISTER_PUSH_ACTION          "com.syberos.service.PushService.unregister"
#define INIT_TOKEN_ACTION               "com.syberos.service.PushService.init.token"


/**
 * Intent 参数Key
 */
#define INTENT_ACTION  "IntentAction"
#define INTENT_EXTRA_AUTHORITY  "ExtraAuthority"
#define INTENT_EXTRA_ACTION  "ExtraAction"
#define INTENT_EXTRA_DATA  "ExtraData"
#define INTENT_EXTRA_ADDRESS  "ExtraAddress"
#define INTENT_EXTRA_PORT  "ExtraPort"
