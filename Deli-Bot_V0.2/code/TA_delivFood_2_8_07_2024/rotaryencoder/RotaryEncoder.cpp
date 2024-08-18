#include "RotaryEncoder.h"
#include "mbed.h"


RotaryEncoder::RotaryEncoder(PinName channelA, PinName channelB, int mode, float ppr) : cA(channelA), cB(channelB), _mode(mode), _ppr(ppr){
    pulse = 0;
    prevT = 0;
    nowT = 0;
    dt = 0;
    pi = 3.14159265358979323846;
    cA.mode(PullUp);
    cB.mode(PullUp);
    if(_mode == 2){
    cA.rise(callback(this, &RotaryEncoder::callback1));
    cA.fall(callback(this, &RotaryEncoder::callback1));
    }
    if(_mode == 4){
    cA.rise(callback(this, &RotaryEncoder::callback1));
    cA.fall(callback(this, &RotaryEncoder::callback1));

    cB.rise(callback(this, &RotaryEncoder::callback2));
    cB.fall(callback(this, &RotaryEncoder::callback2));
    }
    time.reset();
    time.start();
    }

void RotaryEncoder::callback1(){
    if(cB.read() != cA.read()){
        counter++;
        }
    else{
        counter--;
        }
}

void RotaryEncoder::callback2(){
    if(cA.read() != cB.read()){
        counter--;
        }
    else{
        counter++;
        }

}

float RotaryEncoder::getPulse(){
    pulse = counter;
    counter = 0;
    return pulse;
    }  

void RotaryEncoder::resetPulse(){
    pulse = counter/_mode;
    pulse = 0; counter = 0;
    pulse = 0; counter = 0; prevPulse = 0; prevT = 0;
    time.reset();
    time.start();
}

float RotaryEncoder::getDegree(){
    pulse = counter;
    return (pulse/_ppr)*360;
    }

float RotaryEncoder::getRadian(){
    pulse = counter;
    return (pulse/_ppr)*2*pi;
}

float RotaryEncoder::getRPM(){
    nowT = time.read_high_resolution_us();
    dp = counter - prevPulse;
    dt = (nowT - prevT)/1.0e6;
    v = dp/dt;
    prevT = nowT;
    prevPulse = counter;
    return (v/_ppr)*60;
}

float RotaryEncoder::getRadpS(){
    nowT = time.read_high_resolution_us();
    dp = counter - prevPulse;
    dt = (nowT - prevT)/1.0e6;
    v = dp/dt;
    prevT = nowT;
    prevPulse = counter;
    return 2*pi*(v/_ppr);

}
