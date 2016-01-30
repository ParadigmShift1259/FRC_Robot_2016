#include "WPILib.h"
#include "OperatorInputs.h"
#include "drivetrain.h"
#include "compressor.h"
#include "Climber.h"
#include "Opener.h"
#include "Picker.h"
#include "Shooter.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;
	Drivetrain *drivetrain;
	OperatorInputs *inputs;
	Compressor *compressor;
	Climber *climber;
	Opener *opener;
	Picker *picker;
	Shooter *shooter;

	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);
		inputs = new OperatorInputs();
		drivetrain = new Drivetrain( inputs,&m_ds);
		compressor = new Compressor(0);
		picker = new Picker(inputs);
		climber = new Climber(inputs);
		opener = new Opener(inputs);
		shooter = new Shooter(inputs);
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
		autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}


	void AutonomousPeriodic()
	{
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}


	void TeleopInit()
	{
		compressor->Start();
	}


	void TeleopPeriodic()
	{
		drivetrain->setPower();
		drivetrain->childProofShift();
		opener->MoveUp();
		picker->movePicker();
	}


	void TestInit()
	{
	}


	void TestPeriodic()
	{
		drivetrain->testDrive();
	}


	void DisabledInit()
	{
		compressor->Stop();
		drivetrain->setGearLow();
	}
};


START_ROBOT_CLASS(Robot)
