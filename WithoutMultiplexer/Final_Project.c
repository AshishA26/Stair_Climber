/*
	Samuel Ke, Ashish Agrahari, Svamin Bhatnagar, and Jacob Zhuang
	Lego EV3 Group 8-8
	11/03/2023
	Robot Name: Bertha
	Version: 1.0
	Description: Stair Climbing Robot
	Acknowledgements:
		- https://www.robotc.net/files/pdf/lego-natural-language/NL_NXT_Quick.pdf
		- http://cmra.rec.ri.cmu.edu/products/teachingmindstorms/sensing/volumespeed/documents/Sensing_SpeedBasedVolume.pdf
		- https://www.robotc.net/files/pdf/lego-natural-language/NL_TETRIX_Quick.pdf
		- https://www.youtube.com/watch?v=kjoKC0uWtTo&ab_channel=hundredvisionsguy
	- Multiplexer code from learn
  Note: The program assumes common.h, mindsensors-ev3smux.h and UW_sensorMux.c are in the same directory
				as this file.
*/

	/*
		S1 Gyro
		S2 Sound
		S3 Touch
		S4 Ultrasonic

		A Front Left Wheel
		B Back Wheel
		C Belt
		D Front Right Wheel
	*/

//#include "UW_sensorMux.c"

void configureSensors();
void driveMotorsFront(int leftPower, int rightPower);
void driveMotorsFrontBack(int motorPower);
void driveMotorsFrontWithBelt(int motorPower);
//void rotateRobot(int angle);
bool climb(int motorPower);
void screamDetected(int soundLevel, float waitTime);
float measureDist(float waitTime);
void driveDist(int distance, int power);
void pullBeltBackUp(int motorPower);
void moveRobotBackDown(int motorPower);
bool climbAllSteps(bool failedClimb);

// Constants
const int SPEED_SLOW = 20;
const int SPEED_MID = 25;
const float SOUND_LEVEL = 20;
const float TIME_INTERVAL = 10;
const int MAX_DIST = 20;
const int CM_TO_ENC = 360 / (2.0 * PI * 3.4);
const int ROBOT_LENGTH = 20;
const int TURN_SPEED = 10;

task main()
{
	configureSensors();

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
	//playSoundFile("Blip1");

	// Start climbing
	bool failedClimb = false;
	failedClimb = climbAllSteps(failedClimb);

	// Back away from stair
	if (failedClimb)
	{
		driveDist(5, -SPEED_MID);
	}
	// Drive while red isn't detected
	else
	{
		driveMotorsFrontBack(SPEED_SLOW);

		//STOP MOTORS SOMEHOW

		// while (readMuxSensor(msensor_S1_3) != (int)colorRed)
		// {}
	}

	// Stop and play sound
	driveMotorsFrontBack(0);
	//playSoundFile("Blip1");
}

// Sensor Configuration
void configureSensors()
{
	SensorType[S3] = sensorEV3_Touch;
	wait1Msec(50);

	SensorType[S4] = sensorEV3_Ultrasonic;
	wait1Msec(50);

	//**Sound Sensor Config - To Be Added**
	SensorType[S2] = sensorSoundDB;
	wait1Msec(50);

	SensorType[S1] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S1] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);

	// SensorType[S1] = sensorEV3_GenericI2C;
	// wait1Msec(100);
	// // configure each channel on the sensor mux
	// initSensorMux(msensor_S1_2, gyroAngle);
	// wait1Msec(50);
	// initSensorMux(msensor_S1_3, colorMeasureColor);
	// wait1Msec(50);
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
	motor[motorC] = motorPower;
	return;
}

bool climbAllSteps(bool failedClimb)
{
	do
	{
		// Drive motors until possible stairs detected
		driveMotorsFrontBack(SPEED_MID);
		while (measureDist(TIME_INTERVAL) > 20)
		{}

		// Drive 5 seconds slowly until aligned
		// driveMotorsFrontBack(SPEED_SLOW);
		// time1[T1] = 0;
		// while (time1[T1] < 1750)
		// {}

		resetGyro(S1);
		int prevAngle = getGyroDegrees(S1);
		driveMotorsFrontBack(SPEED_SLOW);
		int newAngle = prevAngle + 5;
		while (getGyroDegrees(S1) < newAngle)
		{}

		// Stop and climb
		//driveMotorsFrontBack(0);
		motor[motorB]=0;
		failedClimb = climb(SPEED_SLOW);

		// While the robot has not cleared the stair and has not reached max height
	} while (measureDist(TIME_INTERVAL) < 50 && !failedClimb);

	return failedClimb;
}

// Climb robot
bool climb(int motorPower)
{
	nMotorEncoder[motorC] = 0;
	driveMotorsFrontWithBelt(motorPower);
	// While stair not cleared and max height not reached
	// (Because SensorValue[ULTRASONIC] is 255 when climbing. MAX_DIST is the width of each step = 30 for now)
	float distMeasured = 0;
	while ((distMeasured < MAX_DIST || distMeasured > 100) && SensorValue[S3] == 0)
	{
		distMeasured = measureDist(TIME_INTERVAL);
		//displayString(10, "%f", distMeasured);
	}
	// Climb an additional distance to account for wheel offset
	int reading1 = nMotorEncoder[motorC];
	while (abs(nMotorEncoder[motorC]) < reading1 + (2 * CM_TO_ENC) && SensorValue[S3] == 0)
	{}

	// Failed to climb, go back down
	if (SensorValue[S3] == 1)
	{
		moveRobotBackDown(motorPower);
		return true;
	}
	// Drive robot forward and pull up belt
	else
	{
		driveMotorsFrontWithBelt(0);
		driveDist(ROBOT_LENGTH, motorPower);
		pullBeltBackUp(SPEED_SLOW);

		return false;
	}
}

// Move robot back down
void moveRobotBackDown(int motorPower)
{
	driveMotorsFrontWithBelt(-motorPower);
	while (abs(nMotorEncoder[motorC]) > 0)
	{}
	driveMotorsFrontWithBelt(0);
}

// Pull belt back up
void pullBeltBackUp(int motorPower)
{
	motor[motorC] = -motorPower;
	while (abs(nMotorEncoder[motorC]) > 0)
	{}
	motor[motorC] = 0;
}

// Measure ultrasonic distance
float measureDist(float waitTime)
{
	float average = 0;
	time1[T1] = 0;
	while (time1[T1] < waitTime)
	{}
	int reading1 = SensorValue(S4);
	while (time1[T1] < waitTime * 2)
	{}
	int reading2 = SensorValue(S4);
	while (time1[T1] < waitTime * 3)
	{}
	int reading3 = SensorValue(S4);
	average = (float)(reading1 + reading2 + reading3) / 3.0;
	return average;
}

// Measure sound
void screamDetected(int soundLevel, float waitTime)
{
	float average = 0;
	while (average < soundLevel)
	{
		time1[T1] = 0;
		while (time1[T1] < waitTime)
		{}
		int reading1 = SensorValue(S2);
		while (time1[T1] < waitTime * 2)
		{}
		int reading2 = SensorValue(S2);
		while (time1[T1] < waitTime * 3)
		{}
		int reading3 = SensorValue(S2);
		average = (float)(reading1 + reading2 + reading3) / 3.0;
	}
}

// Drive a given distance
void driveDist(int distance, int power)
{
	nMotorEncoder[motorA] = 0;
	float distToDrive = distance * CM_TO_ENC;
	driveMotorsFrontBack(power);
	while (abs(nMotorEncoder[motorA]) < distToDrive)
	{}
	driveMotorsFrontBack(0);
}


// // Rotate robot
// void rotateRobot(int angle)
// {
// 	int prevAngle = getGyroDegrees(S2);
// 	int newAngle = prevAngle + angle;
// 	if (angle > 0)
// 	{
// 		driveMotorsFront(-TURN_SPEED, TURN_SPEED);
// 		while (readMuxSensor(msensor_S1_2) < newAngle)
// 		{}
// 	}
// 	else
// 	{
// 		driveMotorsFront(TURN_SPEED, -TURN_SPEED);
// 		while (readMuxSensor(msensor_S1_2) > newAngle)
// 		{}
// 	}
// 	driveMotorsFront(0, 0);
// 	return;
// }
