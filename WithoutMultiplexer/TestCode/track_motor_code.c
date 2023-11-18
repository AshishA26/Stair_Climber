void driveMotorsFrontWithBelt(int motorPower) // drive only front motors and belt
{
	motor[motorD] = motor[motorA] = motorPower;
	motor[motorC] = -motorPower;
	return;
}

void moveBeltBackDown(int motorPower)
{
    motor[motorC] = motorPower;
		while(abs(nMotorEncoder(motorC)) < 400)
		{}
    motor[motorC] = 0;
}

void pullBeltBackUp(int motorPower)
{
	motor[motorC] = -motorPower;
	while(abs(nMotorEncoder(motorC)) > 0)
	{}
	motor[motorC] = 0;
}

task main()
{
	nMotorEncoder[motorC] = 0;
	moveBeltBackDown(20);
	pullBeltBackUp(20);
}
