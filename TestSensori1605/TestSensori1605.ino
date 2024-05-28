//PIN CONTROLLO MOTORE
const int ML1I =  3; 
const int ML2I =  4;
const int ONI = 9;
const int ML1 =  6; 
const int ML2 =  7;
const int ON = 5;

//PIN CONTROLLO SENSORI
const int txDestraSu = 10;
const int rxDestraSu = A1;
const int txCentro = 11;
const int rxCentro = A2;
const int txSinistraSu = 12;
const int rxSinistraSu = A3;
const int txSinistraGiu = 13;
const int rxSinistraGiu = A4;
const int txDestraGiu = 8;
const int rxDestraGiu = A5;
const int txFiguraOk = 14;
const int rxFiguraOk = A6;
const int txFiguraNo = 15;
const int rxFiguraNo = A7;

//SOGLIE DEI SENSORI
int sogliaN = 1014;
int soglia = 1014;

//VARIABILI PER VALORI SENSORI
int sensorvalue1;
int sensorvalue2;
int sensorvalue3;
int sensorvalue4;
int sensorvalue5;
int sensorvalue6;
int sensorvalue7;

//PIN CONTROLLO LED E PULSANTE ACCENSIONE
const int LedRosso = 24;
const int LedVerde = 28;
const int PulsanteAccensione = 22;
const int PulsanteSpegnimento = 23;

int valAccensione = 0;
int valSpegnimento = 0;
bool premuto = false;

int buzzer = 26;

//STATO DEI SENSORI
int destraSopra,centro,sinistraSopra,sinistraSotto,destraSotto;

//INIZIALIZZAZIONE DEI CONTATORI DEI PEZZI
int y = 0;
int x = 0;
int z = 0;

void setup(){     

  Serial.begin(9600);

  pinMode(ML1, OUTPUT);   
  pinMode(ML2, OUTPUT);  
  pinMode(ON, OUTPUT);   
  pinMode(ML1I, OUTPUT);   
  pinMode(ML2I, OUTPUT);  
  pinMode(ONI, OUTPUT); 

  pinMode (txDestraSu, OUTPUT);
  pinMode (rxDestraSu, INPUT);
  pinMode (txCentro, OUTPUT);
  pinMode (rxCentro, INPUT);
  pinMode (txSinistraSu, OUTPUT);
  pinMode (rxSinistraSu, INPUT);
  pinMode (txSinistraGiu, OUTPUT);
  pinMode (rxSinistraGiu, INPUT);
  pinMode (txDestraGiu, OUTPUT);
  pinMode (rxDestraGiu, INPUT);
  pinMode (txFiguraOk, OUTPUT);
  pinMode (rxFiguraOk, INPUT);
  pinMode (txFiguraNo, OUTPUT);
  pinMode (rxFiguraNo, INPUT);

  digitalWrite(txDestraSu, HIGH); 
  digitalWrite(txCentro, HIGH); 
  digitalWrite(txSinistraSu, HIGH); 
  digitalWrite(txSinistraGiu, HIGH);
  digitalWrite(txDestraGiu, HIGH);
  digitalWrite(txFiguraOk, HIGH);
  digitalWrite(txFiguraNo, HIGH);

  pinMode(LedRosso,OUTPUT);
  pinMode(PulsanteAccensione,INPUT);

  pinMode(LedVerde,OUTPUT);
  pinMode(PulsanteSpegnimento,INPUT);

  pinMode(buzzer,OUTPUT);
}

void loop(){

  //Inizializzazione dei valori della lettura
  destraSopra = 0;
  centro = 0;
  sinistraSopra = 0;
  sinistraSotto  = 0;
  destraSotto = 0;

  valAccensione = digitalRead(PulsanteAccensione);

  if(valAccensione == 1){
    premuto = !premuto;
    delay(200);
  } 

  if(premuto == true){

    digitalWrite(LedVerde,HIGH);
    digitalWrite(LedRosso,LOW);

    accendiMotore1();

    //Lettura dei valori rilevati dai sensori
    lettura(destraSopra,centro,sinistraSopra,sinistraSotto,destraSotto);

    //Se il sensore posto in basso a destra rileva una figura
    if(destraSotto==1){

      spegniMotore1();

      delay(2000);

      lettura(destraSopra,centro,sinistraSopra,sinistraSotto,destraSotto);

      if(centro == 0){

        Serial.println("non c'è una figura");

      }else if(sinistraSopra == 1 && destraSopra == 0 && centro == 1 && sinistraSotto == 1){ //FIGURA CORRETTA PARTE ALTA FINE

        tone(buzzer,1000);
        delay(500);
        noTone(buzzer);

        //Serial.println("FIGURA CORRETTA PARTE ALTA FINE");
        y++;

        accendiMotore1Veloce();

        delay(2000);

        spegniMotore1();
        
        sensorvalue6 = analogRead(rxFiguraOk);

        //Finchè o premo il pulsante per lo spegnimento o il sensore rileva il passaggio di una figura
        while((premuto==true) && (sensorvalue6 < sogliaN)){

          accendiMotore2();

          valAccensione = digitalRead(PulsanteAccensione);

          sensorvalue6 = analogRead(rxFiguraOk);

          if(valAccensione == 1){
            premuto = !premuto;
          } 
        }

        delay(200);

        spegniMotore2();

      } else if(sinistraSopra == 0 && destraSopra == 1 && centro == 1 && sinistraSotto == 1){ //FIGURA CORRETTA PARTE ALTA INIZIO

        tone(buzzer,1000);
        delay(500);
        noTone(buzzer);

        //Serial.println("FIGURA CORRETTA PARTE ALTA INIZIO");

        y++;

        accendiMotore1Veloce();

        delay(2000);

        spegniMotore1();
        
        sensorvalue6 = analogRead(rxFiguraOk);

        while((premuto==true) && (sensorvalue6 < sogliaN)){

          accendiMotore2();

          valAccensione = digitalRead(PulsanteAccensione);

          sensorvalue6 = analogRead(rxFiguraOk);

          if(valAccensione == 1){
            premuto = !premuto;
          } 
        }

        delay(200);

        spegniMotore2();

      } else if(destraSopra == 0 && centro == 1 && sinistraSopra == 0 && sinistraSotto == 1){ //FIGURA TROPPO BASSA

        tone(buzzer,300);
        delay(500);
        noTone(buzzer);

        //Serial.println("FIGURA TROPPO BASSA");

        x++;

        accendiMotore1Veloce();
        delay(2000);
        spegniMotore1();

        sensorvalue7 = analogRead(rxFiguraNo);

        while((premuto==true) && (sensorvalue7 < sogliaN)){

          accendiMotore2Reverse();

          valAccensione = digitalRead(PulsanteAccensione);

          sensorvalue7 = analogRead(rxFiguraNo);

          if(valAccensione == 1){
            premuto = !premuto;
          } 
        }

        delay(200);

        spegniMotore2();

      } else if(destraSopra == 0 && sinistraSopra == 0 && sinistraSotto == 0 && destraSotto == 1){ //FIGURA TROPPO CORTA PARTE BASSA INIZIO

        tone(buzzer,300);
        delay(500);
        noTone(buzzer);

        //Serial.println("FIGURA TROPPO CORTA PARTE BASSA INIZIO");

        z++;

        accendiMotore1Veloce();
        delay(2000);
        spegniMotore1();

        sensorvalue7 = analogRead(rxFiguraNo);

        while((premuto==true) && (sensorvalue7 < sogliaN)){

          accendiMotore2Reverse();

          valAccensione = digitalRead(PulsanteAccensione);

          sensorvalue7 = analogRead(rxFiguraNo);

          if(valAccensione == 1){
            premuto = !premuto;
          } 
        }

        delay(200);

        spegniMotore2();

      } else if(destraSopra == 1 && sinistraSopra == 0 && sinistraSotto == 0 && destraSotto == 1){ //FIGURA TROPPO CORTA PARTE BASSA FINE

        tone(buzzer,300);
        delay(500);
        noTone(buzzer);

        //Serial.println("FIGURA TROPPO CORTA PARTE BASSA FINE");

        z++;

        accendiMotore1Veloce();
        delay(2000);
        spegniMotore1();

        sensorvalue7 = analogRead(rxFiguraNo);

        while((premuto==true) && (sensorvalue7 < sogliaN)){

          accendiMotore2Reverse();

          valAccensione = digitalRead(PulsanteAccensione);

          sensorvalue7 = analogRead(rxFiguraNo);

          if(valAccensione == 1){
            premuto = !premuto;
          } 
        }

        delay(200);
      
        spegniMotore2();
      }

      //Output
      Serial.print("{\"OK\":");
      Serial.print(y);
      Serial.print(",\"Corte\":");
      Serial.print(z);
      Serial.print(",\"Basse\":");
      Serial.print(x);
      Serial.println("}");
    } 
  }else{ //Se premo il pulsante affinchè si spegna il circuito
    
    digitalWrite(LedVerde,LOW);
    digitalWrite(LedRosso,HIGH);

    spegniMotore1();
  }
}

//Funzione per l'accensione del nastro trasportatore superiore in modalità normale
void accendiMotore1(){
  digitalWrite(ML1I, HIGH);  
  digitalWrite(ML2I, LOW);
  analogWrite(ONI,135);
}

//Funzione per l'accensione del nastro trasportatore superiore in modalità veloce
void accendiMotore1Veloce(){
  digitalWrite(ML1I, HIGH);  
  digitalWrite(ML2I, LOW);
  analogWrite(ONI,180);
}

//Funzione per lo spegnimento del nastro trasportatore superiore 
void spegniMotore1(){
  digitalWrite(ML1I, HIGH);  
  digitalWrite(ML2I, LOW);
  analogWrite(ONI,0);
}

//Funzione per l'accensione del nastro trasportatore inferiore in modalità normale
void accendiMotore2(){
  digitalWrite(ML1, HIGH);  
  digitalWrite(ML2, LOW);
  analogWrite(ON,200);
}

//Funzione per l'accensione del nastro trasportatore inferiore in modalità reverse
void accendiMotore2Reverse(){
  digitalWrite(ML1,LOW);  
  digitalWrite(ML2, HIGH);
  analogWrite(ON,200); 
}

//Funzione per lo spegnimento del nastro trasportatore inferiore 
void spegniMotore2(){
  digitalWrite(ML1, HIGH);  
  digitalWrite(ML2, LOW);
  analogWrite(ON,0);
}

//Funzione per effettuare la lettura dei valori rilevati dai sensori
void lettura(int &a, int &b, int &c, int &d,int &e) {

  sensorvalue1 = analogRead(rxDestraSu);
  sensorvalue2 = analogRead(rxCentro);
  sensorvalue3 = analogRead(rxSinistraSu);
  sensorvalue4 = analogRead(rxSinistraGiu);
  sensorvalue5 = analogRead(rxDestraGiu);

  if(sensorvalue1 >= soglia){
    a = 1;
  }
  
  if(sensorvalue2 >= soglia) {
    b = 1;
  }
  
  if (sensorvalue3 >= soglia) {
    c = 1;
  }

  if (sensorvalue4 >= 600) {
    d = 1;
  }

  if (sensorvalue5 >= soglia) {
    e = 1;
  }
}
