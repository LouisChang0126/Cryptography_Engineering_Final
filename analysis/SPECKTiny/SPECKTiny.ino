#include <Crypto.h>
#include <CryptoLW.h>
#include <SpeckTiny.h>

SpeckTiny speck;

const uint8_t key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                          0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }; // 128-bit key

uint8_t blockSize;

void setup() {
  Serial.begin(9600);
  
  while (!Serial) {
    ; // Wait for Serial to connect
  }

  speck.setKey(key, sizeof(key));
  blockSize = speck.blockSize();
  
  Serial.println("Please enter plaintext:");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readString(); // Read plaintext from Serial input
    input.trim(); // Remove leading and trailing whitespace
    size_t len = input.length();
    if (len > 0) {
      uint8_t plaintext[len + 1];
      uint8_t ciphertext[len + 1];
      uint8_t decryptedtext[len + 1];
      input.getBytes(plaintext, len + 1);

      unsigned long start = micros();
      // Encrypt the plaintext
      for (size_t i = 0; i < len; i += blockSize) {
        speck.encryptBlock(ciphertext + i, plaintext + i);
      }
      
      /*Serial.print("Ciphertext: ");
      for (size_t i = 0; i < len; i++) {
        Serial.print(ciphertext[i], BIN);
      }
      Serial.println();*/
      Serial.print(" Encryption took "); Serial.print(micros()-start); Serial.println(" micros"); start = micros();
      
      // Decrypt the ciphertext
      for (size_t i = 0; i < len; i += blockSize) {
        speck.decryptBlock(decryptedtext + i, ciphertext + i);
      }

      /*Serial.print("Decrypted text: ");
      for (size_t i = 0; i < len; i++) {
        Serial.print((char)decryptedtext[i]);
      }
      Serial.println();*/
      Serial.print(" Decryption took "); Serial.print(micros()-start); Serial.println(" micros"); start = micros();
    }
  }
}
