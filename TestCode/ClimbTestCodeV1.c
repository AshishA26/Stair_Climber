#include "UW_sensorMux.c"

void configureSensors();
void driveMotorsFrontBack(int motorPower);
void driveMotorsFrontWithBelt(int motorPower);
bool climb(int motorPower);
float measureDist(float waitTime);
void driveDist(int distance, int power);
void pullBeltBackUp(int motorPower);
void moveRobotBackDown(int motorPower);

// Constants
const int SPEED_SLOW = 25;
const int SPEED_MID = 40;
const float TIME_INTERVAL = 5;
const int MAX_DIST = 20;
const int CM_TO_ENC = 360/2.0*PI*3.4;
const int ROBOT_LENGTH = 20;

task main ()
{
	configureSensors();

	// Initialize motor encoders
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorC] = 0;
	nMotorEncoder[motorD] = 0;

	// Play sound
	playSoundFile("Blip1");

	bool failedClimb = false;
	do{
		// Drive motors until possible stairs detected
		driveMotorsFrontBack(SPEED_MID);
		while(measureDist(TIME_INTERVAL) > 20)
		{}

		// Drive 5 seconds slowly until aligned
		driveMotorsFrontBack(SPEED_SLOW);
		time1[T1] = 0;
		while(time1[T1] < 2000)
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

  // Stop and play sound
	driveMotorsFrontBack(0);
	playSoundFile("Blip1");
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

	SensorType[S1] = sensorEV3_GenericI2C;
	wait1Msec(100);
  // configure each channel on the sensor mux
	initSensorMux(msensor_S1_2, gyroAngle);
	wait1Msec(50);
  initSensorMux(msensor_S1_3, colorMeasureColor);
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
void driveMotorsFrontWithBelt(int motorPower)
{
	motor[motorD] = motor[motorA] = motorPower;
	motor[motorB] = -motorPower/25;
	motor[motorC] = motorPower;
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

		pullBeltBackUp(SPEED_SLOW);

		return false;
	}
}

// Move belt back down
void moveRobotBackDown(int motorPower)
{
    driveMotorsFrontWithBelt(motorPower);
		while(abs(nMotorEncoder(motorC)) > 0)
		{}
    driveMotorsFrontWithBelt(0);
}

// Pull belt back up
void pullBeltBackUp(int motorPower)
{
	motor[motorC] = -motorPower;
	while(abs(nMotorEncoder(motorC)) > 0)
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
	float reading1 = SensorValue(S4);
	while (time1[T1] < waitTime*2)
	{}
	float reading2 = SensorValue(S4);
	while (time1[T1] < waitTime*3)
	{}
	float reading3 = SensorValue(S4);
	average = (reading1 + reading2 + reading3)/3.0;
	return average;
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
