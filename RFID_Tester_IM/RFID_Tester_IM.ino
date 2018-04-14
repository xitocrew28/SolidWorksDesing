#include <SPI.h>        //<libreria para usar la libreria SPI
#include <MFRC522.h>    //Libreria que incluye el RFID-RC522

/* 
 Conexiones (MFRC522 hacia el ATMEGA328PU
 MFRC522 pin SDA  hacia DIG 9
 MFRC522 pin SCK  hacia DIG 13
 MFRC522 pin MOSI hacia DIG 11
 MFRC522 pin MISO hacia DIG 12
 MFRC522 pin GND  a TIERRA
 MFRC522 pin RST  hacia DIG 10
 MFRC522 pin 3.3V A 3.3. V
*/

// Se definen los pines para el modulo RFID-RC522
#define SDA_PIN  9
#define RST_PIN  10

MFRC522 mfrc522(SDA_PIN, RST_PIN);

#define bien    5
#define mal     7
#define estado  6
#define mus     3

int timer = 50;

String sn = "Ix01170007"; // Aqui grabamos el serial del Validador
String ver = "V0.14";     // Aqui es la version del sketch
char i = '\0';

void setup() 
{
  // Inicializamos los pines de salida
  pinMode(bien, OUTPUT);
  pinMode(mal, OUTPUT);
  pinMode(mus, OUTPUT);
  pinMode(estado, OUTPUT);

  digitalWrite(bien, HIGH);
  digitalWrite(mal, HIGH);
  digitalWrite(estado, HIGH);

  digitalWrite(mus, LOW);
  
  Serial.begin(115200);     // Iniciamos la comunicacion serial
  
  SPI.begin();              // Iniciamos el SPI bus
  mfrc522.PCD_Init();       // Iniciamos el modulo MFRC522  
}

void loop() 
{
  if (Serial.available())
  {
    i = Serial.read();
    switch(i)
    {
      case 's': // Leer el SN del Validador
        Serial.println(sn);
        break;
      case 'v':
        Serial.println(ver);
        break;      
    }
  }
  
  // Aguardamos un momento para revisar si hay un TAG RFID
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    digitalWrite(bien, HIGH);
    digitalWrite(mal, HIGH);
    digitalWrite(estado, HIGH);
    return;
  }
  else
  {
    digitalWrite(bien, LOW);
    digitalWrite(mal, LOW);
    digitalWrite(estado, LOW);
  }
  // Selecciona una de las tarjetas
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
    
  // Muestra el ID de la tarjeta
  Serial.print("CSN del TAG");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  conteudo.toUpperCase();
  
    // Comprobamos si la tarjeta que se ha leido es la correcta
  if (conteudo.substring(1) == "3A E2 2E 98" )
  {
    for (int busy = 0 ; busy <= 15 ; busy++)
    {
      delay(timer);
    }
    digitalWrite(bien, HIGH);
    digitalWrite(mal, HIGH);
    digitalWrite(estado, HIGH);
    
    Serial.println("BEEPER TEST OK");
    Serial.println();

    digitalWrite(bien, LOW);
    beep_largo();
    for (int j = 0 ; j <= 30 ; j++)
    {
      delay(timer);
    }
    
    digitalWrite(bien, HIGH);

    digitalWrite(estado, LOW);
    beep_largo();
    for (int k = 0 ; k <= 30 ; k ++)
    {
      delay(timer);
    }
  
    digitalWrite(estado, HIGH);    
  }
  
   // Comprobamos si la tarjeta que se ha leido es la correcta
  if (conteudo.substring(1) == "63 50 1E 90" )
  {
    for (int busy = 0 ; busy <= 15 ; busy++)
    {
      delay(timer);
    }
    digitalWrite(bien, HIGH);
    digitalWrite(mal, HIGH);
    digitalWrite(estado, HIGH);
    
    Serial.println("BEEPER TEST NOK");
    Serial.println();
    digitalWrite(mal, LOW);    
    beep_corto();
    beep_corto();
    beep_corto();
    beep_corto();
    beep_corto();
    beep_corto();
    beep_corto();
    
    for ( int j = 0 ; j <= 30 ; j ++)
    {
      delay(timer);
    }
    digitalWrite(mal, HIGH);
  }        
    // Si leemos otro CSN solamente avisara que no es correcto
  if (  conteudo.substring(1) != "3A E2 2E 98" && 
        conteudo.substring(1) != "63 50 1E 90" )
  {
    for (int busy = 0 ; busy <= 15 ; busy++)
    {
      delay(timer);
    }
    digitalWrite(bien, HIGH);
    digitalWrite(mal, HIGH);
    digitalWrite(estado, HIGH);
    
    Serial.println("Tag No Valido");
    Serial.println();
   
    for ( int intermitente = 0 ; intermitente <= 6 ; intermitente++)
    {
      digitalWrite(mal, HIGH);
      beep_corto();
      delay(timer);
      digitalWrite(mal, LOW);
      delay(timer);
    }
  }    
}

void beep_corto() 
{
  digitalWrite(mus, HIGH);
  delay(50);
  digitalWrite(mus, LOW);
  delay(100);
}

void beep_largo()
{
  digitalWrite(mus, HIGH);
  delay(250);
  digitalWrite(mus, LOW);
  delay(250);
}
