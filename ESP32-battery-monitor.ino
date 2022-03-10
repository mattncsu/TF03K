/*
  Multiple Serial test

  Receives from the main serial port, sends to the others.
  Receives from serial port 1, sends to the main serial (Serial 0).

  This example works only with boards with more than one serial like Arduino Mega, Due, Zero etc.

  The circuit:
  - any serial device attached to Serial port 1
  - Serial Monitor open on Serial port 0

  created 30 Dec 2008
  modified 20 May 2012
  by Tom Igoe & Jed Roach
  modified 27 Nov 2015
  by Arturo Guadalupi

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/MultiSerialMega
*/


//void setup() {
//  // initialize both serial ports:
//  Serial.begin(115200);
//  Serial2.begin(9600);
//
//}
//
//void loop() {
//  // read from port 1, send to port 0:
//  if (Serial2.available()==17) {
//    while (Serial2.available()){
//    byte inByte = Serial2.read();
//    Serial.print(inByte, HEX);
//    Serial.print(" ");
//    }
//  Serial.println();
//  }
//}



// how much serial data we expect before a newline
const unsigned int MAX_INPUT = 16;

void setup ()
  {
  Serial.begin (115200);
  Serial2.begin(9600);
  } // end of setup


  

// here to process incoming serial data after a terminator received
void process_data (const byte * data)
  {
  // for now just display it
  // (but you could compare it to some value, convert to an integer, etc.)
  for (int i = 0; i <= MAX_INPUT; i++){
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
//  if(data[1]==0xA5){
//    Serial.print("-");
//  }else{
//    Serial.println("Data out of sync! Resetting...");
//    ESP.restart();
//  }

  Serial.print(" Capacity:");
  Serial.print(data[2], DEC);
  Serial.print("% Volts:");

  union stuff2
  {
    int number;
    byte bytes[2];
  };
  stuff2 volts;
  volts.bytes[1]=data[3];
  volts.bytes[0]=data[4];
  //unsigned int volts = data[3]*256+data[4];
  Serial.print(volts.number);
  
  union stuff4
  {
    long number;
    byte bytes[4];
  };
  stuff4 capmah;
  capmah.bytes[3]=data[5];
  capmah.bytes[2]=data[6];
  capmah.bytes[1]=data[7];
  capmah.bytes[0]=data[8];
  Serial.print(" Cap (mAh):");
  Serial.print(capmah.number);

  stuff4 current;
  current.bytes[3]=data[9];
  current.bytes[2]=data[10];
  current.bytes[1]=data[11];
  current.bytes[0]=data[12];
  Serial.print(" Current:");
  Serial.print(current.number);

  union stuff3
  {
    unsigned long number;
    byte bytes[3];
  };

  stuff3 remTime;
  remTime.bytes[2]=data[13];
  remTime.bytes[1]=data[14];
  remTime.bytes[0]=data[15];
  Serial.print(" Remaining:");
  Serial.print(remTime.number);
  
  Serial.println();
  }  // end of process_data
  
void processIncomingByte (const byte inByte)
  {
  static byte input_line [MAX_INPUT];
  static unsigned int input_pos = 0;
      if(inByte==0xA5){
        input_line[0]=0;
        input_pos=1;
      }
      // keep adding if not full ... allow for terminating null byte
      if (input_pos <= (MAX_INPUT))
        input_line [input_pos++] = inByte;
      else{
        input_line [input_pos] = 0;  // terminating null byte
        
        // terminator reached! process input_line here ...
        process_data (input_line);
        
        // reset buffer for next time
        input_pos = 0;  
      }
  } // end of processIncomingByte  

void loop()
  {
  // if serial data available, process it
  while (Serial2.available () > 0)
    processIncomingByte (Serial2.read ());
    
  // do other stuff here like testing digital input (button presses) ...

  }  // end of loop
