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
}


VisionTargeting::~VisionTargeting()
{
}


void VisionTargeting::Init()
{
	m_stage = kReady;
	m_counter = 0;
	m_targeting = 0;
}


void VisionTargeting::Loop(bool target, int delay)
{
	bool alignbutton = m_inputs->xBoxLeftBumper() || target;

	double kP = SmartDashboard::GetNumber("DB/Slider 0", 0);
	if (kP == 0) {kP = 0.2;}

	double kI = SmartDashboard::GetNumber("DB/Slider 1", 0);
	if (kI == 0) {kI = 0.001;}

	double kS = SmartDashboard::GetNumber("DB/Slider 2", 0);
	if (kS == 0) {kS = 0.0025;}

	double kAdj = SmartDashboard::GetNumber("DB/Slider 3", 0);
	if (kAdj == 0) {kAdj = 35;}

	double xpos = m_cvtable->GetNumber("xpos", 0);
	double kPos = (xpos - kAdj) * kS * -1;

	double leftposition = m_drivetrain->LeftTalon()->GetPosition();
	double rightposition = m_drivetrain->RightTalon()->GetPosition();
	double leftvolts = m_drivetrain->LeftTalon()->GetOutputVoltage();
	double rightvolts = m_drivetrain->RightTalon()->GetOutputVoltage();

	SmartDashboard::PutNumber("VT1_leftpos", leftposition);
	SmartDashboard::PutNumber("VT2_rightpos", rightposition);
	SmartDashboard::PutNumber("VT3_kP", kP);
	SmartDashboard::PutNumber("VT4_kI", kI);
	SmartDashboard::PutNumber("VT5_kS", kS);
	SmartDashboard::PutNumber("VT6_kAdj", kAdj);
	SmartDashboard::PutNumber("VT7_kPos", kPos);
	SmartDashboard::PutNumber("VT8_leftvolts", leftvolts);
	SmartDashboard::PutNumber("VT9_rightvolts", rightvolts);

	if (Targeting() || alignbutton)
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
				EnablePID(kP, kI, kPos, kPos);
				m_stage = kTarget;
				m_counter = 15 * delay;
			}
			break;
		case kTarget:
			if ((m_counter % 5) == 0)
				DriverStation::ReportError("Vision Target");
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
			DriverStation::ReportError("Vision Stop");
			if (m_targeting > 0)
			{
				if (m_targeting > 1)
				{
					DisablePID();
					m_stage = kReady;
					m_counter = 0;
				}
				m_targeting--;
				if (m_targeting == 1)
					m_picker->Loop(true, delay);
			}
			if (m_targeting == 0)
			{
				m_shooter->Loop(true, delay);
				m_stage = kShoot;
				m_counter = 15;
			}
			break;
		case kShoot:
			if ((m_counter % 5) == 0)
				DriverStation::ReportError("Vision Shoot");
			if ((m_counter <= 0) || (m_shooter->GetStage() == Shooter::kRelease))
			{
				Stop();
				m_stage = kReady;
				m_counter = 0;
			}
			else
				m_counter--;
			break;
		}
	}
}


void VisionTargeting::Stop()
{
	DisablePID();
	m_stage = kReady;
	m_counter = 0;
	m_targeting = 0;
}


bool VisionTargeting::Targeting()
{
	return (m_targeting > 0) || (m_stage > kReady);
}


void VisionTargeting::EnablePID(double kP, double kI, double kPosLeft, double kPosRight)
{
	m_drivetrain->LeftTalon()->SelectProfileSlot(0);
	m_drivetrain->LeftTalon()->ConfigNominalOutputVoltage(+0.0f, -0.0f);
	m_drivetrain->LeftTalon()->ConfigPeakOutputVoltage(+6.0f, -6.0f);
	m_drivetrain->LeftTalon()->SetAllowableClosedLoopErr(0.0);

	m_drivetrain->RightTalon()->SelectProfileSlot(0);
	m_drivetrain->RightTalon()->ConfigNominalOutputVoltage(+0.0f, -0.0f);
	m_drivetrain->RightTalon()->ConfigPeakOutputVoltage(+6.0f, -6.0f);
	m_drivetrain->RightTalon()->SetAllowableClosedLoopErr(0.0);

	m_drivetrain->LeftTalon()->SetP(kP);
	m_drivetrain->LeftTalon()->SetI(kI);
	m_drivetrain->LeftTalon()->SetD(0);
	m_drivetrain->LeftTalon()->SetF(0);

	m_drivetrain->RightTalon()->SetP(kP);
	m_drivetrain->RightTalon()->SetI(kI);
	m_drivetrain->RightTalon()->SetD(0);
	m_drivetrain->RightTalon()->SetF(0);

	m_drivetrain->LeftTalon()->SetPosition(0);
	m_drivetrain->RightTalon()->SetPosition(0);

	Wait(0.1);

	m_drivetrain->LeftTalon()->SetControlMode(CANTalon::kPosition);
	m_drivetrain->RightTalon()->SetControlMode(CANTalon::kPosition);
	m_drivetrain->LeftTalon()->Set(kPosLeft);
	m_drivetrain->RightTalon()->Set(kPosRight);
}


void VisionTargeting::DisablePID()
{
	m_drivetrain->LeftTalon()->ConfigPeakOutputVoltage(+12.0f, -12.0f);
	m_drivetrain->LeftTalon()->SetControlMode(CANTalon::kPercentVbus);
	m_drivetrain->LeftTalon()->Set(0);

	m_drivetrain->RightTalon()->ConfigPeakOutputVoltage(+12.0f, -12.0f);
	m_drivetrain->RightTalon()->SetControlMode(CANTalon::kPercentVbus);
	m_drivetrain->RightTalon()->Set(0);
}
