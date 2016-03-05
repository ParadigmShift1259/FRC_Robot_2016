// visiontargeting.h


#ifndef SRC_VISIONTARGETING_H_
#define SRC_VISIONTARGETING_H_

#include "OperatorInputs.h"
#include "Drivetrain.h"
#include "Shooter.h"
#include "NetworkTables/networktable.h"
#include "Timer.h"
#include "Commands/PIDSubsystem.h"


#define PID_VISION_P_X -0.005
#define PID_VISION_I_X 0
#define PID_VISION_D_X 0
#define PID_VISION_P_Y 0.005
#define PID_VISION_I_Y 0
#define PID_VISION_D_Y 0


class VisionTargeting : public PIDSubsystem
{
public:
	enum Stage { kInitialX, kInitialY, kFinalX, kFinalY };

	VisionTargeting(OperatorInputs *operatorinputs, Drivetrain *drivetrain, Shooter *shooter);
	~VisionTargeting();
	void Init();
	void Loop();
	double ReturnPIDInput();
	void Stop();
	bool Targeting() {return (m_targeting > 0);}
	void UsePIDOutput(double output);

protected:
	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;
	Shooter *m_shooter;
	shared_ptr<NetworkTable> m_networktable;
	int m_targeting;
	Stage m_stage;
	int m_steady;
};


#endif /* SRC_VISIONTARGETING_H_ */
