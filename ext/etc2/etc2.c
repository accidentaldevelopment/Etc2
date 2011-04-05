#include "etc2.h"

static VALUE rb_mEtc2_hasShadow(VALUE mod) {
#ifdef HAVE_SHADOW_H
	return Qtrue;
#else
	return Qfalse;
#endif
}

#ifdef HAVE_PWD_H
static VALUE rb_cUser_find(VALUE mod, VALUE user_lookup) {
	VALUE obj = rb_funcall(rb_cUser, NEW, 0);
	struct passwd *p;
	if(TYPE(user_lookup) == T_STRING) {
		// printf("String lookup for: %s", STR2CSTR(user_lookup));
		char *username = STR2CSTR(user_lookup);
		p = getpwnam(username);
		if(p == NULL) rb_raise(rb_eArgError, "User not found with username: %s", username);
	}
	else {
		uid_t uid = NUM2UIDT(user_lookup);
		p = getpwuid(uid);
		if(p == NULL) rb_raise(rb_eArgError, "User not found with uid: %d", uid);
	}
	
	rb_iv_set(obj, "@name",   CSTR2STR(p->pw_name));
	rb_iv_set(obj, "@passwd", CSTR2STR(p->pw_name));
	rb_iv_set(obj, "@uid",    UIDT2NUM(p->pw_uid));
	rb_iv_set(obj, "@gid",    UIDT2NUM(p->pw_gid));
	rb_iv_set(obj, "@gecos",  CSTR2STR(p->pw_gecos));
	rb_iv_set(obj, "@dir",    CSTR2STR(p->pw_dir));
	rb_iv_set(obj, "@shell",  CSTR2STR(p->pw_shell));
	return obj;
}

static VALUE rb_cUser_init(VALUE self) {
	user_attr("name", 1, 0);
	user_attr("passwd", 1, 0);
	user_attr("uid", 1, 0);
	user_attr("gid", 1, 0);
	user_attr("gecos", 1, 0);
	user_attr("dir", 1, 0);
	user_attr("shell", 1, 0);

	rb_define_alias(rb_cUser, "full_name", "gecos");
	rb_define_alias(rb_cUser, "home_dir", "dir");
	rb_define_alias(rb_cUser, "homedir", "dir");
	rb_define_alias(rb_cUser, "home", "dir");
	return Qtrue;
}
#endif /* HAVE_PWD_H */

void Init_etc2() {
	rb_mEtc2 = rb_define_module("Etc2");
	rb_define_module_function(rb_mEtc2, "has_shadow?", rb_mEtc2_hasShadow, 0);
	
#ifdef HAVE_PWD_H
	rb_cUser = rb_define_class_under(rb_mEtc2, "User", rb_cObject);
	rb_define_module_function(rb_cUser, "find", rb_cUser_find, 1);
	rb_define_method(rb_cUser, "initialize", rb_cUser_init, 0);
#endif /* HAVE_PWD_H */

#ifdef HAVE_GRP_H
	rb_cGroup = rb_define_class_under(rb_mEtc2, "Group", rb_cObject);
#endif /* HAVE_GRP_H */

#ifdef HAVE_SHADOW_H
	rb_cShadow = rb_define_class_under(rb_mEtc2, "Shadow", rb_cObject);
#endif /* HAVE_SHADOW_H */
}
