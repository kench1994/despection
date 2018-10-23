/*!
* \file ListDelegate.qml
* \brief 知识库任务列表代理
*
*知识库任务列表代理
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class ListDelegate
  \inModule 设备巡检
  \brief 桓乐乐 huanlele@syberos.com
  \ingroup InspectionTask
  知识库任务列表代理
*/
TwoLineItem {
    id : knowledgeTaskDelegate
    property string actionSequence: "-1"
    /** type:string 任务名称*/
    firstLeftText: Name
    secondLeftText: "文档类别: "+ knowledgeTaskList.documentType[Type]
    secondRightText:"文档维护人: " + CreateName
//    secondRightText: Rate
//roles = ["ID", "Code", "Name", "Type", "Class", "KeyWords", "CreateName"];
    onTriggered: {
        transmitManager.resultOfResponse.disconnect(knowledgeTaskDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(knowledgeTaskDelegate.resultResponse)
        knowledgeTaskDelegate.actionSequence = inspectionSystemConfig.getSequence();
        var head = {"Objects" : "EMK_knowledge_One"};
        var para = {"ID" : ID};
        console.log("transmitManager.requestData   EMK_knowledge_One ")
        transmitManager.requestData(head, para, knowledgeTaskDelegate.actionSequence, "获取知识库明细失败");
    }

    /**
     * 接受服务器返回数据的槽函数,加载巡检套件列表
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.debug("---ListDelegate---onResultOfResponse---result, error: action  actionSeqence ",
                      result, error, action, knowledgeTaskDelegate.actionSequence);
        if(action != knowledgeTaskDelegate.actionSequence){
            console.log("action != knowledgeTaskDelegate.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(knowledgeTaskDelegate.resultResponse)
        if(result && error === 0){
            pageStack.push("qrc:/qml/Knowledge/KnowledgeDetail.qml",{"originalData": map});
        }
    }
}
