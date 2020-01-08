#include <stdio.h>
#include <sstream>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <ctype.h>
#include <iomanip>
#include <chrono>

using namespace std;

int camera_number = 0;

float kp = 3;//1.5
float ki = 1;//1
float kd = 0.5;//0.5
int setpoint=320;

int width = 640;
int height = 200;
int x_init_red=0;
int y_init_red=180;
							//nuc  pc
int LowH_red 	= 0; 		//0  
int HighH_red 	= 184;		//184 
int LowS_red 	= 130;      //130  65
int HighS_red 	= 248;      //248  246
int LowV_red 	= 49;		//49   242
int HighV_red 	= 230;		//230  255
int max_radius_red 	= 1000; 
int min_radius_red 	= 30;
int Noise_red 	= 15;
int noise_state = 15;

float delta_t = 0.01;
float initial_time = 0;
