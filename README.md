Etc2
===
This library is a functional clone of Ruby's Etc library, part of the standard library.

Ruby's Etc is basically a very thin layer over the underlying C functions.  Which is fine, but I wanted a more Ruby-esque solution.
This library uses its own objects instead of Structs, and it includes Shadow support.

User
---
The User class represent a single system-level user.  It is the equivalent of a struct passwd.

You can look up users via username or uid:

	Etc2::User.find('brian')
	#<Etc2::User @name="brian", @passwd="brian", @uid=501, @gid=20, @gecos="Brian", @dir="/Users/brian", @shell="/bin/bash">
	Etc2::User.find(501)
	#<Etc2::User @name="brian", @passwd="brian", @uid=501, @gid=20, @gecos="Brian", @dir="/Users/brian", @shell="/bin/bash">

Group
---
The Group class represents a system-level group.  It is the equivalent of a struct group.

The search API is the same as for User:

	Etc2::Group.find 'admin'
	#<Etc2::Group:0x007fbed28f9088 @name="admin", @passwd="*", @gid=80, @mem=["root", "brian"]> 
	Etc2::Group.find 80
	#<Etc2::Group:0x007fbed28a3f48 @name="admin", @passwd="*", @gid=80, @mem=["root", "brian"]>

Shadow
---
The Shadow class is where things can get tricky.  Only certain platforms use shadow for encrypting passwords.  Mac OS X, for example, does not use it.

The class exists on all platforms, but will raise NotImplementedError on systems which don't include shadow.h or the associated library.
