# Stair_Climber
A project made for MTE100 and MTE121 that is able to deliver objects while climbing stairs

## PseudoCode:

``````
Reset Sensors and Motor Encoders as Required

Display Group Name, Robot Number, Robot Name

While sound threshold not met, don't start (Make a function that takes sound measurements as a 3 point average over 15ms)

Play notification sound

**Everything tabbed exists in a do while loop, condition(touch sensor not activated, forward distance within a step length)

Drive forward at a steady speed, while ultrasonic min dist not met (Make a function that takes dist measurements as a 3 point average over 15ms)

Drive slowly for 5 seconds

While ultrasonic max dist not met or max height not met, begin climibing the belt and front wheels simultaneously and a slow rate (speed to be determined based on results)

IF max height reached, go back down, using motorencoders and play error noise and end program

ELSE drive distance of robot length, stop, pick up belt using motorencoder

While color not correct, drive until color is reached

Play notification sound

End Program

---

Project Extension, IF WE HAVE TIME

**Back in the climbing phase, store the distance climbed and distance driven from edge, remember this all only happens if we sucessfully climbed.

Drive back exactly the distance driven, put down the belt slight more than recorded via encoder

Slowly Drive back until distance of bot driven, once reached immediately drive front wheels slowly back and back wheels only a tiny bit forward (This part would take lots of testing)

Drive both simultaneously down at the same rate until motor encoder amount met

Driver back, turn around, and arrive at start.

Play notification

End Program
```