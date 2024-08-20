
#include <WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "KVRENO";
const char* password = "000111000";
// const char* ssid = "Ms";
// const char* password = "@12345678#";

NetworkServer server(80);  //start a server on port 80


#define stepPin 16  //Pull -ve
#define dirPin 17   //DIR -ve
#define limiterPin 5
#define limiterPin_2 4
#define stepsPerRevolution 10000

int mode = 1;
double factor = 0.000251;
// double factor = 0.000601;
double initialPosition = 0.000000;
double destination = 0.000000;
int speed = 1;
boolean isInitialized = false;


void setup() {
  // put your setup code here, to run once:
  // declare pins as output
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  //limiterPin pins
  pinMode(limiterPin, OUTPUT);    // limiterPin mode
  pinMode(limiterPin_2, OUTPUT);  // limiterPin mode

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
            // Serial.println("Param: " + paramName + " = " + paramValue);

            // set values

            if (paramName == "destination") {
              destination = paramValue.toDouble();
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


void setFactor(int speed) {
  factor = speed * factor;
  // Serial.println(" factor ");
  // Serial.println(factor, 6);
  // Serial.println(" ");
}

void move() {
  // control motor movement
  //these four lines result in 1 step
  //max value = 255, max speed = 5 min value = 0

  // delayMicroseconds(60 );                        //delay in milliseconds
  // analogWrite(stepPin, 0);
  // delayMicroseconds(30);

  boolean isLimiting = digitalRead(limiterPin) == 1 || digitalRead(limiterPin_2) == 1;

  // Serial.println(digitalRead(limiterPin));
  if (initialPosition < destination) {
    //set motor spinning direction clockwise
    digitalWrite(dirPin, LOW);
    // start motor movement
    // analogWrite(stepPin, 51 * speed);
    digitalWrite(stepPin, HIGH);

    while (initialPosition <= destination && !isLimiting) {
      Serial.println(initialPosition, 6);  //send position to serial monitor / output program
      initialPosition = initialPosition + factor;
      isLimiting = digitalRead(limiterPin) == 1 || digitalRead(limiterPin_2) == 1;
    }
  } else {
    //set motor spinning direction anti clockwise
    digitalWrite(dirPin, HIGH);
    // start motor movement
    // analogWrite(stepPin, 51 * speed);
    digitalWrite(stepPin, HIGH);
    // move backward
    while (initialPosition >= destination && !isLimiting) {
      Serial.println(initialPosition, 6);  //send position to serial monitor / output program
      initialPosition = initialPosition - factor;
      isLimiting = digitalRead(limiterPin) == 1 || digitalRead(limiterPin_2) == 1;
    }
  }

  /*
  Serial.print("\nDestination ");
  Serial.print(destination);

  Serial.print("\ninitialPosition ");
  Serial.print(initialPosition);
  */
}


void resetMotorPosition() {
  Serial.print("Initializing ");
  digitalWrite(limiterPin, LOW);
  // check if motor has moved to start position: 0
  while (digitalRead(limiterPin) != 1) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nInititalized");
}


/*
void resetMotorPosition() {
  isInitialized = false;
  Serial.print("Initializing ");
  // check if motor has moved to start position: 0
  while (!isInitialized) {
    Serial.print(".");
    delay(500);
    if (digitalRead(limiterPin) == HIGH) {
      isInitialized = true;
    }
  }
  Serial.println("\nInititalized");
}
*/

void stop() {
  digitalWrite(stepPin, LOW);
}
