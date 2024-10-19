int pwm2 = 19;
int dir2 = 18;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(19,OUTPUT);
  pinMode(18,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(pwm2,0);
  int val = Serial.read() - '0';

  if(val == 1){

    while(Serial.available() == 0){
      analogWrite(pwm2,150);
      digitalWrite(dir2,LOW);
    }
  }

  else if(val == 2){

    while(Serial.available() == 0){
      analogWrite(pwm2,150);
      digitalWrite(dir2,HIGH);
    }
  }

  else{
    analogWrite(pwm2,0);
}

  





}
