#define echoPin 2
#define trigPin 3
long duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Serial.println("------------------ setup completed ----------------------");
}

void loop() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // put your main code here, to run repeatedly:
  duration = pulseIn(echoPin, HIGH),
  distance = duration * 0.034 / 2; //0.0135 for inches per second
  Serial.println( String(distance) + " cm");

}
