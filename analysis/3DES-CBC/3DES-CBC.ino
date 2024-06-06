#include <DES.h>
#include "./printf.h"

DES des;
unsigned long ms;
#define STRING_LENGTH 256

void printBinary(byte *input, int length) {
  for (int i = 0; i < length; i++) {
    for (int j = 7; j >= 0; j--) {
      printf("%d", (input[i] >> j) & 1);
    }
  }
  printf("\n");
}

void setup() {
  Serial.begin(9600);
  //delay(100);
  //printf_begin();  
  //delay(100);  

  //printf("\n============================================\n");
  //printf(" Triple DES sample for Arduino/Raspberry pi\n");
  //printf("============================================\n");

  for(int t=0;t<100;t++)
  {
      const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
      const size_t charsetSize = sizeof(charset) - 1;

      char randomString[STRING_LENGTH+1];
      for (size_t i = 0; i < STRING_LENGTH; i++) {
          randomString[i] = charset[random(charsetSize)];
      }
      
      randomString[STRING_LENGTH] = '\0';

      des.init("012345677654321001234567\0",(unsigned long long int)0);
      unsigned long start = micros();
      des.iv_inc();
      byte* plaintext = (byte*)randomString;
      des.calc_size_n_pad(sizeof(plaintext));
      byte plaintext_p[des.get_size()];
      des.padPlaintext(plaintext,plaintext_p);
      byte cyphertext[des.get_size()];
      des.tdesCbcEncipher(plaintext_p,cyphertext);
      //printf("Ciphertext in binary: ");
      //printBinary(cyphertext, des.get_size());
      
      Serial.print(" Decryption took "); Serial.print(micros()-start); Serial.println(" micros"); start = micros();
      des.calc_size_n_pad(sizeof(cyphertext));
      des.tdesCbcDecipher(cyphertext,plaintext_p);
      Serial.print(" Decryption took "); Serial.print(micros()-start); Serial.println(" micros"); start = micros();
  }
  Serial.println("Done");
}

void loop() {

}
