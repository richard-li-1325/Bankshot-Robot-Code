#pragma config(Sensor, port8,  light1,         sensorVexIQ_LED)
#pragma config(Sensor, port9,  light2,         sensorVexIQ_LED)
#pragma config(Motor,  motor1,          intake,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor2,          shooter,       tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor3,          feeder,        tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor7,          left,          tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor12,         right,         tmotorVexIQ, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


void shoot() {		//function to load the ball into the shooter
	float feederPos = 270;	//270 degrees because it moves 90 degrees total, but in a 3:1 gearbox)
	while (getMotorEncoder(feeder) != 270) //moves the ball into shooter
		setMotorTarget(feeder, feederPos, 100);

	setMotorTarget(feeder, 0, -100);		//moves feeder back to starting position at the end of the cycle
}


task main()	{
	resetMotorEncoder(shooter);	//resets shooter encoder
	resetMotorEncoder(feeder);	//resets current position of feeder to zero

	setMotorBrakeMode(shooter, motorCoast);		//sets shooter motor to coast so shooter won't spin down as fast when flywheel is off (e.g., when collecting balls)
	setMotorBrakeMode(left, motorCoast);		//sets drive motors to coast to allows robot to be pushed up ramp
	setMotorBrakeMode(right, motorCoast);

	setMotorReversed(intake, true);					//reverses intake motor because of position
	setMotorReversed(right, true);					//reverses right drive motor because of position

	const float shooterTarget = 145;					//target shooter angular velocity
	int loopCounter = 0;							//counts number of time the main while loop cycles for tracking shooter angular velocity
	bool loading = false;							//prevents user from loading ball into chamber when robot is in the process of firing

	bool flywheelOn = true;								//boolean for if the flywheel is on (starts flywheel at the start of the match so time isn't wasted on spinup
	bool readyToFire = false;							//lets driver know if flywheel is at optimal rpm
	bool eUpPressed = false;							//boolean for turning the flywheel on and off via E up button

	while (true) {

		motor[left] = vexRT(ChA) + vexRT(ChC);		//simple drive code
		motor[right] = vexRT(ChA) - vexRT(ChC);

		if (readyToFire) {							//LEDs will glow blue if at optimal rpm
			setTouchLEDColor(light1, colorBlue);
			setTouchLEDColor(light2, colorBlue);
		}
		else {
			setTouchLEDColor(light1, colorNone);
			setTouchLEDColor(light2, colorNone);
		}

		if (vexRT(BtnRUp) == 1) {								//code to run the intake forwards or backwards
			if (loading == false) {
				setMotorSpeed(intake, 100);
			}
		}
		else if (vexRT(BtnRDown) == 1) {
			setMotorSpeed(intake, -50);
		}
		else {
			setMotorSpeed(intake, 0);
		}


		if (vexRT(BtnLUp) == 1) {				//calls the shoot function to fire ball
			shoot();
		}

		if (vexRT(BtnEUp) == 1) {				//toggles flywheel on and off when E up is pressed
			if (eUpPressed == false) {
				eUpPressed = true;
				flywheelOn = !flywheelOn;
			}

		}
		else {
			eUpPressed = false;
		}

		if (flywheelOn == false) {
			motor[shooter] = 0;
		}

		loopCounter++;					//increases loop count by 1 for each time the while loop runs

		if (loopCounter == 2000) {			//bang bang control loop for shooter motor
			if (flywheelOn) {
				if (getMotorEncoder(shooter) < shooterTarget) {	//spins up if not at optimal rpm
					setMotorSpeed(shooter, 100);
				}

				else {				//turns shooter motor off if target speeds is reached or exceeded
					motor[shooter] = 0;
				}
			}

			if (shooterTarget - getMotorEncoder(shooter) < 25) {      //if shooter velocity is close enough to target velocity, system is ready to fire
				readyToFire = true;
			}
			else {
				readyToFire = false;
			}
			resetMotorEncoder(shooter);		//resets encoder count to 0 every 2000 times the while loops is run (used to track shooter angular velocity)
			loopCounter = 0;
		}
	}
}
