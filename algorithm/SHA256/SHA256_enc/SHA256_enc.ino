#include <Crypto.h>
#include <SHA256.h>

void setup() {

  Serial.begin(9600);
  while (!Serial) { ; }
  Serial.println("Please enter your message(only 4 to 12 digits and letters are allowed):");
  while (Serial.available() == 0) { ; }
  String msg = Serial.readString();
  msg.trim();

  if (msg.length() < 4 || msg.length() > 12) {
    Serial.println("Error: Message length must be between 4 and 12 characters.");
    return;
  }

  SHA256 hasher;
  hasher.reset();
  int ml = msg.length();
  char msgArray[ml+1];
  msg.toCharArray(msgArray, ml+1);

  hasher.update(msgArray, ml);
  
  /* Compute the final hash */
  byte hash[SHA256::HASH_SIZE];
  hasher.finalize(hash, SHA256::HASH_SIZE);
  
  /* hash now contains our 32 byte hash */
  for (byte i=0; i < SHA256::HASH_SIZE; i++)
  {
      if (hash[i]<0x10) { Serial.print('0'); }
      Serial.print(hash[i], HEX);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
