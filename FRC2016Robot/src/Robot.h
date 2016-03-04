/*
 * Robot.h
 *
 *  Created on: Feb 18, 2016
 *      Author: eacobb
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_


#include "WPILib.h"
#include "OperatorInputs.h"
#include "const.h"
#include "drivetrain.h"
#include "camera.h"
#include "compressor.h"
#include "Picker.h"
#include "Shooter.h"
#include "Portcullis.h"
#include "Climber.h"
#include "Autonomous.h"
#include "VisionTargeting.h"


using namespace std;


class Robot: public IterativeRobot
{
private:
	// live window variables
	LiveWindow *m_lw;
	SendableChooser *m_chooser;
	string m_autonamedefault;
	string m_autonamecustom;
	string m_autoselected;

	OperatorInputs *m_inputs;
	Drivetrain *m_drivetrain;
	Camera *m_camera;
	Picker *m_picker;
	Shooter *m_shooter;
	Portcullis *m_portcullis;
	Climber *m_climber;
	Autonomous *m_autonomous;
	VisionTargeting *m_vision;
	Compressor *m_compressor;

	virtual void RobotInit();
	virtual void AutonomousInit();
	virtual void AutonomousPeriodic();
	virtual void TeleopInit();
	virtual void TeleopPeriodic();
	virtual void TestInit();
	virtual void TestPeriodic();
	virtual void DisabledInit();
};


#endif /* SRC_ROBOT_H_ */
