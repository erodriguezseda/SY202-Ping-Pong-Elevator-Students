/*************************************************************************************
Program Name: 
Description: 
Author: 
Date: 
**************************************************************************************/
/************************** Pseudo code*******************************
    Include required libraries
    Declare serial object to PC, Ananlog Input for sensor, PWM output for Air Blower
    Declare variables for duty cycle, error, desired height, etc. 
    Define coefficients of polynomial fit for volatge-to-height
    Define other control parameters such as Kp, Ki, Kd, etc. 
    Set Serial Connection to PC to desired baudrate
    While(true)  //indefinite while loop
        Set Air Blower to zero 
        Ask User for:
            Q1: Control type
            Q2: Desired height of elevator
            Q3: Duration of experiment
            Q4: Frequency of the control experiment
        While (time <= Duration of experiment)  //Enter Control Loop
            Read Sensor
            Convert from voltage to Measured Height
            Compute Error = Desired Height - Measured Height 
            If Logic Control 
                If Error > 1 cm
                    Set Control Signal to go up
                Else-If Error < -1 cm
                    Set Control Signal to go down
                Else
                    Set Control Signal to Zero
                End-If
            Else If P-Control
                Control Signal = Kp*Error
            Else If PI-Control
                Compute Integral of Error Signal
                Control Signal = Kp*Error + Ki*IntegralError
            End-If
            Add "DC Hold Value" to Control Signal
            Cap Control Signal (that is, duty cycle) if it is outside range from 0 to 1 
            Set Air Blower to control signal
            Regulate frequency of control law execution (already on code provided)
        End-While
        Set Air Blower to zero
    End-While
**********************************************************************/
//------------- START PROGRAM
/************** Libraries ******************************************/
#include "mbed.h"
#include "platform/mbed_thread.h"
/************** Declare Input and Output Objects ******************************************/ 
Serial pc(USBTX,USBRX);
PwmOut blower(p21);

//Time control variables
Timer t;                      //[Do not modify] Timer to regulate control sequence
Timer tlog;                   //[Do not modify] Timer to log data
float fs = 10.0;              //[Do not modify] Declare control frequency variable
float Ts = 1/fs;              //[Do not modify] Declare control period variable
float dt = 0.0;               //[Do not modify] Time elapsed
float Tf = 10.0;              //[Do not modify] Duration of experiment

//Other variables
float Height = 0.0;           //[Do not modify] Height in cm of ball     
float ErrorSignal;            //[Do not modify] Error signal
float ControlSignal;          //[Do not modify] Control signal to actuator

//Add other variables, functions, etc.


int main()
{   
    //Set baudrate

    while(1) {
        blower = 0.0;
        //Ask user for experiment paramters
        // Questions Q1, Q2, Q3, Q4
        Ts = 1/fs;
        t.start();
        tlog.start();
        while(tlog.read() <= Tf){
            // Control-Loop; Fill out Lines below
            // Read the sensor and covert to cm
            // Compute error signal
            // Compute control signal
 

            // [Do not modify code below these lines]
            blower = ControlSignal;    //[Do not modify] Send control signal to actuator
            // [Do not modify] Print time variable, height, etc.
            pc.printf("%.3f,%.3f,%.3f,%.3f\n", tlog.read(), Height, ErrorSignal, ControlSignal);
            dt = Ts-t.read();
            t.stop();
            t.reset();
            if (dt >= 0) {
                thread_sleep_for(dt*1000);
            }  //Regulates the control sampling time
            t.start();
        }
        blower = 0.0;
        t.stop();
        t.reset();
        tlog.stop();
        tlog.reset();
    }
}
