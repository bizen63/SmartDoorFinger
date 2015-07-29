/*************************************************** 
 Programa para control inteligente de una puerta con sistema de 
 apertura y cierre por servomotor y que utiliza display LCD para 
 visualizar los mensajes. La unica huella autorizada se corresponde
 con el ID 1
 El conexionado se deduce de los comentarios del codigo
 ****************************************************/
int verde = 6; // Conectar salida digital 6 a led verde
int rojo = 7; // Conectar salida digital 7 a led rojo
int abierto = 0;
int cerrado = 1;// Iniciamos el sistema con la puerta cerrada.
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h> // Carga la librería Servo.h que contiene métodos para trabajar con servos
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Conexiones de la pantalla LCD al adaptador
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Addr, En, Rw, Rs, d4, d5, d6, d7, backlighpin, polarity
int angulo = 179; //inicializa el angulo para puerta cerrada
Servo myservo;  // Crea un objeto servo para controlar el servomotor

int getFingerprintIDez();

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(2, 3);


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  Serial.begin(9600);
 lcd.begin(16,2); //Para arduino UNO A4 (SDA), A5 (SCL)
 myservo.attach(12); // asocia el servo al pin 12
 myservo.write(angulo);
  
  Serial.println("fingertest");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  Serial.println("Waiting for valid finger...");
  digitalWrite(verde, LOW);
  digitalWrite(rojo, LOW);
  //Mensajes a traves del LCD
  lcd.setCursor(0, 0);
        lcd.print("SMARTFINGER DOOR ");
        // Situamos el cursor en la primera casilla de la segunda fila
        lcd.setCursor(0, 1);
        lcd.print("ESPERE...");
 delay(3000);
 Serial.println("PUERTA CERRADA");
 LimpiaFila1();
 PuertaCerrada();
 

}

void loop()                     // run over and over again
{
  getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  if (finger.fingerID == 1){
  
 
  switch (abierto){
     case 0:
  Abre(); //Abre la puerta.
  finger.fingerID=0;  //Borra el ID.
  LimpiaFila1();
  PuertaAbierta();
  digitalWrite(verde, HIGH);
  delay(3000);
  digitalWrite(verde, LOW);
  break;
  case 1:
  Cierra(); //Cierra la puerta.
  finger.fingerID=0;  //Borra el ID.
  LimpiaFila1();
  PuertaCerrada();
  digitalWrite(verde, HIGH);
  delay(3000);
  digitalWrite(verde, LOW);
  break;
  }
  }
  else if (finger.fingerID != 1){
  Serial.println("  PERSONA NO AUTORIZADA ");
  LimpiaFila0();
  NoAcceso();
  digitalWrite(rojo, HIGH);
  delay(3000);
  digitalWrite(rojo, LOW);
  LimpiaFila0(); 
  lcd.setCursor(0, 0);
  lcd.print("SMARTFINGER DOOR ");  
  }
  return finger.fingerID; 
}

void LimpiaFila0()
{
 for(int i = 0; i < 16; i++){
  lcd.setCursor(i, 0);
  lcd.print(" ");
  }
}

void LimpiaFila1()
{
 for(int i = 0; i < 16; i++){
  lcd.setCursor(i, 1);
  lcd.print(" ");
  }
}

void LimpiaDisplay()
{
 for(int i = 0; i < 16; i++){
  lcd.setCursor(i, 0);
  lcd.print(" ");
        lcd.setCursor(i, 1);
  lcd.print(" ");
  }
}

void PuertaAbierta()
{
  lcd.setCursor(0, 1);
  lcd.print("PUERTA ABIERTA");
}

void PuertaCerrada()
{
  lcd.setCursor(0, 1);
  lcd.print("PUERTA CERRADA");
}
void NoAcceso()
{
  lcd.setCursor(0, 0);
  lcd.print("ACCESO DENEGADO");
}

void Espere()
{
  lcd.setCursor(0, 1);
  lcd.print("ESPERE POR FAVOR");
}

void Abre()
{
  angulo = 90;
  myservo.write(angulo);
  delay(15);   
  abierto = 1;
  cerrado = 0;
}
void Cierra()
{
  angulo = 179;
  myservo.write(angulo);
  delay(15);   
  abierto = 0;
  cerrado = 1;
}

