#include "ruby.h"
#include <stdlib.h>

#define CSTR2STR(cstr) rb_str_new2(cstr)
#define STR2CSTR(str) RSTRING_PTR(StringValue(str))
#define NEW rb_intern("new")

static VALUE rb_mEtc2;

#if HAVE_PWD_H
#include <pwd.h>
#define user_attr(name,r,w) rb_define_attr(rb_cUser, name, r, w)

static VALUE rb_cUser;

static VALUE rb_username(VALUE);
static VALUE rb_build_user(VALUE, VALUE);
static VALUE rb_cUser_init(VALUE);

#endif /* HAVE_PWD_H */

#if HAVE_GRP_H
#include <grp.h>
#define group_attr(name,r,w) rb_define_attr(rb_cGroup, name, r, w)

static VALUE rb_cGroup;

#endif /* HAVE_GRP_H */

#if HAVE_SHADOW_H
#include <shadow.h>
#define shadow_attr(name,r,w) rb_define_attr(rb_cShadow, name, r, w)

static VALUE rb_cShadow;

#endif /* HAVE_SHADOW_H */
