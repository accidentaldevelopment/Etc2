#include "ruby.h"
#include <unistd.h>
// Existence of these headers is verified by extconf.rb
#include <pwd.h>
#include <grp.h>

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

// Declarations for module Etc2 and nested classes
static VALUE rb_mEtc2;
static VALUE rb_cUser;
static VALUE rb_cGroup;
static VALUE rb_cShadow;

// Etc2 module functions
static VALUE rb_mEtc2_hasShadow(VALUE);
static VALUE rb_mEtc2_crypt(int, VALUE *, VALUE);

#define user_attr(name,r,w) rb_define_attr(rb_cUser, name, r, w)

static VALUE rb_cUser_find(VALUE, VALUE);
static VALUE rb_cUser_current(VALUE);
static VALUE rb_cUser_init(VALUE);

#define group_attr(name,r,w) rb_define_attr(rb_cGroup, name, r, w)

static VALUE rb_cGroup_find(VALUE, VALUE);
static VALUE rb_cGroup_init(VALUE);

#define shadow_attr(name,r,w) rb_define_attr(rb_cShadow, name, r, w)

static VALUE rb_cShadow_find(VALUE, VALUE);
static VALUE rb_cShadow_init(VALUE);

#endif //ETC2_H
