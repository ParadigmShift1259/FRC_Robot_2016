// visiontargeting.cpp


#include "visiontargeting.h"
#include <vector>


VisionTargeting::VisionTargeting(OperatorInputs *operatorinputs, Drivetrain *drivetrain, Shooter *shooter)
{
	m_inputs = operatorinputs;
	m_drivetrain = drivetrain;
	m_shooter = shooter;
	m_networktable = NetworkTable::GetTable("OpenCV");
	m_targeting = 0;
	m_stage = kInitialX;
	m_steady = 0;
}


VisionTargeting::~VisionTargeting()
{
}


void VisionTargeting::Loop()
{
	bool alignbutton = m_inputs->xBoxLeftBumper();

	// process vision targeting if button pressed or in targeting mode
	if (alignbutton || (m_targeting > 0))
	{
		// obtain area, height, width
		double area = m_networktable->GetNumber("area", 0);
		//double height = m_networktable->GetNumber("height", 0);
		//double width = m_networktable->GetNumber("width", 0);
		// validate object
		if (area > 800)
		{
			// start alignment if button pressed
			if (alignbutton)
			{
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
				if (xpos < -5)			// turn left
				{
					m_drivetrain->setPowerXY(max(-xpos/840.0, 0.13), 0);
					m_steady = 0;
				}
				else
				if (xpos > 5)			// turn right
				{
					m_drivetrain->setPowerXY(min(-xpos/840.0, -0.13), 0);
					m_steady = 0;
				}
				else
				{
					m_drivetrain->setPowerXY(0, 0);
					m_stage = kInitialY;
				}
				break;
			// process coarse Y direction
			case kInitialY:
				if (ypos < -10)			// drive backward
				{
					m_drivetrain->setPowerXY(0, min(ypos/200.0, -0.13));
					m_steady = 0;
				}
				else
				if (ypos > 10)			// drive forward
				{
					m_drivetrain->setPowerXY(0, max(ypos/200.0, 0.13));
					m_steady = 0;
				}
				else
				{
					m_drivetrain->setPowerXY(0, 0);
					m_stage = kFinalX;
				}
				break;
			// process fine X direction
			case kFinalX:
				if (xpos < 0)			// turn left
				{
					m_drivetrain->setPowerXYleft(max(-xpos/840.0, 0.13), 0);
					m_steady = 0;
				}
				else
				if (xpos > 0)			// turn right
				{
					m_drivetrain->setPowerXYright(min(-xpos/840.0, -0.13), 0);
					m_steady = 0;
				}
				else
				{
					m_drivetrain->setPowerXY(0, 0);
					m_stage = kFinalY;
				}
				break;
				// process fine Y direction
			case kFinalY:
				if (ypos < -5)			// drive backward
				{
					m_drivetrain->setPowerXY(0, min(ypos/200.0, -0.13));
					m_steady = 0;
				}
				else
				if (ypos > 5)			// drive forward
				{
					m_drivetrain->setPowerXY(0, max(ypos/200.0, 0.13));
					m_steady = 0;
				}
				else
				{
					m_drivetrain->setPowerXY(0, 0);
					m_stage = kFinalX;
				}
				break;
			}
			// aligned
			if ((xpos == 0) && (ypos > -5) && (ypos < 5))
			{
				m_drivetrain->setPowerXY(0, 0);
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
			m_drivetrain->setPowerXY(0, 0);
			m_stage = kInitialX;
			m_targeting = 0;
			m_steady = 0;
		}

		DriverStation::ReportError("targeting: " + to_string(m_targeting) + "\n");
	}
}
