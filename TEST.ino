void setup() {
  Serial.begin(9600);

}

void loop() {
  Serial.print("A1: ");
  Serial.print(analogRead(A1));
  Serial.print(" | A2: ");
  Serial.println(analogRead(A2));

}
