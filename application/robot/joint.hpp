#ifndef _JOINT_HPP_
#define _JOINT_HPP_

#include "stdint.h"

template <class Driver>
class Joint {
    
private:
    Driver driver;

public:
    
    Joint(const uint8_t id_): driver(id_)
    { }
    
    float now_angle(void)
    {
        return driver.export_get_angle();
    }
    
};

//template <class Driver>
//Joint<Driver>::Joint(const uint8_t id_)
//{
//    driver.export_init(id_);
//}


#endif /* _JOINT_HPP_ */

