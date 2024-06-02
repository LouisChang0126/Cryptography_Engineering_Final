#include <SimpleHOTP.h>

const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const int minLen = 4;
const int maxLen = 12;

String targetHash = "";

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }

  // Encryption
  Serial.println("Please enter your message (only 4 to 12 digits and letters are allowed):");
  while (Serial.available() == 0) { ; }
  String msg = Serial.readString();
  msg.trim();

  if (msg.length() < 4 || msg.length() > 12) {
    Serial.println("Error: Message length must be between 4 and 12 characters.");
    return;
  }

  uint32_t hash[5] = {};
  int ml = msg.length();
  char msgArray[ml + 1];
  msg.toCharArray(msgArray, ml + 1);
  ml *= 8;

  // Generate the HMAC
  SimpleSHA1::generateSHA((uint8_t *)msgArray, ml, hash);

  // Print out the HMAC
  Serial.print("SHA1 hash: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(hash[i], HEX);
  }
  Serial.println();

  // Decryption
  Serial.println("Please enter your target SHA1 hash value:");
  while (targetHash.length() == 0) {
    targetHash = Serial.readString();
    targetHash.trim();
  }

  Serial.print("Target SHA1 hash: ");
  Serial.println(targetHash);

  Serial.println("Starting brute force...");
  unsigned long startTime = millis(); // Start timing
  bruteForce();
  unsigned long endTime = millis(); // End timing
  Serial.print("Time elapsed: ");
  Serial.print(endTime - startTime);
  Serial.println(" ms");
}

void loop() {
  // Empty loop function
}

// Brute force function to find the original string
void bruteForce() {
  char candidate[maxLen + 1];
  candidate[maxLen] = '\0';

  for (int len = minLen; len <= maxLen; len++) {
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

      SimpleSHA1::generateSHA((uint8_t *)msgArray, ml, hash);

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
        return;
      }
    }
  }

  Serial.println("No match found.");
}
