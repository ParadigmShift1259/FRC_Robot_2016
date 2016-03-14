// autonomous.cpp


#include "Autonomous.h"
#include "const.h"
#include <driverstation.h>
#include "VisionTargeting.h"


Autonomous::Autonomous(DriverStation *driverstation, OperatorInputs *operatorinputs, Drivetrain *drivetrain, VisionTargeting *vt)
{
	count = 0;
	oldtime = 0;
	autotimer = new Timer();
	m_visiontargeting = vt;
	m_driverstation = driverstation;
	m_inputs = operatorinputs;
	m_drivetrain = drivetrain;
	m_gyro = new AnalogGyro(ALG_AUTONOMOUS_GYRO);
	m_gyro->Calibrate();
	m_accel = new BuiltInAccelerometer(Accelerometer::Range::kRange_4G);
	m_stage = kStartDriveForward;
	m_driveangle = 0;
	m_counter = 0;
	m_kS = SmartDashboard::GetNumber("DB/Slider 2", 0);
	if (m_kS == 0) {m_kS = 0.0025;}
}


Autonomous::~Autonomous()
{
}


void Autonomous::Init()
{
	m_gyro->Reset();
	m_stage = kStartDriveForward;
	m_driveangle = 0;
	m_counter = 0;
	m_drivetrain->Shift(true);
	m_drivetrain->SetRamp(1.173);
}


void Autonomous::Loop()
{
	cout << "Distance" <<m_drivetrain->GetDistance() << std::endl;
	cout << "Angle" << m_drivetrain->GetAngle() << std::endl;
	double angle = m_gyro->GetAngle();
	SmartDashboard::PutNumber("Gyro Raw", angle);

	double driveangle = angle - m_driveangle;
	SmartDashboard::PutNumber("Gyro Heading", driveangle);

	double xaccel = m_accel->GetX();
	double yaccel = m_accel->GetY();
	double zaccel = m_accel->GetZ();
	SmartDashboard::PutNumber("Xaccel", xaccel);
	SmartDashboard::PutNumber("Yaccel", yaccel);
	SmartDashboard::PutNumber("Zaccel", zaccel);

	double leftposition = m_drivetrain->LeftTalon()->GetPosition();
	double rightposition = m_drivetrain->RightTalon()->GetPosition();

	SmartDashboard::PutNumber("AUT1_leftpos", leftposition);
	SmartDashboard::PutNumber("AUT2_rightpos", rightposition);
	switch (m_stage)
	{
	case kStartDriveForward:
		m_drivetrain->SetDistance(190.0);
		m_stage = kDrivingForward;
		break;
	case kDrivingForward:
		m_drivetrain->DriveAuto();
		if(m_drivetrain->DistanceReached()){
			m_stage = kDelayBeforeShoot;
			m_drivetrain->SetDesiredDriveAngle(0.0);
			m_drivetrain->SetDesiredDriveMagnitude(0.0);
			m_drivetrain->DriveAuto();
			autotimer->Start();
			oldtime = autotimer->Get();
		}
		break;
	case kDelayBeforeShoot:
		if(autotimer->Get()-oldtime > 1){
			m_stage = kShoot;
			count = 0;
		}
		break;
	case kShoot:
		m_drivetrain->Shift(false);
		m_visiontargeting->Loop(true);
		m_stage = kShooting;
		break;
	case kShooting:
		break;
	}
	cout << "Auto Stage " << m_stage << std::endl;
}

void Autonomous::Disable() {
	m_drivetrain->SetRamp(0.0);
}

void Autonomous::Calibrate()
{
	m_gyro->Calibrate();
}
