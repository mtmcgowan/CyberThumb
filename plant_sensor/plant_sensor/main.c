/* Programmer: Matthew McGowan
* Class : NA
* Programming Assignment : hardware_hackathon
* Date : 10/14/17
* Description: This program is a prototype for testing sensor data
*/


#include "functions.h"

int main(void)
{
	int moist_code = 2, moist_days = 6, test_result = -2;
	double moist_peak = 0, light_raw[48] = { 0,2,3,4,5,6,6,3,3,3 },
		moist_raw[48] = { 1,2,3,4,5,1,2,3,4,5,6,7,6,7,5,4 };
	test_result = test_moisture(&moist_raw, 1, &moist_days);

	printf("Moisture Code: %d\nMoisture peak: %.2lf\n Days since watering: %d\nTest result: %d\n", moist_code, moist_peak, moist_days, test_result);
	// test_conditions();
	
	test_result = test_light(&light_raw, 0);
	printf("%.2lf\n%.2lf", light_raw[0], light_raw[6]);
	return 0;
}