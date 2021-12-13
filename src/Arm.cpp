#include "Arm.hpp"
#include <PololuMaestro.h>
extern MicroMaestro maestro;

Arm::Arm(uint8_t wrist, uint8_t thumb, uint8_t index, uint8_t middle, uint8_t ring, uint8_t pinky, uint16_t speed, uint16_t acceleration, uint16_t open, uint16_t close){
    this->wrist = wrist;
    this->thumb = thumb;
    this->index = index;
    this->middle = middle;
    this->ring = ring;
    this->pinky = pinky;
    this->speed = speed;
    this->acceleration = acceleration;
    this->open = open;
    this->close = close;
}

Arm::Arm(const  Arm& copy){
    this->wrist = copy.getWrist();
    this->thumb = copy.getThumb();
    this->index = copy.getIndex();
    this->middle = copy.getMiddle();
    this->ring = copy.getRing();
    this->pinky = copy.getPinky();
    this->speed = copy.getSpeed();
    this->acceleration = copy.getAcceleration();
}

Arm::~Arm(){

}

uint8_t Arm::getWrist()const{
    return this->wrist;
}

uint8_t Arm::getThumb()const{
    return this->thumb;
}

uint8_t Arm::getIndex()const{
    return this->index;
}

uint8_t Arm::getMiddle()const{
    return this->middle;
}

uint8_t Arm::getRing()const{
    return this->ring;
}

uint8_t Arm::getPinky()const{
    return this->pinky;
}

uint16_t Arm::getSpeed()const{
    return this->speed;
}

uint16_t Arm::getAcceleration()const{
    return this->acceleration;
}



uint16_t Arm::getWristPos()const{
    return (maestro.getPosition(this->wrist));
}

uint16_t Arm::getThumbPos()const{
    return (maestro.getPosition(this->thumb));
}


uint16_t Arm::getIndexPos()const{
    return (maestro.getPosition(this->index));
}

uint16_t Arm::getMiddlePos()const{
    return (maestro.getPosition(this->middle));
}

uint16_t Arm::getRingPos()const{
    return (maestro.getPosition(this->ring));
}

uint16_t Arm::getPinkyPos()const{
    return (maestro.getPosition(this->pinky));
}



void Arm::setWrist(uint8_t wrist){
    this->wrist = wrist;
    return;
}

void Arm::setThumb(uint8_t thumb){
    this->thumb = thumb;
    return;
}

void Arm::setIndex(uint8_t index){
    this->index = index;
    return;
}

void Arm::setMiddle(uint8_t middle){
    this->middle = middle;
    return;
}

void Arm::setRing(uint8_t ring){
    this->ring = ring;
    return;
}

void Arm::setPinky(uint8_t pinky){
    this->pinky = pinky;
    return;
}

void Arm::setSpeed(uint16_t speed){
    this->speed = speed;
    return;
}

void Arm::setAcceleration(uint16_t acceleration){
    this->acceleration = acceleration;
    return;
}


// 992 us = 3 968
// 2000 us = 8000
void Arm::openThumb()const{
    maestro.setTarget(this->thumb,this->open);
    return;
}

void Arm::openIndex()const{
    maestro.setTarget(this->index,this->open);
    return;
}

void Arm::openMiddle()const{
    maestro.setTarget(this->middle,this->open);
    return;
}

void Arm::openRing()const{
    maestro.setTarget(this->ring,this->open);
    return;
}

void Arm::openPinky()const{
    maestro.setTarget(this->pinky,this->open);
    return;
}


void Arm::closeThumb()const{
    maestro.setTarget(this->thumb, close);
    return;
}

void Arm::closeIndex()const{
    maestro.setTarget(this->index, close);
    return;
}

void Arm::closeMiddle()const{
    maestro.setTarget(this->middle, close);
    return;
}

void Arm::closeRing()const{
    maestro.setTarget(this->ring, close);
    return;
}

void Arm::closePinky()const{
    maestro.setTarget(this->pinky, close);
    return;
}


void Arm::openFist()const{
    openThumb();
    openIndex();
    openMiddle();
    openRing();
    openPinky();
    return;
}

void Arm::closeFist()const{
    closeThumb();
    closeIndex();
    closeMiddle();
    closeRing();
    closePinky();
    return;
}

void Arm::moveFinger(uint8_t finger, uint16_t pos){
    maestro.setTarget(finger, pos);
}

void Arm::thumbsUp()const{
    openThumb();
    closeIndex();
    closeMiddle();
    closeRing();
    closePinky();
}

void Arm::peace()const{
    closeThumb();
    openIndex();
    openMiddle();
    closeRing();
    closePinky();
}

void Arm::ok()const{
    closeThumb();
    closeIndex();
    openMiddle();
    openRing();
    openPinky();
}

void Arm::countdown()const{
    openFist();
    closePinky();
    delay(1000);
    closeRing();
    delay(1000);
    closeMiddle();
    delay(1000);
    closeIndex();
    delay(1000);
    closeThumb();

}

void Arm::rock()const{
    openPinky();
    closeRing();
    closeMiddle();
    openIndex();
    closeThumb();
}

void Arm::callMe()const{
    openPinky();
    closeRing();
    closeMiddle();
    closeIndex();
    openThumb();
}

