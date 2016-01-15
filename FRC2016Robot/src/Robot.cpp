/**
 * Main Robot class
 *
 * Should be configured properly for the 2016 code
 */

#include "WPILib.h"

class Robot: public IterativeRobot {
private:

	/**
	 * Called upon robot startup
	 */
	void RobotInit() {
	}

	/**
	 * Called upon the start of autonomous
	 */
	void AutonomousInit() {
	}

	/**
	 * Loops in autonomous after the init has finished
	 */
	void AutonomousPeriodic() {
	}

	/**
	 * Called upon at the start of the user operated period
	 */
	void TeleopInit() {

	}

	/**
	 * Loops after the user operated init has finished
	 */
	void TeleopPeriodic() {
	}

	/**
	 * Called upon the start of the test period
	 */
	void TestInit() {
	}

	/**
	 * Loops after the the test init has finished
	 */
	void TestPeriodic() {
	}
};

START_ROBOT_CLASS(Robot)
