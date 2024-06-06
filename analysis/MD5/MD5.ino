#include <MD5.h>
/*
This is en example of how to use my MD5 library. It provides two
easy-to-use methods, one for generating the MD5 hash, and the second
one to generate the hex encoding of the hash, which is frequently used.
*/
#define STRING_LENGTH 256

void generateRandomBytes(byte* buffer, size_t length) {
  for (size_t i = 0; i < length; i++) {
    buffer[i] = random(0, 256);
  }
}

String hexToBin(String hex){
  String bin = "";
  for(int i = 0; i < hex.length(); i++){
    switch(hex[i]){
      case '0': bin += "0000"; break;
      case '1': bin += "0001"; break;
      case '2': bin += "0010"; break;
      case '3': bin += "0011"; break;
      case '4': bin += "0100"; break;
      case '5': bin += "0101"; break;
      case '6': bin += "0110"; break;
      case '7': bin += "0111"; break;
      case '8': bin += "1000"; break;
      case '9': bin += "1001"; break;
      case 'a': bin += "1010"; break;
      case 'b': bin += "1011"; break;
      case 'c': bin += "1100"; break;
      case 'd': bin += "1101"; break;
      case 'e': bin += "1110"; break;
      case 'f': bin += "1111"; break;
    }
  }
  return bin;
}
void setup()
{
  //initialize serial
  Serial.begin(9600);
  //give it a second
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
      
      unsigned long start = micros();
      unsigned char* hash=MD5::make_hash((unsigned char*)hexString.c_str());
      //generate the digest (hex encoding) of our hash
      char *md5str = MD5::make_digest(hash, 16);
      free(hash);
      //Serial.print(" Encryption took "); Serial.print(micros()-start); Serial.println(" micros"); start = micros();
      //print it on our serial monitor
      //Serial.println(md5str);
      //Give the Memory back to the System if you run the md5 Hash generation in a loop
      free(md5str);
  }
  Serial.println("Done");
}

void loop()
{
}
