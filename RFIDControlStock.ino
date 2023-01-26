#include <SPI.h> //libreria SPI
#include <MFRC522.h> //libreria para modulo lector rfid
#define RST_PIN 9  //resetea el lector rfid
#define SS_PIN_ENB 10 // pin de comunicacion sda del lector rfid
MFRC522 RFIDLector(SS_PIN_ENB, RST_PIN); //objeto del lector rfid
String BufferID = ""; //string que guarda codigo valido de tarjeta
String Pin = "aa10f081";
String Card = "632a853e"; 

int stockPin = 0;
int stockTag = 0;
int stockManage = 0; // 0 = sin accion, 1 = incrementa, 2 = disminuye Administracion de almacenamiento

void setup() {
  Serial.begin(9600); //Inicio de comunicacion serial
  SPI.begin();  //inicializacion de bus de comunicacion SPI
  RFIDLector.PCD_Init(); //inicializacion de objeto RFIDLector
  Serial.println("...Iniciando control de stock...");
  Serial.println("Escriba 'i' para aumentar el stock, 'd' para disminuir el stock, 'n' para no realizar accion:");
}

void loop() {
    //Condicional para saber si se ha detectado alguna antena rfid
    if(RFIDLector.PICC_IsNewCardPresent())
    {
      //Condicional para saber el id de la tarjeta
      if(RFIDLector.PICC_ReadCardSerial())
      {
        BufferID = ""; //almacena la id de la tarjeta
        Serial.print("Id Tag:");
        for(byte i = 0; i < RFIDLector.uid.size; i++)
        {
          Serial.print(RFIDLector.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(RFIDLector.uid.uidByte[i], HEX);
          BufferID = BufferID + String(RFIDLector.uid.uidByte[i], HEX);
        }
        Serial.print(",");        
        Serial.print(BufferID);
        Serial.print(",");
        if(stockManage == 0){ // Si stockManage es 1, incrementa la cantidad
            Serial.println();
            Serial.print("Stock Pin: ");
            Serial.print(stockPin);
            Serial.println();}
        if(stockManage == 0){ // Si stockManage es 1, incrementa la cantidad
            Serial.print("Stock Tag: ");
            Serial.print(stockTag);
            Serial.println();}                                    
        if(BufferID == Pin)
        {
          if(stockManage == 1){ // Si stockManage es 1, incrementa la cantidad
            stockPin++;
            Serial.print("Stock Pin: ");
            Serial.print(stockPin);
            Serial.println();
          }else if(stockManage == 2){ //Si stockManage es 2, reduce el cantidad
            if (stockPin > 0) {
              stockPin--;
              Serial.print("Stock Pin: ");
              Serial.print(stockPin);
              Serial.println();
            } else {
              Serial.println("No hay suficiente stock de Pin para disminuir");
            }
          }
        }
        else
        {
          if(stockManage == 1){ // cuando stockManage es 1, incrementa
            stockTag++;
            Serial.print("Stock Tag: ");
            Serial.print(stockTag);
            Serial.println();
          }else if(stockManage == 2){ //cuando stockManage es 2, disminuye
            if (stockTag > 0) {
              stockTag--;
              Serial.print("Stock Tag: ");
              Serial.print(stockTag);
              Serial.println();
            } else {
              Serial.println("No hay suficiente stock de Tag para disminuir");
            }
          }
        }
        RFIDLector.PICC_HaltA();
      }
    }

    if (Serial.available() > 0) {
      char command = Serial.read();
      if(command == 'i'){
        stockManage = 1;
      }else if(command == 'd'){
        stockManage = 2;
      }else if(command == 'n'){
        stockManage = 0;
      }
    }
}
