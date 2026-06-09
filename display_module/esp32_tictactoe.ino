#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Tic Tac Toe ESP32</title>
  <style>
    body { font-family: Arial; text-align:center; background:#111; color:white; }
    h1 { color:#00ffcc; }
    #board { display:grid; grid-template-columns:repeat(3,100px); gap:5px; justify-content:center; margin-top:20px; }
    .cell {
      width:100px; height:100px; font-size:40px;
      background:#222; color:white; display:flex;
      align-items:center; justify-content:center;
      cursor:pointer; border:1px solid #444;
    }
    .cell:hover { background:#333; }
    button { margin-top:20px; padding:10px 20px; }
  </style>
</head>
<body>

<h1>Tic Tac Toe (ESP32 Web Server)</h1>
<div id="board"></div>
<button onclick="resetGame()">Reset</button>

<script>
let board = ["","","","","","","","",""];
let currentPlayer = "X";
let gameActive = true;

function renderBoard(){
  const boardDiv = document.getElementById("board");
  boardDiv.innerHTML = "";

  board.forEach((val, i) => {
    const cell = document.createElement("div");
    cell.className = "cell";
    cell.innerText = val;
    cell.onclick = () => move(i);
    boardDiv.appendChild(cell);
  });
}

function move(index){
  if(board[index] !== "" || !gameActive) return;

  board[index] = currentPlayer;
  checkWinner();
  currentPlayer = currentPlayer === "X" ? "O" : "X";
  renderBoard();
}

function checkWinner(){
  const winPatterns = [
    [0,1,2],[3,4,5],[6,7,8],
    [0,3,6],[1,4,7],[2,5,8],
    [0,4,8],[2,4,6]
  ];

  for(let p of winPatterns){
    let [a,b,c] = p;
    if(board[a] && board[a] === board[b] && board[a] === board[c]){
      alert(board[a] + " Wins!");
      gameActive = false;
    }
  }

  if(!board.includes("") && gameActive){
    alert("Draw!");
    gameActive = false;
  }
}

function resetGame(){
  board = ["","","","","","","","",""];
  currentPlayer = "X";
  gameActive = true;
  renderBoard();
}

renderBoard();
</script>

</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
