#ifndef _LCD_MESSAGE_H_
#define _LCD_MESSAGE_H_

#include "stdint.h"

enum class MessageType {
    MESSAGE_ALARM,
    MESSAGE_NORMAL
};

class LCD_Message
{
    public:
        char* message = nullptr;
        MessageType m_type;
        
        explicit LCD_Message(MessageType m_type, char* message);
    
        uint32_t get_id(void)
        {
            return id;
        }
    
    private:
        uint32_t id;
};


#endif /* _LCD_MESSAGE_H_ */




