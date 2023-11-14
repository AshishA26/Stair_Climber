/*
Samuel Ke, Ashish Agrahari, Svamin Bhatnagar, and Jacob Zhuang
Lego EV3 Group 8-8
11/03/2023
Robot Name: Bertha
Version: 1.0
Description:
Acknowledgements:
- https://www.robotc.net/files/pdf/lego-natural-language/NL_NXT_Quick.pdf
- http://cmra.rec.ri.cmu.edu/products/teachingmindstorms/sensing/volumespeed/documents/Sensing_SpeedBasedVolume.pdf
- https://www.robotc.net/files/pdf/lego-natural-language/NL_TETRIX_Quick.pdf
- https://www.youtube.com/watch?v=kjoKC0uWtTo&ab_channel=hundredvisionsguy
*/

void configureSensors();

void driveMotorsFront(int leftPower, int rightPower);

void driveMotorsFrontBack(int motorPower);

void driveMotorsFrontWithBelt(int motorPower);

void rotateRobot(int angle);

bool climb(int motorPower);

void screamDetected(int soundLevel, float waitTime);

float measureDist(float waitTime);

void driveDist(int distance, int power);

void pullRobotBackUp(int motorPower);

void moveRobotBackDown(int motorPower);

// Constants
const int SPEED_SLOW = 25;
const int SPEED_MID = 40;
const float SOUND_LEVEL = 40;
const float TIME_INTERVAL = 5;
const int MAX_DIST  = 30;
const int CM_TO_ENC = 360/2.0*PI*3.4;
const int ROBOT_LENGTH = 20;
const int TURN_SPEED = 10;

task main ()
{
	configureSensors();

	/*
		S1 multiplexer, c2 to sound, c3 colour
		S2 gyro
		S3 touch
		S4 ultrasonic

		A Top left wheel
		B Back big motor
		C Belt
		D top right wheel
	*/

	// Initialize motor encoders
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorC] = 0;
	nMotorEncoder[motorD] = 0;


	// Display group and name
	displayString(5, "Group: 8-8,");
	displayString(6, "Robot: 33,");
	displayString(7, "Name: Bertha");

	// Wait until a loud noise is detected
	screamDetected(SOUND_LEVEL, TIME_INTERVAL);

	// Play sound
	playSoundFile("Blip1");

	bool failedClimb = false;
	do{
		// Drive motors until possible stairs detected
		driveMotorsFrontBack(SPEED_MID);
		while(measureDist(TIME_INTERVAL) > 10)
		{}

		// Drive 5 seconds slowly until aligned
		driveMotorsFrontBack(SPEED_SLOW);
		time1[T1] = 0;
		while(time1[T1] < 5000)
		{}

    // Stop and climb
		driveMotorsFrontBack(0);
		failedClimb = climb(SPEED_SLOW);

    // While the robot has not cleared the stair and has not reached max height
	} while(measureDist(TIME_INTERVAL) < MAX_DIST && !failedClimb);

  // Back away from stair
	if(failedClimb)
	{
		driveDist(40, -SPEED_MID);
	}
  // Drive while red isn't detected
	else
	{
		driveMotorsFrontBack(SPEED_SLOW);
		while(SensorValue(**Color Sensor**) != (int) colorRed)
		{}
	}

  // Stop and play sound
	driveMotorsFrontBack(0);
	playSoundFile("Blip1");
}

// Sensor Configuration
void configureSensors()
{
	SensorType[S3] = sensorEV3_Touch;

	SensorType[S4] = sensorEV3_Ultrasonic;

	SensorType[**Color Sensor**] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[**Color Sensor**] = modeEV3Color_Color;
	wait1Msec(50);

	//**Sound Sensor Config - To Be Added**

	SensorType[S2] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S2] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S2] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
}

// Drive front and back motors
void driveMotorsFrontBack(int motorPower)
{
	motor[motorD] = motor[motorA] = motorPower;
	motor[motorB] = -motorPower;
	return;
}

// Drive only front motors (can be used for turning)
void driveMotorsFront(int leftPower, int rightPower)
{
	motor[motorD] = rightPower;
	motor[motorA] = leftPower;
	return;
}

// Drive only front motors and belt
void driveMotorsFrontWithBelt(int motorPower)
{
	motor[motorD] = motor[motorA] = motorPower;
	motor[motorC] = -motorPower;
	return;
}

// Rotate robot
void rotateRobot(int angle)
{
	int prevAngle = getGyroDegrees(S2);
	int newAngle = prevAngle + angle;
	if(angle > 0)
	{
		driveMotorsFront(-TURN_SPEED,TURN_SPEED);
		while(getGyroDegrees(S2) < newAngle)
		{}
	}
	else
	{
		driveMotorsFront(TURN_SPEED,-TURN_SPEED);
		while(getGyroDegrees(S2) > newAngle)
		{}

	}
  driveMotorsFront(0,0);
  return;
}

// Climb robot
bool climb(int motorPower)
{
	nMotorEncoder(motorC) = 0;
	driveMotorsFrontWithBelt(motorPower);
  // While stair not cleared and max height not reached
  // (Because SensorValue[ULTRASONIC] is 255 when climbing. MAX_DIST is the width of each step = 30 for now)
	while(SensorValue[S4] < MAX_DIST && SensorValue[S3] == 0)
	{}
  // Climb an additional distance to account for wheel offset
	int reading1 = nMotorEncoder(motorA);
	while (fabs(nMotorEncoder(motorA)) < reading1+(5*CM_TO_ENC) && SensorValue[S3] == 0)
	{}

  // Failed to climb, go back down
	if(SensorValue[S3] == 1)
	{
		moveRobotBackDown(motorPower);
		return true;
	}
  // Drive robot forward and pull up belt
	else
	{
    driveMotorsFrontWithBelt(0);
		driveDist(ROBOT_LENGTH, motorPower);

		pullRobotBackUp(SPEED_SLOW);

		return false;
	}
}

// Move belt back down
void moveRobotBackDown(int motorPower)
{
    driveMotorsFrontWithBelt(-motorPower);
		while(nMotorEncoder(motorC) > 0)
		{}
    driveMotorsFrontWithBelt(0);
}

// Pull belt back up
void pullRobotBackUp(int motorPower)
{
	motor[motorC] = motorPower;
	while(abs(nMotorEncoder(motorC)) > 0)
	{}
	motor[motorC] = 0;
}

// Measure ultrasonic distance
float measureDist(float waitTime)
{
	float average = 0;
	time1[T1] = 0;
	while (time1[T1] > waitTime)
	{}
	float reading1 = SensorValue(S4);
	while (time1[T1] > waitTime*2)
	{}
	float reading2 = SensorValue(S4);
	while (time1[T1] > waitTime*3)
	{}
	float reading3 = SensorValue(S4);
	average = (reading1 + reading2 + reading3)/3.0;
	return average;
}

// Measure sound
void screamDetected (int soundLevel, float waitTime)
{
	float average = 0;
	while (average < soundLevel)
	{
		time1[T1] = 0;
		while (time1[T1] > waitTime)
		{}
		float reading1 = SensorValue(**Sound Sensor**);
		while (time1[T1] > waitTime*2)
		{}
		float reading2 = SensorValue(**Sound Sensor**);
		while (time1[T1] > waitTime*3)
		{}
		float reading3 = SensorValue(**Sound Sensor**);
		average = (reading1 + reading2 + reading3)/3.0;
	}
}

// Drive a given distance
void driveDist(int distance, int power)
{
	nMotorEncoder(motorA) = 0;
	float distToDrive = distance*CM_TO_ENC;
	driveMotorsFrontBack(power);
	while (fabs(nMotorEncoder(motorA)) < distToDrive)
	{}
	driveMotorsFrontBack(0);
}

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
