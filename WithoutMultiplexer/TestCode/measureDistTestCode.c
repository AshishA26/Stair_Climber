const float TIME_INTERVAL = 5;

float measureDist(float waitTime)
{
	float average = 0;
	time1[T1] = 0;
	while (time1[T1] < waitTime)
	{}
	int reading1 = SensorValue(S4);
	while (time1[T1] < waitTime*2)
	{}
	int reading2 = SensorValue(S4);
	while (time1[T1] < waitTime*3)
	{}
	int reading3 = SensorValue(S4);
	average = (float)(reading1 + reading2 + reading3)/3.0;
	return average;
}

task main()
{
	//motor[motorA]=motor[motorD]=motor[motorB] = 25;
	time1[T2] = 0;
	while(time1[T2] < 10000)
	{
		float average = measureDist(TIME_INTERVAL);
		displayString(5, "%f", average);
	}
}
