#include <ESP8266WiFi.h>  // Include the ESP8266 WiFi library
#include <ESP8266WebServer.h>  // Include the ESP8266 WebServer library
#include "FS.h"  // Include the SPIFFS library for file storage

// WiFi credentials
const char* ssid = "Devil";
const char* password = "Warasa1605@";

// Create a web server on port 80
ESP8266WebServer server(80);

// Function to handle file download request
void handleDownload() {
    File file = SPIFFS.open("/testfile.txt", "r");  // Open the file from SPIFFS in read mode
    if (!file) {  // Check if file exists
        server.send(404, "text/plain", "File not found");  // Send 404 error if file is missing
        return;
    }

    server.streamFile(file, "application/octet-stream");  // Stream the file as a binary download
    file.close();  // Close the file after sending
}

void setup() {
    Serial.begin(115200);  // Start the serial communication at 115200 baud
    Serial.println("Starting ESP8266...");

    WiFi.begin(ssid, password);  // Connect to WiFi
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {  // Wait until connected
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected!");  // Print success message
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());  // Print assigned IP address

    // Initialize SPIFFS (file system)
    if (!SPIFFS.begin()) {
        Serial.println("SPIFFS Mount Failed");  // Print error if SPIFFS fails to mount
        return;
    }

    // Create a test file in SPIFFS
    File file = SPIFFS.open("/testfile.txt", "w");  // Open file in write mode
    if (!file) {  // Check if file was created successfully
        Serial.println("Failed to create file");
    } else {
        file.print("This is a test file for ESP8266 SPIFFS HTTP download.");  // Write sample text
        file.close();  // Close the file after writing
    }

    // Define HTTP route for file download
    server.on("/download", HTTP_GET, handleDownload);
    server.begin();  // Start the web server
    Serial.println("HTTP server started.");  // Print confirmation
    Serial.print("Download link: http://");                        
    Serial.print(WiFi.localIP());  // Print device IP
    Serial.println("/download");  // Print full download link
}

void loop() {
    server.handleClient();  // Continuously check for client requests
}
