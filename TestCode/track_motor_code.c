void driveMotorsFrontWithBelt(int motorPower) // drive only front motors and belt
{
	motor[motorD] = motor[motorA] = motorPower;
	motor[motorC] = motorPower;
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

	SensorType[S3] = sensorEV3_Touch;
	wait1Msec(50);

	nMotorEncoder[motorC] = 0;


	//Touch Sensor test code
  motor[motorC] = 20;
	while (SensorValue[S3] == 0)
	{}
	pullBeltBackUp(20);

		//moveBeltBackDown(20);
	//pullBeltBackUp(20);

	//wait1Msec(1000);
	//driveMotorsFrontWithBelt(20);
	//wait1Msec(1000);
	//driveMotorsFrontWithBelt(0);
}
