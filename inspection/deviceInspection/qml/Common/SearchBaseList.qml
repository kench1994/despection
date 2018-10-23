import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../DeviceOperationTask"

BaseList{
    id:searchBaseList

    //设备列表代理
    Component{
        id:deviceListDelegate
        DeviceListDelegate{}
    }
    Component{
        id:faultRepairDeviceDelegate
        OAndMDeviceListDelegate{}
    }

    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            console.log("SearchBaseList.qml  refresh ===", refresh);
        }else if(status == CPageStatus.Show){
            console.log("SearchBaseList.qml  status == CPageStatus.Show")
        }else if(status == CPageStatus.WillHide){
            console.log("SearchBaseList.qml  status == CPageStatus.WillHide")
        }else if(status == CPageStatus.Hide){
            console.log("SearchBaseList.qml  status == CPageStatus.Hide")
        }
    }

    function createVirtualDevice(){
        console.log("createVirtual device  para EquipmentCode is ", searchBaseList.originalData["para"]["EquipmentCode"])
        if(searchBaseList.originalData["para"] != undefined
                && searchBaseList.originalData["para"]["OfflineModel"] == "1"
                && searchBaseList.originalData["para"]["EquipmentCode"] != ""){
            var equipmentCode = searchBaseList.originalData["para"]["EquipmentCode"];
            var values = searchBaseList.originalData["tables"][0]["value"];
            var i = 0;
            for(; i < values.length; i++){
                if(values[i]["EquipmentCode"] == equipmentCode)
                    break;
            }
            console.log("values.length   ", values.length)
            if(i == values.length){
                var virtualValue = {};
                virtualValue["ID"] = "";
                virtualValue["EquipmentCode"] = equipmentCode;
                virtualValue["EquipmentName"] = "离线虚拟设备";
                virtualValue["ModelName"] = "";
                virtualValue["ManufacturerName"] = "";
                values.splice(0, 0, virtualValue);
                //values.push(virtualValue);
            }
            console.log("searchBaseList.originalData[tables][0][value].length   ", searchBaseList.originalData["tables"][0]["value"].length)
        }
    }

    Component.onCompleted: {
        //页面第一次加载，不刷新
        searchBaseList.refresh = false;
        var action = searchBaseList.originalData["head"]["objects"];
        console.log("SearchBaseList  completed  action is  ", action, " pageActionType is ",
                    searchBaseList.otherParam["pageActionType"])
        var roles;
        searchBaseList.titleText = searchBaseList.otherParam["titleText"];
        searchBaseList.emptyText = os.i18n.ctr(qsTr("No Device"));
        if(searchBaseList.otherParam["pageActionType"] == "Ops query"){
            roles = ["ID", "EquipmentName", "FaultType", "FaultStatus", "ReportTime"];
            searchBaseList.delegateCom = faultRepairDeviceDelegate;
        }else if(searchBaseList.otherParam["pageActionType"] == "Failure to declare" ||
                 searchBaseList.otherParam["pageActionType"] == "device search"){
            if(searchBaseList.otherParam["pageActionType"] == "Failure to declare")
                createVirtualDevice();

            roles = ["ID", "EquipmentCode", "EquipmentName", "EquipmentSeries",
                     "EquipmentModelName", "Positions","ClassName",
                     "ModelName", "ManufacturerName", "OtherCode", "Recipient"];
            searchBaseList.delegateCom = deviceListDelegate;
        }

        inspectionTaskModel.setRolesName(roles);
        inspectionTaskModel.setListData(searchBaseList.originalData["tables"][0]["value"]);
        searchBaseList.model = inspectionTaskModel;
    }
}
