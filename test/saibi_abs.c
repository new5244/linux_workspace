#include <stdio.h>
#include <math.h>

#define SAIBI_ABS(x) ( (x) < 0 ? -(x) : (x) )

int main(void)
{
	float pattern_data[2] = { 22.44, 22.45 };
	int feeler_data[2] = { 3223, 3224 };

	printf("SAIBI_ABS() = %d\n", SAIBI_ABS((feeler_data[1] - feeler_data[0])/(pattern_data[1] - pattern_data[0])));
	printf("SAIBI_ABS() =f %.3f\n", SAIBI_ABS( (feeler_data[1] - feeler_data[0])/(pattern_data[1] - pattern_data[0])));
	printf("fabs() = %.3f\n", fabs( (feeler_data[1] - feeler_data[0])/(pattern_data[1] - pattern_data[0])));
	return 0;
}

