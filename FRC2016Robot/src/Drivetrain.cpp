#include "Drivetrain.h"

#include <algorithm>
#include "smartdashboard/smartdashboard.h"
#include <Timer.h>
#include <Talon.h>
#include <CanTalon.h>
#include <Encoder.h>


using namespace std;


Drivetrain::Drivetrain(OperatorInputs *inputs, DriverStation *ds)
{
	LEFT_PORT = 0;
	SECOND_LEFT_PORT = 2;
	RIGHT_PORT = 1;
	SECOND_RIGHT_PORT = 3;
	SHIFT_PORT_LOW = 1;
	SHIFT_PORT_HIGH = 2;
	SHIFT_MODULE = 1;

	Encoder_Top_Speed = 20000;

	joyStickX = 0;
	joyStickY = 0;

	leftPow = 0;
	rightPow = 0;
	totalSpeed = 0;
	sleeptime = 1000;

	speedMult = 1;
	fixNum = 0;
	maxLeftEncoderRate = 0;
	maxRightEncoderRate = 0;
	ratio = 1;
	rightEncoderFix = 0;
	leftEncoderFix = 0;
	sleepTime = 0100;
	isHighGear = false; //Robot starts in low gear
	nemo = false;
	isLeftHigher = true;
	leftSpeed = 0;
	rightSpeed = 0;
	encoderDeadzone = 1000;
	encoderWaitTime = 168;
	leftChildProofSetter = 0;
	rightChildProofSetter = 0;
	childProofConfirmed = false;
	DISTANCE_PER_PULSE = 0.0006708;
	previousTriggerPressed = false; //what the trigger value was before the current press, allows for trigger to stay pressed w/o flipping
	previousLeftPow = 0;
	previousRightPow = 0;
	coasting = 1;
	teleopRamp = 0.08;

	operatorInputs = inputs;
	leftTalons = new CANTalon(LEFT_PORT);
	leftTalons1 = new CANTalon(SECOND_LEFT_PORT);
	rightTalons = new CANTalon(RIGHT_PORT);
	rightTalons1 = new CANTalon(SECOND_RIGHT_PORT);
	gearShift = new Solenoid(SHIFT_MODULE, SHIFT_PORT_LOW);

	leftEncoder = new Encoder(3, 4);
	rightEncoder = new Encoder(5, 6);
	timer = new Timer();
	driverstation = ds;
	//Start all wheels off
	leftTalons->Set(0);
	leftTalons1->Set(0);
	rightTalons->Set(0);
	rightTalons1->Set(0);

	leftTalons->SetFeedbackDevice(CANTalon::QuadEncoder);
	leftTalons1->SetFeedbackDevice(CANTalon::QuadEncoder);
	rightTalons->SetFeedbackDevice(CANTalon::QuadEncoder);
	rightTalons1->SetFeedbackDevice(CANTalon::QuadEncoder);
	leftTalons->ConfigEncoderCodesPerRev(1024);
	leftTalons1->ConfigEncoderCodesPerRev(1024);
	rightTalons->ConfigEncoderCodesPerRev(1024);
	rightTalons1->ConfigEncoderCodesPerRev(1024);

	//Starts in low gear
	gearShift->Set(!isHighGear);
	leftEncoder->SetDistancePerPulse(-DISTANCE_PER_PULSE);
	rightEncoder->SetDistancePerPulse(DISTANCE_PER_PULSE);

}

	    Drivetrain::~Drivetrain()
	    {
	    	delete leftTalons;
	    	delete leftTalons1;
	    	delete rightTalons;
	    	delete rightTalons1;
	    	delete gearShift;
	    	delete leftEncoder;
	    	delete rightEncoder;
	    	delete timer;
	    }

	    void Drivetrain::rampLeftPower(double desiredPow, double rampSpeed) { //Makes it so that robot can't go stop to full
	        if (abs(desiredPow - previousLeftPow) < rampSpeed) {
	            previousLeftPow = desiredPow;
	        } else if (previousLeftPow < desiredPow) {
	            previousLeftPow += rampSpeed;
	        } else if (previousLeftPow > desiredPow) {
	            previousLeftPow -= rampSpeed;
	        }
	        leftTalons->Set(-previousLeftPow);
	        leftTalons1->Set(-previousLeftPow);

	    }

	    void Drivetrain::rampRightPower(double desiredPow, double rampSpeed) { //Makes it so that robot can't go stop to full
	        if (abs(desiredPow - previousRightPow) < rampSpeed) {
	            previousRightPow = desiredPow;
	        } else if (previousRightPow < desiredPow) {
	            previousRightPow += rampSpeed;
	        } else if (previousRightPow > desiredPow) {
	            previousRightPow -= rampSpeed;
	        }
	        rightTalons->Set(previousRightPow);
	        rightTalons1->Set(previousRightPow);

	    }

	    void Drivetrain::resetEncoders() { //Resets current raw encoder value to 0
	        leftEncoder->Reset();
	        rightEncoder->Reset();
	        gearShift->Set(!isHighGear);
	    }



	    double Drivetrain::getRightPulses() { //Returns raw value of right talon encoder
	        return rightEncoder->GetRaw();
	    }

	    double Drivetrain::getLeftPulses() { //Returns raw value of left talon encoder
	        return leftEncoder->GetRaw();
	    }

	    double Drivetrain::getRightEncoderDistance() { //Returns the distance per pulse of the right encoder
	        return rightEncoder->GetDistance();
	    }

	    double Drivetrain::getLeftEncoderDistance() { //Returns the distance per pulse of the left encoder
	        return leftEncoder->GetDistance();
	    }

	    double Drivetrain::fix(double v) {
	        return v / fixNum;
	    }

	    double Drivetrain::LeftMotor() {
	        double fixLeftPow = fix(leftPow);
	        //moved rightSpeed to class scope, it is being set in setPower()

	        if (leftPow != 0 && rightPow != 0) {
	            maxLeftEncoderRate = abs(leftSpeed / leftPow);
	            if (min(abs(leftSpeed), abs(rightSpeed)) > encoderDeadzone) {
	                breakTime();
	            }
	            if (isLeftHigher) {
	                fixLeftPow = ratio * fixLeftPow;
	            }
	            leftChildProofSetter = abs(fixLeftPow);
	        }

	        return (fixLeftPow);
	    }

	    double Drivetrain::RightMotor() {


	        //moved rightSpeed to class scope, it is being set in setPower()
	        double fixRightPow = fix(rightPow);

	        if (leftPow != 0 && rightPow != 0) {
	            maxRightEncoderRate = abs(rightSpeed / rightPow);
	            if (min(abs(leftSpeed), abs(rightSpeed)) > encoderDeadzone) {
	                breakTime();
	            }
	            if (!isLeftHigher) {
	                fixRightPow = ratio * fixRightPow;
	            }
	            rightChildProofSetter = abs(fixRightPow);
	        }

	        return (fixRightPow);

	    }

	    void Drivetrain::compareEncoders() { //If left motor speed is bigger than the right motor speed return true, else false
	        if (maxRightEncoderRate > maxLeftEncoderRate) {
	            ratio = maxLeftEncoderRate / maxRightEncoderRate;
	            leftEncoderFix = maxRightEncoderRate * ratio;
	            isLeftHigher = false;

	        } else if (maxLeftEncoderRate > maxRightEncoderRate) {
	            ratio = maxRightEncoderRate / maxLeftEncoderRate;
	            rightEncoderFix = maxLeftEncoderRate * ratio;
	            isLeftHigher = true;
	        } else {
	            ratio = 1;

	        }
	    }

	    void Drivetrain::breakTime() {
	        SmartDashboard::PutNumber("Ratio", ratio);
	        SmartDashboard::PutBoolean("Left > Right", isLeftHigher);
	        SmartDashboard::PutNumber("Timer time", timer->Get());
	        if (timer->Get() > encoderWaitTime) {
	            compareEncoders();
	            timer->Reset();
	        }
	    }

	    void Drivetrain::setPower() {
	        joyStickX = operatorInputs->joystickX();
	        joyStickY = operatorInputs->joystickY();
	        //set fixnum = the maxiumum value for this angle on the joystick
	        if (joyStickX == 0 || joyStickY == 0) {
	            fixNum = 1;
	        } else {
	            if (abs(joyStickX) > abs(joyStickY)) {
	                double fixNumMult = 1 / abs(joyStickX);
	                fixNum = abs(joyStickY) * fixNumMult + 1;
	            } else {
	                double fixNumMult = 1 / abs(joyStickY);
	                fixNum = abs(joyStickX) * fixNumMult + 1;
	            }
	        }
	        leftPow = -joyStickY + joyStickX;
	        rightPow = -joyStickY - joyStickX;
	        leftSpeed = leftEncoder->GetRate();
	        rightSpeed = rightEncoder->GetRate();

	        rampLeftPower(coasting * LeftMotor(), 0.5 / driverstation->GetInstance().GetBatteryVoltage()); //Left Motors are forward=negative
	        SmartDashboard::PutNumber("LeftTalons", -leftTalons->Get()); //Left Motors are forward=negative
	        SmartDashboard::PutNumber("LeftSpeed", -leftSpeed); //Left Motors are forward=negative

	        rampRightPower(coasting * RightMotor(), 0.5 / driverstation->GetInstance().GetBatteryVoltage()); //Right Motors are forward=positive
	        SmartDashboard::PutNumber("JoystickY", joyStickY);
	        SmartDashboard::PutNumber("RightTalons", rightTalons->Get()); //Right Motors are forward=positive
	        SmartDashboard::PutNumber("RightSpeed", rightSpeed); //Right Motors are forward=positive
	        //System.out.println("High gear :" + isHighGear);
	    }


	    void Drivetrain::shift() {//current setting is start in high gear
	        bool triggerPressed = operatorInputs->joystickTriggerPressed();
	        //System.out.println("Trigger Pressed :" + triggerPressed);
	        if (triggerPressed && !previousTriggerPressed) {
	        	if(isHighGear){
	        		if(leftTalons->GetEncVel() < Encoder_Top_Speed){
		        		leftTalons->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
		        		leftTalons1->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
		        		rightTalons->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
		        		rightTalons1->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
		        		isHighGear = !isHighGear;
		        		gearShift->Set(isHighGear);
		        		leftTalons->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
						leftTalons1->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
						rightTalons->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
						rightTalons1->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	        		}else{
	        			/**
	        			while(leftTalons->GetEncVel() < Encoder_Top_Speed){
	        				rampLeftPower

	        			}
	        			*/


	        		}

	        	}
	        	else{
	        		leftTalons->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	        		leftTalons1->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	        		rightTalons->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	        		rightTalons1->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
	        		isHighGear = !isHighGear;
	        		gearShift->Set(isHighGear);
	        		leftTalons->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	        		leftTalons1->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	        		rightTalons->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	        		rightTalons1->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	        	}
	            isHighGear = !isHighGear;
	            //Shifts gear
	            gearShift->Set(isHighGear);

	        }

	        previousTriggerPressed = triggerPressed;

	    }


	    void Drivetrain::setSpeedPositive() {
	        totalSpeed = (leftPow + rightPow) / 2;
	        if (isHighGear == true) {
	            totalSpeed = (leftPow + rightPow);
	        }
	        if (totalSpeed < 0) {
	            totalSpeed = -totalSpeed;
	        }
	    }

	    void Drivetrain::childProofing() { //Low to high and speed, High to low when speed is under a certain value

	        if (rightChildProofSetter < .75 && leftChildProofSetter < .75) {
	            childProofConfirmed = true;
	        } else {
	            childProofConfirmed = false;
	        }
	    }

	    void Drivetrain::setCoasting(double newCoasting) {
	        coasting = newCoasting;
	    }



