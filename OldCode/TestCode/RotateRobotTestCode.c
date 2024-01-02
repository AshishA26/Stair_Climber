void rotateRobot(int angle)
{
	int currentDegree = getGyroDegrees(S2);
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

	while (abs(getGyroDegrees(S4) - currentDegree)<abs(angle))
	{}

	motor[motorA] = motor[motorD] = 0;
}

task main()
{
	SensorType[S2] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S2] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S2] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);

	rotateRobot(45);
}
