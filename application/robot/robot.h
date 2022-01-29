#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "kinematic/6dof_kinematic.h"
#include "servo_ctrl.h"

class RobotArm
{
    public:
        
        explicit RobotArm(void);
        
        struct Joints {
            ServoObject* j_1 = nullptr;
            ServoObject* j_2 = nullptr;
            ServoObject* j_3 = nullptr;
            ServoObject* j_4 = nullptr;
            ServoObject* j_5 = nullptr;
            ServoObject* j_6 = nullptr;
        };
        struct Joints joints;
        
        const DOF6Kinematic::Joint6D_t RESET_POSE = {0, 0, 0, 0, 0, 0};
        
        DOF6Kinematic::Joint6D_t current_joints = RESET_POSE;
        DOF6Kinematic::Joint6D_t init_pose = RESET_POSE;
        DOF6Kinematic::Pose6D_t current_pose = {};
        
    
    private:
        DOF6Kinematic* dof6_solver = nullptr;
    
};


#endif



