#include "Arm.hpp"
#include <PololuMaestro.h>
extern MicroMaestro maestro;

Arm::Arm(int wrist, int thumb, int index, int middle, int ring, int pinky, int speed, int acceleration, int open, int close){
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

int Arm::getWrist()const{
    return this->wrist;
}

int Arm::getThumb()const{
    return this->thumb;
}

int Arm::getIndex()const{
    return this->index;
}

int Arm::getMiddle()const{
    return this->middle;
}

int Arm::getRing()const{
    return this->ring;
}

int Arm::getPinky()const{
    return this->pinky;
}

int Arm::getSpeed()const{
    return this->speed;
}

int Arm::getAcceleration()const{
    return this->acceleration;
}



void Arm::setWrist(int wrist){
    this->wrist = wrist;
}

void Arm::setThumb(int thumb){
    this->thumb = thumb;
}

void Arm::setIndex(int index){
    this->index = index;
}

void Arm::setMiddle(int middle){
    this->middle = middle;
}

void Arm::setRing(int ring){
    this->ring = ring;
}

void Arm::setPinky(int pinky){
    this->pinky = pinky;
}

void Arm::setSpeed(int speed){
    this->speed = speed;
}

void Arm::setAcceleration(int acceleration){
    this->acceleration = acceleration;
}


// 992 us = 3 968
// 2000 us = 8000
void Arm::openThumb(){
    maestro.setTarget(this->thumb,this->open);
}

void Arm::openIndex(){
    maestro.setTarget(this->index,this->open);
}

void Arm::openMiddle(){
    maestro.setTarget(this->middle,this->open);
}

void Arm::openRing(){
    maestro.setTarget(this->ring,this->open);
}

void Arm::openPinky(){
    maestro.setTarget(this->pinky,this->open);
}


void Arm::closeThumb(){
    maestro.setTarget(this->thumb, close);
}

void Arm::closeIndex(){
    maestro.setTarget(this->index, close);
}

void Arm::closeMiddle(){
    maestro.setTarget(this->middle, close);
}

void Arm::closeRing(){
    maestro.setTarget(this->ring, close);
}

void Arm::closePinky(){
    maestro.setTarget(this->pinky, close);
}


void Arm::openFist(){
    openThumb();
    openIndex();
    openMiddle();
    openRing();
    openPinky();

}

void Arm::closeFist(){
    closeThumb();
    closeIndex();
    closeMiddle();
    closeRing();
    closePinky();
}