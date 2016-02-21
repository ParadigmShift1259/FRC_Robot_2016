// autonomous.h


#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_


#include "OperatorInputs.h"
#include "Drivetrain.h"
#include <AnalogGyro.h>
#include <BuiltinAccelerometer.h>
#include <queue>


using namespace std;


struct Instruction
{
	int time;
	int angle;
	double speed;
};


class Autonomous
{
public:
	enum Stage { kStop, kDrive };

	Autonomous(DriverStation *driverstation, OperatorInputs *operatorinputs, Drivetrain *drivetrain);
	~Autonomous();
	void Init();
	void Loop();
	void Calibrate();

protected:
	DriverStation *m_driverstation;
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;
	AnalogGyro *m_gyro;
	Accelerometer *m_accel;
	queue<Instruction> m_instructions;
	Instruction m_instruction;
	int m_stage;
	double m_driveangle;
	int m_counter;
};


#endif /* AUTONOMOUS_H_ */
