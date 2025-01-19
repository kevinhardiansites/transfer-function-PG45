const int motor1 = 11;
const int motor2 = 1;
const int pEncA = 2;
const int pEncB = 3;

volatile long countPulses = 0;
const int timeSampling = 50; //ms
unsigned long lastTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(pEncA, INPUT);
  pinMode(pEncB, INPUT);

  attachInterrupt(pEncA, ISR_Encoder_A, RISING);
}

void motor(int speed) {
  if (speed > 0) {
    analogWrite(motor1, speed);
    digitalWrite(motor2, LOW);
  } else if (speed < 0) {
    analogWrite(motor1, 255 + speed);
    digitalWrite(motor2, HIGH);
  } else {
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, LOW);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTime = millis();
  float deltaTime = currentTime - lastTime; //ms
  if (deltaTime >= timeSampling){
    float freqSignal = (float)countPulses/(deltaTime/1.0e3); //Hz
    countPulses = 0;
    float rpm = freqSignal * 60.0 / 3840; // 3840 = p*N; p=strep-up gear (19.2); N=pulses per revolution (200);
    
    Serial.println(rpm);
    
    lastTime = currentTime;


    motor(255);
  }
}

void ISR_Encoder_A(){
  if(digitalRead(pEncB)){
    //clockwise
    countPulses++;
  }else{
    //anti clockwise
    countPulses--;
  }
}
