void rotateRobot(int angle)
{
	int currentDegree = getGyroDegrees(S1);
	const int SPEED = 20;

	if (angle>0)
	{
		motor[motorA] = -SPEED;
		motor[motorD] = SPEED;
	}
	else
	{
		motor[motorA] = SPEED;
		motor[motorD] = -SPEED;
	}

	while (abs(getGyroDegrees(S1) - currentDegree)<abs(angle))
	{}

	motor[motorA] = motor[motorD] = 0;
}

task main()
{
	SensorType[S1] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S1] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);

	rotateRobot(45);
}
