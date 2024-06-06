#include <Crypto.h>
#include <CryptoLW.h>
#include <Speck.h>

Speck speck;

const uint8_t key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                          0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }; // 128-bit key

uint8_t blockSize;

#define STRING_LENGTH 256
String get_random_string()
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const size_t charsetSize = sizeof(charset) - 1;

    char randomString[STRING_LENGTH+1];
    for (size_t i = 0; i < STRING_LENGTH; i++) {
        randomString[i] = charset[random(charsetSize)];
    }
    randomString[STRING_LENGTH] = '\0';
    return String(randomString);
}

void setup() {
  Serial.begin(9600);

  speck.setKey(key, sizeof(key));
  blockSize = speck.blockSize();

  for(int t=0;t<10;t++)
  {
    String input = get_random_string();
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
        Serial.print(ciphertext[i], HEX);
        Serial.print(" ");
      }
      Serial.println();*/
      //Serial.print(" Encryption took "); Serial.print(micros()-start); Serial.println(" micros"); start = micros();
      
      // Decrypt the ciphertext
      for (size_t i = 0; i < len; i += blockSize) {
        speck.decryptBlock(decryptedtext + i, ciphertext + i);
      }

      /*Serial.print("Decrypted text: ");
      for (size_t i = 0; i < len; i++) {
        Serial.print((char)decryptedtext[i]);
      }
      Serial.println();*/
      //Serial.print(" Decryption took "); Serial.print(micros()-start); Serial.println(" micros"); start = micros();
    }
  }
  Serial.println("Done");
}

void loop() {

}
