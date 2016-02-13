// robot.cpp


#include "WPILib.h"
#include "OperatorInputs.h"
#include "drivetrain.h"
#include "camera.h"
#include "compressor.h"
#include "Picker.h"
#include "Shooter.h"
#include "Portcullis.h"
#include "Climber.h"


class Robot: public IterativeRobot
{
private:
	// live window variables
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	// main class variables
	OperatorInputs *inputs;
	Drivetrain *drivetrain;
	Camera *camera;
	Compressor *compressor;
	Picker *picker;
	Shooter *shooter;
	Portcullis *portcullis;
	Climber *climber;

	// autonomous variables
	int counter = 0;


void RobotInit()
{
	// live window inits
	chooser = new SendableChooser();
	chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
	chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
	SmartDashboard::PutData("Auto Modes", chooser);

	// main class inits
	inputs = new OperatorInputs();
	drivetrain = new Drivetrain( inputs,&m_ds);
	camera = new Camera(inputs, drivetrain);
	compressor = new Compressor(0);
	picker = new Picker(inputs);
	shooter = new Shooter(inputs);
	portcullis = new Portcullis(inputs);
	climber = new Climber(inputs);

	camera->Init();
}


/**
 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
 * below the Gyro
 *
 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
 * If using the SendableChooser make sure to add them to the chooser code above as well.
 */
void AutonomousInit()
{
	counter = 0;
	autoSelected = *((std::string*)chooser->GetSelected());
	//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
	std::cout << "Auto selected: " << autoSelected << std::endl;

	if (autoSelected == autoNameCustom)
	{
		//Custom Auto goes here
	}
	else
	{
		//Default Auto goes here
	}
}


void AutonomousPeriodic()
{
	if (counter < 4)
	{
		if (drivetrain->getIsDoneDriving() == false)
		{
			drivetrain->driveDistance(2.0);
		}
		if (drivetrain->getIsDoneDriving() == true)
		{
			drivetrain->setAngle(90);
		}
		if (drivetrain->getIsTurning() == true && drivetrain->getIsDoneDriving() == true)
		{
			//drivetrain->turnAngle();
		}
		if (drivetrain->getIsTurning() == false && drivetrain->getIsDoneDriving() == true)
		{
			counter++;
			drivetrain->driveDistance(2);
		}
	}

	/*
	if (autoSelected == autoNameCustom)
	{
		//Custom Auto goes here
	}
	else
	{
		//Default Auto goes here
	}
	*/
}


void TeleopInit()
{
	compressor->Start();
	drivetrain->Init();
	camera->Start();
}


void TeleopPeriodic()
{
	drivetrain->setPower();
	drivetrain->childProofShift();
	camera->Loop();
	picker->Loop();
	portcullis->Loop();
}


void TestInit()
{
	drivetrain->Init();
	camera->Start();
}


void TestPeriodic()
{
	drivetrain->TestLoop();
	camera->Loop();
}


void DisabledInit()
{
	drivetrain->setGearLow();
	camera->Stop();
	compressor->Stop();
}


};


START_ROBOT_CLASS(Robot)
