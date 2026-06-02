
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 Advanced Graphing Calculator</title>
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<style>
body{font-family:Arial;margin:20px}
input,button{padding:8px;margin:4px}
canvas{max-width:100%;height:500px}
</style>
</head>
<body>
<h2>ESP32 Advanced Graphing Calculator</h2>

<input id="funcs" size="60"
value="Math.sin(x);x*x;Math.cos(x)">

<button onclick="drawGraph()">Draw</button>
<button onclick="zoomIn()">Zoom +</button>
<button onclick="zoomOut()">Zoom -</button>

<p>Separate multiple functions with ;</p>

<canvas id="chart"></canvas>

<script>
let xmin=-10,xmax=10;
let chart=null;

function makeData(expr){
    let pts=[];
    for(let x=xmin;x<=xmax;x+=0.1){
        try{
            let y=Function("x","return "+expr)(x);
            if(isFinite(y)){
                pts.push({x:x,y:y});
            }
        }catch(e){}
    }
    return pts;
}

function drawGraph(){
    let funcs=document.getElementById("funcs").value.split(";");

    let datasets=[];

    funcs.forEach((f,i)=>{
        datasets.push({
            label:f,
            data:makeData(f),
            parsing:false,
            showLine:true,
            pointRadius:0
        });
    });

    if(chart) chart.destroy();

    chart=new Chart(document.getElementById('chart'),{
        type:'scatter',
        data:{datasets:datasets},
        options:{
            responsive:true,
            scales:{
                x:{type:'linear',min:xmin,max:xmax},
                y:{type:'linear'}
            }
        }
    });
}

function zoomIn(){
    xmin/=1.5;
    xmax/=1.5;
    drawGraph();
}

function zoomOut(){
    xmin*=1.5;
    xmax*=1.5;
    drawGraph();
}

drawGraph();
</script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", webpage);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
