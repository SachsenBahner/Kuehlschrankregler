
// 740 = 5°

// bei 670 abschalten 

#define tempSensor A7
#define kompressor 3  // Pin D3
#define anzMessungen 10
#define sollTemp 5
#define margin 1

unsigned long messpause = 60000;
unsigned int intervallTempSensor = 500;

unsigned long aktuelleZeit = 0;
unsigned long vergangeneZeit = 0;

enum stateList { PAUSE, MESSEN, REAGIEREN };
enum stateList state = MESSEN;

unsigned int messwerte[anzMessungen];
unsigned int messID = 0;


void setup() {
  pinMode(tempSensor, INPUT);
  pinMode(kompressor, OUTPUT);

  Serial.begin(9600);
  Serial.println("Keeping u cool");
}

void loop() {
  aktuelleZeit = millis();

  switch (state) {
    case MESSEN:
      if (aktuelleZeit - vergangeneZeit >= intervallTempSensor) {
        messwerte[messID] = analogRead(tempSensor);
        Serial.println(messID);
        Serial.println(messwerte[messID]);
        messID++;
        vergangeneZeit = aktuelleZeit;

        if (messID >= anzMessungen) {
          state = REAGIEREN;
        }
      }

      break;


      
    case REAGIEREN:
      Serial.println("REAGIEREN");
      messID = 0;

      long mw = 0;
      for (messID; messID < anzMessungen; messID++) {
        mw += messwerte[messID];
      }
      mw = mw / anzMessungen;

      //int temperatur = map(mw, 0, 1023, 30, 1); // TODO: Mapping auf Temperatur
      //Serial.println(temperatur);

      //if (temperatur - margin > sollTemp){ // 10° - 2° >  7°
      
      if (mw < 520){
        digitalWrite(kompressor, LOW);  // LOW = an
      }
      else if (mw > 670){
        digitalWrite(kompressor, HIGH); // High = aus
      }

      messID = 0;
      state = MESSEN;

      delay(messpause);
      
      asm volatile("jmp 0x00");  // simple watchdog
      
      break;

      
    case PAUSE:
      Serial.println("PAUSE");
      if (aktuelleZeit - vergangeneZeit >= messpause){
        state = MESSEN;
      }
      break;
  }
}
