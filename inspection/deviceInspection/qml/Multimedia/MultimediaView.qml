/*!
* \file MultimediaView.qml
* \brief 巡检多媒体模块
*
*巡检多媒体模块，并且以列表形式显示多媒体文件，供提交使用
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import org.nemomobile.thumbnailer 1.0
import "../Common"
/*!
  \class MultimediaView
  \inModule 公共控件
  \brief 巡检多媒体模块
  \ingroup common
  巡检多媒体模块
*/

Column{
    id:insphotoList
    /** type:alias 保存编辑图片的listmodel */
    property alias phmodel: photoModel
    /** associatedPhoto   关联图片   addPhoto    添加照片*/
    property string actionType: "addPhoto"
    /*多媒体模块互动类型 addMultimedia:可以添加多媒体文件;　showMultimedia:只能查看多媒体文件*/
    property string interactionType: "addMultimedia"
    property var parentPageId
    property bool clearCache: true
    property int audioRecorderTimer: 30
    property int videoRecorderTimer: 30
    property int attachmentsCount: 5
    property var attachments: []
    width: parent.width
    spacing: 2
    ListItem{
        id:addAttachItem
        mainTitle:  interactionType == "addMultimedia" ?
                        "已经添加" + photoModel.count  + "个附件(最多"  + attachmentsCount + "个)" : "附件数量" + photoModel.count + "个"
        type: interactionType == "addMultimedia" ? "category" : ""
//        subTitle: photoView.visible ?  "点击收回附件列表" : "点击展开附件列表"
//        endImage: interactionType == "addMultimedia"? "qrc:/icon/multimedia_add_attach.png" : ""
        endImage: "qrc:/icon/multimedia_add_attach.png"
        endImagePressed: "qrc:/icon/multimedia_add_attach.png"
        onClicked: {
            if(interactionType == "addMultimedia"){
                if(photoModel.count >= attachmentsCount){
                    CTOAST.requestToast("附件数量已到上限","","");
                    return;
                }
                multimediaMenu.show();
            }
        }
    }
    ListView{
        id:photoView
        width: parent.width
        clip: true
        model:photoModel
        height: 100 * photoModel.count
        interactive: false
        boundsBehavior:Flickable.StopAtBounds
        delegate:MultimediaListDelegate{
            id:listDelegate
            multimediaDescription: fileRemark
            multimediaTitle: fileTitle
            multimediaFilePath: filePath
            multimediaIcon: fileIcon
            multimediaType: fileType
            multimediaDate: rootWindow.getCurDate();
            dataOk : dataIsOk
            error : errorText
            canDelete: insphotoList.interactionType == "addMultimedia"
            //点击进入预览界面
            onClicked: {
                if(multimediaType == "image"){
                        pageStack.push("qrc:/qml/Multimedia/EditImage.qml", {
                                           "editImagePath": multimediaFilePath, "actionType": "modifyPhoto", "fileTitle": multimediaTitle,
                                           "fileRemark" : multimediaDescription, "fileDate" : multimediaDate, "parentPage":insphotoList});
                } else if(multimediaType == "audio") {
                    pageStack.push("qrc:/qml/Multimedia/AudioPlayPage.qml", {
                                       "filePath": multimediaFilePath,  "actionType": "playAudio", "fileTitle": multimediaTitle, "fileRemark" : multimediaDescription,
                                       "fileDate" : multimediaDate, "parentPage":insphotoList});
                } else if(multimediaType == "video"){
                    //gApp.openUrl("video://play?fileuri=file://" + multimediaFilePath);
                    pageStack.push("qrc:/qml/Multimedia/VideoPlayPage.qml", {
                                       "filePath": multimediaFilePath, "actionType": "playVideo", "fileTitle": multimediaTitle,
                                       "fileRemark" : multimediaDescription, "fileDate" : multimediaDate, "parentPage":insphotoList});
                }
            }
            onEndImageClicked: {
                var imgList = [];
                imgList.push(photoModel.get(index).filePath);
                insphotoList.removeImage(imgList);
                photoModel.remove(index, 1);
            }
        }
    }
    MultimediaPopMenu{
        id:multimediaMenu
        showType : insphotoList.actionType
        onClicked: {
            switch(type){
            case 1://camera
                console.log("qrc:/qml/Multimedia/StartCamera.qml");
                pageStack.push("qrc:/qml/Multimedia/StartCamera.qml", { "parentPage":insphotoList, "actionType":"addPhoto"});
                break;
            case 2://vedio
                pageStack.push("qrc:/qml/Multimedia/VideoRecorder.qml", {"parentPage": insphotoList, "recordTotalTime": videoRecorderTimer});
                break;
            case 3://audio
                pageStack.push("qrc:/qml/Multimedia/AudioRecorderPage.qml", {"parentPage": insphotoList, "recordTotalTime" : audioRecorderTimer});
                break;
            }
        }
    }
    function saveMultimediaFile(filePath, fileType, fileTitle, fileRemark) {
        console.log("saveMultimediaFile  ", filePath, "  fileType  ", fileType, "title ", fileTitle,  "  fileRemark ", fileRemark)
        var icon;
        var descript = "";
        if(fileType == "image"){
            icon = filePath == "" ? "" : "file://" + filePath;
            descript = "this is a image";
        } else if(fileType == "audio") {
            icon = "qrc:/icon/multimedia_audio.png";
            descript = "this is a audio";
        } else if(fileType == "video") {
            icon = filePath == "" ? "" : "image://nemoThumbnail/" + "file://" + filePath;
            descript = "this is a video";
        }
        if(fileRemark == "")
            fileRemark = descript;
        var i = 0;
        for(; i < photoModel.count; i++){
            if(filePath == photoModel.get(i).filePath){
                photoModel.set(i, {"filePath" : filePath, "fileIcon" : icon,  "fileType" : fileType, "fileTitle" : fileTitle,
                                   "fileRemark" : fileRemark, "dataIsOk": true,"fileUrl": "", "errorText" : ""})
                break;
            }
        }
        if(i >= photoModel.count){
            photoModel.append({"filePath" : filePath, "fileIcon" : icon, "fileType" : fileType, "fileTitle" : fileTitle,
                                  "fileRemark" : fileRemark, "dataIsOk": true, "fileUrl": "", "errorText" : ""});

        }
        photoView.model = tmpModel;
        photoView.model = photoModel;
        photoView.update();
    }


    function removeImage(imagePathList) {
        multimediaUtils.removeMultimediaFile(imagePathList, false);
    }

    function getMultimediaData(){
        var attchments = [];
        for(var i = 0; i < phmodel.count; i++){
            var attchment = {};
            var filePath = phmodel.get(i)["filePath"];
            var pos = String(filePath).lastIndexOf("/");
            attchment["Name"] = String(filePath).substring(pos + 1);
            attchment["Title"] = phmodel.get(i)["fileTitle"];
            attchment["Description"] = phmodel.get(i)["fileRemark"];
            attchment["Path"] = phmodel.get(i)["filePath"];
            attchment["MultimediaType"] = phmodel.get(i)["fileType"];
            console.log("MultimeidaView  nameis ", attchment["Name"], " title ", attchment["Title"], " description ", attchment["Description"],
                        "path ", attchment["Path"], "  type ", attchment["MultimediaType"] );
            attchments.push(attchment);
        }
        return attchments;
    }
    ListModel{
        id:tmpModel
    }
    ListModel{
        id:photoModel
    }

    Connections{
        target: httpRequest
        //void responseDownLoadFile(const QString& downLoadUrl, const QString &filePath, const bool result, const QString &errorText);
        onResponseDownLoadFile:{
            for(var i = 0; i < phmodel.count; i++){
                var fileUrl = phmodel.get(i)["fileUrl"];
                if(fileUrl == downLoadUrl){
                    if(result){
                        var fileType = phmodel.get(i)["fileType"];
                        var fileIcon = getIcon(filePath, fileType);
                        console.log("fileType ", fileType, "  filePath is ", filePath, " fileIcon " , fileIcon);
                        photoModel.set(i, { "dataIsOk" : true, "filePath" : filePath, "fileIcon" : fileIcon})
                    } else
                        photoModel.set(i, { "dataIsOk" : false, "filePath" : filePath, "errorText" : "下载失败"})
                    break;
                }
            }
            photoView.update();
        }
    }

    Connections {
        target: rootWindow
        onOpenCamera:{
            console.log("====MultimediaView=====onKeyReleasedChanged==000===")
            if(insphotoList.visible == true && parentPageId.status == CPageStatus.Show && interactionType == "addMultimedia"){
                multimediaMenu.hide();
                if(photoModel.count < attachmentsCount){
                    pageStack.push("qrc:/qml/Multimedia/StartCamera.qml", { "parentPage":insphotoList, "actionType":"addPhoto"});
                }else{
                    CTOAST.requestToast("附件数量已到上限","","");
                }
            }
        }
    }

    function getIcon(filePath, fileType){
        var fileIcon;
        if(filePath == "")
            fileIcon = "";
        else {
            if(fileType == "image"){
                fileIcon = "file://" + filePath;
            } else if(fileType == "audio") {
                fileIcon = "qrc:/icon/multimedia_audio.png";
            } else if(fileType == "video") {
                fileIcon = "image://nemoThumbnail/" + "file://" + filePath;
            }
        }
        return fileIcon;
    }

    function setMultimediaData(attachments){
        var attachCount = attachments.length;
        var downLoadFilesUrl = [];
        for(var i = 0; i < attachCount; i++){
            var tmpAttach = attachments[i];
            var fileType = tmpAttach["MultimediaType"];
            var filePath = tmpAttach["Path"];
            var fileTitle = tmpAttach["Title"];
            var fileRemark = tmpAttach["Description"];
            var dataIsOk = tmpAttach["DataIsOk"];
            if(dataIsOk == undefined){
                dataIsOk = true;
                console.log("dataIsOk == undefined  set dataIsOk is true");
            }
            var fileUrl = "";
            if(!dataIsOk){
                downLoadFilesUrl.push(tmpAttach["Url"]);
                fileUrl = tmpAttach["Url"];
            }
            var fileIcon = getIcon(filePath, fileType);
            console.log("setMultimediaData  filePath ", filePath, "  fileIcon ", fileIcon, " fileType", fileType, "  fileTitle ", fileTitle, " dataIsOk ", dataIsOk, "  fileUrl ", fileUrl)
            photoModel.append({"filePath" : filePath, "fileIcon" : fileIcon, "fileType" : fileType, "fileTitle" : fileTitle,
                                  "fileRemark" : fileRemark, "dataIsOk": dataIsOk, "fileUrl": fileUrl, "errorText" : ""});
        }
        if(downLoadFilesUrl.length > 0)
            httpRequest.downloadFile(downLoadFilesUrl)
    }

    Component.onCompleted: {
    }
    Component.onDestruction: {
        if(clearCache){
            var imgList = [];
            for(var i = 0; i < photoModel.count; i++){
                console.log("88888888888888888888888888888 ", photoModel.get(i)["filePath"])
                imgList.push(photoModel.get(i)["filePath"]);
            }
            if(imgList.length > 0)
                multimediaUtils.removeMultimediaFile(imgList);
        }
    }
}







