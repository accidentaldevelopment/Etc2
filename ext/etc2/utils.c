#include "utils.h"
#include "etc2.h"
 
void generate_salt(char *salt) {
	// #define size 12
	// char salt[size];
	int size = (int)sizeof(salt);
	int i;
	unsigned int seed;
	char itoa64_s[] = "./ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

	// Prevent the same salt from being used more than once
	// Can happen if the the function is called multiple times very quickly
	int random = open("/dev/urandom", O_RDONLY, 0);
	if(random > 0) {
	  if (read(random, (void*) &seed, size) != size)
	    close(random);
	}
	// Fallback
	else
	  seed = time(NULL);

	srand(seed);

	for(i=0; i<size; i++)
	  salt[i] = itoa64_s[rand() & 0x3f];

	// char *final_salt = malloc(size*sizeof(*final_salt));

	// return salt;
	// strcpy(final_salt, "$1$");
	// strcat(final_salt, salt);
	// return final_salt;
}