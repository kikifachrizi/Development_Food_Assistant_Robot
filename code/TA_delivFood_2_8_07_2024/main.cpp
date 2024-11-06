#include "mbed.h"
#include "RotaryEncoder.h"

DigitalIn pid_sw(PA_3);

DigitalOut m2A(PB_0);
DigitalOut m2B(PB_1);
PwmOut m2(PB_8);

DigitalOut m1A(PB_2);
DigitalOut m1B(PB_10);
PwmOut m1(PB_9);

DigitalOut m3A(PB_13);
DigitalOut m3B(PB_12);
PwmOut m3(PA_8);

RotaryEncoder enc_m1(PA_6,PA_7,2,134);
RotaryEncoder enc_m2(PA_0,PA_1,2,134);
RotaryEncoder enc_m3(PB_4,PB_5,2,134);

Serial esp(PA_11, PA_12);

const int numChars = 100;
char receivedChars[numChars];   // an array to store the received data
char *vx;
char *dx;
float dX = 0;
float dY = 0;
float Exr = 0;
float Eyr = 0;
float Eth = 0;
float Vy = 0;
float Vx = 0;
float tetta = 0;
float kar=0;

bool newData = false;

void recvWithEndMarker() {
    static int ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (esp.readable() > 0 && newData == false){
        rc = esp.getc();
//        printf("%c\n",rc);

        if (rc != endMarker){
              receivedChars[ndx] = rc;
              ndx++;
              if (ndx >= numChars) {
                  ndx = numChars;
              }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void olahDataString(){
        vx = strtok(receivedChars,";");
        Vx = atof(vx);
        
        vx = strtok(NULL, ";"); // this continues where the previous call left off
        Vy = atof(vx);        // convert this part to an integer

        vx = strtok(NULL, ";");
        tetta = atof(vx); // convert this part to a float
}
void olahDataOdom() {
    dx = strtok(receivedChars, ";");
    Exr = atof(dx);

    dx = strtok(NULL, ";");
    Eyr = atof(dx);

    dx = strtok(NULL, ";");
    Eth = atof(dx);

    // dx = strtok(NULL, ";");
    // kar = atof(dx);    

}

extern serial_t     stdio_uart; 
extern int          stdio_uart_inited; 

Ticker pid;
float dt = 0.01;
float e[3];
float eI[3];
float eD[3];
float laste[3];
float w[3];
float hP[3];
float hI[3];
float hD[3];
float setI[3];
float pidPwm[3];
float pwmLebih[3];
float lastPid[3];
float pid_var[3];

// ideal
// float kp = 0.235;
// float ki = 0.010; 
// float kd = 0.0005;

// tambah beban & gesekan
float kp = 0.235;
float ki = 0.015; 
float kd = 0.0005;

void pid_start(){ 
    pid_var[0] = enc_m1.getPulse();
    pid_var[1] = enc_m2.getPulse();
    pid_var[2] = enc_m3.getPulse();

    if(pid_sw.read() == 0){

        w[2] = Exr*0.5 + Eyr*-0.86602540378 + Eth;
        w[1] = Exr*0.5 + Eyr*0.86602540378 + Eth;
        w[0] = Exr*-1 + Eyr*0 + Eth;

        for(int i = 0;i < 3;i++){
            e[i] = w[i] - pid_var[i];
            eI[i] += e[i];
            eD[i] = e[i] - laste[i];

            hP[i] = e[i]*kp;
            hI[i] = eI[i]*ki*dt;
            hD[i] = eD[i]*kd/dt;

            if(hI[i] >=5){setI[i] = 1;}
            else if(hI[i] < -5){setI[i] = 1;}
            else{
                setI[i] = hI[i];
            }

            pidPwm[i] = hP[i]+setI[i]+hD[i];
            laste[i] = e[i];
                    
            if(pidPwm[i] >= 1){pwmLebih[i] = 1;}
            else if(pidPwm[i] < -1){pwmLebih[i] = -1;}
            else{
                pwmLebih[i] = pidPwm[i];
            }

        }

        //motor direksi start
        if(pidPwm[0] < 0){m1A = 1; m1B = 0;}
        else{m1A = 0; m1B = 1;}
        if(pidPwm[1] < 0){m2A = 1; m2B = 0;}
        else{m2A = 0; m2B = 1;}
        if(pidPwm[2] < 0){m3A = 1; m3B = 0;}
        else{m3A = 0; m3B = 1;}
        //motor direksi end

        m1 = fabs(pwmLebih[0]);
        m1.period(0.01);
        m2 = fabs(pwmLebih[1]);
        m2.period(0.01);
        m3 = fabs(pwmLebih[2]);
        m3.period(0.01);
    }else{
        for(int i = 0;i<3;i++){
            hP[i] = 0;
            hI[i] = 0;
            setI[i] = 0;
            hD[i] = 0;
            pidPwm[i] = 0;
            pwmLebih[i] = 0;
            laste[i] = 0;
            e[i] = 0;
            eI[i] =0;
            eD[i] = 0;
        }
        m1 = 0;m1A =0;m1B =0;
        m2 = 0;m2A =0;m2B =0;
        m3 = 0;m3A =0;m3B =0;
    }
}
float targetOdom = 16000;
float err_odom;
Timer rec;
char *data_;
float data__ = 0;

int main()
{
    esp.baud(9600);
    serial_init(&stdio_uart, PA_9, PA_10); // other than Serial2
    stdio_uart_inited = 1; 
    printf("new code\r\n");
    pid.attach(&pid_start, 0.01);
    

    while (true) {
        // Exr = 0;
        // Eyr = 3;
        // printf("%f ; %f ; %f \n", encoderPos[0],encoderPos[1],encoderPos[2]);
        // printf("%f ; %f ; %f \n", pid_var[0],pid_var[1],pid_var[2]);
        recvWithEndMarker();
        if(newData == true){
          olahDataOdom(); 

        }
            newData = false;
        }
}
