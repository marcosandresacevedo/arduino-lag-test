long unsigned startMicros;
volatile long unsigned duration;
volatile long unsigned ISRCounter = 0;

int pinButton = 5; // Pulls button LOW
int pinMister = 2; // Registers press (needs to be an interrupt pin)

int delayPress = 100; //Time between HIGH and LOW toggles

void setup() {

  pinMode(pinButton,INPUT);
  
  pinMode(pinMister,INPUT);

  // External interrupt 
  attachInterrupt(digitalPinToInterrupt(pinMister), timeArrival, FALLING);

  // Waits for serial monitor before starting
  while (!Serial);
  Serial.begin(115200);
  Serial.println("Starting!");
}


void loop() {

  // Random button presses
  // delayPress = random(50,75);
 
  // Start timer
  startMicros = micros();
  
  // Controller button to ground
  pinMode(pinButton,OUTPUT);
  
  // Pause before and after button press
  delay(delayPress);

  // Reset pinButton to floating
  pinMode(pinButton,INPUT);

  delay(delayPress);

}


// Interrupt function, just timestamps and prints :)
void timeArrival(){

  ISRCounter += 1;
  duration = micros() - startMicros;
  
//  Uncomment this block for running stats
  clearScreen();

  Serial.print("Samples: "); 
  Serial.println(ISRCounter);

  Serial.print("Average: ");
  Serial.print(getAverage(duration)/1000,4);  
  Serial.println(" ms");

  Serial.print("Max:     ");
  Serial.print(getMax(duration)/1000,3);
  Serial.println(" ms");

  Serial.print("Min:     ");
  Serial.print(getMin(duration)/1000,3);
  Serial.println(" ms");

  Serial.print("Std Dev: ");
  Serial.print(getStdDev(duration)/1000,3);
  Serial.println(" ms");

// Comment out this bit if you use running stats, otherwise print latency  
//  Serial.println(duration/float(1000));

}

// Clears screen between stats outputs
void clearScreen(){
  Serial.write(27);      
  Serial.print("[2J");
  Serial.write(27);
  Serial.print("[H");
}


// Functions to calculate Average, Min, Max, and Std Dev

float getAverage(unsigned long int newNum){
  static unsigned long int numSamples = 1;
  static float curAvg;
  
  curAvg = curAvg + (newNum - curAvg) / numSamples;
  numSamples++;
  
  return curAvg;
}

float getMax(unsigned long int newNum){
  static unsigned long int maxVal = 0;
  
  if(newNum > maxVal){
    maxVal = newNum;
  }

  return maxVal;
}

float getMin(unsigned long int newNum){
  static unsigned long int minVal = 4294967295;
  
  if(newNum < minVal){
    minVal = newNum;
  }

  return minVal;
}


float getStdDev(unsigned long int newNum){
  
    static float M = 0.0;
    static float S = 0.0;
    static unsigned long int i = 1;

    float tmpM = M;
    M += (newNum - tmpM) / i;
    S += (newNum - tmpM) * (newNum - M);
    i++;

    return sqrt(S/(i-2));
}
