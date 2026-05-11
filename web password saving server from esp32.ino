
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>

const char* ssid = "ESP32 WEB SERVER";
const char* hotspotPassword = "ENTER YOUR PASSWORD";

String adminPassword = "ENTRE ADMIN PASS";

AsyncWebServer server(80);
Preferences prefs;

String htmlLogin = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Vault Login</title>
<style>
body{
font-family:Arial;
background:#111;
color:white;
text-align:center;
padding-top:100px;
}
input{
padding:10px;
margin:10px;
width:250px;
}
button{
padding:10px 20px;
background:#00cc66;
border:none;
color:white;
font-size:16px;
}
</style>
</head>
<body>
<h1>ESP32 Password Vault</h1>
<form action="/login" method="GET">
<input type="password" name="pass" placeholder="Admin Password">
<br>
<button type="submit">Login</button>
</form>
</body>
</html>
)rawliteral";

String getDashboard()
{
String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Password Vault</title>
<style>
body{
font-family:Arial;
background:#222;
color:white;
padding:20px;
}
input{
padding:10px;
margin:5px;
width:250px;
}
button{
padding:10px;
background:#0099ff;
color:white;
border:none;
}
.card{
background:#333;
padding:10px;
margin:10px 0;
border-radius:10px;
}
</style>
</head>
<body>
<h1>Password Vault</h1>

<form action="/save" method="GET">
<input type="text" name="site" placeholder="Website/App Name" required>
<br>
<input type="text" name="username" placeholder="Username" required>
<br>
<input type="text" name="password" placeholder="Password" required>
<br>
<button type="submit">Save Password</button>
</form>

<h2>Saved Passwords</h2>
)rawliteral";

int total = prefs.getInt("count", 0);

for(int i=0; i<total; i++)
{
String key = "entry" + String(i);
String data = prefs.getString(key.c_str(), "");

page += "<div class='card'>";
page += data;
page += "</div>";
}

page += "</body></html>";

return page;
}

void setup()
{
Serial.begin(115200);

prefs.begin("vault", false);

WiFi.softAP(ssid, hotspotPassword);

Serial.println("Hotspot Started");
Serial.println(WiFi.softAPIP());

server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
{
request->send(200, "text/html", htmlLogin);
});

server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request)
{
if(request->hasParam("pass"))
{
String pass = request->getParam("pass")->value();

if(pass == adminPassword)
{
request->send(200, "text/html", getDashboard());
}
else
{
request->send(200, "text/html", "<h1>Wrong Password</h1>");
}
}
});

server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request)
{
if(
request->hasParam("site") &&
request->hasParam("username") &&
request->hasParam("password")
)
{
String site = request->getParam("site")->value();
String username = request->getParam("username")->value();
String password = request->getParam("password")->value();

int count = prefs.getInt("count", 0);

String saveData = "<b>Site:</b> " + site +
"<br><b>User:</b> " + username +
"<br><b>Password:</b> " + password;

String key = "entry" + String(count);
prefs.putString(key.c_str(), saveData);

count++;
prefs.putInt("count", count);

request->redirect("/dashboard");
}
});

server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request)
{
request->send(200, "text/html", getDashboard());
});

server.begin();
}

void loop()
{
}
