
#include <WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "KVRENO";
const char* password = "000111000";
// const char *ssid = "Ms";
// const char *password = "@12345678#";

NetworkServer server(80);  //start a server on port 80


#define stepPin 16  //Pull -ve
#define dirPin 17   //DIR -ve
#define ledPin 2
#define stepsPerRevolution 10000

int mode = 1;
double pos = 0.000000;
double factor = 0.000251;
// double factor = 0.000601;

boolean isCalibrated = false;

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
  //Serial.println("--------- Initialization Completed ------------------ ");
}


void loop() {
  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {
    // if you get a client,
    //Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";      // make a String to hold incoming data from the client
    while (client.connected()) {  // loop while the client's connected
      if (client.available()) {   // if there's bytes to read from the client,
        char c = client.read();   // read a byte, then
        //Serial.write(c);
        // Create a JSON object
        StaticJsonDocument<200> doc;
        doc["message"] = "success";
        // Serialize JSON to a string
        char jsonBuffer[256];
        serializeJson(doc, jsonBuffer);
        // print it out the serial monitor
        if (c == '\n') {  // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            //client.println("Content-type:text/html");
            client.println("Content-type:application/json");
            client.println();

            // the content of the HTTP response follows the header:
            //client.print("Success");
            client.print(jsonBuffer);

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

        if (currentLine.startsWith("GET ") && currentLine.endsWith("/L")) {
          double* _movementRange = getMovementRange(currentLine);
          double startPosition = _movementRange[0];
          double endPosition = _movementRange[1];
          int speed = getSpeed(currentLine);
          setFactor(speed);

          Serial.println("");
          Serial.println(currentLine);
          Serial.println("");

          Serial.print("start Position ");
          Serial.print(startPosition);
          Serial.println("");
          Serial.println(" end position ");
          Serial.print(endPosition);

          if (startPosition < endPosition) {
            // move forward
            for (pos = startPosition; pos <= endPosition; pos = pos + factor) {
              move(speed);
              //Serial.println(pos, 6);
            }

            // do {
            //   //   Send count up to serial
            //   //Serial.println(pos);
            //   move(speed);
            //   pos = pos + factor;
            // } while (startPosition < endPosition);
          } else {
            // move backward
            for (pos = startPosition; pos >= endPosition; pos = pos - factor) {
              move(speed);
              // Serial.println(i, 6);
            }
          }
        }
        // Check to see if the client request was "GET /L" or "GET /R":
        //if (currentLine.endsWith("GET /L")) {
        /*
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
          */
      }
    }
    // close the connection:
    client.stop();
    //Serial.println("Client Disconnected.");
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
  Serial.println("-------------- Count up -------------------");
  pos = 0;
  setFactor(speed);
  long time = millis();
  do {
    //   Send count up to serial
    //Serial.println(pos);
    move(speed);
    pos = pos + factor;
  } while (pos < distance);

  // stop motor movement
  analogWrite(stepPin, 0);
  Serial.println(millis() - time);
}

void moveLeft(double distance, int speed) {
  //set the spinning direction clockwise
  digitalWrite(dirPin, LOW);
  //Send count down to serial
  Serial.println("-------------- Count down -------------------");
  setFactor(speed);
  pos = distance;
  do {
    //Serial.println(pos);
    // control motor movement
    move(speed);
    pos = pos - factor;

  } while (pos >= 0);
  // stop motor movement
  analogWrite(stepPin, 0);
}

void setFactor(int speed) {
  factor = speed * factor;
  Serial.println(" factor ");
  Serial.println(factor, 6);
  Serial.println(" ");
}

void move(int speed) {
  // control motor movement
  //these four lines result in 1 step
  Serial.println(pos, 6);            //send pos to serial monitor / output program
  analogWrite(stepPin, 51 * speed);  //max value = 255, max speed = 5 min value = 0
  // delayMicroseconds(60 );                        //delay in milliseconds
  // analogWrite(stepPin, 0);
  // delayMicroseconds(30);
}

void stop() {
  digitalWrite(stepPin, LOW);
}


double* getMovementRange(String line) {
  // Dynamically allocate memory for an array of doubles
  double* arr = new double[2];
  //Serial.println("");
  //Serial.print("distance: ");
  //String distance = line.substring(5, line.length() - 2);
  int separatorIndex = line.indexOf(':');
  String startPos = line.substring(5, separatorIndex);
  String endPos = line.substring(separatorIndex + 1, line.length() - 4);

  Serial.println("start pos: ");
  Serial.print(startPos);
  Serial.println("");

  Serial.println("end pos: ");
  Serial.print(endPos);
  Serial.println("");
  //Serial.print(distance);
  //Serial.println("");

  arr[0] = startPos.toDouble();
  arr[1] = endPos.toDouble();

  return arr;
}

double getDistance(String line) {
  //Serial.println("");
  //Serial.print("distance: ");
  //String distance = line.substring(5, line.length() - 2);
  String distance = line.substring(5, line.length() - 4);
  int separatorIndex = line.lastIndexOf(':');
  String startPos = line.substring(5, separatorIndex);
  String endPos = line.substring(separatorIndex + 1, line.length() - 4);
  Serial.print("start pos: ");
  Serial.println(startPos);
  Serial.println("");

  Serial.print("end pos: ");
  Serial.println(endPos);
  Serial.println("");
  //Serial.print(distance);
  //Serial.println("");

  return distance.toDouble();
}


int getSpeed(String line) {
  //Serial.println("");
  //Serial.print("speed: ");
  String speed = line.substring(line.length() - 3, line.length() - 2);
  Serial.println("speed ");
  Serial.println(speed);
  Serial.println(" ");

  return speed.toInt();
}