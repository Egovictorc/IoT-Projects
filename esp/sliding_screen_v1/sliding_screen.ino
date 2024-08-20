
#include <WiFi.h>
#include <ArduinoJson.h>

// const char* ssid = "KVRENO";
// const char* password = "000111000";
const char* ssid = "Ms";
const char* password = "@12345678#";

NetworkServer server(80);  //start a server on port 80


#define stepPin 16  //Pull -ve
#define dirPin 17   //DIR -ve
#define ledPin 5
#define stepsPerRevolution 10000

int mode = 1;
double currentPosition = 0.000000;
double factor = 0.000251;
// double factor = 0.000601;
double startPosition = 0.000000;
double endPosition = 0.000000;
int speed = 1;
boolean isInitialized = false;
boolean isFirstTime = true;

void setup() {
  // put your setup code here, to run once:
  // declare pins as output
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(ledPin, INPUT);  // ledPin mode

  //initialize serial communication
  Serial.begin(115200);
  Serial.println("--------- Serial communicatiton started on 115200 ------------------ ");

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
  // Start initialization

  resetMotorPosition();
}


void loop() {
  NetworkClient client = server.accept();  // listen for incoming clients


  if (client) {
    Serial.println("New Client Connected");

    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);

    // Clear out any remaining HTTP headers
    client.flush();

    // Initialize JSON response
    String jsonResponse = "{";

    // Extract query parameters from GET request
    if (request.startsWith("GET")) {
      int paramsStart = request.indexOf('?');  // Find the start of the query string

      if (paramsStart != -1) {
        // Extract the query string
        String queryString = request.substring(paramsStart + 1, request.indexOf(' ', paramsStart));
        Serial.println("Query string: " + queryString);

        // Parse the query parameters (split by '&' and '=')
        String param;
        while (queryString.length() > 0) {
          int paramEnd = queryString.indexOf('&');

          if (paramEnd == -1) {
            param = queryString;
            queryString = "";
          } else {
            param = queryString.substring(0, paramEnd);
            queryString = queryString.substring(paramEnd + 1);
          }

          // Split parameter into name and value
          int separatorIndex = param.indexOf('=');
          if (separatorIndex != -1) {
            String paramName = param.substring(0, separatorIndex);
            String paramValue = param.substring(separatorIndex + 1);
            Serial.println("Param: " + paramName + " = " + paramValue);

            // set values

            if (paramName == "startPosition") {
              if(isFirstTime) {
                // do nothing, use 0.000000 as the start point
                isFirstTime = false;
              } else {
                startPosition = paramValue.toDouble();
              }
            } else if (paramName == "endPosition") {
              endPosition = paramValue.toDouble();
            } else if (paramName == "speed") {
              speed = paramValue.toInt();
              setFactor(speed);
            }



            // Add to JSON response
            jsonResponse += "\"" + paramName + "\": \"" + paramValue + "\",";
          }
        }
      }
    }

    // control motor movement and send command through serial
    move();
    // Remove trailing comma and close JSON object
    if (jsonResponse.endsWith(",")) {
      jsonResponse = jsonResponse.substring(0, jsonResponse.length() - 1);
    }
    jsonResponse += "}";

    // Send JSON response back to the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println(jsonResponse);

    // Close the connection
    client.stop();
    //Serial.println("Client Disconnected");
  }
}


// void moveRight(double distance, int speed) {
//   //set the spinning direction clockwise
//   digitalWrite(dirPin, HIGH);
//   Serial.println("-------------- Count up -------------------");
//   currentPosition = 0;
//   setFactor(speed);
//   long time = millis();
//   do {
//     //   Send count up to serial
//     //Serial.println(currentPosition);
//     move();
//     currentPosition = currentPosition + factor;
//   } while (currentPosition < distance);

//   // stop motor movement
//   analogWrite(stepPin, 0);
//   Serial.println(millis() - time);
// }

// void moveLeft(double distance, int speed) {
//   //set the spinning direction clockwise
//   digitalWrite(dirPin, LOW);
//   //Send count down to serial
//   Serial.println("-------------- Count down -------------------");
//   setFactor(speed);
//   currentPosition = distance;
//   do {
//     //Serial.println(currentPosition);
//     // control motor movement
//     move();
//     currentPosition = currentPosition - factor;

//   } while (currentPosition >= 0);
//   // stop motor movement
//   analogWrite(stepPin, 0);
// }

void setFactor(int speed) {
  factor = speed * factor;
  Serial.println(" factor ");
  Serial.println(factor, 6);
  Serial.println(" ");
}

void move() {
  // control motor movement
  //these four lines result in 1 step

  analogWrite(stepPin, 51 * speed);  //max value = 255, max speed = 5 min value = 0
  // delayMicroseconds(60 );                        //delay in milliseconds
  // analogWrite(stepPin, 0);
  // delayMicroseconds(30);

  if (startPosition < endPosition) {
    // move forward
    for (currentPosition = startPosition; currentPosition <= endPosition; currentPosition += factor) {
      Serial.println(currentPosition, 6);  //send position to serial monitor / output program
    }
  } else {
    // move backward
    for (currentPosition = startPosition; currentPosition >= endPosition; currentPosition -= factor) {
      Serial.println(currentPosition, 6);  //send position to serial monitor / output program
    }
  }
}


void resetMotorPosition() {
  isInitialized = false;
  Serial.print("Initializing ");
  // check if motor has moved to start position: 0
  while (!isInitialized) {
    Serial.print(".");
    delay(500);
    if (digitalRead(ledPin) == HIGH) {
      isInitialized = true;
    }
  }
  Serial.println("\nInititalized");
}


void stop() {
  digitalWrite(stepPin, LOW);
}
