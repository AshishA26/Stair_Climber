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

void configureSensors();
// void driveMotorsFront(int leftPower, int rightPower);
void driveMotorsFrontBack(int motorPower);
void driveAllMotors(int motorPower);
//void rotateRobot(int angle);
bool climb(int motorPower);
void screamDetected(int soundLevel, float waitTime);
float measureDist(float waitTime);
void driveDist(int distance, int power);
void pullBeltBackUp(int motorPower);
void moveRobotBackDown(int motorPower);
bool climbAllSteps(bool failedClimb);

// Constants
const int SPEED_SLOW = 10;
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
	SensorType[S1] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S1] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);

	SensorType[S2] = sensorSoundDB;
	wait1Msec(50);
	
	SensorType[S3] = sensorEV3_Touch;
	wait1Msec(50);

	SensorType[S4] = sensorEV3_Ultrasonic;
	wait1Msec(50);
}

// Drive front and back motors
void driveMotorsFrontBack(int motorPower)
{
	motor[motorD] = motor[motorA] = motorPower;
	motor[motorB] = -motorPower;
	return;
}

// Drive only front motors and belt
void driveAllMotors(int motorPower)
{
	motor[motorD] = motor[motorA] = (int)(motorPower*0.5);
	motor[motorC] = motorPower;
	motor[motorB] = (int)(motorPower*0.2);
	return;
}

bool climbAllSteps(bool failedClimb)
{
	do
	{
		// Drive motors until possible stairs detected
		driveMotorsFrontBack(SPEED_MID);
		while (measureDist(TIME_INTERVAL) > 20)
		{
			displayBigTextLine(1,"Sonar is %d", measureDist(TIME_INTERVAL));
		}

		resetGyro(S1);
		driveMotorsFrontBack(SPEED_SLOW);
		int newAngle = 5;
		while (getGyroDegrees(S1) < newAngle)
		{
			displayBigTextLine(3,"Gyro is %d", getGyroDegrees(S1));
		}

		displayBigTextLine(5,"Ready to climb");

		failedClimb = climb(SPEED_SLOW);

		// While the robot has not cleared the stair and has not reached max height
	} while (measureDist(TIME_INTERVAL) < 50 && !failedClimb);

	return failedClimb;
}

// Climb robot
bool climb(int motorPower)
{
	nMotorEncoder[motorC] = 0;
	driveAllMotors(motorPower);

	// While stair not cleared and max height not reached
	while (getGyroDegrees(S1)>-5 && SensorValue[S3] == 0)
	{
		displayBigTextLine(3,"Gyro is %d", getGyroDegrees(S1));
	}

	// Failed to climb, go back down
	if (SensorValue[S3] == 1)
	{
		displayBigTextLine(5,"Failed, going down");
		moveRobotBackDown(motorPower);
		return true;
	}
	// Drive robot forward and pull up belt
	else
	{
		displayBigTextLine(5,"Going forward 10cm");
		driveAllMotors(0);
		driveDist(ROBOT_LENGTH, motorPower);

		displayBigTextLine(5,"Pulling belt up");
		pullBeltBackUp(SPEED_SLOW);

		return false;
	}
}

// Move robot back down
void moveRobotBackDown(int motorPower)
{
	driveAllMotors(-motorPower);
	while (abs(nMotorEncoder[motorC]) > 0)
	{}
	driveAllMotors(0);
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

// Drive only front motors (can be used for turning)
// void driveMotorsFront(int leftPower, int rightPower)
// {
// 	motor[motorD] = rightPower;
// 	motor[motorA] = leftPower;
// 	return;
// }

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
