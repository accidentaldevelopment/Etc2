#include "ruby.h"
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <time.h>

#ifdef HAVE_SHADOW_H
#include <shadow.h>
#endif

#ifndef ETC2_H
#define ETC2_H 1

// Convert a C char array into a tainted Ruby String.  Source is external so it should be tainted
#define CSTR2STR(cstr) rb_tainted_str_new2(cstr)
// Convert a Ruby String into a C char array
#define STR2CSTR(str) StringValueCStr(str)

// Utility functions for Etc2.crypt()
void generate_salt(char*);

// Declarations for module Etc2 and nested classes
VALUE rb_mEtc2;
VALUE rb_cUser;
VALUE rb_cGroup;
#ifdef HAVE_SHADOW_H
VALUE rb_cShadow;
#endif

// Etc2 module functions
VALUE rb_mEtc2_hasShadow(VALUE);
VALUE rb_mEtc2_c_crypt(int, VALUE *, VALUE);

// User declarations
#define user_attr(name,r,w) rb_define_attr(rb_cUser, name, r, w)

VALUE rb_cUser_find(VALUE, VALUE);
VALUE rb_cUser_current(VALUE);
VALUE rb_cUser_getpwent(VALUE);
VALUE rb_cUser_setpwent(VALUE);
VALUE rb_cUser_endpwent(VALUE);

// Group declarations
#define group_attr(name,r,w) rb_define_attr(rb_cGroup, name, r, w)

VALUE rb_cGroup_find(VALUE, VALUE);
VALUE rb_cGroup_getgrent(VALUE);
VALUE rb_cGroup_setgrent(VALUE);
VALUE rb_cGroup_endgrent(VALUE);

// Shadow declarations
#ifdef HAVE_SHADOW_H
#define shadow_attr(name,r,w) rb_define_attr(rb_cShadow, name, r, w)

VALUE rb_cShadow_find(VALUE, VALUE);
VALUE rb_cShadow_getspent(VALUE);
VALUE rb_cShadow_setspent(VALUE);
VALUE rb_cShadow_endspent(VALUE);
#endif 

#endif //ETC2_H
