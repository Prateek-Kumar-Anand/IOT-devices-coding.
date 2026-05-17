#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_Jokes_Server";
const char* password = "12345678";

WebServer server(80);

String jokes[] = {
  "WiFi strong hai, par motivation weak hai.",
  "Coding karte karte chai bhi compile ho gayi.",
  "Internet slow nahi hai, patience fast khatam ho raha hai.",
  "Mere bugs bhi mujhe ignore karte hain.",
  "Laptop garam hai, future uncertain hai.",
  "Homework pending, par GitHub green hona chahiye.",
  "RAM kam hai, dreams unlimited.",
  "ESP32 bhi soch raha hoga, ab kya upload karega?",
  "Error 404: Motivation not found.",
  "Phone 1%, confidence 100%.",
  "Mai coding nahi karta, bugs collect karta hu.",
  "Light gayi, project save nahi hua.",
  "WiFi password sabko chahiye, notes kisi ko nahi.",
  "Exam se pehle sab engineer ban jaate hain.",
  "Google bina coder aadha adhura hota hai.",
  "Sleep mode sirf devices ke liye nahi hota.",
  "ESP32 ka hotspot bhi mujhse zyada stable hai.",
  "Coding me semicolon bhi attitude dikhata hai.",
  "Ctrl + Z real life me bhi hona chahiye.",
  "Jab code chale bina error ke, tabhi darr lagta hai.",
  "Coding aur maggi dono 2 minute wale jhoot hain.",
  "HTML easy hai, CSS attitude deta hai.",
  "Project complete hone ke baad hi idea aata hai.",
  "99 bugs in the code, fix one, 199 more.",
  "Browser refresh hi asli debugging hai.",
  "Password itna strong ki owner hi bhool gaya.",
  "Coding ka matlab: stack overflow search.",
  "Ek bug fix karo, teen naye unlock ho jaate hain.",
  "Life bhi beta version lagti hai.",
  "Mobile storage full, memes successful.",
  "Jab code run kare bina issue ke, screenshot le lo.",
  "Mouse ka wire bhi life se zyada uljha hai.",
  "Update available: laziness patch installed.",
  "Coding night me hoti hai, regret morning me.",
  "WiFi disconnect hua aur sabka asli chehra dikh gaya.",
  "Browser tabs itni ki browser bhi confuse.",
  "ESP32 ka hotspot: free internet nahi milega.",
  "Calculator banaya, ab NASA ready.",
  "Project ka UI acha ho to bugs maaf.",
  "Mobile hang ho gaya, emotions bhi.",
  "Coding ka first rule: save often.",
  "Debugging matlab detective banna.",
  "Kabhi kabhi code chal jata hai sympathy me.",
  "Programmer ka gym: keyboard typing.",
  "GitHub pe upload karke professional feel aati hai.",
  "Browser cache aur dimag dono clear karna padta hai.",
  "Coding karte waqt time travel ho jata hai.",
  "Mera code works on my machine only.",
  "Light mode users suspicious lagte hain.",
  "CSS center karna NASA mission jaisa hai."
};

int totalJokes = sizeof(jokes) / sizeof(jokes[0]);

void handleRoot() {

  int randomIndex = random(totalJokes);

  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>ESP32 Joke Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <style>
      body{
        background:#121212;
        color:white;
        font-family:Arial;
        text-align:center;
        padding-top:60px;
      }

      .box{
        width:80%;
        margin:auto;
        background:#1e1e1e;
        padding:30px;
        border-radius:20px;
        box-shadow:0 0 15px cyan;
      }

      h1{
        color:cyan;
      }

      p{
        font-size:22px;
        margin-top:30px;
      }

      button{
        margin-top:30px;
        padding:15px 25px;
        border:none;
        border-radius:10px;
        background:cyan;
        color:black;
        font-size:18px;
        cursor:pointer;
      }

      button:hover{
        background:white;
      }
    </style>

  </head>

  <body>

    <div class="box">
      <h1>😂 ESP32 Joke Server 😂</h1>

      <p>)rawliteral";

  html += jokes[randomIndex];

  html += R"rawliteral(</p>

      <button onclick="location.reload()">
        New Joke
      </button>
    </div>

  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void setup() {

  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println("");
  Serial.println("Hotspot Started");
  Serial.println(WiFi.softAPIP());

  randomSeed(millis());

  server.on("/", handleRoot);

  server.begin();

  Serial.println("Web Server Started");
}

void loop() {
  server.handleClient();
}
