#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);
WebSocketsServer webSocket(81);

// Game state
char board[9];
char playerTurn = 'X';
int playerCount = 0;

// HTML GAME
const char page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 Tic Tac Toe</title>
<style>
body { text-align:center; font-family:Arial; background:#111; color:white; }
.grid { display:grid; grid-template-columns:repeat(3,100px); justify-content:center; margin-top:20px; }
.cell {
  width:100px; height:100px; font-size:40px;
  display:flex; align-items:center; justify-content:center;
  border:2px solid white; cursor:pointer;
}
</style>
</head>

<body>
<h2>ESP32 Multiplayer Tic Tac Toe</h2>
<div id="status">Connecting...</div>
<div class="grid" id="grid"></div>

<script>
let ws = new WebSocket("ws://" + location.hostname + ":81/");
let board = Array(9).fill("");

ws.onmessage = (event) => {
  let data = JSON.parse(event.data);

  if(data.type === "board") {
    board = data.board;
    draw();
  }

  if(data.type === "status") {
    document.getElementById("status").innerText = data.msg;
  }
};

function draw() {
  let g = document.getElementById("grid");
  g.innerHTML = "";

  for(let i=0;i<9;i++){
    let div = document.createElement("div");
    div.className = "cell";
    div.innerText = board[i];

    div.onclick = () => {
      ws.send(JSON.stringify({move:i}));
    };

    g.appendChild(div);
  }
}
draw();
</script>

</body>
</html>
)rawliteral";

void sendBoard() {
  String msg = "{\"type\":\"board\",\"board\":[";
  for(int i=0;i<9;i++){
    msg += "\"";
    msg += board[i];
    msg += "\"";
    if(i<8) msg += ",";
  }
  msg += "]}";
  webSocket.broadcastTXT(msg);
}

void sendStatus(String s) {
  webSocket.broadcastTXT("{\"type\":\"status\",\"msg\":\"" + s + "\"}");
}

bool checkWin(char p) {
  int w[8][3] = {
    {0,1,2},{3,4,5},{6,7,8},
    {0,3,6},{1,4,7},{2,5,8},
    {0,4,8},{2,4,6}
  };

  for(int i=0;i<8;i++){
    if(board[w[i][0]]==p && board[w[i][1]]==p && board[w[i][2]]==p)
      return true;
  }
  return false;
}

bool fullBoard() {
  for(int i=0;i<9;i++) if(board[i]==' ') return false;
  return true;
}

void resetGame() {
  for(int i=0;i<9;i++) board[i]=' ';
  playerTurn = 'X';
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  if(type == WStype_CONNECTED){
    playerCount++;
    sendStatus("Player joined. Total: " + String(playerCount));
    sendBoard();
  }

  if(type == WStype_TEXT){
    String msg = String((char*)payload);

    int move = msg.substring(msg.indexOf(":")+1, msg.indexOf("}")).toInt();

    if(board[move] == ' '){
      board[move] = playerTurn;

      if(checkWin(playerTurn)){
        sendBoard();
        sendStatus(String(playerTurn) + " Wins!");
        resetGame();
      }
      else if(fullBoard()){
        sendBoard();
        sendStatus("Draw!");
        resetGame();
      }
      else {
        playerTurn = (playerTurn=='X') ? 'O' : 'X';
        sendStatus(String("Turn: ") + playerTurn);
      }

      sendBoard();
    }
  }
}

void handleRoot() {
  server.send_P(200, "text/html", page);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }

  Serial.println(WiFi.localIP());

  for(int i=0;i<9;i++) board[i]=' ';

  server.on("/", handleRoot);
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  server.handleClient();
  webSocket.loop();
}
