/*
  Samuel Ke, Ashish Agrahari, Svamin Bhatnagar, and Jacob Zhuang
  Lego EV3 Group 8-8
  11/03/2023
*/

//Robot Name: Bertha

// Right front motor is D, left is C

const int MAX_DIST  = 30; //Width of each step

void rotateRobot(int angle);

void configureSensors();

void driveMotor(int motorPower);

void climb();

void screamDetected (int soundLevel);

double measureDist ();

task main () 
{
	configureSensors();
  
  /*
    Reset Sensors and Motor Encoders as Required
    
    Display Group Name, Robot Number, Robot Name
    
    While sound threshold not met, don't start (Make a function that takes sound measurements as a 3 point average over 15ms)
    
    Play notification sound
    
    **Everything tabbed exists in a do while loop, condition(touch sensor not activated, forward distance within a step length)
    	Drive forward at a steady speed, while ultrasonic min dist not met (Make a function that takes dist measurements as a 3 point average over 15ms)
    
    	Drive slowly for 5 seconds
    
    	While ultrasonic max dist not met or max height not met, begin climibing the belt and front wheels simultaneously and a slow rate (speed to be determined based on results)
    
    	IF max height reached, go back down, using motorencoders and play error noise and end program
    
    	ELSE drive distance of robot length, stop, pick up belt using motorencoder
    
    While color not correct, drive until color is reached
    
    Play notification sound
    
    End Program
    
    ---
    
    Project Extension, IF WE HAVE TIME
    
    **Back in the climbing phase, store the distance climbed and distance driven from edge, remember this all only happens if we sucessfully climbed.
    
    Drive back exactly the distance driven, put down the belt slight more than recorded via encoder
    
    Slowly Drive back until distance of bot driven, once reached immediately drive front wheels slowly back and back wheels only a tiny bit forward (This part would take lots of testing)
    
    Drive both simultaneously down at the same rate until motor encoder amount met
    
    Driver back, turn around, and arrive at start.
    
    Play notification
    
    End Program
  */
  
  //initialize motor encoders
  nMotorEncoder[motorA] = 0;
  nMotorEncoder[motorB] = 0;
  nMotorEncoder[motorC] = 0;
  nMotorEncoder[motorD] = 0;
  
  //display groups and name
  displayString(5, "Group: 8, Robot: 33, Name: Bertha?");
  
  const double SOUND_LEVEL = 40;
  //stuck in the function until something loud is detected
  screamDetected(SOUND_LEVEL);
  
  //play sound
  playSoundFile("Blip1");
	
  //drive motors for 5 seconds
  
  driveMotors(25,25);
  time1[T1] = 0;
	while(time1[T1] < 5000)
  {}
  
  while(sensorValue(colourSensor value something here) != (int) colorRed)
  {
    driveMotors(25,25);
    const double STAIR_LENGTH = 30;
    //needs work here
    while(nMotorEnconder[motorA] < STAIR_LENGTH)
    {}
    
    climb();
	}
  
  //play sound
  playSoundFile("Blip1");
}

void configureSensors() //Sensor #s to be determined and added
{
	SensorType[] = sensorEV3_Touch;

	SensorType[] = sensorEV3_Ultrasonic;

	SensorType[] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[] = modeEV3Color_Color;
	wait1Msec(50);

	SensorType[] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
}

void driveMotors(int leftPower, int rightPower) // Make all motor inputs slow
{
  motor[motorD] = rightPower;
  motor[motorC] = leftPower;
  return;
}

void rotateRobot(int angle) // Make all motor inputs slow
{
  int prevAngle = getGyroDegrees(S2)
  int newAngle = prevAngle + angle;
  if(angle > 0)
  {
   	driveMotors(-10,10);
    while(getGyroDegrees(S4) < newAngle)
    {}
    return;
  }
  else
  {
    driveMotors(10,-10);
    while(getGyroDegrees(S2) > newAngle)
    {}
    return;
	}
}


void climb()
{
  nMotorEncoder[TRACK_MOTOR] = 0;
  motor[FRONT_WHEELS] = motor[TRACK_MOTOR] = 20;
  while(SensorValue[ULTRASONIC] > MAX_DIST && SensorValue[TOUCH] == 0) //Because SensorValue[ULTRASONIC] is 255 when climbing
  {}
  motor[FRONT_WHEELS] = motor[TRACK_MOTOR] = 0;
  if(SensorValue[ULTRASONIC] < MAX_DIST)
  {
    motor[TRACK_MOTOR] = -20;
    while(nMotorEncoder[TRACK_MOTOR] > 0)
    {}
    motor[TRACK_MOTOR] = 20;
  }
	return;
}

double measureDist (){
  const double WAIT_TIME = 5;
  double average = 0;
  double reading1 = sensorValue (**sensorport**);
  time1[T1] = 0;
  while (time1[T1] > WAIT_TIME)
  {}
  double reading2 = sensorValue (*sensorport);
  while (time1[T1] > WAIT_TIME*2)
  {}
  double reading3 = sensorValue (****sensorport here**);
  average = (reading1 + reading2 + reading3)/3.0;
  return average;
}

void screamDetected (int soundLevel){
  const double WAIT_TIME = 5;
  double average = 0;
  while (average < soundLevel){
    double reading1 = sensorValue (**sensorport**);
    time1[T1] = 0;
    while (time1[T1] > WAIT_TIME)
    {}
    double reading2 = sensorValue (*sensorport);
    while (time1[T1] > WAIT_TIME*2)
    {}
    double reading3 = sensorValue (****sensorport here**);
    average = (reading1 + reading2 + reading3)/3.0;
	}
}










