#include <stdlib.h> // for rand() and srand()

int randomIntegerInRange(int min, int max){
    // returns random integer, min <= x <= max
    int randomInt = rand();                     //   0 <= randomInt <= RAND_MAX
	randomInt = randomInt % ( max - min + 1);   // 0,1,2,......(max-min) inclusive
	randomInt += min;                           // min, min+1, ..... max inclusive
	return randomInt;

} // end randomIntegerInRange function
