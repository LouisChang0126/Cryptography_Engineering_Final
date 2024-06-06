#include <Crypto.h>
#include <CryptoLW.h>
#include <Ascon128.h>

Ascon128 ascon;

const uint8_t key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                          0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }; // 128-bit key
const uint8_t iv[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                         0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }; // 128-bit IV

uint8_t tag[16];

#define STRING_LENGTH 256
void generateRandomBytes(byte* buffer, size_t length) {
  for (size_t i = 0; i < length; i++) {
    buffer[i] = random(0, 256);
  }
}

void setup() {
  Serial.begin(9600);
  
  for(int t=0;t<1;t++)
  {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const size_t charsetSize = sizeof(charset) - 1;

    char randomString[STRING_LENGTH+1];
    for (size_t i = 0; i < STRING_LENGTH; i++) {
        randomString[i] = charset[random(charsetSize)];
    }
    randomString[STRING_LENGTH] = '\0';

    String input = String(randomString);
    size_t len = input.length();
    uint8_t plaintext[len + 1];
    uint8_t ciphertext[len + 1];
    uint8_t decryptedtext[len + 1];
    input.getBytes(plaintext, len + 1);
    
    unsigned long start = micros();
    ascon.setKey(key, sizeof(key));
    ascon.setIV(iv, sizeof(iv));
    
    ascon.encrypt(ciphertext, plaintext, len);
    
    ascon.computeTag(tag, sizeof(tag));
    
    Serial.print("Ciphertext: ");
    for (size_t i = 0; i < len; i++) {
        Serial.print(ciphertext[i], BIN);
    }
    Serial.println();
    
    Serial.print("Tag: ");
    for (size_t i = 0; i < sizeof(tag); i++) {
        Serial.print(tag[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    Serial.print(" Encryption took "); Serial.print(micros()-start); Serial.println(" micros");
    
    ascon.clear();
    ascon.setKey(key, sizeof(key));
    ascon.setIV(iv, sizeof(iv));
    
    ascon.decrypt(decryptedtext, ciphertext, len);
    
    bool tagValid = ascon.checkTag(tag, sizeof(tag));
    
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
  Serial.println("Done");
}

void loop() {
  
}
