import QtQuick 2.0
Item {
    id:root
    width:parent.width
    height: 480
    visible: true
    property double volume: 0
    property bool stopdraw: true
    function setVolume(value){
        if(root.stopdraw == false)
            root.volume = value;
    }

    function startDrawLine(){
        root.stopdraw = false;
        tm.start();
    }

    function stopDrawLine(){
        root.stopdraw = true;
        tm.stop();
        root.volume = 0;
        console.log("unction stopDrawLine voluem is  ", root.volume)
        canvas.requestPaint();
    }

    Canvas {
        id: canvas
        anchors.fill: parent
        property double  offsetX: 0
        property double scaleX : 0.02618;
        property double scaleY1 : 1;
        property double scaleY2 : 0.6;
        property double scaleY3 : 0.4;
        property int baseY : 240;
        onPaint: {
            var ctx = getContext('2d')
            ctx.clearRect(0, 0, canvas.width, canvas.height) ;
            drawSinLine(ctx, canvas.scaleX, canvas.scaleY2 * root.volume, canvas.baseY, canvas.offsetX, 1.0, "#fffafa");
            drawSinLine(ctx, canvas.scaleX, canvas.scaleY3 * root.volume, canvas.baseY, canvas.offsetX, 1.0, "#fffacd");
            drawSinLine(ctx, canvas.scaleX, canvas.scaleY1 * root.volume, canvas.baseY, canvas.offsetX, 2.0, "#ccff99");
        }
        function drawSinLine(ctx, scaleX, scaleY, baseY, offset, lineWidth, lineColor){
            ctx.lineWidth = lineWidth;
            ctx.strokeStyle = lineColor;
            ctx.beginPath()
            for(var i = 0; i < 120; i++){
                var x1 = i * 4;
                var x2 = (i + 1) * 4;
                var y1 = Math.sin(x1 * scaleX - offset) * scaleY + baseY;
                var y2 = Math.sin(x2 * scaleX - offset) * scaleY + baseY;
                ctx.moveTo(x1 , y1);
                ctx.lineTo(x2 , y2);
            }
            ctx.stroke();
        }
    }
    Timer{
        id:tm
        interval: 100
        running: false
        repeat: true
        onTriggered: {
            if(canvas.offsetX >= 12.2664)
                canvas.offsetX = 0;
            else
                canvas.offsetX += 0.2618;
            canvas.requestPaint();
        }
    }
}
