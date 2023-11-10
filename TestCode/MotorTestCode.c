
void driveMotors(int leftPower, int rightPower) // Make all motor inputs slow
{
  motor[motorD] = rightPower;
  motor[motorA] = leftPower;
  return;
}

task main ()
{
  driveMotors(50,50);
  wait1Msec(1000);
  driveMotors(0,0);
}
