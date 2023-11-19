const float SOUND_LEVEL = 30;
const float TIME_INTERVAL = 5;

void screamDetected (int soundLevel, float waitTime)
{
	float average = 0;
	while (average < soundLevel)
	{
		time1[T1] = 0;
		while (time1[T1] < waitTime)
		{}
		int reading1 = SensorValue(S2);
		while (time1[T1] < waitTime*2)
		{}
		int reading2 = SensorValue(S2);
		while (time1[T1] < waitTime*3)
		{}
		int reading3 = SensorValue(S2);
		average = (float)(reading1 + reading2 + reading3)/3.0;
		displayString(7, "%f", average);
	}
	displayString(5,"Too loud");
	wait1Msec(10000);
}

task main()
{
	SensorType[S2] = sensorSoundDB;
	screamDetected(SOUND_LEVEL, TIME_INTERVAL);
}
