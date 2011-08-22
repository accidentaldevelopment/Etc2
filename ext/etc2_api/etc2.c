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

/* @private
 * @overload Etc2.c_crypt(key)
 *	@param [String] key Text to encrypt
 *	@return [String] +key+ encrypted with a random salt
 * @overload Etc2.c_crypt(key, salt)
 * 	@param [String] key Text to encrypt
 * 	@param [string] salt The salt used for the hash
 * 	@return [String] +key+ encrypted with +salt+
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
VALUE rb_mEtc2_c_crypt(int argc, VALUE *argv, VALUE mod) {
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
			// case T_HASH:
			// 	result = crypt_with_hash(txt, salt);
			// 	break;
			default:
				rb_raise(rb_eArgError, "String or Hash expected");
		}
	}
	
	return CSTR2STR(result);
#endif
}

// Instantiates and returns a new User object based on the supplied struct
VALUE setup_user(struct passwd *p) {
	VALUE obj = rb_class_new_instance(0, NULL, rb_cUser);
	rb_iv_set(obj, "@name",   CSTR2STR(p->pw_name));
	rb_iv_set(obj, "@uid",    UIDT2NUM(p->pw_uid));
	rb_iv_set(obj, "@gid",    UIDT2NUM(p->pw_gid));
	rb_iv_set(obj, "@gecos",  CSTR2STR(p->pw_gecos));
	rb_iv_set(obj, "@dir",    CSTR2STR(p->pw_dir));
	rb_iv_set(obj, "@shell",  CSTR2STR(p->pw_shell));
	
	return obj;
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
	
	return setup_user(p);
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

/* 
 * Same as getpwent(3)
 * @return [User] The next (or first) User object in the database
 */
VALUE rb_cUser_getpwent(VALUE self) {
	struct passwd *p;
	if( (p = getpwent()) )
		return setup_user(p);
	return Qnil;
}

/*
 * Same as setpwent(3)
 */
VALUE rb_cUser_setpwent(VALUE self) {
	setpwent();
	return Qnil;
}

/*
 * Same as endpwent(3)
 */
VALUE rb_cUser_endpwent(VALUE self) {
	endpwent();
	return Qnil;
}

// Group definitions
VALUE setup_group(struct group *g) {
	VALUE obj = rb_class_new_instance(0, NULL, rb_cGroup);
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

/*
 * call-seq:
 *   (Group) find(lookup)
 * Find a group based on the groupname or GID
 *
 * @param  [String,Fixnum] lookup The groupname or GID to search for
 * @return [Group]    A group object or nil if one couldn't be found
 * @raise  [ArgumentError] If the groupname or gid can't be found
 */
VALUE rb_cGroup_find(VALUE mod, VALUE group_lookup) {
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
	
	return setup_group(g);
}

/* 
 * Same as getgrent(3)
 * @return [Group] The next (or first) Group object in the database
 */
VALUE rb_cGroup_getgrent(VALUE self) {
	struct group *g;
	if( (g = getgrent()) )
		return setup_group(g);
	return Qnil;
}

/* 
 * Same as setgrent(3)
 */
VALUE rb_cGroup_setgrent(VALUE self) {
	setgrent();
	return Qnil;
}

/*
 * Same as endgrent(3)
 */
VALUE rb_cGroup_endgrent(VALUE self) {
	endgrent();
	return Qnil;
}

// Shadow definitions
VALUE setup_shadow(struct spwd *s) {
#ifdef HAVE_SHADOW_H
	VALUE obj = rb_class_new_instance(0, NULL, rb_cShadow);
	rb_iv_set(obj, "@name",   CSTR2STR(s->sp_namp));
	rb_iv_set(obj, "@passwd", CSTR2STR(s->sp_pwdp));
	return obj;
#else
	return Qnil;
#endif
}

/*
 * call-seq:
 *   (Shadow) find(lookup)
 * Find a user's shadow entry based on the username
 *
 * @param  [String] lookup The username to search for
 * @return [Shadow]    A shadow object or nil if one couldn't be found
 * @raise  [ArgumentError] If the username can't be found
 */
VALUE rb_cShadow_find(VALUE mod, VALUE shadow_lookup){
#ifndef HAVE_SHADOW_H
	rb_raise(rb_eNotImpError, "shadow not available on this platform");
	return Qnil;
#else
	struct spwd *s;
	char *shadowname = STR2CSTR(shadow_lookup);
	s = getspnam(shadowname);
	if (getuid() != 0 || geteuid() != 0) rb_raise(rb_const_get(rb_mErrno, rb_intern("EACCES")), "Must be root to access shadow database");
	else if(s == NULL) rb_raise(rb_eArgError, "Shadow entry not found for username: %s", shadowname);
	
	return setup_shadow(s);
#endif
}

/* 
 * Same as getspent(3)
 * @return [Shadow] The next (or first) Shadow object in the database
 */
VALUE rb_cShadow_getspent(VALUE self) {
	struct spwd *s;
	if( (s = getspent()) )
		return setup_shadow(s);
	return Qnil;
}

/*
 * Same as setspent(3)
 */
VALUE rb_cShadow_setspent(VALUE self) {
	setspent();
	return Qnil;
}

/*
 * Same as endspent(3)
 */
VALUE rb_cShadow_endspent(VALUE self) {
	endspent();
	return Qnil;
}

void Init_etc2_api() {
	rb_mEtc2 = rb_define_module("Etc2");
	rb_define_module_function(rb_mEtc2, "has_shadow?", rb_mEtc2_hasShadow, 0);
	rb_define_module_function(rb_mEtc2, "c_crypt", rb_mEtc2_c_crypt, -1);
	
	rb_cUser = rb_define_class_under(rb_mEtc2, "User", rb_cObject);
	rb_define_module_function(rb_cUser, "find", rb_cUser_find, 1);
	rb_define_module_function(rb_cUser, "current", rb_cUser_current, 0);
	rb_define_module_function(rb_cUser, "getpwent", rb_cUser_getpwent, 0);
	rb_define_module_function(rb_cUser, "setpwent", rb_cUser_setpwent, 0);
	rb_define_module_function(rb_cUser, "endpwent", rb_cUser_endpwent, 0);
	
	rb_cGroup = rb_define_class_under(rb_mEtc2, "Group", rb_cObject);
	rb_define_module_function(rb_cGroup, "find", rb_cGroup_find, 1);
	rb_define_module_function(rb_cGroup, "getgrent", rb_cGroup_getgrent, 0);
	rb_define_module_function(rb_cGroup, "setgrent", rb_cGroup_setgrent, 0);
	rb_define_module_function(rb_cGroup, "endgrent", rb_cGroup_endgrent, 0);

	rb_cShadow = rb_define_class_under(rb_mEtc2, "Shadow", rb_cObject);
	rb_define_module_function(rb_cShadow, "find", rb_cShadow_find, 1);
	rb_define_module_function(rb_cShadow, "getspent", rb_cShadow_getspent, 0);
	rb_define_module_function(rb_cShadow, "setspent", rb_cShadow_setspent, 0);
	rb_define_module_function(rb_cShadow, "endspent", rb_cShadow_endspent, 0);
}
