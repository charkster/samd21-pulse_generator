
/* Hex characters are read from the usb serial port and then driven on a gpio pin.
   PERIOD should be integer values of 2 and above. If the last value is high the
   gpio pin will drive high until a new pattern is given.
 */

byte          gpio_data_array [1000]; // 8 * 1000 = 8k bytes of sram
uint32_t      char_counter = 0;
volatile char inChar;
String        hexChar; 

#define PERIOD 2       // options are 2 and larger integers
#define PIN 7          // 7 is PB9 on XIAO


void setup() {
  SerialUSB.begin(115200);         // Data will be revieved on the USB serial port
  while(!SerialUSB);               // Wait for the SerialUSB port to be ready
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);
}

void loop() {
  while (SerialUSB.available() > 0)
  {
    inChar = SerialUSB.read();
    if (inChar != '\n' && ((inChar >= '0' && inChar <= '9') || (inChar >= 'a' && inChar <= 'f') || (inChar >= 'A' && inChar <= 'F'))) // read in hex characters
    {
      hexChar = inChar;
      gpio_data_array[char_counter++] = (byte)strtol(&hexChar[0],NULL,16); // this is inefficient as only a nibble is used
    }
    else if (char_counter != 0)// a end-of-line signals that we are ready to drive data
    {
      while (char_counter > 0)
      {
        char_counter--;
        for (int i = 0; i < 4; i++) {
          if (bitRead(gpio_data_array[char_counter],i) == 1)
          {
            digitalWrite(PIN, HIGH);
            __asm__ __volatile__ ("nop\n\t");
            #if PERIOD > 2
            delayMicroseconds(PERIOD-2);
            #endif
          }
          else
          {
            digitalWrite(PIN, LOW);
            __asm__ __volatile__ ("nop\n\t");
            #if PERIOD > 2
            delayMicroseconds(PERIOD-2);
            #else
            __asm__ __volatile__ ("nop\n\t");
            #endif
          }
        }
      }
    }
  }
}
