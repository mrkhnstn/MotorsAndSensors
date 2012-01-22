
int ledPin = 13;   // select the pin for the LED
int val = 12;       // variable to store the data from the serial port
byte sendVal = 32;

const int bufferSize = 2; // = 2 * numServos + 2 (two bytes per number, one extra number for initial command)
byte readBuffer[bufferSize];
int readIndex = 0;

void setup() {
  pinMode(ledPin,OUTPUT);   // declare the LED's pin as output
  Serial.begin(115200);        // connect to the serial port
}

void loop () {
  // read the serial port
  while (Serial.available() > 0) {   
    readBuffer[readIndex] = Serial.read();
    readIndex++;
    if(readIndex == bufferSize){ // read done
      readIndex = 0; // reset readIndex for next read operation
      int i = 0;
      word temp = word(readBuffer[i],readBuffer[i+1]); // first number is command number
      Serial.write(highByte(temp));
      Serial.write(lowByte(temp));
    }
  }
  
  // if the input is '-1' then there is no data
  // at the input, otherwise check out if it is 'a'
  // if it is, send back a message "ABC"
  // and turn on and off the LED on pin 13
  // 
  // note, it takes about 5-10 seconds for the tx/rx of arduino to get
  // settled.  wait briefly after uploading, then open the serial monitor
  // when you type in a (and press send or return) you should see ABC returned
  // in the console below. check this first, and then try the openframeworks example
  //
  // in OF, you need to specify clearly which serial port you are communicating on. 
  // take a look above at "tools > serial port"
  // ie, "COM7" or "dev/tty...."
  // and make sure that is put into your code when you use ofSerial

/*
  if (val != -1) {
    if (val == 'a') {
      Serial.print("ABC");        // send back a message simple as ABC
      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
    }
  }
*/
}
