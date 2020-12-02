
#define BUTTONLED 9
#define BUZZER 10
#define ENABLE 11
#define COMPLETE 12
#define LED 13

#define PRESSDELAY 250
#define RELEASEDELAY 50
#define BLINKSPEED 450

#define FAILTONE 120
#define FAILTONETIME 5000
#define PASSTONE 4000
#define FAILDELAY 55000
#define CHIRPTIME 100
#define CHIRPDELAY 400

int playing = 0;
long int tEnable = 0;
int filterDuration = 1000;

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(ENABLE, INPUT_PULLUP);
  pinMode(COMPLETE, OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(BUTTONLED,OUTPUT);

  digitalWrite(COMPLETE,HIGH);
  digitalWrite(BUZZER,LOW);
  digitalWrite(LED,LOW);
  digitalWrite(BUTTONLED,LOW);

  DDRC = 0;
  PORTC = 0x3F;

  Serial.begin(9600);
}

volatile uint8_t numactions = 0;
volatile uint8_t failyet = 0;

void loop() {
  if(digitalRead(ENABLE) == HIGH){
    tEnable = millis();
  }
  
  if(digitalRead(ENABLE) == LOW && (millis()-tEnable) >= filterDuration){
    playing = 1;
    digitalWrite(BUTTONLED,HIGH);
  }
  
  if (playing == 1){
    uint8_t state = 0;
  
    Serial.println(InputStates());
  
    while(!state){
      state = InputStates();
      updateLed();
    }
    delay(PRESSDELAY);
  
    tone(BUZZER, PASSTONE, CHIRPTIME);
    
    while(InputStates());
    delay(RELEASEDELAY);
  
    Serial.println(state);
    
    if(state != (1<<numactions))
      failyet = 1;
  
    numactions++;
    if(numactions == 6){
      if(failyet == 0)
        wincondition();
      else{
        failcondition();
        numactions = 0;
        failyet = 0;
      }
    }
  }
}

void wincondition(){
  tone(BUZZER,PASSTONE,CHIRPTIME);
  delay(CHIRPDELAY);
  tone(BUZZER,PASSTONE,CHIRPTIME);
  delay(CHIRPDELAY);
  tone(BUZZER,PASSTONE,CHIRPTIME);
  delay(CHIRPDELAY);
  digitalWrite(COMPLETE,LOW);
  digitalWrite(BUTTONLED,HIGH);
  while(1);
}

void failcondition(){
  digitalWrite(LED,HIGH);               
  digitalWrite(BUTTONLED,LOW);          
  tone(BUZZER, FAILTONE, FAILTONETIME);
  delay(FAILDELAY);                     
  twoChirp();
  digitalWrite(LED,LOW);
  digitalWrite(BUTTONLED,HIGH);
  delay(CHIRPTIME);
}

uint8_t InputStates(){
  return ~ ((PINC & 0x3F) | 0xC0);
}

void updateLed(){
  if((millis()/BLINKSPEED)%5)
    digitalWrite(LED,LOW);
  else
    digitalWrite(LED,HIGH);
}

void twoChirp(){
  digitalWrite(LED,HIGH);
  tone(BUZZER, FAILTONE);
  delay(CHIRPTIME);
  noTone(BUZZER);
  digitalWrite(LED,LOW);
  delay(CHIRPDELAY);
  digitalWrite(LED,HIGH);
  tone(BUZZER, FAILTONE);
  delay(CHIRPTIME);
  noTone(BUZZER);
  digitalWrite(LED,LOW);
  delay(CHIRPDELAY);
}
