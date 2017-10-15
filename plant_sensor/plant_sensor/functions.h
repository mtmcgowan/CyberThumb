#pragma once
/* Programmer: Matthew McGowan
* Class : NA
* Programming Assignment : hardware_hackathon
* Date : 10/14/17
* Description: This program is a prototype for testing sensor data
*/

// Guard statements
#ifndef myfunc
#define myfunc

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include "time.h"



// Function prototypes
// int read_moisture(FILE *moistfile, FILE *timefile);
void test_conditions(void);
int read_plant(FILE *plant_file, int *temp_code, int *light_code, int *moist_code);
int read_data(FILE *data_file, double *data);
int read_moist_days(FILE *test_file, int *moist_days);

//int convert_light(double *light_data);
//int convert_moist(double *moist_data);

int test_light(double *light_data, int light_code);
double light_conv_func(double raw_value);

int test_temp(double *temp_data, int temp_code);
int test_moisture(double *moist_data, int moist_code, int *moist_days);
double find_max(double *data);


int update_test_file(int light_status, int temp_status, int moist_status);


double read_double(FILE *infile);
// int read_integer(FILE *infile);


#endif