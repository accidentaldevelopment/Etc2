#ifndef ETC2_H
#include "etc2.h"
#endif
#include <fcntl.h>

#define SYM_MD5     ID2SYM(rb_intern("md5"))
#define SYM_SHA256  ID2SYM(rb_intern("sha256"))
#define SYM_SHA512  ID2SYM(rb_intern("sha512"))
#define SYM_SALT    ID2SYM(rb_intern("salt"))
#define SYM_TYPE    ID2SYM(rb_intern("type"))

void generate_salt(char*);
char* crypt_with_hash(VALUE,VALUE);