/*
  ESP32 Mini Chat Server
  ESP32 DevKit V1
  Max Users: 5
  Max Messages: 15
  Max Message Size: 70 words
  Temporary RAM Storage
*/

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ENTER HOTSPOT NAME";
const char* password = "ENTRE YOU PASSOWRD";

const String LOGIN_PASS = "Entre your login pass";

WebServer server(80);

struct ChatMessage {
  String user;
  String text;
};

ChatMessage messages[15];
int msgCount = 0;

String htmlPage() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 Chat</title>

<style>
body{
  background:#111;
  color:white;
  font-family:Arial;
  margin:0;
  padding:10px;
}

.box{
  background:#1e1e1e;
  padding:10px;
  border-radius:10px;
  margin-bottom:10px;
}

input{
  width:95%;
  padding:10px;
  margin:5px 0;
  border:none;
  border-radius:8px;
  background:#333;
  color:white;
}

button{
  width:100%;
  padding:10px;
  border:none;
  border-radius:8px;
  background:#00b894;
  color:white;
  font-size:16px;
}

#chat{
  height:300px;
  overflow-y:auto;
  background:#000;
  padding:10px;
  border-radius:10px;
}
</style>
</head>

<body>

<div class="box">
<h2>ESP32 Mini Chat</h2>

<input id="user" placeholder="Username">
<input id="pass" type="password" placeholder="Password">
<button onclick="login()">Login</button>
</div>

<div id="main" style="display:none;">

<div id="chat"></div>

<input id="msg" maxlength="400" placeholder="Message">
<button onclick="sendMsg()">Send</button>

</div>

<script>

let username = "";

function login(){

let u = document.getElementById("user").value;
let p = document.getElementById("pass").value;

fetch("/login?user="+u+"&pass="+p)
.then(r=>r.text())
.then(data=>{

if(data=="OK"){
  username=u;
  document.getElementById("main").style.display="block";
  loadChat();
  setInterval(loadChat,2000);
}
else{
  alert("Wrong Password");
}

});

}

function sendMsg(){

let msg=document.getElementById("msg").value;

fetch("/send?user="+username+"&msg="+encodeURIComponent(msg))
.then(()=>{
 document.getElementById("msg").value="";
 loadChat();
});

}

function loadChat(){

fetch("/messages")
.then(r=>r.text())
.then(data=>{
 document.getElementById("chat").innerHTML=data;
});

}

</script>

</body>
</html>
)rawliteral";

  return page;
}

bool wordLimit(String text) {
  int words = 0;
  bool inWord = false;

  for (int i = 0; i < text.length(); i++) {
    char c = text[i];

    if (c != ' ' && !inWord) {
      words++;
      inWord = true;
    }

    if (c == ' ') {
      inWord = false;
    }
  }

  return words <= 70;
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleLogin() {

  String pass = server.arg("pass");

  if (pass == LOGIN_PASS) {
    server.send(200, "text/plain", "OK");
  } else {
    server.send(200, "text/plain", "FAIL");
  }
}

void handleSend() {

  String user = server.arg("user");
  String msg = server.arg("msg");

  if (!wordLimit(msg)) {
    server.send(200, "text/plain", "WORD_LIMIT");
    return;
  }

  if (msg.length() < 1) {
    server.send(200, "text/plain", "EMPTY");
    return;
  }

  if (msgCount < 15) {

    messages[msgCount].user = user;
    messages[msgCount].text = msg;
    msgCount++;

  } else {

    for (int i = 0; i < 14; i++) {
      messages[i] = messages[i + 1];
    }

    messages[14].user = user;
    messages[14].text = msg;
  }

  server.send(200, "text/plain", "SENT");
}

void handleMessages() {

  String out = "";

  for (int i = 0; i < msgCount; i++) {

    out += "<div style='margin-bottom:10px;padding:8px;background:#222;border-radius:8px;'>";
    out += "<b>" + messages[i].user + ":</b><br>";
    out += messages[i].text;
    out += "</div>";
  }

  server.send(200, "text/html", out);
}

void setup() {

  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println("");
  Serial.println("WiFi Started");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/send", handleSend);
  server.on("/messages", handleMessages);

  server.begin();

  Serial.println("Server Started");
}

void loop() {
  server.handleClient();
}
