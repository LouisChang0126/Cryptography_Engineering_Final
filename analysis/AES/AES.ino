#include <AESLib.h>

#define BLOCK_SIZE 16
#define KEY_LENGTH 32
#define STRING_LENGTH 256

uint8_t key[KEY_LENGTH] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                           0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                           0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
                           0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
AESLib aesLib;


// Function to generate a random byte array
void generateRandomBytes(byte* buffer, size_t length) {
  for (size_t i = 0; i < length; i++) {
    buffer[i] = random(0, 256);
  }
}

void setup() {
  Serial.begin(9600);
  for(int t=0;t<100;t++)
  {
      randomSeed(analogRead(0));
      byte randomBytes[STRING_LENGTH];
      generateRandomBytes(randomBytes, STRING_LENGTH);

      String hexString;
      for (size_t i = 0; i < STRING_LENGTH; i++) {
        if (randomBytes[i] < 16) {
          hexString += "0"; // Add leading zero for single digit hex numbers
        }
        hexString += String(randomBytes[i], HEX);
      }

      String input = hexString;
      size_t len = input.length();
      if (len > 0) {
        uint8_t plaintext[len + 1];
        uint8_t ciphertext[len + 1];
        uint8_t decryptedtext[len + 1];
        input.getBytes(plaintext, len + 1);
        
        unsigned long start = micros();
        uint8_t iv[BLOCK_SIZE];
        aesLib.gen_iv(iv);
        
        aesLib.encrypt(plaintext, len + 1, ciphertext, key, KEY_LENGTH, iv);
        
        /*Serial.print("Ciphertext: ");
        for (size_t i = 0; i < len+1 ; i++) {
          Serial.print(ciphertext[i], BIN);
        }
        Serial.println();*/
        //Serial.print(" Encryption took "); Serial.print(micros()-start); Serial.println(" micros"); start = micros();
        
        aesLib.decrypt(ciphertext, len + 1, decryptedtext, key, KEY_LENGTH, iv);
        
        /*Serial.print("Decrypted text: ");
        for (size_t i = 0; i < len+1 ; i++) {
          Serial.print((char)decryptedtext[i]);
        }*/
        //Serial.print(" Decryption took "); Serial.print(micros()-start); Serial.println(" micros"); start = micros();
    }
  }
  Serial.println("Done");
}

void loop() {

}
