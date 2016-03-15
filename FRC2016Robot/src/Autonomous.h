// autonomous.h


#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_


#include "OperatorInputs.h"
#include "Drivetrain.h"
#include "VisionTargeting.h"
#include <AnalogGyro.h>
#include <BuiltinAccelerometer.h>
#include <queue>


using namespace std;


class Autonomous
{
public:
	enum Stage { kStop, kDrive, kDriveLoop, kShoot, kShootLoop };

	Autonomous(DriverStation *driverstation, OperatorInputs *operatorinputs, Drivetrain *drivetrain, VisionTargeting *vision);
	~Autonomous();
	void Init();
	void Loop();
	void Calibrate();

protected:
	DriverStation *m_driverstation;
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;
	VisionTargeting *m_vision;
	AnalogGyro *m_gyro;
	Accelerometer *m_accel;
	int m_stage;
	double m_driveangle;
	int m_counter;
};


#endif /* AUTONOMOUS_H_ */
