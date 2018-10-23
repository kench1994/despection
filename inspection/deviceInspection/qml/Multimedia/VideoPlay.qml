import QtQuick 2.0
import	QtMultimedia	5.0
import	com.syberos.basewidgets	2.0
import org.nemomobile.thumbnailer 1.0
CPage{
    id:videoPlayPage
    width:parent.width
    height:parent.height
    color:	"lightsteelblue"
    property string videoPath : ""
    Component.onCompleted:	{
        gScreenInfo.setStatusBar(true)
        gScreenInfo.setStatusBarStyle("transwhite")
    }
    property	bool	started:	false
    contentAreaItem:	Rectangle	{
        anchors.fill:	parent
        color:	"black"
        MediaPlayer {
            id: player
            source: "file://" + videoPlayPage.videoPath
            autoPlay: true
        }
        VideoOutput {
            id: videoOutput
            source: player
            anchors.fill: parent
        }
    }
}
