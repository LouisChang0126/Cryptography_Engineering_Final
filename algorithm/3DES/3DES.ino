#include <DES.h>
DES des;

String  input;

char buf[30];
void setup() {
  Serial.begin(9600);
  Serial.println("Hello!");

}


void printArray(byte output[])
{
  for (int i = 0; i < 8; i++)
  {
    if (output[i] < 0x10)
    {
      Serial.print("0");
    }
    Serial.print(output[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
void loop() {

  while (Serial.available() > 0) {

    String  input = Serial.readString(); // read the incoming data as string
    char buf[30];
/*
    input.toCharArray(buf, 9);
    Serial.println(buf);
*/
    input.toCharArray(buf, input.length() + 1);
    Serial.println(buf);

    
    byte out[8];
    byte in[] = {buf};
    Serial.println(buf);
    byte key[] = {
      0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e, // key A
      0x92, 0x2f, 0xb5, 0x10, 0xc7, 0x1f, 0x43, 0x6e, // key B
      0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e, // key C (in this case A)
    };

    Serial.println();
    Serial.println("====== Triple-DES test ======");

    //encrypt
    Serial.print("Encrypt...");
    unsigned long time = micros();
    des.tripleEncrypt(out, in, key);
    time = micros() - time;
    Serial.print("done. (");
    Serial.print(time);
    Serial.println(" micros)");
    printArray(out);

    //decrypt
    for (int i = 0; i < 8; i++)
    {
      in[i] = out[i];
    }
    Serial.print("Decrypt...");
    time = micros();
    des.tripleDecrypt(out, in, key);
    time = micros() - time;
    Serial.print("done. (");
    Serial.print(time);
    Serial.println(" micros)");
    printArray(out);
    delay(2000);
  }


}
