// autonomous.h


#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_


#include "OperatorInputs.h"
#include "Drivetrain.h"
#include <AnalogGyro.h>
#include <BuiltinAccelerometer.h>
#include <queue>
#include "VisionTargeting.h"
#include "timer.h"


using namespace std;


class Autonomous
{
public:
	enum Stage { kStartDriveForward, kDrivingForward, kDelayBeforeShoot, kShoot, kShooting };

	struct Instruction
	{
		int time;
		int angle;
		double distance;
		double kP;
		double kI;
		Stage stage;
	};

	Autonomous(DriverStation *driverstation, OperatorInputs *operatorinputs, Drivetrain *drivetrain, VisionTargeting*);
	~Autonomous();
	void Init();
	void Loop();
	void Disable();
	void Calibrate();

protected:
	DriverStation *m_driverstation;
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;
	AnalogGyro *m_gyro;
	Accelerometer *m_accel;
	queue<Instruction> m_instructions;
	Instruction m_instruction;
	VisionTargeting *m_visiontargeting;
	int m_stage;
	double m_driveangle;
	int m_counter;
	double m_kS;
	Timer *autotimer;
	double oldtime;
	int count;
};


#endif /* AUTONOMOUS_H_ */
