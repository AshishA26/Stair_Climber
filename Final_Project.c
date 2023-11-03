/*
  Samuel Ke, Ashish Agrahari, Svamin Bhatnagar, and Jacob Zhuang
  Lego EV3 Group 8-8
  11/03/2023
*/

void configureSensors();

task main()
{
	configureSensors();

}

void configureSensors() //Sensor #s to be determined and added
{
	SensorType[] = sensorEV3_Touch;

	SensorType[] = sensorEV3_Ultrasonic;

	SensorType[] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[] = modeEV3Color_Color;
	wait1Msec(50);

	SensorType[] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
}
