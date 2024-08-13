
#include <WiFi.h>

const char *ssid = "Ms";
const char *password = "@12345678#";

NetworkServer server(80);


#define stepPin 16  //Pull -ve
#define dirPin 17   //DIR -ve
#define ledPin 2
#define stepsPerRevolution 10000

int mode = 1;
int pos = 0;
void setup() {
  // put your setup code here, to run once:
  // declare pins as output
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  //initialize serial communication
  Serial.begin(115200);
  Serial.println("--------- Serial communicatiton started on 115200 ------------------ ");
  pinMode(5, OUTPUT);  // set the LED pin mode

  delay(10);
  Serial.println("------------- Connecting to Provisioned WIFI ------------------ ");
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("--------- Initialization Completed ------------------ ");
}


void loop() {

  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Success");
            //client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            //client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /L" or "GET /R":
        //if (currentLine.endsWith("GET /L")) {
        if (currentLine.startsWith("GET ") && currentLine.endsWith("/L")) {
          double distance = getDistance(currentLine);
          int speed = getSpeed(currentLine);
          digitalWrite(5, HIGH);  // GET /H turns the LED on
          moveLeft(distance, speed);
        }
        //if (currentLine.endsWith("GET /R")) {
        if (currentLine.startsWith("GET ") && currentLine.endsWith("/R")) {
          double distance = getDistance(currentLine);
          int speed = getSpeed(currentLine);
          digitalWrite(5, LOW);  // GET /L turns the LED off
          moveRight(distance, speed);
        }

        if (currentLine.endsWith("GET /S")) {
          digitalWrite(5, LOW);  // GET /L turns the LED off
          stop();
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
  // put your main code here, to run repeatedly:
  //spin the stepper motor 1 revolution slowly
  // Serial.println("--------------- Forward Direction --------------- ");

  // if (mode == 1) {
  //   //set the spinning direction clockwise
  //   digitalWrite(dirPin, HIGH);
  //   for (pos = 0; pos < stepsPerRevolution; pos++) {
  //     //these four lines result in 1 step
  //     digitalWrite(stepPin, HIGH);
  //     delayMicroseconds(30);
  //     digitalWrite(stepPin, LOW);
  //     delayMicroseconds(30);
  //   }
  //   mode = 2;
  // }


  // delay(5000);  // delay before changing direction
  // Serial.println("--------------- Backward Direction ---------------- ");

  // if (mode == 2) {
  //   //set the spinning direction anti clockwise
  //   digitalWrite(dirPin, LOW);
  //   for (pos = 0; pos < stepsPerRevolution; pos++) {
  //     //these four lines result in 1 step
  //     digitalWrite(stepPin, HIGH);
  //     delayMicroseconds(30);
  //     digitalWrite(stepPin, LOW);
  //     delayMicroseconds(30);
  //   }
  //   mode = 1;
  // }

  // delay(5000);  // delay before changing direction
}


void moveRight(double distance, int speed) {
  //set the spinning direction clockwise
  digitalWrite(dirPin, HIGH);
  for (pos = 0; pos < stepsPerRevolution * distance; pos++) {
    //these four lines result in 1 step
    digitalWrite(stepPin, HIGH);
    delay(100 / speed);
    digitalWrite(stepPin, LOW);
    delay(100 / speed);
  }
}

void moveLeft(double distance, int speed) {
  //set the spinning direction clockwise
  digitalWrite(dirPin, LOW);
  for (pos = 0; pos < stepsPerRevolution * distance; pos++) {
    //these four lines result in 1 step
    digitalWrite(stepPin, HIGH);
    delay(100 / speed);
    digitalWrite(stepPin, LOW);
    delay(100 / speed);
  }
}

/*
void moveRight(double distance) {
  //set the spinning direction clockwise
  digitalWrite(dirPin, HIGH);
  for (pos = 0; pos < stepsPerRevolution * distance; pos++) {
    //these four lines result in 1 step
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(30);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(30);
  }
}
*/
/*
void moveLeft(double distance, int speed) {
  //set the spinning direction clockwise
  digitalWrite(dirPin, LOW);
  for (pos = 0; pos < stepsPerRevolution * distance; pos++) {
    //these four lines result in 1 step
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(30);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(30);
  }
}
*/



void stop() {
  digitalWrite(stepPin, LOW);
}

double getDistance(String line) {
  Serial.println("");
  Serial.print("distance: ");
  //String distance = line.substring(5, line.length() - 2);
  String distance = line.substring(5, line.length() - 4);
  Serial.print(distance);
  Serial.println("");

  return distance.toDouble();
}


int getSpeed(String line) {
  Serial.println("");
  Serial.print("speed: ");
  String speed = line.substring(line.length() - 3, line.length() - 2);
  Serial.print(speed);
  Serial.println("");

  return speed.toInt();
}