const int CM_TO_ENC = 360/(2.0*PI*3.4);

void driveMotorsFrontBack(int motorPower)
{
	motor[motorD] = motor[motorA] = motorPower;
	motor[motorB] = -motorPower;
	return;
}

void driveMotorsFront(int leftPower, int rightPower)
{
	motor[motorD] = rightPower;
	motor[motorA] = leftPower;
	return;
}

void driveMotorsFrontWithBelt(int motorPower)
{
	motor[motorD] = motor[motorA] = motorPower;
	motor[motorC] = -motorPower;
	return;
}

void driveDist(int distance, int power)
{
	nMotorEncoder(motorA) = 0;
	float distToDrive = distance*CM_TO_ENC;
	driveMotorsFrontBack(power);
	while (fabs(nMotorEncoder(motorA)) < distToDrive)
	{}
	driveMotorsFrontBack(0);
}

task main ()
{
  //driveMotorsFront(20, 20);
  //wait1Msec(1000);
  //driveMotorsFront(0, 0);

  //driveMotorsFrontBack(20);
  //wait1Msec(1000);
  //driveMotorsFrontBack(0);

  //driveMotorsFrontWithBelt(20);
  //driveMotorsFrontBack(20);
  //wait1Msec(1000);
  //driveMotorsFrontWithBelt(0);
  //driveMotorsFrontBack(0);

	//driveDist(20, 20);
}
