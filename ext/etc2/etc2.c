#include "etc2.h"

/*
 * Returns true if support for shadow.h was included
 */
VALUE rb_mEtc2_hasShadow(VALUE mod) {
#ifdef HAVE_SHADOW_H
	return Qtrue;
#else
	return Qfalse;
#endif
}

/*
 * Document-method: crypt
 * call-seq:
 *   Etc2.crypt(key, salt) -> encrypted_string
 *
 * Returns +key+ encrypted with +salt+
 * 
 * Default encryption type is DES.
 *
 * == Encryption Algorithm
 * On systems that support it, you can change the encryption by specifying a magic number in the salt.
 * This is done by formatting the salt like:
 *   $n$saltsalt$
 * where +n+ is the magic number.
 *
 * Available magic numbers and their encryption types are:
 *
 * - $1$saltsalt$ MD5
 * - $5$saltsalt$ SHA256
 * - $6$saltsalt$ SHA512
 */
VALUE rb_mEtc2_crypt(int argc, VALUE *argv, VALUE mod) {
#ifndef HAVE_CRYPT
	rb_notimplement();
	return Qnil;
#else
	VALUE txt, salt;
	char *result;
	rb_scan_args(argc, argv, "11", &txt, &salt);
	
	if(NIL_P(salt)) {
		char gen_salt[12];
		generate_salt(gen_salt);
		result = crypt(STR2CSTR(txt), gen_salt);
	} else {
		switch(TYPE(salt)) {
			case T_STRING:
				result = crypt(STR2CSTR(txt), STR2CSTR(salt));
				break;
			case T_HASH:
				result = crypt_with_hash(txt, salt);
				break;
			default:
				rb_raise(rb_eArgError, "String or Hash expected");
		}
	}
	
	return CSTR2STR(result);
#endif
}

/*
 * call-seq:
 *   (User) find(lookup)
 * Find a user based on the username or UID
 *
 * @param  [String,Fixnum] lookup The username or UID to search for
 * @return [User]    A user object or nil if one couldn't be found
 * @raise  [ArgumentError] If the username or uid can't be found
 */
VALUE rb_cUser_find(VALUE mod, VALUE user_lookup) {
	VALUE obj = rb_class_new_instance(0, NULL, rb_cUser); //rb_funcall(rb_cUser, NEW, 0);
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

/*
 * Builds a user object for the current user.  <br />
 * Based on getlogin() or $USER, if getlogin() isn't available. <br/><br/>
 * <strong>Note:</strong> It's probably pretty easy to trick this; I wouldn't rely on it for security.
 *
 * @return [User] A User object for the current user
 */
VALUE rb_cUser_current(VALUE mod) {
#ifdef HAVE_GETLOGIN
	return rb_cUser_find(mod, CSTR2STR(getlogin()));
#else
	return rb_cUser_find(mod, CSTR2STR(getenv("USER")));
#endif
}

VALUE rb_cUser_init(VALUE self) {
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

VALUE rb_cGroup_find(VALUE mod, VALUE group_lookup) {
	VALUE obj = rb_class_new_instance(0, NULL, rb_cGroup);
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
	char **mem;
	for(mem=g->gr_mem; *mem; mem++)
		rb_ary_push(mem_ary, CSTR2STR(*mem));
	
	rb_iv_set(obj, "@name",   CSTR2STR(g->gr_name));
	rb_iv_set(obj, "@passwd", CSTR2STR(g->gr_passwd));
	rb_iv_set(obj, "@gid",    UIDT2NUM(g->gr_gid));
	rb_iv_set(obj, "@mem",    mem_ary);
	return obj;
}

VALUE rb_cGroup_init(VALUE self) {
	group_attr("name", 1, 0);
	group_attr("passwd", 1, 0);
	group_attr("gid", 1, 0);
	group_attr("mem", 1, 0);
	return Qtrue;
}

VALUE rb_cShadow_find(VALUE mod, VALUE shadow_lookup){
#ifndef HAVE_SHADOW_H
	rb_raise(rb_eNotImpError, "shadow not available on this platform");
	return Qnil;
#else
	VALUE obj = rb_class_new_instance(0, NULL, rb_cShadow);
	struct spwd *s;
	char *shadowname = STR2CSTR(shadow_lookup);
	s = getspnam(shadowname);
	if (getuid() != 0 || geteuid() != 0) rb_raise(rb_const_get(rb_mErrno, rb_intern("EACCES")), "Must be root to access shadow database");
	else if(s == NULL) rb_raise(rb_eArgError, "Shadow entry not found for username: %s", shadowname);
	
	rb_iv_set(obj, "@name",   CSTR2STR(s->sp_namp));
	rb_iv_set(obj, "@passwd", CSTR2STR(s->sp_pwdp));
	return obj;
#endif
}

VALUE rb_cShadow_init(VALUE self){
#ifndef HAVE_SHADOW_H
	rb_raise(rb_eNotImpError, "shadow not available on this platform");
	return Qnil;
#else
	shadow_attr("name", 1, 0);
	shadow_attr("passwd", 1, 0);
	
	rb_define_alias(rb_cShadow, "password", "passwd");
	rb_define_alias(rb_cShadow, "encrypted_password", "passwd");
	return Qtrue;
#endif
}

void Init_etc2() {
	rb_mEtc2 = rb_define_module("Etc2");
	rb_define_const(rb_mEtc2, "VERSION", VERSION);
	rb_define_module_function(rb_mEtc2, "has_shadow?", rb_mEtc2_hasShadow, 0);
	
	rb_define_module_function(rb_mEtc2, "crypt", rb_mEtc2_crypt, -1);
	
	rb_cUser = rb_define_class_under(rb_mEtc2, "User", rb_cObject);
	rb_define_module_function(rb_cUser, "find", rb_cUser_find, 1);
	rb_define_module_function(rb_cUser, "current", rb_cUser_current, 0);
	rb_define_method(rb_cUser, "initialize", rb_cUser_init, 0);
	
	rb_cGroup = rb_define_class_under(rb_mEtc2, "Group", rb_cObject);
	rb_define_module_function(rb_cGroup, "find", rb_cGroup_find, 1);
	rb_define_method(rb_cGroup, "initialize", rb_cGroup_init, 0);

	rb_cShadow = rb_define_class_under(rb_mEtc2, "Shadow", rb_cObject);
	rb_define_module_function(rb_cShadow, "find", rb_cShadow_find, 1);
	rb_define_method(rb_cShadow, "initialize", rb_cShadow_init, 0);
}
