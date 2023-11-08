/*
  Samuel Ke, Ashish Agrahari, Svamin Bhatnagar, and Jacob Zhuang
  Lego EV3 Group 8-8
  11/03/2023
*/

//Robot Name: Bertha


/*References: https://www.robotc.net/files/pdf/lego-natural-language/NL_NXT_Quick.pdf
- http://cmra.rec.ri.cmu.edu/products/teachingmindstorms/sensing/volumespeed/documents/Sensing_SpeedBasedVolume.pdf
- https://www.robotc.net/files/pdf/lego-natural-language/NL_TETRIX_Quick.pdf
- https://www.youtube.com/watch?v=kjoKC0uWtTo&ab_channel=hundredvisionsguy
*/

// Right front motor is D, left is C

const int MAX_DIST  = 30; //Width of each step

const float TIME_INTERVAL = 5;

void rotateRobot(int angle);

void configureSensors();

void driveMotor(int motorPower);

void climb(int motorPower);

void screamDetected(int soundLevel, float timeInterval);

double measureDist(float timeInterval);

void driveDist(int distance, int power);

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
  displayString(5, "Group: 8-8,");
  displayString(6, "Robot: 33,");
  displayString(7, "Name: Bertha");

  const float SOUND_LEVEL = 40;

  //stuck in the function until something loud is detected
  screamDetected(SOUND_LEVEL, TIME_INTERVAL);

  //play sound
  playSoundFile("Blip1");

  bool failedClimb = false;
  do{
    //drive motors until possible stairs detected
    driveMotors(40,40);
    while(measureDist(TIME_INTERVAL) < 10)
    {}

    //drive 5 seconds until aligned
    driveMotors(25,25);
    time1[T1] = 0;
    while(time1[T1] < 5000)
    {}

    driveMotors(0,0);
    failedClimb = climb();

  } while(measureDist(WAIT TIME) < STEP DIST && !failedClimb);

  if(failedClimb)
  {
   driveDist(distance, power);
  }
  else
  {
    motor[TRACK_MOTOR] = 25;
    while(nMotorEncoder[TRACK_MOTOR] > 0)
    {}
    motor[TRACK_MOTOR] = 0;

    driveMotors(25,25);
    while(sensorValue(colourSensor value) != (int) colorRed)
    {}
  }
  driveMotors(0,0);
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


bool climb(int motorPower)
{
  nMotorEncoder[TRACK_MOTOR] = 0;
  motor[FRONT_WHEELS] = motor[TRACK_MOTOR] = motorPower;
  while(SensorValue[ULTRASONIC] < MAX_DIST && SensorValue[TOUCH] == 0) //Because SensorValue[ULTRASONIC] is 255 when climbing. MAX_DIST is the width of each step = 30 for now
  {}

  if(SensorValue[TOUCH] == 1)
  {
    motor[TRACK_MOTOR] = motor[FRONT_WHEELS] = -motorPower;
    while(nMotorEncoder[TRACK_MOTOR] > 0)
    {}
    motor[TRACK_MOTOR] = 0;
    return true;
  }
  else
  {
    motor[TRACK_MOTOR] = 0;
    nMotorEncoder[FRONT_WHEELS] = 0;
    driveDist(distance, power);
    return false;
  }
}

float measureDist(float waitTime)
{
  float average = 0;
  float reading1 = SensorValue (**sensorport**);
  time1[T1] = 0;
  while (time1[T1] > waitTime)
  {}
  float reading2 = SensorValue (*sensorport);
  while (time1[T1] > waitTime*2)
  {}
  float reading3 = SensorValue (****sensorport here**);
  average = (reading1 + reading2 + reading3)/3.0;
  return average;
}

void screamDetected (int soundLevel, float waitTime){
  float average = 0;
  while (average < soundLevel)
  {
    float reading1 = sensorValue (**sensorport**);
    time1[T1] = 0;
    while (time1[T1] > waitTime)
    {}
    float reading2 = sensorValue (*sensorport);
    while (time1[T1] > waitTime*2)
    {}
    float reading3 = sensorValue (****sensorport here**);
    average = (reading1 + reading2 + reading3)/3.0;
	}
}

void driveDist(int distance, int power)
{
  nMotorEnconder(motorA) = 0;
  float distToDrive = (dist*(360/2.0*PI*2.75));
  motor[motorA] = motor [motorD] = power;
  while (nMotorEncoder(motorA) < distance)
  {}
  motor[motorA] = motor[motorD] = 0;
}
