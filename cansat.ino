/* This program is intended for use as a test program for the CanSat course at NAROM, Andøya Rocket Range 2013.

** Beerware, this code is free to use, but if you use it and happen to meet me, you can buy me a beer
**   by Thomas Gansmoe, Narom AS (www.narom.no / www.rocketrange.no)  */
  
#define BITRATE 9600         //Bitrate for the serial interface (Radio and logger)
#define Vcc 5.0              //Supply voltage


// **** [ Sensor calibration ] *************************************************************

// NTC-sensor variables
#define NTC_Grad -20         //The gradiant value for the NTC sensor
#define NTC_Off 75           //The offset value for the NTC sensor

// Accelerometer-sensor settings
#define AccSensMode 0        //Accelerometer sensitivity mode. 0 = +- 3g, 1 = +- 9g (Not implemented in the code yet)
#define Acc3 0.440          //Acc sensitivity mode 0 = 440 mV/g (from datasheet)
#define Acc9 0.118           //Acc sensitivity mode 1 = 118 mV/g (from datasheet)
#define Acc0 1.65            //Acc 0g voltage (from datasheet)
#define AccxOff 0.0          //Offset values - Calibrated by finding voltage at zero G.
#define AccyOff 0.0
#define AcczOff 0.0

// LM335 Temp sensor
#define TmpOffset 0.0        //Volts at zero degree Celsius
#define TmpSens 100          //Sensitivity. Deg.Celsius /volt

/*******************************************************************************/

int LOOPTIME = 500;         //Interval time in millisecounds
//Configure sensor output format
int OutFormatTmp = 0;       //Sets the output format. (0 = Volt, 1 = Deg.C)
int OutFormatNTC = 0;       //Sets the output format. (0 = Volt, 1 = Deg.C)
int OutFormatPressure = 0;  //Sets the output format. (0 = Volt, 1 = hPa)
int OutFormatAcc = 0;       //Sets the output format. (0 = Volt, 1 = G)

unsigned long time;
long int counter=0, number=0;
float G = 9.81; 

void setup() {
  Serial.begin(BITRATE);
  
}

float SecFromStart(){
  time = millis();
  int sec = time/1000;
  int des = (time%1000)/10;
  float result = (float)sec+((float)des)/100;
  return result;
}

void printdata(){
  Serial.print("Counter: ");
  Serial.print(counter);
  Serial.print(" | Time[s]: ");
  Serial.print(SecFromStart());
    
  PrintTmp(OutFormatTmp);
  PrintNTC(OutFormatNTC);
  PrintPressure(OutFormatPressure);
  PrintAcc(OutFormatAcc);
  
  Serial.println();
}

float BitToVolt(int n){    //Function to convert raw ADC-data (0-255) to volt
  int raw = analogRead(n);
  float volt = (float)raw*5.000/1023;
  return volt;
}

// The LM335 temp sensor 
void PrintTmp(int Format){
  analogReference(INTERNAL);
  analogRead(5);
  delay(10);
  //float Tmp, volt = BitToVolt(5);  //Reads Analog5
  float Tmp, volt = analogRead(5)*1.1/1023;
  analogReference(DEFAULT);
  analogRead(5);
  delay(10);
  Serial.print(" | Temp: ");
  switch (Format){
    case 0:
      Serial.print(volt,3);
      Serial.print(" V");
      break;
    case 1:
      Serial.print((volt - TmpOffset)*TmpSens);
      Serial.print(" Deg.C");
      break;
    }    
  }

// The NTC sensor 
void PrintNTC(int OutFormatNTC){
  float volt = BitToVolt(0);
  Serial.print(" | NTC: ");
  switch (OutFormatNTC){
    case 0:
      Serial.print(volt);
      Serial.print(" V");
      break;
    case 1:
      Serial.print(NTC_Off + NTC_Grad * volt);
      Serial.print(" Deg.C");
      break;
    }    
  }
  
// The Pressure sensor (MPX4115)
void PrintPressure(int OutFormatPressure){
  float volt = BitToVolt(1), hPa;
  Serial.print(" | Pressure: ");
  hPa = 10*(volt/(0.009*Vcc)+(0.095/0.009));
      
  switch (OutFormatPressure){
    case 0:
      Serial.print(volt);
      Serial.print(" V");
      break;
    case 1:
      Serial.print(hPa);
      Serial.print(" hPa");
      break;
    }    
  }
  
// The Accelerometer sensor (MMA7361L)
float AccCalc(float volt){
  float mv2, gradient;
  if (AccSensMode == 0){ gradient = Acc3;}
  else {gradient = Acc9;}
  mv2 = (volt-Acc0)/gradient;
  return mv2;
}

void PrintAcc(int OutFormatAcc){
  float voltx = BitToVolt(2)+AccxOff, volty = BitToVolt(3)+AccyOff, voltz = BitToVolt(4)+AcczOff, Gx, Gy, Gz;
  Serial.print(" | Acceleration [x,y,z]: ");
  Gx = AccCalc(voltx);
  Gy = AccCalc(volty);
  Gz = AccCalc(voltz);
  
  switch (OutFormatAcc){
    case 0:
      Serial.print(voltx);
      Serial.print(" V, ");
      Serial.print(volty);
      Serial.print(" V, ");
      Serial.print(voltz);
      Serial.print(" V, ");
      break;
    case 1:
      Serial.print(Gx);
      Serial.print(" G, ");
      Serial.print(Gy);
      Serial.print(" G, ");
      Serial.print(Gz);
      Serial.print(" G ");
      break;
    }    
  }
  

void ReceiveSerial(){
  while (Serial.available()){
    delay(10);
    char RxChar = (char)Serial.read(); 
    switch(RxChar){
      case '1':
        LOOPTIME=1000;
        break;
      case '2':
        LOOPTIME=500;
        break;
      case '5':
        LOOPTIME=200;
        break;
      case 'R':
          counter = 0;
          break;
      case 'V':
          OutFormatNTC = 0;
          OutFormatAcc = 0;
          OutFormatPressure = 0;
          OutFormatTmp = 0;
          break;
      case 'S':
          OutFormatNTC = 1;
          OutFormatAcc = 1;
          OutFormatPressure = 1;
          OutFormatTmp = 1;
          break;
      }
  }
}

  
void loop() {
  long int loop_start = millis(), loop_end;
  printdata();
  ReceiveSerial();  
  counter++;
  loop_end = millis();
  if (LOOPTIME>(loop_end-loop_start)){      //Sets the delay to aquire right loop time
    delay(LOOPTIME-(millis()-loop_start));  
  }
}
