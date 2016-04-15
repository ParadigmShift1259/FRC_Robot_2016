// autonomous.h


#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_


#include "OperatorInputs.h"
#include "Drivetrain.h"
#include "VisionTargeting.h"
#include <AnalogGyro.h>
#include <BuiltinAccelerometer.h>


using namespace std;


class Autonomous
{
public:
	enum Stage { kIdle, kStart, kDrive, kStop, kShoot };

	Autonomous(DriverStation *driverstation, OperatorInputs *operatorinputs, Drivetrain *drivetrain, VisionTargeting *vision);
	~Autonomous();
	void Init();
	void Loop(int delay = 1);
	void Calibrate();

protected:
	DriverStation *m_driverstation;
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;
	VisionTargeting *m_vision;
	AnalogGyro *m_gyro;
	Accelerometer *m_accel;
	int m_stage;
	int m_counter;
	double m_maxspeed;
	double m_minspeed;
	double m_rate;
	double m_speed;
	double m_distance;
};


#endif /* AUTONOMOUS_H_ */
