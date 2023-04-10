#include <Servo.h>
#include <HCSR04.h>

#define pneu1A 4
#define vel1 3
#define pneu1B 2
#define pneu2A 7
#define vel2 6
#define pneu2B 5
#define servo 8
#define p_trigger 9
#define p_echo 10

int vpneuA, vpneuB;
int angulo, giro;
float centimetros;

UltraSonicDistanceSensor distancia(p_trigger, p_echo);
Servo servoMotor;

void andar(void);
int definir_lado(void);
void virar_direita(void);
void parar(void);
void re(void);
void freiar(void);
void piscar_vermelho(void);
void piscar_verde(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(pneu1A, OUTPUT);
  pinMode(pneu1B, OUTPUT);
  pinMode(pneu2A, OUTPUT);
  pinMode(pneu2B, OUTPUT);
     
  digitalWrite(pneu1A, LOW);
  digitalWrite(pneu1B, LOW);

  digitalWrite(pneu2A, LOW);
  digitalWrite(pneu2B, LOW);

  vpneuA = 120;
  vpneuB = 100;
  giro = 500;
  angulo = 90;
  servoMotor.attach(servo);
  servoMotor.write(angulo);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  centimetros = distancia.measureDistanceCm();
  Serial.print("Distancia: ");
  Serial.println(centimetros);
  if(centimetros > 30){
    andar();
    Serial.println("Andando...");
  }else{
    freiar();
    delay(500);
    int lado = definir_lado();
    delay(500);
    if(lado == -1){
      re();
      delay(500);
      virar_direita();
    }else if(lado == 1){
      virar_direita();
      freiar();
    }else if(lado == 0){
      virar_esquerda();  
      parar();
    }else{
      andar();
    }
  }
  delay(750);
}//Fim do Loop

void andar(){
  analogWrite(vel1, vpneuA);
  analogWrite(vel2, vpneuB);

  digitalWrite(pneu1A, HIGH);
  digitalWrite(pneu1B, LOW);
  digitalWrite(pneu2A, HIGH);
  digitalWrite(pneu2B, LOW);
  }

void re(){
  analogWrite(vel1, vpneuA);
  analogWrite(vel2, vpneuB);

  digitalWrite(pneu1A, LOW);
  digitalWrite(pneu1B, HIGH);
  digitalWrite(pneu2A, LOW);
  digitalWrite(pneu2B, HIGH);
  }

void freiar(){
  analogWrite(vel1, 255);
  analogWrite(vel2, 255);

  digitalWrite(pneu1A, HIGH);
  digitalWrite(pneu1B, HIGH);
  digitalWrite(pneu2A, HIGH);
  digitalWrite(pneu2B, HIGH);
  }

void virar_direita(){
  analogWrite(vel1, vpneuA);
  analogWrite(vel2, vpneuB);

  digitalWrite(pneu1A, LOW);
  digitalWrite(pneu1B, HIGH);
  digitalWrite(pneu2A, HIGH);
  digitalWrite(pneu2B, LOW);
  delay(giro);
  }

void virar_esquerda(){
  analogWrite(vel1, vpneuA);
  analogWrite(vel2, vpneuB);

  digitalWrite(pneu1A, HIGH);
  digitalWrite(pneu1B, LOW);
  digitalWrite(pneu2A, LOW);
  digitalWrite(pneu2B, HIGH);
  delay(giro);
  }

void parar(){
  digitalWrite(pneu1A, LOW);
  digitalWrite(pneu1B, LOW);
  digitalWrite(pneu2A, LOW);
  digitalWrite(pneu2B, LOW);
  }

int definir_lado(){
  float direita = 0, esquerda = 0, frente = 0;
  //Olhando para esquerda
  int i;
  for(i = angulo;i>=0; i--){
    servoMotor.write(i);
    delay(15);
    }
  direita = distancia.measureDistanceCm();
  delay(500);
  for(int j = 0;j<=180; j++){
    servoMotor.write(j);
    delay(15);
    }
  esquerda = distancia.measureDistanceCm();
  delay(500);
  for(int k = 180;k>=90;k--){
    servoMotor.write(k);
    delay(15);
    }
  delay(500);
  frente = distancia.measureDistanceCm();
  if(frente > 30){
    return 2;
    }
  if(direita <= 30 && esquerda <= 30){
    return -1;
  }
  
  if(direita > esquerda){
    return 1;
  }else{
    return 0;  
  }
  
}
