// visiontargeting.cpp


#include "visiontargeting.h"
#include <vector>
#include <cmath>


VisionTargeting::VisionTargeting(OperatorInputs *operatorinputs, Drivetrain *drivetrain, Shooter *shooter, Picker *picker)
{
	m_inputs = operatorinputs;
	m_drivetrain = drivetrain;
	m_shooter = shooter;
	m_picker = picker;
	m_cvtable = NetworkTable::GetTable("OpenCV");
	m_stage = kReady;
	m_counter = 0;
	m_targeting = 0;
	m_shoot = false;
}


VisionTargeting::~VisionTargeting()
{
}


void VisionTargeting::Init()
{
	m_stage = kReady;
	m_counter = 0;
	m_targeting = 0;
	m_shoot = false;
}


void VisionTargeting::Loop(bool target, int nocamdelay)
{
	bool alignbutton = m_inputs->xBoxLeftBumper() || target;

	double kP = SmartDashboard::GetNumber("DB/Slider 0", 0);
	if (kP == 0) {kP = 0.2;}

	double kI = SmartDashboard::GetNumber("DB/Slider 1", 0);
	if (kI == 0) {kI = 0.001;}

	double kS = SmartDashboard::GetNumber("DB/Slider 2", 0);
	if (kS == 0) {kS = 0.0025;}

	double kAdj = SmartDashboard::GetNumber("DB/Slider 3", 0);
	if (kAdj == 0) {kAdj = 25;}

	double xpos = m_cvtable->GetNumber("xpos", 0);
	double kPos = (xpos - kAdj) * kS * -1;

	double leftposition = m_drivetrain->LeftTalon()->GetPosition();
	double rightposition = m_drivetrain->RightTalon()->GetPosition();

	SmartDashboard::PutNumber("VT1_leftpos", leftposition);
	SmartDashboard::PutNumber("VT2_rightpos", rightposition);
	SmartDashboard::PutNumber("VT3_kP", kP);
	SmartDashboard::PutNumber("VT4_kI", kI);
	SmartDashboard::PutNumber("VT5_kS", kS);
	SmartDashboard::PutNumber("VT6_kAdj", kAdj);
	SmartDashboard::PutNumber("VT7_kPos", kPos);

	if (m_shoot)
	{
		DriverStation::ReportError("Shooting\n");
		if (m_shooter->GetStage() == Shooter::kRelease)
			Stop();
	}
	else
	if ((m_targeting > 0) || alignbutton)
	{
		switch (m_stage)
		{
		case kReady:
			if (alignbutton)
			{
				m_targeting = 2;
			}
			if (m_targeting > 0)
			{
				DriverStation::ReportError("Targeting\n");

				m_drivetrain->EnablePID(kP,kI,0,0,kPos,kPos);

				m_stage = kTarget;
				m_counter = 15 * nocamdelay;
			}
			break;
		case kTarget:
			if ((m_counter % 5) == 0)
				DriverStation::ReportError("Waiting: " + to_string(m_counter) + "\n");
			if (m_counter > 0)
			{
				m_counter--;
			}
			else
			{
				m_stage = kStop;
				m_counter = 0;
			}
			break;
		case kStop:
			DriverStation::ReportError("Stopping\n");
			if (m_targeting > 0)
			{
				if (m_targeting > 1)
				{
					m_drivetrain->DisablePID();
					m_stage = kReady;
					m_counter = 0;
				}
				m_targeting--;
				if (m_targeting == 1)
					m_picker->Loop(true, nocamdelay);
			}
			if (m_targeting == 0)
			{
				m_shooter->Loop(true, nocamdelay);
				m_shoot = true;
				m_stage = kReady;
				m_counter = 0;
			}
			break;
		}
	}
}


void VisionTargeting::Stop()
{
	m_drivetrain->DisablePID();
	m_stage = kReady;
	m_counter = 0;
	m_targeting = 0;
	m_shoot = false;
}
