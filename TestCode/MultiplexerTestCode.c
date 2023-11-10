'  ** Sensor MUX Test 2 **

'  By David Musgrave

'  http://WinthropDC.com

'  Last Modified: 22-Aug-2017

' ** Setup Starts Here *******************************************************************************************************

' Initialise Variables
Finished = "False"
Clicks = ""

' Mindsensors.com EV3 Sensor Multiplexer
I2CPort = 4
I2CBus = 80
I2CChannel = 0
I2CAddr = 0
I2CReg = 0
I2CWriteByte = 0
I2CWriteData = Vector.Init(2, 0)
I2CReadByte = 0
I2CReadData = Vector.Init(2, 0)

UltraSonic = 0
Gyro = 0
Light = 0

' ** Subroutines Starts Here *******************************************************************************************************

Sub InitUltrasonic
  I2CChannel = 1
  I2CAddr = I2CBus + 1 * (I2CChannel - 1)
  I2CReg = 82 ' 0x52 Sensor Mode

  Sensor.WriteI2CRegister(I2CPort, I2CAddr, I2CReg, 0) ' Ultrasonic Mode 0 = CM
  ReadUltrasonic()
EndSub

Sub ReadUltrasonic ' Out: UltraSonic  in cm
  I2CChannel = 1
  I2CAddr = I2CBus + 1 * (I2CChannel - 1)
  I2CReg = 84 ' 0x54 Sensor Data
  I2CReadData = Sensor.ReadI2CRegisters(I2CPort, I2CAddr, I2CReg, 2)

  UltraSonic = (I2CReadData[1] * 256 + I2CReadData[0]) / 10
EndSub

Sub InitLight
  I2CChannel = 2
  I2CAddr = I2CBus + 1 * (I2CChannel - 1)
  I2CReg = 82 ' 0x52 Sensor Mode
  Sensor.WriteI2CRegister(I2CPort, I2CAddr, I2CReg, 0) ' Color Sensor Mode 0 = Reflected

  ReadLight()
EndSub

Sub ReadLight ' Out: Light
  I2CChannel = 2
  I2CAddr = I2CBus + 1 * (I2CChannel - 1)
  I2CReg = 84 ' 0x54 Sensor Data
  I2CReadData = Sensor.ReadI2CRegisters(I2CPort, I2CAddr, I2CReg, 2)

  Light = I2CReadData[1] * 256 + I2CReadData[0]
EndSub

Sub InitGyro
  I2CChannel = 3
  I2CAddr = I2CBus + 1 * (I2CChannel - 1)
  I2CReg = 82 ' 0x52 Sensor Mode
  Sensor.WriteI2CRegister(I2CPort, I2CAddr, I2CReg, 1) ' Gyro Mode 1 = Rate
  Sensor.WriteI2CRegister(I2CPort, I2CAddr, I2CReg, 0) ' Gyro Mode 0 = Angle

  ReadGyro()
EndSub

Sub ReadGyro ' Out: Gyro in Degrees
  I2CChannel = 3
  I2CAddr = I2CBus + 1 * (I2CChannel - 1)
  I2CReg = 84 ' 0x54 Sensor Data
  I2CReadData = Sensor.ReadI2CRegisters(I2CPort, I2CAddr, I2CReg, 2)

  Gyro = I2CReadData[1] * 256 + I2CReadData[0]
  If Gyro >= 32768 Then
    Gyro = Gyro - 65536
  EndIf
  Gyro = -Gyro ' Inverted due to mounting position
EndSub

Sub ResetGyro
  ReadGyro()
  While Gyro <> 0
    InitGyro()
    Program.Delay(15)
  EndWhile
EndSub

' ** Main Program Starts Here *******************************************************************************************************

LCD.Clear()
LCD.Write( 0*8, 0*10, "SensorMUX Test")
LCD.Write( 0*8, 2*10, "Port: " + I2CPort)
LCD.Write(10*8, 2*10, "Address: " + I2CBus)

InitGyro()
InitUltrasonic()
InitLight()
While Finished = "False"
  ReadUltrasonic()
  LCD.Write(0*8, 4*10, "Ultrasonic: "+ Text.GetSubText("     "+(Ultrasonic),Text.GetLength((Ultrasonic)),6) +" cm")
  ReadGyro()
  LCD.Write(0*8, 6*10, "Gyro      : "+ Text.GetSubText("     "+(Gyro),Text.GetLength((Gyro)),6))
  ReadLight()
  LCD.Write(0*8, 8*10, "Light     : "+ Text.GetSubText("     "+(Light),Text.GetLength((Light)),6))

  Clicks = Buttons.GetClicks()
  If Text.IsSubText(Clicks, "E") Then
    Finished = "True"
  EndIf
EndWhile
