#include "ruby.h"
#include <unistd.h>
// Existence of these headers is verified by extconf.rb
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>

#ifdef HAVE_SHADOW_H
#include <shadow.h>
#endif

#ifndef ETC2_H
#define ETC2_H 1

// Version of library
#define VERSION rb_tainted_str_new2("1.0.0")

// Convert a C char array into a tainted Ruby String.  Source is external so it should be tainted
#define CSTR2STR(cstr) rb_tainted_str_new2(cstr)
// Convert a Ruby String into a C char array
#define STR2CSTR(str) StringValueCStr(str)

// Utility constants for Etc2.crypt()
#define SYM_MD5     ID2SYM(rb_intern("md5"))
#define SYM_SHA256  ID2SYM(rb_intern("sha256"))
#define SYM_SHA512  ID2SYM(rb_intern("sha512"))
#define SYM_SALT    ID2SYM(rb_intern("salt"))
#define SYM_TYPE    ID2SYM(rb_intern("type"))

// Utility functions for Etc2.crypt()
void generate_salt(char*);
char* crypt_with_hash(VALUE,VALUE);

// Declarations for module Etc2 and nested classes
static VALUE rb_mEtc2;
static VALUE rb_cUser;
static VALUE rb_cGroup;
static VALUE rb_cShadow;

// Etc2 module functions
VALUE rb_mEtc2_hasShadow(VALUE);
VALUE rb_mEtc2_crypt(int, VALUE *, VALUE);

#define user_attr(name,r,w) rb_define_attr(rb_cUser, name, r, w)

VALUE rb_cUser_find(VALUE, VALUE);
VALUE rb_cUser_current(VALUE);
VALUE rb_cUser_init(VALUE);

#define group_attr(name,r,w) rb_define_attr(rb_cGroup, name, r, w)

VALUE rb_cGroup_find(VALUE, VALUE);
VALUE rb_cGroup_init(VALUE);

#define shadow_attr(name,r,w) rb_define_attr(rb_cShadow, name, r, w)

VALUE rb_cShadow_find(VALUE, VALUE);
VALUE rb_cShadow_init(VALUE);

#endif //ETC2_H
