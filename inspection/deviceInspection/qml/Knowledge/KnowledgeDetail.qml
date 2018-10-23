/*!
* \file KnowledgeDetail.qml
* \brief 知识库详情
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/8
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import QtWebKit 3.0
import QtWebKit.experimental 1.0
import "../Common"
 CommonPage{
        id: knowledgeDetailPage
        property var historyUrlArray: []
        property var originalData
        property string htmlStr: ""
        statusBarHoldEnabled: true
        statusBarHoldItemColor: "white"
        orientationLock: CPageOrientation.LockPortrait
        contentAreaItem:  Item {
            id: content
            Rectangle{
                id:pageHead
                width: parent.width
                height: title.height
                CommonTitlebar{
                    id: title
                    width: parent.width
                    leftItemEnabled: true
                    titleItemEnabled: true
                    rightItemEnabled: false
                    leftItemText:""
                    titleText:originalData["tables"][0]["value"][0]["Name"]
                    onLeftItemTriggered: pageStack.pop()
                    anchors.top: parent.top
                    onLeftItemChanged:leftItem.backgroundEnabled = false
                    leftItemBackgroundEnabled: false
                }
            }

            WebView {
                id: webview
                visible: false
                anchors {
                    top: pageHead.bottom
                    bottom: navigatRect.top
                    left: parent.left
                    right: parent.right
                }
                 property string firstUrl: "http://www.sina.com.cn/"
                clip: true
                experimental.userAgent:"Mozilla/5.0 (Linux; Android 4.0.3; HTC One X Build/IML74K) AppleWebKit/535.19 (KHTML, like Gecko) Chrome/18.0.1025.133 Mobile Safari/535.19"
                experimental.minimumScale: false
                experimental.preferredMinimumContentsWidth: webview.width
                experimental.preferences.minimumFontSize:18

                Component.onCompleted: {
                    knowledgeDetailPage.htmlStr = originalData["tables"][0]["value"][0]["DocumentContent"];
//                    htmlStr = String(htmlStr).replace(/&amp;/ig, "&");
//                    htmlStr = String(htmlStr).replace(/&quot;/ig, "\"");
//                    htmlStr = String(htmlStr).replace(/&apos;/ig, "'");
//                    htmlStr = String(htmlStr).replace(/&lt;/ig, "<");
//                    htmlStr = String(htmlStr).replace(/&gt;/ig, ">");
//                    console.log("1111111111111htmlStr is ", htmlStr);
                    loadHtml(knowledgeDetailPage.htmlStr);
                    visible = true
                }

                onLoadProgressChanged: {
                    console.log("---------onLoadProgressChanged-----------loadProgress:",loadProgress)
                }

                onLoadingChanged: {
                    console.log("---------onLoadingChanged-----------loading:",loading)
                }
                onCreateNewPage:{
                    console.log("this is test for open url!!!!!!!!!!!!!!!!!!!!!!!!!!!!",url)
                    knowledgeDetailPage.historyUrlArray.push(url);
                    webview.url = url;
                }
                onNavigationRequested:{
                    console.log("---------onNavigationRequested-----------navigationType:",request.navigationType, "   url is ", request.url.toString());
                }
            }
            Row{
                id:navigatRect
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 30
                anchors.rightMargin: 30
                height: 80
                anchors.bottom: parent.bottom
                spacing: 20
                Rectangle{
                    width: 200
                    height: parent.height - 20
                    anchors.verticalCenter: parent.verticalCenter
                    border.width: 2
                    border.color: "#D9D9D9"
                    radius: 10
                    Text {
                        text: "后退"
                        anchors.centerIn: parent
                    }
                    color: backMouse.pressed ? "#D9D9D9" :webview.canGoBack || knowledgeDetailPage.historyUrlArray.length ? "white" : "#D9D9D9"
                    MouseArea{
                        id:backMouse
                        anchors.fill: parent
                        onClicked: {
                            console.log("webview.goBack  canGoBack ", webview.canGoBack , "  historyarray length is ", knowledgeDetailPage.historyUrlArray.length)
                            if(webview.canGoBack){
                                webview.goBack();
                                return;
                            }

                            if(knowledgeDetailPage.historyUrlArray.length > 1){
                                webview.url = knowledgeDetailPage.historyUrlArray[knowledgeDetailPage.historyUrlArray.length - 2];
                                knowledgeDetailPage.historyUrlArray.pop();
                            } else if(knowledgeDetailPage.historyUrlArray.length == 1){
                                webview.loadHtml(knowledgeDetailPage.htmlStr);
                                knowledgeDetailPage.historyUrlArray.pop();
                            }
                        }
                    }
                }

                Rectangle{
                    width: 200
                    height: parent.height - 20
                    anchors.verticalCenter: parent.verticalCenter
                    border.width: 2
                    border.color: "#D9D9D9"
                    radius: 10
                    color: refreshMouse.pressed ? "#D9D9D9" : "white"
                    Text {
                        text: "刷新"
                        anchors.centerIn: parent
                    }
                    MouseArea{
                        id:refreshMouse
                        anchors.fill: parent
                        onClicked: {
                            console.log("webview.refresh")
                            if(webview.canGoBack || knowledgeDetailPage.historyUrlArray.length > 0)
                                webview.reload();
                            else if(!webview.canGoBack && knowledgeDetailPage.historyUrlArray.length == 0)
                                webview.loadHtml(knowledgeDetailPage.htmlStr);
                        }
                    }
                }
            }
        }
    }
