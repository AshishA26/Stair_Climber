#pragma config(Motor, motorA, MOTOR_FL,   tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorB, MOTOR_BELT, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorC, MOTOR_BACK, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorD, MOTOR_FR,   tmotorEV3_Large, PIDControl, encoder)
// The code above was automatically generated by 'ROBOTC' configuration wizard
// Used for mapping names to motorA, motorB, etc. to make the code easier to read

/*
Samuel Ke, Ashish Agrahari, Svamin Bhatnagar, and Jacob Zhuang
Lego EV3 Group 8-8
11/20/2023
Robot Name: Bertha
Version: 1.0
Description: Stair Climbing Robot
Acknowledgements:
- https://www.robotc.net/files/pdf/lego-natural-language/NL_NXT_Quick.pdf
- http://cmra.rec.ri.cmu.edu/products/teachingmindstorms/sensing/volumespeed/documents/Sensing_SpeedBasedVolume.pdf
- https://www.robotc.net/files/pdf/lego-natural-language/NL_TETRIX_Quick.pdf
- https://www.youtube.com/watch?v=kjoKC0uWtTo&ab_channel=hundredvisionsguy
- https://robotics.stackexchange.com/questions/751/confused-about-the-variables-in-robotc
*/

/*
Constraints:
- Can go up 1 or 2 books, possibly 3
- Can go down 1 or 2 books
- Stops after it senses that it is not going down anymore steps
- Start on left side of books as it turns slightly right
*/

/*
S1 Gyro
S2 Sound
S3 Touch
S4 Color
A Front Left Wheel
B Back Wheel
C Belt
D Front Right Wheel
*/

// Function prototypes:
void configureSensors();
void driveMotorsFrontBack(int motorPowerF,int motorPowerB);
void driveAllMotorsForClimbing(int motorPower, bool goingUp);
bool climb(int motorPower);
void screamDetected(int soundLevel, float waitTime);
void driveDist(int distance, int motorPower);
void pullBeltBackUp(int motorPower);
void moveRobotBackDown(int motorPower);
bool climbAllSteps(bool failedClimb);
void climbDownAllSteps(int motorPower);
void moveBeltDown(int motorPower, int amount);

// Constants
const int SPEED_SLOW = 20;
const float SOUND_LEVEL = 20;
const float TIME_INTERVAL = 10;
const int CM_TO_ENC = 360 / (2.0 * PI * 3.4);
const int ROBOT_LENGTH = 10;
const int TILT_AMOUNT = 5;
const int ENC_LIMIT_FOR_CLIMB_DOWN = 400;
const int TILT_AMOUNT_FOR_CLIMB_DOWN = 2; //use to be 5
const int WAIT_TIME_FOR_CLIMB_DOWN = 4000;

task main()
{
	// Configure sensors
	configureSensors();

	// Initialize motor encoders
	nMotorEncoder[MOTOR_FL] = 0;
	nMotorEncoder[MOTOR_BACK] = 0;
	nMotorEncoder[MOTOR_BELT] = 0;
	nMotorEncoder[MOTOR_FR] = 0;

	// Display group and name
	displayString(1, "Group: 8-8,");
	displayString(2, "Robot: 33,");
	displayString(3, "Name: Bertha");

	// Wait until a loud noise is detected
	screamDetected(SOUND_LEVEL, TIME_INTERVAL);

	// Play sound
	displayBigTextLine(5,"Starting");
	playSoundFile("Confirm");
	wait1Msec(2000);

	// Start climbing up steps
	bool failedClimb = false;
	failedClimb = climbAllSteps(failedClimb);

	// Back away from stair if failed climb
	if (failedClimb)
	{
		// Back a bit away from stairs
		driveDist(5, -SPEED_SLOW);

		// Stop and play sound
		driveMotorsFrontBack(0,0);
		displayBigTextLine(5,"Failed climb");
		playSoundFile("Error alarm");
		wait1Msec(2000);
	}
	else
	{
		// Stop and play sound
		driveMotorsFrontBack(0,0);
		displayBigTextLine(5,"Please take");
		displayBigTextLine(7,"your item");
		playSoundFile("Confirm");
		wait1Msec(2000);

		// Wait until a loud noise is detected meaning that the
		// user is ready for robot to go back down
		screamDetected(SOUND_LEVEL, TIME_INTERVAL);
		displayBigTextLine(7," ");
		displayBigTextLine(5,"Going down");

		// Play sound
		playSoundFile("Confirm");
		wait1Msec(2000);

		//Drive back down all stairs
		climbDownAllSteps(SPEED_SLOW);

		// Play sound to show its done
		displayBigTextLine(5,"Done delivery");
		playSoundFile("Confirm");
		wait1Msec(2000);
	}
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

	SensorType[S4] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Color_Color;
	wait1Msec(50);

	return;
}

// Drive front and back motors at same speed
void driveMotorsFrontBack(int motorPowerF,int motorPowerB)
{
	motor[MOTOR_FR] = motor[MOTOR_FL] = motorPowerF;
	motor[MOTOR_BACK] = -motorPowerB; // Back motor is physically backwards
	return;
}

// Switch on all motors at different speeds for climbing
void driveAllMotorsForClimbing(int motorPower, bool goingUp)
{
	if (goingUp)
	{
		motor[MOTOR_FR] = motor[MOTOR_FL] = motorPower;
		motor[MOTOR_BELT] = (int)(motorPower*2);
		motor[MOTOR_BACK] = (int)(-motorPower*0.2); // Back motor is physically backwards
	}
	// If the robot has failed and goes down, then the back motor should be faster
	// in order to not flip over
	else
	{
		motor[MOTOR_FR] = motor[MOTOR_FL] = motorPower;
		motor[MOTOR_BELT] = (int)(motorPower*2);
		motor[MOTOR_BACK] = (int)(-motorPower); // Back motor is physically backwards
	}

	// The ratios above were found through testing

	return;
}

// Function that climbs all steps
bool climbAllSteps(bool failedClimb)
{

	// Loop that repeats until robot detects the color green or it fails the climb
	do
	{
		// Drive motors until stairs detected using tilt or if it senses the color green for the safe zone.
		// If it detects positive tilt, it means it is up against the wall and is ready to climb
		resetGyro(S1);
		driveMotorsFrontBack(SPEED_SLOW, SPEED_SLOW);
		while (getGyroDegrees(S1) < TILT_AMOUNT && SensorValue[S4] != (int)colorGreen)
		{}

		// If it exited the while loop because of the tilt, then start climbing.
		if (SensorValue[S4] != (int)colorGreen)
		{
			displayBigTextLine(5,"Ready to climb");

			failedClimb = climb(SPEED_SLOW);
		}
	} while (!failedClimb && SensorValue[S4] != (int)colorGreen);

	return failedClimb;
}

// Function that climbs one step
bool climb(int motorPower)
{
	// Reset belt motor's encoder
	nMotorEncoder[MOTOR_BELT] = 0;

	// Switch on motors needed for climbing
	driveAllMotorsForClimbing(motorPower, true);

	// While downwards tilt not detected, and touch sensor not activated (i.e. max height not reached)
	while (getGyroDegrees(S1)>-TILT_AMOUNT && SensorValue[S3] == 0)
	{}

	// If it fails to climb, (i.e. max height was reached), go back down
	if (SensorValue[S3] == 1)
	{
		displayBigTextLine(5,"Failed climb");
		moveRobotBackDown(motorPower);
		return true;
	}

	// If it successfully climbed, drive robot forward the robot's front portion length and pull up the belt
	else
	{
		displayBigTextLine(5,"Going forward");

		// Stop belt motor
		motor[MOTOR_BELT]=0;

		// Drive distance forward of ROBOT_LENGTH
		driveDist(ROBOT_LENGTH, motorPower);

		displayBigTextLine(5,"Pulling belt up");

		// Switch on front and back motors while pulling up the belt to ensure robot does not tip over
		driveMotorsFrontBack(motorPower+5, motorPower);

		// Pull up the belt
		pullBeltBackUp(SPEED_SLOW);

		return false;
	}
}

// Function that moves the robot back down based on the belt motor's encoder
void moveRobotBackDown(int motorPower)
{
	driveAllMotorsForClimbing(-motorPower, false);
	while (abs(nMotorEncoder[MOTOR_BELT]) > 0)
	{}
	driveAllMotorsForClimbing(0, false);

	return;
}

// Function that pulls the belt back up
void pullBeltBackUp(int motorPower)
{
	motor[MOTOR_BELT] = -motorPower;
	while (abs(nMotorEncoder[MOTOR_BELT]) > 0)
	{}
	motor[MOTOR_BELT] = 0;

	return;
}

// Function that averages 3 readings from the sound sensor and waits until the average sound level
// surpasses the minimum required sound level
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

	return;
}

// Function that drives the robot a given distance
void driveDist(int distance, int motorPower)
{
	nMotorEncoder[MOTOR_FL] = 0;
	float distToDrive = distance * CM_TO_ENC;
	driveMotorsFrontBack(motorPower, motorPower);
	while (abs(nMotorEncoder[MOTOR_FL]) < distToDrive)
	{}
	driveMotorsFrontBack(0,0);

	return;
}

// Function that moves the belt of the robot down a certain amount of encoder counts
void moveBeltDown(int motorPower, int amount)
{
	motor[MOTOR_BELT] = motorPower;
	while(abs(nMotorEncoder[MOTOR_BELT]) < amount)
	{}
	motor[MOTOR_BELT] = 0;

	return;
}

// Function that makes the robot go backwards and down steps
void climbDownAllSteps(int motorPower)
{
	// Reset belts motor encoder
	nMotorEncoder[MOTOR_BELT] = 0;

	// Move the belt down a certain amount, so the robot becomes a pyramid shape
	moveBeltDown(motorPower, ENC_LIMIT_FOR_CLIMB_DOWN);

	// Start going backwards
	driveMotorsFrontBack(-motorPower, -motorPower);
	wait1Msec(1000);

	// Keep climbing down until the bot is good to stop
	bool isGoodToStop = false;
	while(!isGoodToStop)
	{
		int change = 0, oldDeg = 0, newDeg = 0;
		time1[T1] = 0;

		// While loop that only exits if has been on a flat surface for 4 seconds
		while(time1[T1] < WAIT_TIME_FOR_CLIMB_DOWN)
		{
			//Check change in degrees
			oldDeg = abs(getGyroDegrees(S1));
			wait1Msec(50);
			newDeg = abs(getGyroDegrees(S1));
			change = abs(newDeg - oldDeg);

			// If the angle change is >5, reset the timer
			if(change > TILT_AMOUNT_FOR_CLIMB_DOWN)
			{
				//displayBigTextLine(5,"Went down a step");
				time1[T1] = 0;
			}
		}

		displayBigTextLine(5,"Stopping bot");
		isGoodToStop = true;
	}

	// Stop motors
	driveMotorsFrontBack(0, 0);

	// Straighten out robot
	pullBeltBackUp(motorPower);

	return;
}
