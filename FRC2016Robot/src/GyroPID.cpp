#include "GyroPID.h"
#include "SmartDashboard/SmartDashboard.h"
#include "LiveWindow/LiveWindow.h"

GyroPID::GyroPID(double p, double i, double d, Gyro* roboGyro, MecanumDriveTrain* driveTrain) :
		PIDSubsystem("GyroPID", p, i, d)
{
	this->driveTrain = driveTrain;
	this->roboGyro = roboGyro;

	SetAbsoluteTolerance(0.0);

	// Use these to get going:
	// SetSetpoint() -  Sets where the PID controller should move the system
	//                  to
	// Enable() - Enables the PID controller.
}

void GyroPID::SetPIDValues(double p,double i,double d) {
	GetPIDController()->SetPID(p,i,d);
}

double GyroPID::ReturnPIDInput()
{

	return roboGyro->GetAngle();

	// Return your input value for the PID loop
	// e.g. a sensor, like a potentiometer:
	// yourPot->SetAverageVoltage() / kYourMaxVoltage;	
}

void GyroPID::UsePIDOutput(double output)
{
	driveTrain->SetGyroOffset(output);
	// Use output to drive your system, like a motor
	// e.g. yourMotor->Set(output);
}

void GyroPID::Reset() {
	GetPIDController()->Reset();
}

double GyroPID::GetSetpoint() {
	return setpoint;
}

void GyroPID::SetSetpointRelative(double setpointChange) {
	setpoint = setpoint + setpointChange;
	GetPIDController()->SetSetpoint(setpoint);
}

void GyroPID::SetSetpoint(double setpoint) {
	this->setpoint = setpoint;
	GetPIDController()->SetSetpoint(setpoint);
}

void GyroPID::InitDefaultCommand()
{
	// Set the default command for a subsystem here.
	//setDefaultCommand(new MySpecialCommand());
}
