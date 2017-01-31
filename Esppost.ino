#include "Arduino.h"
#include <SoftwareSerial.h>
#include "MyKey.h"

#define DEBUG false

const String HOST = "192.168.43.114"; //your localhost at same ssid, ifconfig

//init

SoftwareSerial ser(A2, A3); // RX 2, TX 3

//The setup function is called once at startup of the sketch
void setup() {
	delay(500);
	Serial.begin(9600);
	ser.begin(9600); // 9600 stable baud rate for uno
	delay(100);
	connectWifi();
	delay(100);
}

// The loop function is called in an endless loop
void loop() {
	sendDataID("4500A", "tempe bongkrek");
	delay(1000);// gunakan millis

}

void sendDataID(String prm1, String prm2) {
	String request = "{\"kode\":\"" +prm1+ "\",\"nama\":\"" + prm2 + "\"}";

	String cmd = "AT+CIPSTART=\"TCP\",\"";

	cmd += HOST;
	cmd += "\",80\r\n";
	sendCommand(cmd, 600, DEBUG);
	delay(30);

	String cmd2 = "POST /wsphpserver/api.php";
	//cmd2 += String(svalFertInterval, DEC);

	cmd2 += " HTTP/1.1\r\n";
	cmd2 += "Host: " + HOST + "\r\n";
	cmd2 += "Accept: application/json\r\n";
	cmd2 += "Content-Type: application/json\r\n";
	cmd2 += "Content-Length: ";
	cmd2 += request.length();
	cmd2 += "\r\n\r\n";
	cmd2 += request;

	String pjg = "AT+CIPSEND=";
	pjg += cmd2.length();
	pjg += "\r\n";

	String closeCommand = "AT+CIPCLOSE";
	closeCommand += "\r\n";

	sendCommand(pjg, 600, DEBUG);
	delay(100);
	sendCommand(cmd2, 1000, DEBUG);
	delay(100);
	sendCommand(closeCommand, 500, DEBUG);
	delay(100);
}

String sendCommand(String command, const int timeout, boolean debug) {
	String response = "";

	ser.print(command); // send the read character to the esp8266

	long int time = millis();

	while ((time + timeout) > millis()) {
		while (ser.available()) {
			// The esp has data so display its output to the serial window
			char c = ser.read(); // read the next character.
			response += c;
		}
	}

	if (debug) {
		Serial.print(response);
	}

	return response;
}

void connectWifi() {
	//Set-wifi
	Serial.print("Restart Module...\n");
	sendCommand("AT+RST\r\n", 1000, DEBUG);
	delay(500);
	Serial.print("Set wifi mode...\n");
	sendCommand("AT+CWMODE=1\r\n", 1000, DEBUG); //
	delay(500);
	Serial.print("Connect to access point...\n");
	sendCommand("AT+CWJAP=\"" + SSID + "\",\"" + PASS + "\"\r\n", 1000, DEBUG);
	delay(2000);
	Serial.print("Check IP Address...\n");
	sendCommand("AT+CIFSR\r\n", 1000, DEBUG); // get ip address
	delay(500);
}
