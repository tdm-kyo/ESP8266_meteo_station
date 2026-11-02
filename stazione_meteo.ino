#include <Adafruit_SHT4x.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "TIM-37896522";
const char* password = "WKuUdXzwkE8hK73qFq3m";

Adafruit_SHT4x sht4;
int myVariable = 0;

ESP8266WebServer server(80);

void handleRoot() {
  // 1. Increment the variable just to see it change on refresh
  myVariable++; 

  // 2. Build the HTML string, injecting the variable directly into it
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<title>ESP8266 Variable Display</title>";
  html += "<style>body{font-family:Arial; text-align:center; margin-top:50px;} h1{color:#333;}</style></head>";
  html += "<body>";
  html += "<h1>ESP8266 Web Server</h1>";
  
  // Here is where the variable is injected
  html += "<p>The current value of the variable is: <strong>" + String(myVariable) + "</strong></p>";
  
  html += "<p><button onclick=\"location.reload()\">Refresh Page</button></p>";
  html += "</body></html>";

  // 3. Send the HTML string back to the browser
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(9600);

  if (!sht4.begin()) {
    Serial.println("SHT4x non trovato!");
    while (1);
  }

  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
   }
  
  // Print the IP address
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Define what to do when a client requests the root URL
  server.on("/", handleRoot);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  sensors_event_t humidity, temp;

  sht4.getEvent(&humidity, &temp);

  Serial.print("Temp: ");
  Serial.print(temp.temperature);
  Serial.print(" °C   -   Umidità: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");
  server.handleClient();
  delay(1000);
}
