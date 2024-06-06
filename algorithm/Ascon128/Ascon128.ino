#include <Ascon128.h>

Ascon128 ascon;

const size_t keySize = 16; // 128-bit key size
const size_t ivSize = 16;  // 128-bit IV size
const size_t tagSize = 16; // 128-bit tag size

uint8_t key[keySize] = {0}; // Initialize with zeros for simplicity
uint8_t iv[ivSize] = {0};   // Initialize with zeros for simplicity
uint8_t tag[tagSize] = {0};

void setup() {
    Serial.begin(9600);
    while (!Serial) { ; }

    // Generate a key and IV for this session
    randomSeed(analogRead(0));
    for (size_t i = 0; i < keySize; i++) {
        key[i] = random(256);
    }
    for (size_t i = 0; i < ivSize; i++) {
        iv[i] = random(256);
    }

    Serial.println("Enter plaintext:");
    // Wait for input
    while (Serial.available() == 0) { ; }
    String input = Serial.readString();
    size_t inputLength = input.length();

    // Allocate buffers
    uint8_t plaintext[inputLength];
    uint8_t ciphertext[inputLength];
    uint8_t decrypted[inputLength];

    // Copy input to plaintext buffer
    input.getBytes(plaintext, inputLength);

    // Set key and IV
    ascon.setKey(key, keySize);
    ascon.setIV(iv, ivSize);

    // Encrypt the plaintext
    ascon.encrypt(ciphertext, plaintext, inputLength);

    // Compute the authentication tag
    ascon.computeTag(tag, tagSize);

    // Print the encrypted data and tag
    Serial.println("Ciphertext: ");
    for (size_t i = 0; i < inputLength; i++) {
        Serial.print(ciphertext[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    Serial.println("Tag: ");
    for (size_t i = 0; i < tagSize; i++) {
        Serial.print(tag[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    // Decrypt the ciphertext
    ascon.setKey(key, keySize);
    ascon.setIV(iv, ivSize);
    ascon.decrypt(decrypted, ciphertext, inputLength);

    // Check the authentication tag
    if (ascon.checkTag(tag, tagSize)) {
        Serial.println("Decrypted text: ");
        Serial.write(decrypted, inputLength);
    } else {
        Serial.println("Tag verification failed!");
    }
}

void loop() {
  // put your main code here, to run repeatedly:
}
