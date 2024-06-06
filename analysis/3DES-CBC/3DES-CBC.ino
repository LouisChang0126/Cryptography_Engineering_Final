#include <DES.h>
#include "./printf.h"

DES des;
unsigned long ms;

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
  delay(100);
  printf_begin();  
  delay(100);  

  //printf("\n============================================\n");
  //printf(" Triple DES sample for Arduino/Raspberry pi\n");
  //printf("============================================\n");

  for(int t=0;t<10;t++)
  {
      des.init("012345677654321001234567\0",(unsigned long long int)0);
      ms = micros();
      des.iv_inc();
      byte plaintext[] = "Abstract We study the security of popular password managers and their policies on automatically filling in Web passwords. We examine browser built-in password managers, mobile password managers, and 3rd party managers. We observe significant differences in";
      des.calc_size_n_pad(sizeof(plaintext));
      byte plaintext_p[des.get_size()];
      des.padPlaintext(plaintext,plaintext_p);
      byte cyphertext[des.get_size()];
      des.tdesCbcEncipher(plaintext_p,cyphertext);
      //printf("Ciphertext in binary: ");
      //printBinary(cyphertext, des.get_size());
      
      //printf(" Encryption took %lu micros\n",(micros()  - ms));
      ms = micros();
      des.calc_size_n_pad(sizeof(cyphertext));
      des.tdesCbcDecipher(cyphertext,plaintext_p);
      //printf(" Decryption took %lu micros\n\n",(micros() - ms));
  }
  Serial.println("Done");
}

void loop() {

}
