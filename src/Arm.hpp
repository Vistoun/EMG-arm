class Arm{
public:
    Arm();
    Arm(int, int, int, int, int, int, int, int, int, int);
    Arm(const Arm&);
    virtual ~Arm();

    int getWrist()const;
    int getThumb()const;
    int getIndex()const;
    int getMiddle()const;
    int getRing()const;
    int getPinky()const;
    int getSpeed()const;
    int getAcceleration()const;

    int getWristPos()const;
    int getThumbPos()const;
    int getIndexPos()const;
    int getMiddlePos()const;
    int getRingPos()const;
    int getPinkyPos()const;
    int getSpeedPos()const;


    void setWrist(int);
    void setThumb(int);
    void setIndex(int);
    void setMiddle(int);
    void setRing(int);
    void setPinky(int);
    void setSpeed(int);
    void setAcceleration(int);
    
    void openThumb();
    void openIndex();
    void openMiddle();
    void openRing();
    void openPinky();

    void closeThumb();
    void closeIndex();
    void closeMiddle();
    void closeRing();
    void closePinky();

    void openFist();
    void closeFist();    

    void moveFinger(int, int);

    //Gesta
    void thumbsUp();
    void peace();
    void ok();
    void countdown();
    void rock();
    void callMe();
    


private:
int wrist,thumb,index,middle,ring, pinky, speed, acceleration, open, close;

//bool kontrola(int, int, int, int, int, int, int, int)const;

};