#include <Arduino.h>

class Arm{
public:
    Arm();
    Arm(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t, uint16_t, uint16_t, uint16_t);
    Arm(const Arm&);
    virtual ~Arm();

    uint8_t getWrist()const;
    uint8_t getThumb()const;
    uint8_t getIndex()const;
    uint8_t getMiddle()const;
    uint8_t getRing()const;
    uint8_t getPinky()const;
    uint16_t getSpeed()const;
    uint16_t getAcceleration()const;

    uint16_t getWristPos()const;
    uint16_t getThumbPos()const;
    uint16_t getIndexPos()const;
    uint16_t getMiddlePos()const;
    uint16_t getRingPos()const;
    uint16_t getPinkyPos()const;
    uint16_t getSpeedPos()const;


    void setWrist(uint8_t);
    void setThumb(uint8_t);
    void setIndex(uint8_t);
    void setMiddle(uint8_t);
    void setRing(uint8_t);
    void setPinky(uint8_t);
    void setSpeed(uint16_t);
    void setAcceleration(uint16_t);
    
    void openThumb()const;
    void openIndex()const;
    void openMiddle()const;
    void openRing()const;
    void openPinky()const;

    void closeThumb()const;
    void closeIndex()const;
    void closeMiddle()const;
    void closeRing()const;
    void closePinky()const;

    void openFist()const;
    void closeFist()const;    

    void moveFinger(uint8_t, uint16_t);

    //Gesta
    void thumbsUp()const;
    void peace()const;
    void ok()const;
    void countdown()const;
    void rock()const;

    void callMe()const;
    


private:
uint8_t wrist, thumb, index, middle, ring, pinky; 
uint16_t speed, acceleration, open, close;


};
