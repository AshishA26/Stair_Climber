/*
The program assumes common.h, mindsensors-ev3smux.h and UW_sensorMux.c are in the same directory
as this file.
*/
#include "UW_sensorMux.c"

// Configure your sensor channel type by calling initSensorMux.  The following are available:
// colorReflectedLight
// colorAmbientLight
// colorMeasureColor
// gyroAngle
// gyroRate
// sonarCM
// sonarInches
// sonarPresence
// touchStateBump

/*
msensor specifies sensor port, and channel
e.g. msensor_S1_1 is sensor port S1 and channel C1
*/

/*
The example program assumes the sensor mux is plugged into S1, and the channels are:
touch sensor, ultrasonic sensor, and gyro
*/

task main()
{
// Configure sensor port
	SensorType[S1] = sensorEV3_GenericI2C;
	wait1Msec(100);

	// configure each channel on the sensor mux
	if (!initSensorMux(msensor_S1_1, touchStateBump))
		return;
	if (!initSensorMux(msensor_S1_2, sonarCM))
		return;
	if (!initSensorMux(msensor_S1_3, gyroAngle))
		return;

	while (!getButtonPress(buttonAny))
	{
		displayBigTextLine(4,"C1 %d",readMuxSensor(msensor_S1_1));
		displayBigTextLine(8,"C2 %d",readMuxSensor(msensor_S1_2));
		displayBigTextLine(12,"C3 %d",readMuxSensor(msensor_S1_3));
		wait1Msec(250);
	}
}
