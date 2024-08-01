/*#include <Wire.h>

#include <TEA5767Radio.h>*/

#include <Arduino.h>
#include <Wire.h>
#include <radio.h>
#include <TEA5767.h>

// C++ code
//

#define FIX_BAND RADIO_BAND_FM

/// The station that will be tuned by this sketch is 89.30 MHz.
#define FIX_STATION 8000

TEA5767 radio;    // Create an instance of Class for Si4703 Chip

uint8_t test1;
byte test2;


#define BOTAO A0

#define ANALOG_POTEN A1

#define EXIBER_1 12
#define EXIBER_2 2


#define SINC_AZUL 3
#define SINC_VERDE 4
#define SINC_VERMELHO 5

//TEA5767Radio radio = TEA5767Radio();

int level_radio = 80;
bool trava_sessao = false;
int disp_pinos[7] = {6, 7, 8, 9, 10, 11, 13};
int digitos[11][7] = {
  {1, 1, 1, 0, 1, 1, 1}, //0  
  {0, 0, 1, 0, 0, 0, 1}, //1  
  {1, 1, 0, 1, 0, 1, 1}, //2
  {0, 1, 1, 1, 0, 1, 1}, //3
  {0, 0, 1, 1, 1, 0, 1}, //4
  {0, 1, 1, 1, 1, 1, 0}, //5
  {1, 1, 1, 1, 1, 1, 0}, //6
  {0, 0, 1, 0, 0, 1, 1}, //7
  {1, 1, 1, 1, 1, 1, 1}, //8
  {0, 1, 1, 1, 1, 1, 1}, //9
  {0, 0, 0, 1, 0, 0, 0}, //-
};

bool loopPaused = false;
bool lastButtonState = 1;
    
void setup()
{
  Wire.begin();
  //radio.setFrequency(91.1);
  radio.init();
  radio.setMono(false);
  
  // Opcional: ajustar o volume
    
  
    pinMode(BOTAO, INPUT_PULLUP);
    pinMode(ANALOG_POTEN, INPUT);
    pinMode(EXIBER_1, OUTPUT);
    pinMode(EXIBER_2, OUTPUT);
    pinMode(SINC_VERDE, OUTPUT);
    pinMode(SINC_AZUL, OUTPUT);
    pinMode(SINC_VERMELHO, OUTPUT);
    Serial.begin(9600);
    for(int i=0; i <7; i++) {
      pinMode(disp_pinos[i], OUTPUT);
    }
 
  
}

void digito(int d){
  for(int i=0; i <7; i++) {
      digitalWrite(disp_pinos[i], digitos[d] [i]);
    }
}

void loop()
{
  bool buttonState = digitalRead(BOTAO);
 Serial.println(digitalRead(BOTAO));
  if (digitalRead(BOTAO) == 1 && lastButtonState == 0) {
    delay(50);
    Serial.println("Pressionou");
   
      loopPaused = !loopPaused;
      Serial.println(loopPaused ? "Loop pausado" : "Loop retomado");
    
  }

  lastButtonState = buttonState;

  
  
   delay(300);

   
  
  if (loopPaused == false) {
 
 int valor_potenciometro = map(analogRead(ANALOG_POTEN), 0, 1023, 0, 101);
 int unidade_estacao = (valor_potenciometro / 10) % 10 ;

 
 radio.setMute(false);
  
  if(valor_potenciometro != 100 && valor_potenciometro != 0 && trava_sessao == true ){
    trava_sessao = false;
    Serial.println("==========================================");
    Serial.print("DESBLOQUEADO PELA ALTERNANCIA: ");
    Serial.println(level_radio);
  }

  switch(level_radio){
      case 80:
        analogWrite(SINC_VERDE, 255);
        analogWrite(SINC_AZUL, 10);
        analogWrite(SINC_VERMELHO, 0);
      break;
      case 90:
        analogWrite(SINC_VERDE, 0);
        analogWrite(SINC_AZUL, 30);
        analogWrite(SINC_VERMELHO, 0);
      break;
      case 100:
        analogWrite(SINC_VERDE, 0);
        analogWrite(SINC_AZUL, 20);
        analogWrite(SINC_VERMELHO, 255);
      break;
    }
  
  if(trava_sessao == false){
    
  if(valor_potenciometro == 100 ){
    switch(level_radio){
      case 80:
        level_radio = 90;
        

          break;
      case 90: 
        level_radio = 100;
        
          break;
    }
    trava_sessao = true;
    digito(10);
    digitalWrite(EXIBER_1, LOW);
    digitalWrite(EXIBER_2, LOW);
    Serial.println("==========================================");
    Serial.println("BLOQUEADO ATÉ ALTERNANCIA");
    return; 
    
  } else if(valor_potenciometro == 0){
    switch(level_radio){
      case 100:
        level_radio = 90;
        
          break;
      case 90: 
        level_radio = 80;
        
        
          break;
    }
    trava_sessao = true;
    digito(10);
    digitalWrite(EXIBER_1, LOW);
    digitalWrite(EXIBER_2, LOW);
    Serial.println("==========================================");
    Serial.println("BLOQUEADO ATÉ ALTERNANCIA");
    return;
  } 
  
  
  //SINALIZAÇÃO DEZENA
    /*switch(level_radio){
      case 80:
        analogWrite(SINC_VERDE, 255);
        analogWrite(SINC_AZUL, 10);
        analogWrite(SINC_VERMELHO, 0);
      break;
      case 90:
        analogWrite(SINC_VERDE, 0);
        analogWrite(SINC_AZUL, 30);
        analogWrite(SINC_VERMELHO, 0);
      break;
      case 100:
        analogWrite(SINC_VERDE, 0);
        analogWrite(SINC_AZUL, 20);
        analogWrite(SINC_VERMELHO, 255);
      break;
    }*/
  
  
  
 Serial.print("VALOR POTEN: ");
  Serial.print(valor_potenciometro);
 Serial.print(" | INTEIRO: ");
  Serial.print(unidade_estacao);                     
 Serial.print(" | RESTO: ");
  Serial.print((valor_potenciometro-(unidade_estacao*10)));
  
 digito(unidade_estacao);


// DEFINA A ESTAÇÃO
  double FREQUENCY = (level_radio*100)+(valor_potenciometro*10);
  radio.setBandFrequency(FIX_BAND, FREQUENCY);
 
  Serial.print(" | ESTACAO: ");
  Serial.println(FREQUENCY);
  
  
  //.X da estação
  if((valor_potenciometro-(unidade_estacao*10)) < 5){
    digitalWrite(EXIBER_2, LOW);
    digitalWrite(EXIBER_1, HIGH);
    
  }else{
    digitalWrite(EXIBER_1, LOW);
    digitalWrite(EXIBER_2, HIGH);
    
  }
 delay(1000);
 
  //SALTO DE DEZENA 80-90-100
  
  
  }
  }else{
    radio.setMute(true);
    digito(10);
    digitalWrite(EXIBER_1, LOW);
    digitalWrite(EXIBER_2, LOW);
    analogWrite(SINC_VERDE, 0);
        analogWrite(SINC_AZUL, 0);
        analogWrite(SINC_VERMELHO, 0);
    }
}
