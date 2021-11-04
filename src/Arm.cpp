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



int Arm::getWristPos()const{
    return (maestro.getPosition(this->wrist));
}

int Arm::getThumbPos()const{
    return (maestro.getPosition(this->thumb));
}


int Arm::getIndexPos()const{
    return (maestro.getPosition(this->index));
}

int Arm::getMiddlePos()const{
    return (maestro.getPosition(this->middle));
}

int Arm::getRingPos()const{
    return (maestro.getPosition(this->ring));
}

int Arm::getPinkyPos()const{
    return (maestro.getPosition(this->pinky));
}



void Arm::setWrist(int wrist){
    this->wrist = wrist;
    return;
}

void Arm::setThumb(int thumb){
    this->thumb = thumb;
    return;
}

void Arm::setIndex(int index){
    this->index = index;
    return;
}

void Arm::setMiddle(int middle){
    this->middle = middle;
    return;
}

void Arm::setRing(int ring){
    this->ring = ring;
    return;
}

void Arm::setPinky(int pinky){
    this->pinky = pinky;
    return;
}

void Arm::setSpeed(int speed){
    this->speed = speed;
    return;
}

void Arm::setAcceleration(int acceleration){
    this->acceleration = acceleration;
    return;
}


// 992 us = 3 968
// 2000 us = 8000
void Arm::openThumb(){
    maestro.setTarget(this->thumb,this->open);
    return;
}

void Arm::openIndex(){
    maestro.setTarget(this->index,this->open);
    return;
}

void Arm::openMiddle(){
    maestro.setTarget(this->middle,this->open);
    return;
}

void Arm::openRing(){
    maestro.setTarget(this->ring,this->open);
    return;
}

void Arm::openPinky(){
    maestro.setTarget(this->pinky,this->open);
    return;
}


void Arm::closeThumb(){
    maestro.setTarget(this->thumb, close);
    return;
}

void Arm::closeIndex(){
    maestro.setTarget(this->index, close);
    return;
}

void Arm::closeMiddle(){
    maestro.setTarget(this->middle, close);
    return;
}

void Arm::closeRing(){
    maestro.setTarget(this->ring, close);
    return;
}

void Arm::closePinky(){
    maestro.setTarget(this->pinky, close);
    return;
}


void Arm::openFist(){
    openThumb();
    openIndex();
    openMiddle();
    openRing();
    openPinky();
    return;
}

void Arm::closeFist(){
    closeThumb();
    closeIndex();
    closeMiddle();
    closeRing();
    closePinky();
    return;
}

void Arm::moveFinger(int finger, int pos){
    maestro.setTarget(finger, pos);
}

void Arm::thumbsUp(){
    openThumb();
    closeIndex();
    closeMiddle();
    closeRing();
    closePinky();
}

void Arm::v(){
    closeThumb();
    openIndex();
    openMiddle();
    closeRing();
    closePinky();
}

void Arm::ok(){
    closeThumb();
    closeIndex();
    openMiddle();
    openRing();
    openPinky();
}

void Arm::countdown(){
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

void Arm::rock(){
    openPinky();
    closeRing();
    closeMiddle();
    openIndex();
    closeThumb();
}

void Arm::callMe(){
    openPinky();
    closeRing();
    closeMiddle();
    closeIndex();
    openThumb();
}