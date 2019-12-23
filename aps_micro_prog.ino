#include <Servo.h>

#define SERVO 13 // Porta Digital 6 PWM


//Controle do servo motor
Servo servoMotor; // Variável Servo
int posServo;
boolean statusServo = true; // false = fechado | true = aberto
boolean ledManual = false;

byte for_sist_temp[10][3] = { { 0,0,0 },  // = Digito 0
                           { 0,0,1 },  // = Digito 1
                           { 0,1,0 },  // = Digito 2
                           { 0,1,1 },  // = Digito 3
                           { 1,0,0 },  // = Digito 4
                           { 1,0,1 },  // = Digito 5
                           { 1,1,0 },  // = Digito 6
                           { 1,1,1 },  // = Digito 7
                           { 0,0,0 },  // = Digito 0
                           { 1,1,1 },  // = Digito 7
                                 };

byte seven_seg_digits[10][7] = { { 1,1,1,0,1,1,1 },  // = Digito 0
                                 { 1,0,0,0,0,0,1 },  // = Digito 1
                                 { 0,1,1,1,0,1,1 },  // = Digito 2
                                 { 1,1,0,1,0,1,1 },  // = Digito 3
                                 { 1,0,0,1,1,0,1 },  // = Digito 4
                                 { 1,1,0,1,1,1,0 },  // = Digito 5
                                 { 1,1,1,1,1,1,0 },  // = Digito 6
                                 { 1,0,0,0,0,1,1 },  // = Digito 7
                                 { 1,1,1,1,1,1,1 },  // = Digito 8
                                 { 1,1,0,1,1,1,1 },  // = Digito 9
                                 };
String comando = "MENU \n Digite 0 para sair \n Digite 1 para Sistema temporizado \n Digite 2 para Sistema manual para leds \n Digite 3 para abrir/fechar porta";

void setup() 
{  
  pinMode(2, OUTPUT); //Pino 2 do Arduino ligado ao segmento A  
  pinMode(3, OUTPUT); //Pino 3 do Arduino ligado ao segmento B
  pinMode(4, OUTPUT); //Pino 4 do Arduino ligado ao segmento C
  pinMode(5, OUTPUT); //Pino 5 do Arduino ligado ao segmento D
  pinMode(6, OUTPUT); //Pino 6 do Arduino ligado ao segmento E
  pinMode(7, OUTPUT); //Pino 7 do Arduino ligado ao segmento F
  pinMode(8, OUTPUT); //Pino 8 do Arduino ligado ao segmento G
  pinMode(9, OUTPUT); //Vai ser o pino manual
  pinMode(10, OUTPUT); //led 1
  pinMode(11, OUTPUT); //led 2
  pinMode(12, OUTPUT); //led 3
  pinMode(13, OUTPUT); //Servo motor
  
  Serial.begin(9600);
  Serial.println(comando);

  servoMotor.attach(SERVO);
  servoMotor.write(0);  

}

void sevenSegWrite(byte digit){
  byte pin = 2;
  byte pin_led = 10;

  //Percorre o array ligando os segmentos correspondentes ao digito
  for (byte segCount = 0; segCount < 10; ++segCount)  
  { 
    digitalWrite(pin, seven_seg_digits[digit][segCount]);
    digitalWrite(pin_led, seven_seg_digits[digit][segCount]);
    ++pin;
    ++pin_led;
  }
} 


void displayseven(){
  //Contador de 0 a 15, ligando os segmentos correspondentes
  //0 a 9 = liga os segmentos correspondentes aos numeros
  //10 a 15 = Forma as letras A,B,C,D,E,F
  int limit = 15;
  for (byte count = 0; count < limit; count++)
  {
     delay(500);
     sevenSegWrite(count);
  }
  delay(4000);
}


void servo(){

  if (statusServo == true){
    for(posServo = 0; posServo < 120; posServo++)
    {
      servoMotor.write(posServo);
       delay(10);
    }
  }else{
    for(posServo = 120; posServo >= 0; posServo--)
    {
      servoMotor.write(posServo);
      delay(10);
    }
  }
}

int status = 999;
void loop() 
{
  if (status == 999){
    
    char c = Serial.read(); //VARIÁVEL RESPONSÁVEL POR RECEBER O CARACTER DIGITADO NA ENTRADA DE DADOS DO MONITOR SERIAL
  
    switch(c){
      case '0':
        Serial.println("Saindo..."); 
         status = 0;
         break;
      case '1':
        Serial.println("O modo automatico das LEDS foi ativado"); 
        displayseven();
      case '2':
        if (!ledManual){
          Serial.println("O modo manual das LEDS foi ativado"); 
          digitalWrite(9, HIGH);
        }else{
          Serial.println("O modo manual das LEDS foi desativado"); 
         digitalWrite(9, LOW);
        }
        ledManual = !ledManual;
        break;
      case '3':
        if (statusServo){
         Serial.println("O portão está sendo fechada"); 
        }else{
          Serial.println("O portão está sendo aberta");
        }
        servo();
       statusServo = !statusServo;
        break;
    }
  }
}
