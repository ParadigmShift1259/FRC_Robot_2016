//  drivetrain.cpp

#include "Const.h"
#include "Drivetrain.h"
#include "smartdashboard/smartdashboard.h"
#include <Timer.h>
#include <Talon.h>
#include <CANTalon.h>
#include <Solenoid.h>
#include <Encoder.h>
#include <cmath>

using namespace std;

#ifdef FIRSTDRIVE

/**
 * Constructs all the motors and solenoids needed for  drive
 *
 * Sets the slave talons to follow mode
 * Sets the motors to coast
 * Sets the sensors to quadrature encoders
 */
Drivetrain::Drivetrain(OperatorInputs *inputs, DriverStation *ds) {
	m_inputs = inputs;
	m_driverstation = ds;
	m_userforward = START_FORWARD;
	m_shiftstate = START_HIGH
	;

	m_lefttalonlead = new CANTalon(CAN_MASTER_LEFT_PORT);
	m_lefttalonfollow = new CANTalon(CAN_SLAVE_LEFT_PORT);

	m_righttalonlead = new CANTalon(CAN_MASTER_RIGHT_PORT);
	m_righttalonfollow = new CANTalon(CAN_SLAVE_RIGHT_PORT);

	m_shifter = new Solenoid(PCM_SHIFT_MODULE, PCM_SHIFT_PORT_LOW);

	m_lefttalonlead->SetControlMode(CANTalon::kVoltage);
	m_righttalonlead->SetControlMode(CANTalon::kVoltage);

	m_lefttalonfollow->SetControlMode(CANTalon::kFollower);
	m_righttalonfollow->SetControlMode(CANTalon::kFollower);

	m_lefttalonfollow->Set(CAN_MASTER_LEFT_PORT);
	m_righttalonfollow->Set(CAN_MASTER_RIGHT_PORT);

	m_lefttalonlead->SetFeedbackDevice(CANTalon::QuadEncoder);
	m_righttalonlead->SetFeedbackDevice(CANTalon::QuadEncoder);

	m_lefttalonlead->SetVoltageRampRate(PERCENT_RAMP_PER_SECOND);
	m_righttalonlead->SetVoltageRampRate(PERCENT_RAMP_PER_SECOND);

	m_lefttalonlead->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Coast);
	m_lefttalonfollow->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Coast);
	m_righttalonlead->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Coast);
	m_righttalonfollow->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Coast);

	m_lefttalonlead->SetInverted(LEFT_INVERTED);
	m_lefttalonfollow->SetInverted(LEFT_INVERTED);
	m_righttalonlead->SetInverted(RIGHT_INVERTED);
	m_righttalonfollow->SetInverted(RIGHT_INVERTED);

}

/**
 * Autonomous is not done yet, so this has not been implemented
 */
void Drivetrain::AutonoumousLoop() {
	m_shifter->Set(m_shiftstate);
}

/**
 * Teleop Loop code
 */
void Drivetrain::TeleopLoop() {
	//checks to make sure the user should be moving the robot
	if (m_usercontrol && !m_shiftbraking) {
		//checks the current shift state to choose if the high gear scales or low gear scales are used
		double magnitude = m_inputs->xBoxLeftX();
		//this may need to change based on the definition of forward (the not may need to be removed)
		//todo figure this out
		if (!m_userforward) {
			magnitude *= -1.0;
		}
		//calculates the drive outputs based on the magnitudes given
		if (m_shiftstate) {
			Drive((magnitude * DRIVE_SCALE_MAGNITUDE_LOW_GEAR),
					(m_inputs->xBoxLeftY() * DRIVE_SCALE_ROTATION_LOW_GEAR));
		} else {
			Drive((magnitude * DRIVE_SCALE_MAGNITUDE_HIGH_GEAR),
					(m_inputs->xBoxLeftY() * DRIVE_SCALE_ROTATION_HIGH_GEAR));
		}
	}
	//takes care of updating the current state
	UpdateStates();
	//starts shifting
	if (m_inputs->xBoxLeftTrigger()) {
		if (m_requestshift) {
			DisableShift();
		} else {
			Shift();
		}
	}
	//display the encoder rotation positions
	cout << "Left Rotations" << GetLeftRotations() << endl;
	cout << "Right Rotations" << GetRightRotations() << endl;
}

/**
 * drives using the robot drive class, this should never be changed
 * it is for interfacing with vision
 */
void Drivetrain::Drive(double magnitude, double direction) {
	m_robotdrive->ArcadeDrive(magnitude, direction);
}

void Drivetrain::DisableShift() {
	m_requestshift = false;
	m_shiftbraking = false;
}

/**
 * changes the direction of the robot, does not affect vision
 */
bool Drivetrain::ChangeDirection() {
	m_userforward = !m_userforward;
	return m_userforward;
}

/**
 * disables user code, meant only for vision targeting and autonomous
 * VERY DANGEROUS, USE WITH CAUTION
 */
void Drivetrain::DisableUserControl() {
	m_usercontrol = false;
}

/**
 * Enables the user after the automated process has finished, or can be called if a user interrupt has occured
 */
void Drivetrain::EnableUserControl() {
	m_usercontrol = true;
}

/**
 * MUST be called if using the shift function
 * Does not automatically check if shift is done, it just checks the state and reflects
 * the braking state in the motor controllers
 */
void Drivetrain::UpdateStates() {
	//brakes if in the braking state
	if (m_shiftbraking) {
		m_lefttalonlead->ConfigNeutralMode(
				CANSpeedController::NeutralMode::kNeutralMode_Brake);
		m_lefttalonfollow->ConfigNeutralMode(
				CANSpeedController::NeutralMode::kNeutralMode_Brake);
		m_righttalonlead->ConfigNeutralMode(
				CANSpeedController::NeutralMode::kNeutralMode_Brake);
		m_righttalonfollow->ConfigNeutralMode(
				CANSpeedController::NeutralMode::kNeutralMode_Brake);
		Stop();
	} else {
		//otherwise coast
		m_lefttalonlead->ConfigNeutralMode(
				CANSpeedController::NeutralMode::kNeutralMode_Coast);
		m_lefttalonfollow->ConfigNeutralMode(
				CANSpeedController::NeutralMode::kNeutralMode_Coast);
		m_righttalonlead->ConfigNeutralMode(
				CANSpeedController::NeutralMode::kNeutralMode_Coast);
		m_righttalonfollow->ConfigNeutralMode(
				CANSpeedController::NeutralMode::kNeutralMode_Coast);
	}
	//checks if further braking is required
	if (m_requestshift) {
		Shift();
	}
	//shift to the current state
	m_shifter->Set(m_shiftstate);
}

/**
 * stops the drivetrain, restores user control, and resets the shift request
 */
void Drivetrain::Stop() {
	m_requestshift = false;
	m_shiftbraking = false;
	m_usercontrol = true;
	Drive(0, 0);
}

/**
 * requests a shift if over the speed limit
 * if under the speed limit, changes the shift state
 */
void Drivetrain::Shift() {
	double currentrightspeed = DRIVE_ENC_CPR * m_righttalonlead->GetSpeed();
	double currentleftspeed = DRIVE_ENC_CPR * m_lefttalonlead->GetSpeed();
	if (currentrightspeed > DRIVE_ENC_CPR * MAX_RPS_FOR_SHIFT || currentleftspeed > DRIVE_ENC_CPR * MAX_RPS_FOR_SHIFT) {
		m_shiftbraking = true;
		m_requestshift = true;
	} else {
		m_shiftstate = !m_shiftstate;
		m_shiftbraking = false;
		m_requestshift = false;
	}
}

/**
 * manuall override method for the shift speed check
 */
void Drivetrain::OverwrittenShift() {
	m_shiftstate = !m_shiftstate;
	m_shifter->Set(m_shiftstate);
}

/**
 * resets the encoder positions to zero
 */
void Drivetrain::ResetEncoderPosition() {
	m_lefttalonlead->SetPosition(0);
	m_righttalonlead->SetPosition(0);
}

#else

Drivetrain::Drivetrain(OperatorInputs *inputs, DriverStation *ds) {
	m_inputs = inputs;
	m_driverstation = ds;

	m_lefttalonlead = new CANTalon(CAN_MASTER_LEFT_PORT);
	m_lefttalonfollow = new CANTalon(CAN_SLAVE_LEFT_PORT);
// set leftTalons1 to follow leftTalons
//	m_lefttalonfollow->SetControlMode(CANSpeedController::ControlMode::kFollower);
//	m_lefttalonfollow->Set(CAN_MASTER_LEFT_PORT);

	m_righttalonlead = new CANTalon(CAN_MASTER_RIGHT_PORT);
	m_righttalonfollow = new CANTalon(CAN_SLAVE_RIGHT_PORT);
// set rightTalons1 to follow rightTalons
//	m_righttalonfollow->SetControlMode(CANSpeedController::ControlMode::kFollower);
//	m_righttalonfollow->Set(CAN_MASTER_RIGHT_PORT);

	m_lefttalonlead->Set(0);
	m_lefttalonfollow->Set(0);
//	m_lefttalonlead->SetFeedbackDevice(CANTalon::QuadEncoder);
//	m_lefttalonlead->ConfigEncoderCodesPerRev(1024);
	m_righttalonlead->Set(0);
	m_righttalonlead->Set(0);
//	m_righttalonlead->SetFeedbackDevice(CANTalon::QuadEncoder);
//	m_righttalonlead->ConfigEncoderCodesPerRev(1024);

//todo why use brake mode?
	m_lefttalonlead->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_lefttalonfollow->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_righttalonlead->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_righttalonfollow->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Brake);

//Setup Encoders
//leftEncoder = new Encoder(3, 4);
//rightEncoder = new Encoder(5, 6);
//leftEncoder->SetDistancePerPulse(-CAN_DISTANCE_PER_PULSE);
//rightEncoder->SetDistancePerPulse(CAN_DISTANCE_PER_PULSE);
	m_leftencoderfix = 0;
	m_rightencoderfix = 0;
	m_leftencodermax = 0;
	m_rightencodermax = 0;
	m_leftspeed = 0;
	m_rightspeed = 0;
	m_leftposition = 0;
	m_rightposition = 0;

	m_shifter = new Solenoid(PCM_SHIFT_MODULE, PCM_SHIFT_PORT_LOW);
// Robot starts in low gear
	m_ishighgear = false;
// Starts in low gear
	m_shifter->Set(FLIP_HIGH_GEAR ^ m_ishighgear);
	m_isdownshifting = false;

	m_leftpow = 0;
	m_rightpow = 0;
	m_ratiolr = 1;
	m_isleftfaster = true;
	m_previousx = 0;
	m_previousy = 0;
	m_coasting = 1;
	m_invertleft = INVERT_LEFT;
	m_invertright = INVERT_RIGHT;
	m_direction = DT_DEFAULT_DIRECTION;

	m_timerencoder = new Timer();
}

void Drivetrain::Init() {
	m_leftpow = 0;
	m_rightpow = 0;
	m_leftencodermax = 0;
	m_rightencodermax = 0;
	m_ratiolr = 1;
	m_isleftfaster = true;
	m_leftspeed = 0;
	m_rightspeed = 0;
	m_leftposition = 0;
	m_rightposition = 0;
	m_previousx = 0;
	m_previousy = 0;
	m_coasting = 1;
	m_lefttalonlead->Set(0);
	m_lefttalonfollow->Set(0);
	m_righttalonlead->Set(0);
	m_righttalonfollow->Set(0);
	m_timerencoder->Reset();
}

void Drivetrain::Loop() {
	static unsigned int loopcnt = 0;
	static unsigned int shiftcnt = 0;
	double x;
	double y;

//x = m_inputs->joystickX()+
	x = m_inputs->xBoxLeftX();

	if (m_isdownshifting)
	y = 0;
	else
	y = m_inputs->xBoxLeftY();

	Drive(x, y, true);

	bool shift = m_inputs->xBoxLeftTrigger();
//if (m_inputs->joystickTrigger() ||
	if (shift) {
		shiftcnt += 4;
		if (m_ishighgear) {
			m_isdownshifting = true;
			shiftcnt += 2;
		} else {
			Shift();
			m_isdownshifting = false;
			shiftcnt += 1;
		}
	}
	SmartDashboard::PutNumber("SHIFT1_x", abs(m_previousx * X_SCALING));
	SmartDashboard::PutNumber("SHIFT2_y", abs(m_previousy * Y_SCALING));
	SmartDashboard::PutNumber("SHIFT3_Top", ENCODER_TOP_SPEED);
	SmartDashboard::PutNumber("SHIFT4_Count", loopcnt);
	SmartDashboard::PutNumber("SHIFT5_SHIFT", shift);
	SmartDashboard::PutNumber("SHIFT6_SCNT", shiftcnt);
	SmartDashboard::PutNumber("SHIFT7_IS", m_isdownshifting);
	SmartDashboard::PutNumber("SHIFT8_absx",
			(abs(m_previousx * X_SCALING) < ENCODER_TOP_SPEED));
	SmartDashboard::PutNumber("SHIFT9_absy",
			(abs(m_previousy * Y_SCALING) < ENCODER_TOP_SPEED));
	if (m_isdownshifting && (abs(m_previousx * X_SCALING) < ENCODER_TOP_SPEED)
			&& (abs(m_previousy * Y_SCALING) < ENCODER_TOP_SPEED)) {
		loopcnt++;
		Shift();
		m_isdownshifting = false;
	}
}

void Drivetrain::Stop() {
	m_ishighgear = false;
	m_shifter->Set(true ^ m_ishighgear); //todo explain this
}

void Drivetrain::Drive(double x, double y, bool ramp) {

	double yd = y * m_direction;
	double maxpower;
	if (x == 0 || yd == 0) {
		maxpower = 1;
	} else {
		if (abs(x) > abs(yd))
		maxpower = (abs(yd) / abs(x)) + 1;
		else
		maxpower = (abs(x) / abs(yd)) + 1;
	}
	if (!ramp) {
		m_previousx = x; //rampInput(previousX, joyStickX, BatteryRampingMin, BatteryRampingMax);
		m_previousy = yd;
		m_leftpow = m_previousy - m_previousx;
		m_rightpow = m_previousy + m_previousx;
	} else {
		double rampmin = RAMPING_RATE_MIN
		/ m_driverstation->GetInstance().GetBatteryVoltage();
		double rampmax = RAMPING_RATE_MAX
		/ m_driverstation->GetInstance().GetBatteryVoltage();
		SmartDashboard::PutNumber("DT6_BatVol",
				m_driverstation->GetInstance().GetBatteryVoltage());
		;
		m_previousx = x;	//rampInput(previousX, joyStickX, rampmin, rampmax);
		m_previousy = Ramp(m_previousy, yd, rampmin, rampmax);
		m_leftpow = m_previousy * Y_SCALING - m_previousx * X_SCALING;
		m_rightpow = m_previousy * Y_SCALING + m_previousx * X_SCALING;
	}
	/*m_leftspeed = m_lefttalonlead->GetSpeed();
	 m_rightspeed = m_righttalonlead->GetSpeed();
	 m_leftposition = m_lefttalonlead->GetPosition();
	 m_rightposition = m_righttalonlead->GetPosition();*/

	m_lefttalonlead->Set(m_invertleft * m_coasting * LeftMotor(maxpower));
	m_lefttalonfollow->Set(m_invertleft * m_coasting * LeftMotor(maxpower));
	m_righttalonlead->Set(m_invertright * m_coasting * RightMotor(maxpower));
	m_righttalonfollow->Set(m_invertright * m_coasting * RightMotor(maxpower));
	SmartDashboard::PutNumber("DT1_TurningRamp", m_previousx); //Left Motors are forward=negative
	SmartDashboard::PutNumber("DT3_LeftPow", m_invertleft * m_leftpow);//Left Motors are forward=negative
	SmartDashboard::PutNumber("DT2_DrivingRamp", m_previousy);//Right Motors are forward=positive
	SmartDashboard::PutNumber("DT4_RightPow", m_invertright * m_rightpow);//Right Motors are forward=positive
	SmartDashboard::PutNumber("DT5_Gear", m_ishighgear);

	/*SmartDashboard::PutNumber("leftSpeed", m_leftspeed);
	 SmartDashboard::PutNumber("leftPosition", m_leftposition);
	 SmartDashboard::PutNumber("rightSpeed", m_rightspeed);
	 SmartDashboard::PutNumber("rightPosition", m_rightposition);*/
}

/*
 void Drivetrain::DriveXY(double joyStickX, double joyStickY)
 {
 double invMaxValueXPlusY;

 //set fixnum = the maxiumum value for this angle on the joystick
 if (joyStickX == 0 || joyStickY == 0)
 {
 invMaxValueXPlusY = 1;
 }
 else
 {
 if (abs(joyStickX) > abs(joyStickY))
 {
 double invMaxValueXPlusYMult = 1 / abs(joyStickX);
 invMaxValueXPlusY = abs(joyStickY) * invMaxValueXPlusYMult + 1;
 //Invert for later use
 invMaxValueXPlusY = 1 / invMaxValueXPlusY;
 }
 else
 {
 double invMaxValueXPlusYMult = 1 / abs(joyStickY);
 invMaxValueXPlusY = abs(joyStickX) * invMaxValueXPlusYMult + 1;
 //Invert for later use
 invMaxValueXPlusY = 1 / invMaxValueXPlusY;
 }
 }
 //double invBatteryVoltage = 1 / driverstation->GetInstance().GetBatteryVoltage();
 //double BatteryRampingMin = RAMPING_RATE_MIN*invBatteryVoltage;
 //double BatteryRampingMax = RAMPING_RATE_MAX*invBatteryVoltage;
 m_previousx = joyStickX;//rampInput(previousX, joyStickX, BatteryRampingMin, BatteryRampingMax); //Left Motors are forward=negative
 m_previousy = joyStickY;//rampInput(previousY, joyStickY, BatteryRampingMin, BatteryRampingMax); //Right Motors are forward=positive
 //leftPow = previousY * Y_SCALING - previousX * X_SCALING;
 m_leftpow = m_previousy - m_previousx;
 //rightPow = previousY * Y_SCALING + previousX * X_SCALING;
 m_rightpow = m_previousy + m_previousx;
 m_leftspeed = m_lefttalonlead->GetSpeed();
 m_rightspeed = m_righttalonlead->GetSpeed();
 m_leftposition = m_lefttalonlead->GetPosition();
 m_rightposition = m_righttalonlead->GetPosition();

 m_lefttalonlead->Set(m_invertleft * m_coasting * LeftMotor(invMaxValueXPlusY) * LEFT_MOTOR_SCALING);
 m_righttalonlead->Set(m_invertright * m_coasting * RightMotor(invMaxValueXPlusY) * RIGHT_MOTOR_SCALING);

 SmartDashboard::PutNumber("LeftPow", m_invertleft*m_leftpow); //Left Motors are forward=negative
 SmartDashboard::PutNumber("RightPow", m_invertright*m_rightpow); //Right Motors are forward=positive
 }


 void Drivetrain::setPowerXYleft(double joyStickX, double joyStickY)
 {
 double invMaxValueXPlusY;

 //set fixnum = the maxiumum value for this angle on the joystick
 if (joyStickX == 0 || joyStickY == 0)
 {
 invMaxValueXPlusY = 1;
 }
 else
 {
 if (abs(joyStickX) > abs(joyStickY))
 {
 double invMaxValueXPlusYMult = 1 / abs(joyStickX);
 invMaxValueXPlusY = abs(joyStickY) * invMaxValueXPlusYMult + 1;
 //Invert for later use
 invMaxValueXPlusY = 1 / invMaxValueXPlusY;
 }
 else
 {
 double invMaxValueXPlusYMult = 1 / abs(joyStickY);
 invMaxValueXPlusY = abs(joyStickX) * invMaxValueXPlusYMult + 1;
 //Invert for later use
 invMaxValueXPlusY = 1 / invMaxValueXPlusY;
 }
 }
 //double invBatteryVoltage = 1 / driverstation->GetInstance().GetBatteryVoltage();
 //double BatteryRampingMin = RAMPING_RATE_MIN*invBatteryVoltage;
 //double BatteryRampingMax = RAMPING_RATE_MAX*invBatteryVoltage;
 previousX = joyStickX;//rampInput(previousX, joyStickX, BatteryRampingMin, BatteryRampingMax); //Left Motors are forward=negative
 previousY = joyStickY;//rampInput(previousY, joyStickY, BatteryRampingMin, BatteryRampingMax); //Right Motors are forward=positive
 //leftPow = previousY * Y_SCALING - previousX * X_SCALING;
 leftPow = previousY - previousX;
 //rightPow = previousY * Y_SCALING + previousX * X_SCALING;
 rightPow = previousY + previousX;
 leftSpeed = leftTalons->GetSpeed();
 rightSpeed = rightTalons->GetSpeed();
 leftPosition = leftTalons->GetPosition();
 rightPosition = rightTalons->GetPosition();

 leftTalons->Set(invertLeft * coasting * LeftMotor(invMaxValueXPlusY) * LEFT_MOTOR_SCALING);
 //rightTalons->Set(invertRight * coasting * RightMotor(invMaxValueXPlusY) * RIGHT_MOTOR_SCALING);
 rightTalons->Set(0);

 SmartDashboard::PutNumber("LeftPow", invertLeft*leftPow); //Left Motors are forward=negative
 SmartDashboard::PutNumber("RightPow", invertRight*rightPow); //Right Motors are forward=positive
 }


 void Drivetrain::setPowerXYright(double joyStickX, double joyStickY)
 {
 double invMaxValueXPlusY;

 //set fixnum = the maxiumum value for this angle on the joystick
 if (joyStickX == 0 || joyStickY == 0)
 {
 invMaxValueXPlusY = 1;
 }
 else
 {
 if (abs(joyStickX) > abs(joyStickY))
 {
 double invMaxValueXPlusYMult = 1 / abs(joyStickX);
 invMaxValueXPlusY = abs(joyStickY) * invMaxValueXPlusYMult + 1;
 //Invert for later use
 invMaxValueXPlusY = 1 / invMaxValueXPlusY;
 }
 else
 {
 double invMaxValueXPlusYMult = 1 / abs(joyStickY);
 invMaxValueXPlusY = abs(joyStickX) * invMaxValueXPlusYMult + 1;
 //Invert for later use
 invMaxValueXPlusY = 1 / invMaxValueXPlusY;
 }
 }
 //double invBatteryVoltage = 1 / driverstation->GetInstance().GetBatteryVoltage();
 //double BatteryRampingMin = RAMPING_RATE_MIN*invBatteryVoltage;
 //double BatteryRampingMax = RAMPING_RATE_MAX*invBatteryVoltage;
 previousX = joyStickX;//rampInput(previousX, joyStickX, BatteryRampingMin, BatteryRampingMax); //Left Motors are forward=negative
 previousY = joyStickY;//rampInput(previousY, joyStickY, BatteryRampingMin, BatteryRampingMax); //Right Motors are forward=positive
 //leftPow = previousY * Y_SCALING - previousX * X_SCALING;
 leftPow = previousY - previousX;
 //rightPow = previousY * Y_SCALING + previousX * X_SCALING;
 rightPow = previousY + previousX;
 leftSpeed = leftTalons->GetSpeed();
 rightSpeed = rightTalons->GetSpeed();
 leftPosition = leftTalons->GetPosition();
 rightPosition = rightTalons->GetPosition();

 //leftTalons->Set(invertLeft * coasting * LeftMotor(invMaxValueXPlusY) * LEFT_MOTOR_SCALING);
 leftTalons->Set(0);
 rightTalons->Set(invertRight * coasting * RightMotor(invMaxValueXPlusY) * RIGHT_MOTOR_SCALING);

 SmartDashboard::PutNumber("LeftPow", invertLeft*leftPow); //Left Motors are forward=negative
 SmartDashboard::PutNumber("RightPow", invertRight*rightPow); //Right Motors are forward=positive
 }
 */

//Sets the motors to coasting mode, shifts, and then sets them back to break mode
void Drivetrain::Shift() {
	m_lefttalonlead->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Coast);
	m_lefttalonfollow->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Coast);
	m_righttalonlead->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Coast);
	m_righttalonfollow->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Coast);
	m_ishighgear = !m_ishighgear;
	m_shifter->Set(FLIP_HIGH_GEAR ^ m_ishighgear);
	m_lefttalonlead->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_lefttalonfollow->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_righttalonlead->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Brake);
	m_righttalonfollow->ConfigNeutralMode(
			CANSpeedController::NeutralMode::kNeutralMode_Brake);
}

// change direction and return true if going forward
bool Drivetrain::ChangeDirection() {
	m_direction *= -1.0;
	return (m_direction == DT_DEFAULT_DIRECTION); //todo explain this
}

double Drivetrain::Ramp(double previous, double desired, double rampmin,
		double rampmax) {
	double newpow = previous;
	double delta = abs(desired - previous);
//Makes it so that robot can't go stop to full
	if (delta <= rampmin)
	newpow = desired;
	else if (previous < desired)
	newpow += max((delta * rampmax), rampmin);
	else if (previous > desired)
	newpow -= max((delta * rampmax), rampmin);
//leftTalons1->Set(-previousLeftPow);
	return newpow;
}

/*
 void Drivetrain::rampRightPower(double desiredPow, double rampSpeedMin, double rampSpeedMax)
 {
 //Makes it so that robot can't go stop to full
 if (abs(desiredPow - previousRightPow) < rampSpeedMin)
 {
 previousRightPow = desiredPow;
 }
 else
 if (previousRightPow < desiredPow)
 {
 previousRightPow += rampSpeedMin;
 }
 else
 if (previousRightPow > desiredPow)
 {
 previousRightPow -= rampSpeedMin;
 }
 rightTalons->Set(previousRightPow);
 //rightTalons1->Set(previousRightPow);
 }
 */

double Drivetrain::LeftMotor(double &maxpower) {
	double leftpow = m_leftpow * LEFT_MOTOR_SCALING / maxpower;
//moved rightSpeed to class scope, it is being set in setPower()

	/*if (m_leftpow != 0 && m_rightpow != 0)
	 {
	 m_leftencodermax = abs(m_leftspeed / m_leftpow);
	 if (min(abs(m_leftspeed), abs(m_rightspeed)) > ENCODER_TOP_SPEED)
	 CheckEncoderTimer();
	 if (m_isleftfaster)
	 leftpow = m_ratiolr * leftpow;
	 }*/
	return leftpow;
}

double Drivetrain::RightMotor(double &maxpower) {
//moved rightSpeed to class scope, it is being set in setPower()

	double rightpow = m_rightpow * RIGHT_MOTOR_SCALING / maxpower;

	/*if (m_leftpow != 0 && m_rightpow != 0)
	 {
	 m_rightencodermax = abs(m_rightspeed / m_rightpow);
	 if (min(abs(m_leftspeed), abs(m_rightspeed)) > ENCODER_TOP_SPEED)
	 CheckEncoderTimer();
	 if (!m_isleftfaster)
	 rightpow = m_ratiolr * rightpow;
	 }*/
	return rightpow;
}

void Drivetrain::SetRatioLR() {
//If left motor speed is bigger than the right motor speed return true, else false

	if (m_rightencodermax > m_leftencodermax) {
		m_ratiolr = m_leftencodermax / m_rightencodermax;
		m_isleftfaster = false;
	} else if (m_leftencodermax > m_rightencodermax) {
		m_ratiolr = m_rightencodermax / m_leftencodermax;
		m_isleftfaster = true;
	} else {
		m_ratiolr = 1;
	}
}

void Drivetrain::ResetEncoders() {
//	leftEncoder->Reset();
//	rightEncoder->Reset();
}

void Drivetrain::CheckEncoderTimer() {
//	SmartDashboard::PutNumber("Ratio", m_ratiolr);
//	SmartDashboard::PutBoolean("Left > Right", m_isleftfaster);
//	SmartDashboard::PutNumber("Timer time", m_timerencoder->Get());
	if (m_timerencoder->Get() > ENCODER_WAIT_TIME) {
		SetRatioLR();
		m_timerencoder->Reset();
	}
}
#endif

/**
 * Deletes all pointers
 */
Drivetrain::~Drivetrain() {
	delete m_lefttalonlead;
	delete m_lefttalonfollow;
	delete m_righttalonlead;
	delete m_righttalonfollow;
	//delete leftEncoder;
	//delete rightEncoder;
	delete m_shifter;
#ifdef FIRSTDRIVE
	delete m_robotdrive;
#else
	delete m_timerencoder;
#endif
}
