#ifndef GyroPID_H
#define GyroPID_H

#include <MecanumDriveTrain.h>
#include "Commands/PIDSubsystem.h"
#include "WPILib.h"
#include "Gyro.h"
class GyroPID: public PIDSubsystem
{
private:
	Gyro* roboGyro;
	MecanumDriveTrain* driveTrain;

	double setpoint;

public:
	GyroPID(double p, double i, double d, Gyro* roboGyro, MecanumDriveTrain* driveTrain);
	double ReturnPIDInput();
	double GetSetpoint();
	void UsePIDOutput(double output);
	void SetPIDValues(double p,double i,double d);
	void InitDefaultCommand();
	void SetSetpoint(double setpoint);
	void SetSetpointRelative(double setpoint);
	void Reset();
};

#endif
