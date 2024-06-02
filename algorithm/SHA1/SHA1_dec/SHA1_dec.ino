#include <SimpleHOTP.h>

String targetHash = "";
const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
char candidate[13];

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }

  Serial.println("Please enter your target SHA1 hash value:");
  while (targetHash.length() == 0) {
    targetHash = Serial.readString();
    targetHash.trim();
  }

  Serial.print("Target SHA1 hash: ");
  Serial.println(targetHash);

  Serial.println("Starting brute force...");
  bruteForce();
  Serial.println("Done.");
}

void loop() {

}

void bruteForce() {
  for (int len = 4; len <= 12; len++) {
    for (int i = 0; i < pow(sizeof(charset) - 1, len); i++) {
      int num = i;
      for (int j = 0; j < len; j++) {
        candidate[j] = charset[num % (sizeof(charset) - 1)];
        num /= (sizeof(charset) - 1);
      }
      candidate[len] = '\0';

      String candidateString = String(candidate);
      uint32_t hash[5] = {};
      int ml = candidateString.length();
      char msgArray[ml + 1];
      candidateString.toCharArray(msgArray, ml + 1);
      ml *= 8;
      
      SimpleSHA1::generateSHA((uint8_t*)msgArray, ml, hash);

      String generatedHash = "";
      for (int i = 0; i < 5; i++) {
        if (hash[i] < 0x10000000) {
          generatedHash += '0';
        }
        generatedHash += String(hash[i], HEX);
      }

      if (generatedHash.equalsIgnoreCase(targetHash)) {
        Serial.print("Found match: ");
        Serial.println(candidateString);
        while (true) { ; }
      }
    }
  }
}
