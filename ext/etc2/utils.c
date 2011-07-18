#include "utils.h"

// Points the parameter at a randomly generated salt 
void generate_salt(char salt[]) {
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
	else  // fallback
	  seed = time(NULL);

	srand(seed);

	for(i=0; i<size; i++)
	  salt[i] = itoa64_s[rand() & 0x3f];
}

char* crypt_with_hash(VALUE txt, VALUE hash) {
	VALUE salt = rb_hash_aref(hash, SYM_SALT);
	VALUE type = rb_hash_aref(hash, SYM_TYPE);
	
	if(!NIL_P(type)) {
#if (__GLIBC__ != 2)
		rb_raise(rb_eNotImpError, "Passing :type is not implemented on this platform.  Use the default.");
#else
		// actually work
#endif
	}
	
	return crypt(STR2CSTR(txt), STR2CSTR(salt));
}