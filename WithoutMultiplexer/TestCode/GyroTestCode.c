task main()
{
	SensorType[S1] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S1] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);

	time1[T2] = 0;
	while(time1[T2] < 10000)
	{
		displayString(8,"%d",getGyroDegrees(S1));
	}

}
