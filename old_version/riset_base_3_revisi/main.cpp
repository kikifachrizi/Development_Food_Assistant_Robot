#include "mbed.h"
#include "RotaryEncoder.h"
#include "PID_lib.h"
#include "BNO055.h"

I2C    i2c(PB_9, PB_8);         // SDA, SCL
BNO055 imu(i2c, PA_8);          // Reset
 
BNO055_ID_INF_TypeDef bno055_id_inf;
BNO055_EULER_TypeDef  euler_angles;

Serial pc(USBTX, USBRX);
Serial esp(PA_11,PA_12);

RotaryEncoder eB(PC_3, PC_0, 2, 270);
RotaryEncoder eC(PB_10,PA_9, 2, 270);
RotaryEncoder eA(PC_11,PC_12,2, 270);

PID_lib m1(PA_5,PA_6, PA_7);
PID_lib m2(PB_13,PB_14, PB_15);
PID_lib m3(PB_5,PB_4, PB_3);

Timer t;
Timer t_pid;

double kp1 = 0;
double ki1 = 0;
double kd1 = 0;

double kp2 = 0.0075;
double ki2 = 0;
double kd2 = 0.3;

double kp3 = 0.0075;
double ki3 = 0;
double kd3 = 0.3;

// double kp2 = 2.0533;
// double ki2 = 2.0091;
// double kd2 = 0.15528;

// double kp3 = -1.7665;
// double ki3 = -3.4051;
// double kd3 = -0.22912;

float target = 100.0;

void record();
void test_pid();
void heading();

float pwm = 0.12;

int main()
{
    esp.baud(115200);
    t.reset();
    t.start();
    t_pid.reset();
    t_pid.start();


    // pc.printf("Bosch Sensortec BNO055 test program on " __DATE__ "/" __TIME__ "\r\n");
    // if (imu.chip_ready() == 0){
    //     pc.printf("Bosch BNO055 is NOT avirable!!\r\n");
    // }
    // imu.read_id_inf(&bno055_id_inf);
    // pc.printf("CHIP:0x%02x, ACC:0x%02x, MAG:0x%02x, GYR:0x%02x, , SW:0x%04x, , BL:0x%02x\r\n",bno055_id_inf.chip_id, bno055_id_inf.acc_id, bno055_id_inf.mag_id,bno055_id_inf.gyr_id, bno055_id_inf.sw_rev_id, bno055_id_inf.bootldr_rev_id);
    // pc.printf("start Cd\n");

    // test_pid();
    // record();
    while (true) {
        // pc.printf("%f ; %f ; %f\n", eA.getPulse(),eB.getPulse(),eC.getPulse());
        // record();

        char remot = esp.getc();
        // pc.printf("%c ; %f\n", remot, pwm);
        if(remot == 'j'){
            NVIC_SystemReset();
        }else if(remot == 'b'){
            // pc.printf("%c\n",remot);
            // mundur
            // m1.manualPwm(0.1);
            // m2.manualPwm(0.110);
            // m3.manualPwm(0.118);
            m2.manualPwm(pwm);
            m3.manualPwm(-pwm);
            // pc.printf("%f\n", pwm);
        }else if(remot == 'c'){
            // // kanan
            // m1.manualPwm(0.2);
            // m2.manualPwm(-0.15);
            // m3.manualPwm(0.15);
            m1.manualPwm(pwm);
            m2.manualPwm(pwm);
            m3.manualPwm(pwm);
        }else if(remot == 'a'){
            // maju
            m2.manualPwm(-pwm);
            m3.manualPwm(pwm);
            // m2.pid_speed(50, kp2, ki2,kd2, eB.getRPM(), t_pid.read_high_resolution_us());
            // m3.pid_speed(-50, kp3, ki3,kd3, eC.getRPM(), t_pid.read_high_resolution_us());
        }else if(remot == 'd'){
            // kiri
            // m1.manualPwm(-0.2);
            // m2.manualPwm(0.15);
            // m3.manualPwm(-0.15);
            m1.manualPwm(-pwm);
            m2.manualPwm(-pwm);
            m3.manualPwm(-pwm);
        }else if(remot == 't'){
            // tambah pwm
            if(pwm >= 1.0){
                pwm = 1.0;
            }else{pwm += 0.01;}
        }else if(remot == 's'){
            // kurangi pwm
            if(pwm <= 0.0){pwm = 0.0;}else{
            pwm -= 0.01;}
        }else if(remot == 'f'){
            // record data (x)
            record();
        }else if(remot == 'h'){
            // heading
            // heading();
            test_pid();
        }else if(remot == 'l'){
            // rotasi kanan
            m1.manualPwm(-pwm);
            m2.manualPwm(pwm);
            m3.manualPwm(pwm);
        }else if(remot == 'k'){
            // rotasi kanan
            m1.manualPwm(pwm);
            m2.manualPwm(-pwm);
            m3.manualPwm(-pwm);
        }else if(remot == 'x'){
            m1.stop();
            m2.stop();
            m3.stop();
            t_pid.stop();
            t_pid.reset();
        }


    }
}

void test_pid(){
    // wait(2);
    t.reset();
    t.start();
    t_pid.reset();
    t_pid.start();
    while (t.read_ms() < 10000) {
        m2.pid_speed(100, kp2, ki2,kd2, eB.getRPM(), t_pid.read_high_resolution_us());
        m3.pid_speed(-100, kp3, ki3,kd3, eC.getRPM(), t_pid.read_high_resolution_us());

        // printf("%f;%f;%f;%f;%llu\n",m2.pwm_read(),m3.pwm_read(),mtrB,mtrC,t.read_high_resolution_us());
        // printf("%f %f\n",mtrB,mtrC);
    }
    m1.stop();
    m2.stop();
    m3.stop();
    t_pid.stop();
    t_pid.reset();
    t.stop();
}

void record(){
       t.reset();
       t.start();
       while(t.read_ms() < 15000){
    //    manual
        // m1.manualPwm(1, 0.1);
            m2.manualPwm(-0.120);
            m3.manualPwm(0.140);

            // m2.manualPwm(1.0);
            // m3.manualPwm(1.0);


        //record data
        // float check2 =  m2.record_data(1, 0.10, eB.getFreq(),t.read_high_resolution_us());
        // float check3 =  m3.record_data(1, 0.10, eC.getFreq(),t.read_high_resolution_us());

        
        // pc.printf("%f\n", check);
        // pc.printf("%llu\n", t3.read_high_resolution_us());
        // printf("-0.120;0.140;%f;%f;%llu\n",eB.getRPM(),eC.getRPM(),t.read_high_resolution_us());

        printf("100 %f %f\n",eB.getRPM(),eC.getRPM());

        // pid test
        // m1.pid_speed(-50, kp2, ki2,kd2, eA.getRPM(), t_pid.read_high_resolution_us());
        // m2.pid_speed(50, kp2, ki2,kd2, eB.getRPM(), t_pid.read_high_resolution_us());
        // m3.pid_speed(-50, kp3, ki3,kd3, eC.getRPM(), t_pid.read_high_resolution_us());

        // printf("0.1;%.2f;%.2f;%llu\n",cek2,cek3,t.read_high_resolution_us());

   }
   m1.stop();
   m2.stop();
   m3.stop();
   t_pid.stop();
   t_pid.reset();
   t.stop();
}
void heading(){
    // heading logic start
        imu.get_euler_angles(&euler_angles);
        //   pc.printf("Heading:%+6.1f [deg], Roll:%+6.1f [deg], Pich:%+6.1f [deg]\r\n",euler_angles.h, euler_angles.r, euler_angles.p);
        //   wait_ms(0.5);
   
        // pc.printf("%+6.1f [deg]\n", euler_angles.h); 

        float err_deg = target - euler_angles.h;
        // pc.printf("%f\n", err_deg);
        // pc.printf("target : %f , err : %f , degree : %f\n",target, err_deg, euler_angles.h);
        pc.printf("%f;%f;%f;%llu\n",target, err_deg, euler_angles.h,t.read_high_resolution_us());

        while(err_deg > 10 || err_deg < -10){
            if(err_deg > 0){
                // jalan kanan
                m1.manualPwm(0.06);
                m2.manualPwm(0.06);
                m3.manualPwm(0.06);
            }else if(err_deg < 0){
                // jalan kiri
                m1.manualPwm(0.05);
                m2.manualPwm(0.05);
                m3.manualPwm(0.05);
            }
            // pc.printf("target : %f , err : %f , degree : %f\n",target, err_deg, euler_angles.h);
            pc.printf("%f;%f;%f;%llu\n",target, err_deg, euler_angles.h,t.read_high_resolution_us());
            imu.get_euler_angles(&euler_angles);
            err_deg = target - euler_angles.h;
        }
        m1.stop();
        m2.stop();
        m3.stop();
        pc.printf("robot stop\n");
// heading logic stop
}
