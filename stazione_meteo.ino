#include <Adafruit_SHT4x.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

Adafruit_SHT4x sht4;
sensors_event_t humidity, temp;

ESP8266WebServer server(80);

void handleRoot() {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<link rel=\"preconnect\" href=\"https://fonts.googleapis.com\">";
  html += "<link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin>";
  html += "<link href=\"https://fonts.googleapis.com/css2?family=Stalinist+One&display=swap\" rel=\"stylesheet\">";
  html += "<title>espMeteoStation8266</title>";
  html += "<style>";
  html += "body";
  html += "{";
  html += "    font-family: \"Stalinist One\", sans-serif;";
  html += "}";
  html += "h1 {";
  html += "  font-size: 40px;";
  html += "  text-align: center;";
  html += "}";
  html += "p";
  html += "{";
  html += "    font-size: 20px;";
  html += "    text-align: center;";
  html += "}";
  html += "pre";
  html += "{";
  html += "    text-align: center;";
  html += "}";
  html += "</style>";
  html += "</head>";
  html += "<body style=\"background-color: rgba(0, 0, 0, 0.904);\"";
  html += "font-family: 'Brush Script MT', cursive;>";
  html += "<h1 style=\"color:white\";>40.7471&deg;N, 14.4982&deg;E </h1>";
  html += "<p style=\"color:white\";>Temperature: "+String(temp.temperature)+"&deg;C</p>";
  html += "<p style=\"color:white\";>Humidity: "+String(humidity.relative_humidity)+"%</p>";;
  html += "<br><br><br><br><br><br><br><br>";
  html += "<pre style=\"color:white\">";
  html += " _____                                           _        _             \n";
  html += "|     |___ ___ ___    ___ ___ _ _ ___ ___ ___   |_|___   | |___ _ _ ___ \n";
  html += "|  |  | . | -_|   |  |_ -| . | | |  _|  _| -_|  | |_ -|  | | . | | | -_|\n";
  html += "|_____|  _|___|_|_|  |___|___|___|_| |___|___|  |_|___|  |_|___|\\_/|___|\n";
  html += "      |_|                                                               ";
  html += "</pre>";
  html += "";
  html += "</body>";
  html += "</html>";

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
    if (!MDNS.begin("esp8266-meteo-station")) {
    Serial.println("Errore avvio mDNS!");
  } else {
    Serial.println("mDNS avviato: http://esp8266-meteo-station.local");
  }
  
  MDNS.addService("http", "tcp", 80);
}

void loop() {
  
  MDNS.update();
  sht4.getEvent(&humidity, &temp);

  //remove comment for serial console debug info
  /*
  Serial.print("Temp: ");
  Serial.print(temp.temperature);
  Serial.print(" °C   -   Hum: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");
  */
  
  server.handleClient();
  delay(1000);
}
