#include <SimpleHOTP.h>

void setup () {
  // this array will contain the final hmac
  Serial.begin(9600);
  while (!Serial) { ; }
  Serial.println("Please enter your message(only 4 to 12 digits and letters are allowed):");
  while (Serial.available() == 0) { ; }
  String msg = Serial.readString();
  
  uint32_t hash[5];
  int ml = msg.length();
  char msgArray[ml+1];
  msg.toCharArray(msgArray, ml+1);
  ml *= 8;
  
  // generate the hmac
  SimpleSHA1::generateSHA(msgArray, ml, hash);

  // print out the HMAC
  for (int i = 0; i < 5; i++) {
    Serial.print(hash[i], HEX);
  }
}


void loop () {
  
}
