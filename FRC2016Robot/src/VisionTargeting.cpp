// visiontargeting.cpp


#include "visiontargeting.h"
#include <vector>


VisionTargeting::VisionTargeting(OperatorInputs *operatorinputs, Drivetrain *drivetrain, Shooter *shooter) :
	PIDSubsystem("VisionTargeting",PID_VISION_P_X,PID_VISION_I_X,PID_VISION_D_X)
{
	m_inputs = operatorinputs;
	m_drivetrain = drivetrain;
	m_shooter = shooter;
	m_networktable = NetworkTable::GetTable("OpenCV");
	m_targeting = 0;
	m_stage = kInitialX;
	m_steady = 0;
	SetOutputRange(-1,1);
	SetSetpoint(0);
}


VisionTargeting::~VisionTargeting()
{
}


void VisionTargeting::Init()
{
	Disable();
	GetPIDController()->SetPID(PID_VISION_P_X, PID_VISION_I_X, PID_VISION_D_X);
	m_stage = kInitialX;
}

void VisionTargeting::Loop()
{
	bool alignbutton = m_inputs->xBoxLeftBumper();

	// process vision targeting if button pressed or in targeting mode
	if (alignbutton || (m_targeting > 0))
	{
		// obtain area, height, width
		double area = m_networktable->GetNumber("area", 0);

		// validate object
		if (area > 800)
		{
			// start alignment if button pressed
			if (alignbutton)
			{
				Enable();
				m_stage = kInitialX;
				m_targeting = 400;
				m_steady = 0;
			}
			// obtain x and y offsets from opencv
			double xpos = m_networktable->GetNumber("xpos", 0);
			double ypos = m_networktable->GetNumber("ypos", 0);

			switch (m_stage)
			{
			// process coarse X direction
			case kInitialX:
				if ((xpos*xpos) > 25)			// turn
				{
					m_steady = 0;
				}
				else
				{
					Disable();
					GetPIDController()->SetPID(PID_VISION_P_Y, PID_VISION_I_Y, PID_VISION_D_Y);
					m_drivetrain->Drive(0, 0);
					m_stage = kInitialY;
					Enable();
				}
				break;
			// process coarse Y direction
			case kInitialY:
				if ((ypos*ypos) > 100)			// drive backward
				{
					m_steady = 0;
				}
				else
				{
					Disable();
					GetPIDController()->SetPID(PID_VISION_P_X, PID_VISION_I_X, PID_VISION_D_X);
					m_drivetrain->Drive(0, 0);
					m_stage = kFinalX;
					Enable();
				}
				break;
			// process fine X direction
			case kFinalX:
				if (xpos < 0)			// turn left
				{
					m_steady = 0;
				}
				else
				if (xpos > 0)			// turn right
				{
					m_steady = 0;
				}
				else
				{
					Disable();
					GetPIDController()->SetPID(PID_VISION_P_Y, PID_VISION_I_Y, PID_VISION_D_Y);
					m_drivetrain->Drive(0, 0);
					m_stage = kFinalY;
					Enable();
				}
				break;
				// process fine Y direction
			case kFinalY:
				if ((ypos*ypos) > 25)			// drive backward
				{
					m_steady = 0;
				}
				else
				{
					Disable();
					GetPIDController()->SetPID(PID_VISION_P_X, PID_VISION_I_X, PID_VISION_D_X);
					m_drivetrain->Drive(0, 0);
					m_stage = kFinalX;
					Enable();
				}
				break;
			}
			// aligned
			if ((xpos == 0) && (ypos > -5) && (ypos < 5))
			{
				Disable();
				m_drivetrain->Drive(0, 0);
				// verify steady state count
				m_steady += 1;
			}
			m_targeting -= 1;
			// at steady state
			if (m_steady > 20)
			{
				m_targeting = 0;
				m_shooter->Loop(true);
			}
		}
		else
		{
			Disable();
			GetPIDController()->SetPID(PID_VISION_P_X, PID_VISION_I_X, PID_VISION_D_X);
			m_drivetrain->Drive(0, 0);
			m_stage = kInitialX;
			m_targeting = 0;
			m_steady = 0;
		}

		DriverStation::ReportError("targeting: " + to_string(m_targeting) + "\n");
	}
	else
	if (m_targeting == 0)
	{
		Disable();
		GetPIDController()->SetPID(PID_VISION_P_X, PID_VISION_I_X, PID_VISION_D_X);
		m_stage = kInitialX;
	}

}


double VisionTargeting::ReturnPIDInput()
{
	DriverStation::ReportError("ReturnPID"+std::to_string(m_networktable->GetNumber("xpos", 0))+"\n");
	switch (m_stage)
	{
	case kInitialX:
	case kFinalX:
		return m_networktable->GetNumber("xpos", 0);
	case kInitialY:
	case kFinalY:
		return m_networktable->GetNumber("ypos", 0);
	}
	return 0;
}


void VisionTargeting::Stop()
{
	Disable();
	GetPIDController()->SetPID(PID_VISION_P_X, PID_VISION_I_X, PID_VISION_D_X);
	m_stage = kInitialX;
}

void VisionTargeting::UsePIDOutput(double output)
{
	if (GetPIDController()->IsEnabled())
	{
		DriverStation::ReportError("UsePIDOutput"+std::to_string(output)+"\n");
		switch (m_stage)
		{
		// process coarse X direction
		case kInitialX:
				m_drivetrain->Drive(-output, 0);
			break;
		// process coarse Y direction
		case kInitialY:
				m_drivetrain->Drive(0, output);
			break;
		// process fine X direction
		case kFinalX:
			if (output > 0)			// turn left
				m_drivetrain->Drive(-output, output);
			else
			if (output < 0)			// turn right
				m_drivetrain->Drive(-output, -output);
			break;
			// process fine Y direction
		case kFinalY:
				m_drivetrain->Drive(0, output);
			break;
		}
	}
}
