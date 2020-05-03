#include "main.h"
#include "motors.cpp"
extern pros::Controller master;

extern pros::Motor left_roller_mtr;
extern pros::Motor right_roller_mtr;
extern pros::Motor top_roller_mtr;
extern pros::Motor left_outtake_mtr;
extern pros::Motor right_outtake_mtr;
extern pros::Motor left_rainbow_mtr;
extern pros::Motor right_rainbow_mtr;

extern pros::Motor right_front_mtr;
extern pros::Motor left_front_mtr;
extern pros::Motor right_back_mtr;
extern pros::Motor left_back_mtr;

#define DEFAULT_CURRENT 2500
pros::Mutex base_mutex;
pros::Mutex intake_mutex;

void initialize()
{
	/**
	 * Runs initialization code. This occurs as soon as the program is started.
	 *
	 * All other competition modes are blocked by initialize; it is recommended
	 * to keep execution time for this mode under a few seconds.
	 */
	pros::lcd::initialize();
}
void disabled()
{
	/**
	 * Runs while the robot is in the disabled state of Field Management System or
	 * the VEX Competition Switch, following either autonomous or opcontrol. When
	 * the robot is enabled, this task will exit.
	 */
}
void competition_initialize()
{
	/**
	 * Runs after initialize(), and before autonomous when connected to the Field
	 * Management System or the VEX Competition Switch. This is intended for
	 * competition-specific initialization routines, such as an autonomous selector
	 * on the LCD.
	 *
	 * This task will exit when the robot is enabled and autonomous or opcontrol
	 * starts.
	 */
}
void autonomous()
{

}



/////////////////////////////////////////////////
//             Driver Functions                //
/////////////////////////////////////////////////
void intakeMove(int sideRollerPower)
{
  right_roller_mtr = -sideRollerPower;
  left_roller_mtr = -sideRollerPower;
}

void indexMove(int indexPower)
{
  top_roller_mtr = indexPower;
}

void outtakeMove(int indexPower)
{
  right_outtake_mtr = indexPower;
  left_outtake_mtr = indexPower;
}

void rainbowMove(int rainbowPower)
{
  right_rainbow_mtr = rainbowPower;
  left_rainbow_mtr = rainbowPower;
}

void intakeControl(void* param)
{
  //manage intake
  std::uint32_t now = pros::millis();
  double maxTimeBeforeSecondClick = 200;
  double intakeClickTime = 0;

  int left1;
  int left2;
  int right1;
  int right2;
  int x;

  while (true) {
    left1 = master.get_digital(DIGITAL_L1);
    left2 = master.get_digital(DIGITAL_L2);
    right1 = master.get_digital(DIGITAL_R1);
    right2 = master.get_digital(DIGITAL_R2);
    x = master.get_digital(DIGITAL_X);
    if (intake_mutex.take(2)) {
      if (left1 && !right1 && !left2 && !right2) { //rollers in
        intakeMove(127);
        indexMove(0);
      } else if (left1 && right1 && !left2 && !right2) { //rollers and index in
        intakeMove(127);
        indexMove(127);
      } else if (!left1 && right1 && left2 && !right2) { //rollers and index out
        intakeMove(-127);
        indexMove(-127);
      } else if (!left1 && !right1 && left2 && !right2) { // rollers out
        intakeMove(-127);
        indexMove(0);
      } else if (!left1 && right1 && !left2 && right2) {
        rainbowMove(127);
        outtakeMove(127);
      } else if (!left1 && right1 && !left2 && !right2) { // rainbow up
        rainbowMove(127);
      } else if (!left1 && !right1 && !left2 && right2) { //rainbow down
        rainbowMove(-127);
      // } else if (!left1 && right1 && !left2 && right2) {
      //   rainbowMove(127);
      //   outtakeMove(127);
      } else if (left1 && right1 && left2 && right2) { //all score
        intakeMove(127);
        indexMove(127);
        rainbowMove(127);
        outtakeMove(127);
      } else if (!left1 && !right1 && left2 && right2) { //throw up
        intakeMove(-127);
        indexMove(-127);
        rainbowMove(-127);
      } else if (x) { //descore top ball
        outtakeMove(-127);
      } else { //shut off else
        intakeMove(0);
        indexMove(0);
        rainbowMove(0);
        outtakeMove(0);
      }
      intake_mutex.give();
     }
   pros::Task::delay_until(&now, 20);
 }
}

void baseControl(void* param)
{
  //manage base
  std::uint32_t now = pros::millis();
  right_front_mtr.set_current_limit(DEFAULT_CURRENT);
	left_front_mtr.set_current_limit(DEFAULT_CURRENT);
	right_back_mtr.set_current_limit(DEFAULT_CURRENT);
	left_back_mtr.set_current_limit(DEFAULT_CURRENT);
  while (true) {
    if (base_mutex.take(2)) {
      int left = master.get_analog(ANALOG_LEFT_Y);
      int right = master.get_analog(ANALOG_RIGHT_Y);

      left_back_mtr = left;
      right_back_mtr = right;
      left_front_mtr = left;
      right_front_mtr = right;

			base_mutex.give();
    }
    pros::Task::delay_until(&now, 20);
  }
}







void opcontrol()
{
	while (true)
	{
		pros::Task intake_task(intakeControl,(void*)"DUMMY");
		pros::Task base_task(baseControl,(void*)"DUMMY");
	}
}
