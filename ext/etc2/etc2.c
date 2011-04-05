#include "etc2.h"

static VALUE rb_mEtc2_hasShadow(VALUE mod) {
#ifdef HAVE_SHADOW_H
	return Qtrue;
#else
	return Qfalse;
#endif /* HAVE_SHADOW_H */
}

#ifdef HAVE_PWD_H
static VALUE rb_cUser_find(VALUE mod, VALUE user_lookup) {
	VALUE obj = rb_funcall(rb_cUser, NEW, 0);
	struct passwd *p;
	if(TYPE(user_lookup) == T_STRING) {
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

#ifdef HAVE_GRP_H
static VALUE rb_cGroup_find(VALUE mod, VALUE group_lookup) {
	VALUE obj = rb_funcall(rb_cGroup, NEW, 0);
	struct group *g;
	if(TYPE(group_lookup) == T_STRING) {
		char *groupname = STR2CSTR(group_lookup);
		g = getgrnam(groupname);
		if(g == NULL) rb_raise(rb_eArgError, "Group not found with groupname: %s", groupname);
	}
	else {
		gid_t gid = NUM2UIDT(group_lookup);
		g = getgrgid(gid);
		if(g == NULL) rb_raise(rb_eArgError, "Group not found with gid: %d", gid);
	}
	
	VALUE mem_ary = rb_ary_new();
	char **mem = g->gr_mem;
	while(*mem) {
		rb_ary_push(mem_ary, CSTR2STR(*mem)); //rb_cUser_find((VALUE)NULL, CSTR2STR(*mem)));
		mem++;
	}
	
	rb_iv_set(obj, "@name",   CSTR2STR(g->gr_name));
	rb_iv_set(obj, "@passwd", CSTR2STR(g->gr_passwd));
	rb_iv_set(obj, "@gid",    UIDT2NUM(g->gr_gid));
	rb_iv_set(obj, "@mem",    mem_ary);
	return obj;
}

static VALUE rb_cGroup_init(VALUE self) {
	group_attr("name", 1, 0);
	group_attr("passwd", 1, 0);
	group_attr("gid", 1, 0);
	group_attr("mem", 1, 0);
	return Qtrue;
}
#endif /* HAVE_GRP_H */

#ifdef HAVE_SHADOW_H
static VALUE rb_cShadow_find(VALUE mod, VALUE shadow_lookup){
	VALUE obj = rb_funcall(rb_cShadow, NEW, 0);
	struct spwd *s;
	char *shadowname = STR2CSTR(shadow_lookup);
	s = getspnam(shadowname);
	if(s == NULL) rb_raise(rb_eArgError, "Shadow entry not found for username: %s", shadowname);
	
	rb_iv_set(obj, "@name",   CSTR2STR(s->sp_namp));
	rb_iv_set(obj, "@passwd", CSTR2STR(s->sp_pwdp));
	return obj;
}

static VALUE rb_cShadow_init(VALUE self){
	shadow_attr("name", 1, 0);
	shadow_attr("passwd", 1, 0);
	
	rb_define_alias(rb_cShadow, "password", "passwd");
	rb_define_alias(rb_cShadow, "encrypted_password", "passwd");
	return Qtrue;
}
#endif /* HAVE_SHADOW_H */

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
	rb_define_module_function(rb_cGroup, "find", rb_cGroup_find, 1);
	rb_define_method(rb_cGroup, "initialize", rb_cGroup_init, 0);
#endif /* HAVE_GRP_H */

#ifdef HAVE_SHADOW_H
	rb_cShadow = rb_define_class_under(rb_mEtc2, "Shadow", rb_cObject);
	rb_define_module_function(rb_cShadow, "find", rb_cShadow_find, 1);
	rb_define_method(rb_cShadow, "initialize", rb_cShadow_init, 0);
#endif /* HAVE_SHADOW_H */
}
