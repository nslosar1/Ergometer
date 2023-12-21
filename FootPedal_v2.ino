/*
  Prints the # of times the InputPin pin goes to a voltage high
  (i.e. the # of time the pedal is pressed) as well as the
  total amount of work done

  Tools > Serial Monitor to view results in real time
*/
//*****************************************************************************
//Set these variables accordingly
float timer = .3; //Number of Minutes You Want to Time
int InputPin = A5; //The Pin the Arduino will detect
//COMMENT OUT THE PORTION OF THE CODE YOU ARE NOT USING FOR WORK FUNCTION
//*****************************************************************************

//-------------------------------------------------------------------------------------------------
//***ANALYTICAL VALUES***
float k =  179.13; //Spring (Band) Constant
float C_min = .869; //Circumference of unstretched band    
float Lf = .320; //Length of Band at Max Extension in cm (On Device) (L min in equation)
float Li = .277; //Length of Band at Min Extension in cm (On Device) (L max in equation)
float L_og = .3048; //Length of Band at Min Extension in cm (On Device) (L_original in equation)
float F0 = 27.688; //Force at Zero Extension
//-------------------------------------------------------------------------------------------------

//****************************************************************************************************
//***EXPERIMENTAL VALUES***
float force = 21.8; // The avg perpendicular force measured by force gauge
float radius = 27.8; // The radius in cm of the point where the force gauge connected to the pedal
float theta = 26; // The change in angle in degrees
//****************************************************************************************************

//***DO NOT CHANGE THIS PORTION OF THE CODE (with the exception of)***
//General house keeping variables
int counter = 0; //DO NOT CHANGE (Counts how many times pedal was pressed)
float countDelay = 250; //DO NOT CHANGE (The recovery time after a press where another press cannot be recorded)
float StartTime = 0; //DO NOT CHANGE (Time of first pedal push)
float PauseCounter = 0; //DO NOT CHANGE (Counter for Reset)
int AttemptNumber = 1; //DO NOT CHANGE (Counts attempt number)
bool Start=false; //DO NOT CHANGE (Needed so StartTime doesn't start by itself)
//ANALYTICAL variable conversions
float radians = theta / 360 * 2 * 3.14; //DO NOT CHANGE The change in angle in radians
float r_m = radius / 100; //DO NOT CHANGE (The radius in m)

/*
  Initializes the input pin and LED
*/
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(InputPin, INPUT);
  Serial.begin(9600);
}

/*
  Measures voltage and returns information every time pedal is pressed
  The LED will also illuminate for every press
*/
void loop() {
  //Waits untill Pedal makes contact to start timer and set StartTime
  float voltage = analogRead(InputPin) * (5.0 / 1023.0);
  while(Start==false){
    voltage = analogRead(InputPin) * (5.0 / 1023.0);
    if (voltage > 4.5) {
      Start=true;
      StartTime=millis();
    }
      else {
        StartTime=millis();
    }
  }
//Checks of timer has expired and waits for 3 second pedal push for reset
  if (timer<=((millis()-StartTime)/60000)){
      Serial.println(String(timer) + " MINUTE TIMER IS UP ");
      while(PauseCounter<30){
        if(voltage > 4.5){
          voltage = analogRead(InputPin) * (5.0 / 1023.0);
          delay(100);
          PauseCounter+=1;
          if(PauseCounter==1){
          Serial.println("Resetting in 3...");
          }else if(PauseCounter==10){
          Serial.println("Resetting in 2...");
          }else if(PauseCounter==20){
          Serial.println("Resetting in 1...");
          }
        }
        else{
          voltage = analogRead(InputPin) * (5.0 / 1023.0);
          PauseCounter=0;
        }
      }
      //Reset confirmed and waits until pedal is released to reset variables
      Serial.println("\n" "RESET. Touch pedal to begin again." "\n");
      while(voltage > 4.5){
        voltage = analogRead(InputPin) * (5.0 / 1023.0);
        delay(100);
        }
      AttemptNumber += 1;
      PauseCounter=0;
      StartTime= millis();
      Start=false;
      counter = 0;
      
  }
  //updates timer and adds 1 to counter if contact is made
  if (voltage > 4.5) {
    digitalWrite(LED_BUILTIN, HIGH);
    if (counter == 0){
        StartTime= millis();
        unsigned long runMillis= (millis()-StartTime);
        unsigned long allSeconds= (millis()-StartTime)/1000;
        int runHours= allSeconds/3600;
        int secsRemaining=allSeconds%3600;
        int runMinutes=secsRemaining/60;
        int runSeconds=secsRemaining%60;
        char buf[21];
        sprintf(buf,"Runtime: %02d:%02d ",runMinutes,runSeconds);
        Serial.println("Runetime: START" "\t" "Attempt Number: " + String(AttemptNumber));
      }
      else{
        unsigned long runMillis= (millis()-StartTime);
        unsigned long allSeconds=(millis()-StartTime)/1000;
        int runHours= allSeconds/3600;
        int secsRemaining=allSeconds%3600;
        int runMinutes=secsRemaining/60;
        int runSeconds=secsRemaining%60;
        char buf[21];
        sprintf(buf,"Runtime: %02d:%02d ",runMinutes,runSeconds);
        Serial.println(buf);
      }
    counter = counter + 1;
    Serial.println(String(counter) + "\t" + String(work(counter)) + " J" + "\n" );
    delay(countDelay);
    while (voltage > 1){
      voltage = analogRead(InputPin) * (5.0 / 1023.0);
    }
    digitalWrite(LED_BUILTIN, LOW);
    delay(countDelay);
    
  }
}

/*
  Returns the total amount of work done for COUNT pedal presses
  Based on eqn W = 0.5Fx, where F = kx + f_0
*/
//**************************************************************************************************************************
//***COMMENT OUT THE PORTION OF THE CODE YOU ARE NOT USING***
float work (int count) {
  
////***USE THIS PORTION OF THE CODE TO PLUG IN ANALYTICAL VALUES***
//  return count *( (((((k*C_min)/(4*Li))*sq(Lf)))+(F0-(k*L_og))*Lf)- (((((k*C_min)/(4*Li))*sq(Li)))+(F0-(k*L_og))*Li));

//***USE THIS PORTION OF THE CODE TO PLUG IN EXPERIMENTAL VALUES***
  return count * force * r_m * radians;
//**************************************************************************************************************************
}
