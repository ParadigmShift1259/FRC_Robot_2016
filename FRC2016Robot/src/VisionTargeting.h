// visiontargeting.h


#ifndef SRC_VISIONTARGETING_H_
#define SRC_VISIONTARGETING_H_


#include "OperatorInputs.h"
#include "Drivetrain.h"
#include "Shooter.h"
#include "NetworkTables/networktable.h"


class VisionTargeting
{
public:
	enum Stage { kReady, kTarget, kStop, kShoot };

	VisionTargeting(OperatorInputs *operatorinputs, Drivetrain *drivetrain, Shooter *shooter, Picker *picker);
	~VisionTargeting();
	void Init();
	void Loop(bool target = false, int delay = 1);
	void Stop();
	bool Targeting();
	void EnablePID(double kP, double kI, double kPosLeft, double kPosRight);
	void DisablePID();

protected:
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;
	Shooter *m_shooter;
	Picker *m_picker;
	shared_ptr<NetworkTable> m_cvtable;
	Stage m_stage;
	int m_counter;
	int m_targeting;
};


#endif /* SRC_VISIONTARGETING_H_ */
