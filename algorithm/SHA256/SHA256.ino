#include <Crypto.h>
#include <SHA256.h>

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
  Serial.print("SHA256 hash: ");
  for (byte i=0; i < SHA256::HASH_SIZE; i++)
  {
      if (hash[i]<0x10) { Serial.print('0'); }
      Serial.print(hash[i], HEX);
  }
  Serial.println();

  // Decryption
  Serial.println("Please enter your target SHA256 hash value:");
  while (targetHash.length() == 0) {
    if (Serial.available() > 0) {
      targetHash = Serial.readString();
      targetHash.trim();
    }
  }

  Serial.print("Target SHA256 hash: ");
  Serial.println(targetHash);

  // Brute force
  char current[maxLen + 1];
  current[maxLen] = '\0';

  Serial.println("Starting brute force...");
  unsigned long startTime = millis(); // Start timing
  for (int length = minLen; length <= maxLen; length++) {
    if (bruteForce(current, length)) {
      Serial.print("Found match: ");
      Serial.println(current);
      break;
    }
  }
  unsigned long endTime = millis(); // End timing
  Serial.print("Time elapsed: ");
  Serial.print(endTime - startTime);
  Serial.println(" ms");
}

void loop() {
  // Empty loop function
}

bool bruteForce(char *current, int length) {
  int indices[length];
  for (int i = 0; i < length; i++) {
    indices[i] = 0;
  }

  while (true) {
    for (int i = 0; i < length; i++) {
      current[i] = charset[indices[i]];
    }

    String generatedHash = calculateSHA256(current);
    if (generatedHash.equalsIgnoreCase(targetHash)) {
      return true;
    }

    // Update indices
    int pos = length - 1;
    while (pos >= 0) {
      indices[pos]++;
      if (indices[pos] < sizeof(charset) - 1) {
        break;
      }
      indices[pos] = 0;
      pos--;
    }

    if (pos < 0) {
      break;  // All possibilities have been tested
    }
  }

  return false;
}

// Function to calculate SHA256 hash of a given input
String calculateSHA256(const char *input) {
  SHA256 hasher;
  hasher.reset();
  hasher.update(input, strlen(input));

  byte hash[SHA256::HASH_SIZE];
  hasher.finalize(hash, SHA256::HASH_SIZE);

  String hashString;
  for (byte i = 0; i < SHA256::HASH_SIZE; i++) {
    if (hash[i] < 0x10) {
      hashString += '0';  // Ensure each byte is represented by two hex characters
    }
    hashString += String(hash[i], HEX);
  }
  return hashString;
}
