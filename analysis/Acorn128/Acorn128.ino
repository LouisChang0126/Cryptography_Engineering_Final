#include <Crypto.h>
#include <CryptoLW.h>
#include <Acorn128.h>

Acorn128 acorn;

const uint8_t key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                          0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }; // 128-bit key
const uint8_t iv[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                         0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }; // 128-bit IV

uint8_t tag[16];

void setup() {
  Serial.begin(9600);
  
  while (!Serial) { ; }

  Serial.println("Enter plaintext:");
  while (Serial.available() == 0) { ; }
  String input = Serial.readString();
  Serial.println(input);
  input.trim();
  size_t len = input.length();
  if (len > 0) {
    unsigned long start = micros();
    uint8_t plaintext[len + 1];
    uint8_t ciphertext[len + 1];
    uint8_t decryptedtext[len + 1];
    input.getBytes(plaintext, len + 1);
    
    acorn.setKey(key, sizeof(key));
    acorn.setIV(iv, sizeof(iv));
    
    acorn.encrypt(ciphertext, plaintext, len);
    
    acorn.computeTag(tag, sizeof(tag));
    
    Serial.print("Ciphertext: ");
    for (size_t i = 0; i < len; i++) {
        Serial.print(ciphertext[i], BIN);
    }
    Serial.println();
    /*
    Serial.print("Tag: ");
    for (size_t i = 0; i < sizeof(tag); i++) {
        Serial.print(tag[i], HEX);
        Serial.print(" ");
    }
    Serial.println();*/
    Serial.print(" Encryption took "); Serial.print(micros()-start); Serial.println(" micros");
    start = micros();
    
    acorn.clear();
    acorn.setKey(key, sizeof(key));
    acorn.setIV(iv, sizeof(iv));
    
    acorn.decrypt(decryptedtext, ciphertext, len);
    
    bool tagValid = acorn.checkTag(tag, sizeof(tag));
    
    /*if (tagValid) {
        Serial.print("Decrypted text: ");
        for (size_t i = 0; i < len; i++) {
            Serial.print((char)decryptedtext[i]);
        }
        Serial.println();
    } else {
        Serial.println("Tag verification failed!");
    }*/
    Serial.print(" Decryption took "); Serial.print(micros()-start); Serial.println(" micros");
  }
}

void loop() {
  
}
