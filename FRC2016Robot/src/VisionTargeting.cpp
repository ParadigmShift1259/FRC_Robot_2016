// visiontargeting.cpp


#include "visiontargeting.h"
#include <vector>


VisionTargeting::VisionTargeting(OperatorInputs *operatorinputs, Drivetrain *drivetrain)
{
	m_inputs = operatorinputs;
	m_drivetrain = drivetrain;
	m_networktable = NetworkTable::GetTable("OpenCV");
	m_targeting = 0;
	m_fixy = true;
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
		if (area > 1000)
		{
			if (alignbutton)
			{
				m_targeting = 300;
				DriverStation::ReportError("Set Targeting to 100\n");
			}

			double xpos = m_networktable->GetNumber("xpos", 0);
			double ypos = m_networktable->GetNumber("ypos", 0);

			//DriverStation::ReportError("xpos: " + to_string(xpos) + "\n");
			//DriverStation::ReportError("ypos: " + to_string(ypos) + "\n");
			if (m_fixy)
			{
				if (ypos < 0)			// drive backward
				{
					//m_drivetrain->setPowerXY(0, -0.2);
					m_drivetrain->setPowerXY(0, min(ypos/150.0, -0.25));
				}
				else
				if (ypos > 0)			// drive forward
				{
					//m_drivetrain->setPowerXY(0, 0.2);
					m_drivetrain->setPowerXY(0, max(ypos/150.0, 0.25));
				}
				else
					m_fixy = false;
			}
			else
			{
				if (xpos < 0)			// turn left
				{
					//m_drivetrain->setPowerXY(0.25, 0);
					m_drivetrain->setPowerXY(max(-xpos/840.0, 0.15), 0);
				}
				else
				if (xpos > 0)			// turn right
				{
					//m_drivetrain->setPowerXY(-0.25, 0);
					m_drivetrain->setPowerXY(min(-xpos/840.0, -0.15), 0);
				}
				else
					m_fixy = true;
			}
			if ((xpos == 0) && (ypos == 0))
				m_drivetrain->setPowerXY(0, 0);
			m_targeting -= 1;
		}
		else
			m_targeting = 0;

		DriverStation::ReportError("targeting: " + to_string(m_targeting) + "\n");
	}
}
