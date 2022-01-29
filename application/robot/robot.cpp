#include "robot.h"

RobotArm::RobotArm(void)
{
    joints.j_1 = new ServoObject(0X01);
}


