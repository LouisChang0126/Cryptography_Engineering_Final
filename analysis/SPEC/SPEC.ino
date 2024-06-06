#include <Crypto.h>
#include <CryptoLW.h>
#include <Speck.h>

Speck speck;

const uint8_t key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                          0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }; // 128-bit key

uint8_t blockSize;
uint8_t ciphertext[16];
uint8_t decryptedtext[16];

void setup() {
  Serial.begin(9600);
  
  while (!Serial) {
    ; // Wait for Serial to connect
  }

  speck.setKey(key, sizeof(key));
  blockSize = speck.blockSize();
  
  Serial.println("Please enter 16-byte plaintext:");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readString(); // Read plaintext from Serial input
    input.trim(); // Remove leading and trailing whitespace
    size_t len = input.length();
    if (len == 16) {
      uint8_t plaintext[16];
      input.getBytes(plaintext, len + 1);

      // Encrypt the plaintext
      speck.encryptBlock(ciphertext, plaintext);
      
      Serial.print("Ciphertext: ");
      for (size_t i = 0; i < blockSize; i++) {
        Serial.print(ciphertext[i], HEX);
        Serial.print(" ");
      }
      Serial.println();

      // Decrypt the ciphertext
      speck.decryptBlock(decryptedtext, ciphertext);

      Serial.print("Decrypted text: ");
      for (size_t i = 0; i < blockSize; i++) {
        Serial.print((char)decryptedtext[i]);
      }
      Serial.println();
      
    } else {
      Serial.println("Please enter exactly 16 bytes of plaintext:");
    }
  }
}
