# Bankshot-Robot-Code
Code for a Bankshot Demonstration robot (Video: https://goo.gl/photos/xo2HMqQiTmndychLA)

Written in ROBOTC, using the VEX IQ platform to play in the 2015 - 2016 VEX IQ Game: Bankshot
https://www.youtube.com/watch?v=X9GIF34HEvE (animation)

Brief Code Overview:
- Drive controlled by two joysticks, skid-skeer drivetrain style. Left joystick controls forward-backward, right joystick controls left-right

- Spinning rollers controlled by controller suck in balls and index them for shooter mechanism. Coded to run at full speed when intaking and half speed when spitting balls out. Runs when triggers are pressed

- Spinning wheel controlled fires balls into goal. Wheel RPM maintained by bang-bang control loop to ensure consistent shot from set shooting position. LED lights indicate if wheel rpm is close enough to target rpm to score ball. Code maintains constant flywheel velocity to reduce spinup time. Flywheel toggles on or off via button on controller.

- L shaped mechanism pushes ball into flywheel shooter. Function used to automatically load ball and retract mechanism by moving mechanism 90 degrees and then retracting. Function prevents intake from being run during loading sequence to prevent balls jamming inside robot
