#include "etc2.h"

#if HAVE_PWD_H
static VALUE rb_build_user(VALUE mod, VALUE username) {
	VALUE obj = rb_funcall(rb_cUser, NEW, 0);
	char* user = STR2CSTR(username);
	struct passwd *p = getpwnam(user);
	if (p == NULL) 
		rb_raise(rb_eArgError, "User not found: %s", user);
	
	rb_iv_set(obj, "@name", CSTR2STR(p->pw_name));
	rb_iv_set(obj, "@gecos", CSTR2STR(p->pw_gecos));
	rb_iv_set(obj, "@dir", CSTR2STR(p->pw_dir));
	return obj;
}

static VALUE rb_cUser_init(VALUE self) {
	user_attr("name", 1, 0);
	user_attr("gecos", 1, 0);
	user_attr("dir", 1, 0);

	rb_define_alias(rb_cUser, "full_name", "gecos");
	rb_define_alias(rb_cUser, "home_dir", "dir");
	rb_define_alias(rb_cUser, "homedir", "dir");
	rb_define_alias(rb_cUser, "home", "dir");
	return Qtrue;
}
#endif /* HAVE_PWD_H */

void Init_etc2() {
	rb_mEtc2 = rb_define_module("Etc2");
	
#if HAVE_PWD_H
	rb_cUser = rb_define_class_under(rb_mEtc2, "User", rb_cObject);
	rb_define_module_function(rb_cUser, "from_name", rb_build_user, 1);
	rb_define_method(rb_cUser, "initialize", rb_cUser_init, 0);
#endif /* HAVE_PWD_H */

#if HAVE_GRP_H
	rb_cGroup = rb_define_class_under(rb_mEtc2, "Group", rb_cObject);
#endif /* HAVE_GRP_H */

#if HAVE_SHADOW_H
	rb_cShadow = rb_define_class_under(rb_mEtc2, "Shadow", rb_cObject);
#endif /* HAVE_SHADOW_H */
}
