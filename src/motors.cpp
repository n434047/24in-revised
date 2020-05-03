#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Motor right_front_mtr(17, true);
pros::Motor left_front_mtr(20);
pros::Motor right_back_mtr(16, true);
pros::Motor left_back_mtr(19);

pros::Motor left_roller_mtr(3, true);
pros::Motor right_roller_mtr(5);
pros::Motor top_roller_mtr(11, true);
pros::Motor left_outtake_mtr(12);
pros::Motor right_outtake_mtr(9, true);
pros::Motor left_rainbow_mtr(13, true);
pros::Motor right_rainbow_mtr(8);
