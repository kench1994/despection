/*!
* \file EditImage.qml
* \brief 图片编辑页面
*
*此界面图片编辑页面，并将编辑好的图片保存
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import QtMultimedia 5.0
import com.syberos.basewidgets 2.0
import com.syberos.imageproc 1.0
import "../Common"
/*!
  \class EditImage
  \inModule 公共控件
  \brief 图片编辑页面
  \ingroup common
  此界面图片编辑页面，并将编辑好的图片保存
*/
CommonPage {
    id: editImagePage
    /** type:string  addPhoto 编辑新增照片; modifyPhoto，编辑已经编辑过的照片*/
    property string actionType: ""

    /** type:string 进行编辑的图片的路径 */
    property string editImagePath: ""

    property string fileTitle: ""
    property string fileRemark: ""
    property string fileDate: ""
    /** type:var  祖先控件的id*/
    property var parentPage

    statusBarHoldEnabled: true
    statusBarHoldItemColor: "black"
    contentAreaItem:Rectangle{
        id:imageEdit
        color:"black"
            Item{
                id:imgEditRect
                width: 480
                anchors.top: parent.top
                height: 640
                ImageProc{
                    id:imgproc
                    visible: editImagePage.actionType == "addPhoto"
                    anchors.fill: parent
                    editColor:"white"
                    focus: true
                }
                Image{
                    id:previewImg
                    anchors.fill: parent
                    visible: editImagePage.actionType == "modifyPhoto"
                    source: "file://" + editImagePage.editImagePath
                }
            }

            Rectangle{
                width: parent.width
                height: 100
                anchors.top:imgEditRect.bottom
                visible:editImagePage.actionType == "addPhoto"
                color:"#4f4f4f"
                GridView{
                    id:editColorView
                    property int selectedIndex: 0
                    width: parent.width
                    height: 72
                    anchors.verticalCenter: parent.verticalCenter
                    cellHeight:72
                    cellWidth: Math.floor(width/7)
                    model: editColorModel
                    delegate:Item{
                        width: editColorView.cellWidth
                        height: editColorView.cellHeight
                        Rectangle{
                            width: editColorView.selectedIndex == index ? 60 : 46
                            height: editColorView.selectedIndex == index ? 60 : 46
                            radius: editColorView.selectedIndex == index ? 30 : 23
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            color:editColor
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                editColorView.selectedIndex = index;
                                imgproc.editColor = editColor
                            }
                        }
                    }
                }
                ListModel{
                    id:editColorModel
                    ListElement{
                        editColor:"#fdffff"
                    }
                    ListElement{
                        editColor:"#09f953"
                    }
                    ListElement{
                        editColor:"#cd191f"
                    }
                    ListElement{
                        editColor:"#0190f4"
                    }
                    ListElement{
                        editColor:"#f8f00a"
                    }
                    ListElement{
                        editColor:"#ce0bf8"
                    }
                    ListElement{
                        editColor:"#000000"
                    }
                }
            }
            Rectangle{
                id:imgEditAction
                anchors.bottom: parent.bottom
                width: parent.width
                height: 60
                color: "black"
                visible:editImagePage.actionType == "addPhoto"
                Item{
                    width:lastStepText.width * 1.5
                    height: lastStepText.height * 1.5
                    anchors.left: parent.left
                    anchors.leftMargin: 40
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        id:lastStepText
                        text: "取消"
                        font.pixelSize: 24
                        color:"#ffffff"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            ensureSubmitDlg.type = "lastStep"
                            ensureSubmitDlg.show();
                        }
                    }
                }

                Item{
                    width:resetText.width
                    height: resetText.height * 1.5
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        id:resetText
                        text: "清除标记"
                        font.pixelSize: 24
                        color:"#ffffff"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            imgproc.imagePath = editImagePage.editImagePath
                        }
                    }
                }

                Item{
                    width:saveText.width
                    height: saveText.height * 1.5
                    anchors.right: parent.right
                    anchors.rightMargin: 40
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        id:saveText
                        text: "使用照片"
                        font.pixelSize: 24
                        color:"#ffffff"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            var path = imgproc.saveEditImage("");
                            console.log("save path is  ", path)
                            pageStack.push("qrc:/qml/Multimedia/MultimediaAddNotePage.qml", {
                                               "filePath":path,
                                               "multimediaType": "image",
                                               "parentPage":editImagePage.parentPage});
                        }
                    }
                }
            }

            Item{
                id:previewTitleAndRemark
                anchors.top: imgEditRect.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                visible: editImagePage.actionType == "modifyPhoto"
                Column{
                    width: parent.width
                    spacing: 5
                    Text{
                        id:titleText
                        font.pixelSize: 22
                        color: "white"
                        wrapMode: Text.WrapAnywhere
                        text:editImagePage.fileTitle
                    }
                    Text{
                        id:dateText
                        font.pixelSize: 18
                        color:"#e0e0e0"
                        text:editImagePage.fileDate
                    }
                    Text{
                        id:remarkText
                        font.pixelSize: 18
                        color:"#e0e0e0"
                        width: parent.width
                        wrapMode: Text.WrapAnywhere
                        text:editImagePage.fileRemark
                    }
                }
            }
    }

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            if(editImagePage.actionType == "modifyPhoto"){
                pageStack.pop();
            } else if(editImagePage.actionType == "addPhoto"){
                console.log("fault report   onKeyBackTriggered")
                ensureSubmitDlg.type = "lastStep"
                ensureSubmitDlg.show();
            }
        }
    }

    CDialog{
        id:ensureSubmitDlg
        property string type: ""
        titleText : "提示"
        messageText:"你尚未保存,确定退出吗?"
        acceptedButtonText: "确定"
        rejectButtonText: "取消"
        onAccepted:{
            if(type == "lastStep"){
                if(editImagePage.actionType == "addPhoto"){
                    var imgList = [];
                    imgList.push(editImagePage.editImagePath);
                    editImagePage.parentPage.removeImage(imgList);
                    pageStack.pop();
                }
            }
        }
    }

    onStatusChanged: {
        if (status == CPageStatus.Show && editImagePage.actionType == "addPhoto") {
            rootWindow.dialogShow = true;
        } else if (status == CPageStatus.WillHide){
            rootWindow.dialogShow = false;
        }
    }

    Component.onCompleted: {
        console.log("editImagePage.editImagePath  is ", editImagePage.editImagePath);
        imgproc.imagePath = editImagePage.editImagePath;
        editImagePage.statusBarHoldEnabled = true;
        gScreenInfo.setStatusBarStyle("black");
        gScreenInfo.setStatusBar(true)
    }
}
