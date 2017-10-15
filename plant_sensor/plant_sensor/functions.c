/* Programmer: Matthew McGowan
* Class : NA
* Programming Assignment : hardware_hackathon
* Date : 10/14/17
* Description: This program is a prototype for testing sensor data
*/

#include "functions.h"



/* Function: test_conditions()
Date Created: 10/14/17
Date Last Modified:
Description: This function processes the last 24 hours of plant data and updates the test result file
Input: NA
Output: integer indicating whether the function was successfully executed
Prereq: Data files should be in the same location as the binary
Result: test result file will be updated with new results
*/
void test_conditions(void)
{
	int light_code = 0, temp_code = 0, moist_code = 0, 
		moist_days = 0, light_test = 0, temp_test = 0, moist_test = 0,
		light_avg = 0, temp_avg = 0, moist_peak = 0;
	double light_data[48] = { 0 }, temp_data[48] = { 0 }, moist_data[48] = { 0 };

	// char plant_name[20];

	// Initialize file pointers (I think...)
	FILE *plant_file = NULL;
	FILE *light_file = NULL;
	FILE *temp_file = NULL;
	FILE *moist_file = NULL;
	FILE *test_file = NULL;

	// Open the files
	plant_file = fopen("plant_file.txt", "r");
	light_file = fopen("light_file.txt", "r");
	temp_file = fopen("temp_file.txt", "r");
	moist_file = fopen("moist_file.txt", "r");
	test_file = fopen("test_file.txt", "r"); // opened just to read in previous results

	// Read in the data
	read_plant(plant_file, &light_code, &temp_code, &moist_code); // Read in the plant codes
	read_moist_days(test_file, &moist_days); // read in the moist days value
	read_data(light_file, &light_data); // Read in light data
	read_data(temp_file, &temp_data); // Read in temperature data
	read_data(moist_file, &moist_data); // Read in moisture data
	
	
	// Test whether conditions are optimal
	light_test = test_light(&light_data, light_code);
	temp_test = test_temp(&temp_data, temp_code);
	moist_test = test_moisture(&moist_data, moist_code, &moist_days);

	printf("Light code: %d\nTemp code: %d\nMoisture code: %d\n", light_code, temp_code, moist_code);

	// Update the test results file
	fclose(test_file); // close the test_file
	test_file = fopen("test_file.txt", "w"); // reopen the test file erasing previous results	
	fprintf(test_file, "%d\n%d\n%d\n%d", light_test, temp_test, moist_test, moist_days);
}


/* Function: read_plant()
Date Created: 10/14/17
Date Last Modified:
Description: This function reads in the optimal growing condition categories for a plant from a .txt file
Input: Pointers to growing condition categories (temp_code, light_code, moist_code)
Output: integer indicating whether the function was successfully executed
Prereq: A properly formatted plant file should be available
Result: Growing condition variables will be set correctly to the specified plant
*/
int read_plant(FILE *plant_file, int *light_code, int *temp_code, int *moist_code)
{
	// fscanf(plant_file, "%s", *plant_name);
	*light_code = read_integer(plant_file);
	*temp_code = read_integer(plant_file);
	*moist_code = read_integer(plant_file);
}

/* Function: read_data()
Date Created: 10/14/17
Date Last Modified:
Description: This function reads in a data file and tests whether the conditions are optimal or need to be changed
Input: The temperature category for the plant to be tested
Output: integer
Prereq: A properly formatted temp file should be available
Result: Temp and time arrays will contain yesterdays data
*/

int read_data(FILE *data_file, double *data)
{
	int index = 0;

	for (; index < 47; index++) // this loop assumes there are only 48 data points
	{
		data[index] = read_double(data_file);
	}

	return 0;
}

/* Function: convert_light()
Date Created: 10/14/17
Date Last Modified:
Description: This function converts raw output from the arduino sensor and converts it to the SI lux unit
Input: A pointer to a vector of light values
Output: integer indicating sucessful execution
Prereq: A light_data vector
Result: The light
*/


/*
* Function: read_double()
* Date Created: 9/21/2017
* Date Last Modified: 9/25/2017
* Description: This function reads in a double float from an open  filestream.
* Input parameters: An opened filestream
* Returns: The next double value in the stream
Preconditions: Start of the program.
Postconditions: A double value will be available for further computation.
*/

double read_double(FILE *infile)
{
	double temp_double = 0.0;
	fscanf(infile, "%lf", &temp_double);
	return temp_double;
}


/* Function: read_integer()
* Date Created: 9/21/2017
* Date Last Modified: 9/23/2017
* Description: This function reads in an integer from an open filestream.
* Input parameters: An opened filestream
* Returns: The next integer value in the stream
* Preconditions: Start of the program.
* Postconditions: An integer value will be available for further computation.
*/
int read_integer(FILE *infile)
{
	int temp_int = 0;
	fscanf(infile, "%d", &temp_int);
	return temp_int;
}

/* Function: read_moist_days()
* Date Created: 9/21/2017
* Date Last Modified: 9/23/2017
* Description: This function reads in how many days since the last moisture event from yesterdays results
* Input parameters: pointer to data file, pointer to variable storing how many days since last watering event
* Returns: an integer indicating sucessful execution
* Preconditions: 
* Postconditions:  
*/
int read_moist_days(FILE *test_file, int *moist_days)
{
	read_integer(test_file);
	read_integer(test_file);
	read_integer(test_file);
	*moist_days = read_integer(test_file);
}

/* Function: test_moisture()
* Date Created: 9/21/2017
* Date Last Modified: 9/23/2017
* Description: This function tests whether watering is necessary or not
* Input parameters: 
* Returns: 
* Preconditions:
* Postconditions:
*/

int test_moisture(double *moist_data, int moist_code, int *moist_days)
{
	int wet = 0, test_result = -2, ideal_freq = 0;
	double moist_peak = find_max(&moist_data);

	switch (moist_code) {
	case 1:
		ideal_freq = -1;
		break;
	case 2:
		ideal_freq = 7;
		break;
	case 3:
		ideal_freq = 14;
		break;
	}
	
	if (moist_peak > 30) wet = 1;

	if (wet == 0 && (*moist_days >= ideal_freq)) {
		test_result = -1;
		*moist_days++;
	} 
	else if (wet == 0 && (*moist_days < ideal_freq)) {
		test_result = 0;
		*moist_days++;
	}
	else if (wet == 1 && (*moist_days < ideal_freq)) {
		test_result = 1;
		*moist_days = 0;
	}
	else {
		test_result = 0;
		*moist_days = 0;
	}
	
	return test_result;
}

/* Function: test_light()
* Date Created: 10/14/17
* Date Last Modified: 
* Description: This function converts light to LUX units and tests growing conditions
* Input parameters: pointer to light data, light code
* Returns: an integer indicating growth conditions
* Preconditions:
* Postconditions:
*/
int test_light(double *light_data, int light_code)
{
	int index = 0;
	double sum_lux = 0, avg_lux = 0, min_lux = 0, max_lux = 0; // running sum of lux, average daily light in lux

	for (; index < 48; index++)
	{
		sum_lux += light_conv_func(light_data[index]);
	}

	avg_lux = sum_lux / 48;

	switch (light_code) {
	case 1:
		min_lux = 10764;
		max_lux = 16146;
		break;
	case 2:
		min_lux = 1615;
		max_lux = 5382;
		break;
	case 3:
		min_lux = 807;
		max_lux = 2153;
		break;
	case 4:
		min_lux = 269;
		max_lux = 807;
		break;
	}
	if (avg_lux > max_lux)
		return 1;
	else if (avg_lux < min_lux)
		return -1;
	else
		return 0;
}

double light_conv_func(double raw_value)
{
	double light_lux = 0;
	// Some function
	light_lux = raw_value;
	return light_lux;
}

/* Function: test_temp()
* Date Created: 10/14/17
* Date Last Modified:
* Description: This function tests growing conditions based on the temperature
* Input parameters: pointer to temperature data, temperature code
* Returns: an integer indicating growth conditions
* Preconditions:
* Postconditions:
*/
int test_temp(double *temp_data, int temp_code)
{
	int index = 0;
	double sum_temp = 0, avg_temp = 0, min_temp = 0, max_temp = 0; // running sum of temperature, average daily temperature

	for (; index < 48; index++)
	{
		sum_temp += temp_data[index];
	}

	avg_temp = sum_temp / 48;

	switch (temp_code) {
	case 1:
		min_temp = 11.5;
		max_temp = 19.5;
		break;
	case 2:
		min_temp = 16.5;
		max_temp = 26.5;
		break;
	case 3:
		min_temp = 22;
		max_temp = 32;
		break;
	}
	if (avg_temp > max_temp)
		return 1;
	else if (avg_temp < min_temp)
		return -1;
	else
		return 0;
}

double find_max(double *data) 
{
	double max = -1;
	for (int i = 0; i < 48; i++) {
		if (data[i] > max)
			max = data[i];
	}
	return max;
}
