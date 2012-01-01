Etc2
===
This library is a functional clone of Ruby's Etc library, part of the standard library.

Ruby's Etc is basically a very thin layer over the underlying C functions.  Which is fine, but I wanted a more Ruby-esque solution.
This library uses its own objects instead of Structs, and it includes Shadow support (as long the platform does).

etc2/replace
---

If you'd like, you can require 'etc2/replace' instead just etc2.  This will assign Etc2 to Etc, effectively replacing the built in Etc module.

It also assigns the built in Etc to BuiltinEtc, so you can still access that library if needed

\#user, \#group, \#shadow
---
The Etc2 module defines three module functions: *user*, *group*, and *shadow*.

These functions are identical to the *find()* methods in each of the classes (see below).  
They're just a little less typing since you don't need to specify the class itself.

\#crypt
---
Etc2 includes a module_function implementation of crypt().  It basically just calls the system version of the same function.

On systems with GNU it will use that version of crypt, meaning you can select the algorithm by adding a magic number to the beginning of the salt.
<table>
	<tr>
		<th>Magic</th><th>Hash Algorithm</th>
	<tr>
		<td>$1$</td><td>MD5</td>
	</tr><tr>
		<td>$5$</td><td>SHA256</td>
	</tr><tr>
		<td>$6$</td><td>SAH512</td>
	</tr>
</table>

	Etc2.crypt('text','$1$asdfqwer') #=> MD5 hash
	Etc2.crypt('text','$6$asdfqwer') #=> SHA512 hash

On Mac OS X systems you can use the "Extended crypt" feature by prefixing the salt with an underscore.

If you don't supply a salt, one will be randomly generated, but this prevents you from using the extended features of crypt().

See *man crypt(3)*  on your specific platform for more information.

User
---
The User class represent a single system-level user.  It is the equivalent of a struct passwd.

You can look up users via username or uid:

	Etc2::User.find('brian')
	#<Etc2::User @name="brian", @uid=501, @gid=20, @gecos="Brian", @dir="/Users/brian", @shell="/bin/bash">
	Etc2::User.find(501)
	#<Etc2::User @name="brian", @uid=501, @gid=20, @gecos="Brian", @dir="/Users/brian", @shell="/bin/bash">

The User object does not hold the pw_passwd data.  I have no access to a machine that still uses passwd to store passwords, so 
I have no reason to store that data anywhere.

There are two versions of the *find()* function:

  1. Etc2::User.find returns nil if the user can't be found
  2. Etc2::User.find! raises an exception if the user can't be found

Group
---
The Group class represents a system-level group.  It is the equivalent of a struct group.

The search API is the same as for User:

	Etc2::Group.find('admin')
	#<Etc2::Group:0x007fbed28f9088 @name="admin", @passwd="*", @gid=80, @mem=["root", "brian"]> 
	Etc2::Group.find(80)
	#<Etc2::Group:0x007fbed28a3f48 @name="admin", @passwd="*", @gid=80, @mem=["root", "brian"]>
	
As with User, the find method has two versions: one that raises an exception and the other that returns nil.

Shadow
---
The Shadow class is where things can get tricky.  Only certain platforms use shadow for encrypting passwords.  Mac OS X, for example, does not use it.

The class does not exist on systems that don't include shadow.

The API should be looking pretty familiar at this point:

	# As root
	Etc2::Shadow.find('brian')
	#<Etc2::Shadow @name="brian", @passwd="$5$a8s76df5$0tgaFba/asd8f9a096asdtasd/a67s5ah475s876a8s7f8a65s87658a76s4d8h48asdg/">
	Etc2::Shadow.find(501)
	#<Etc2::Shadow @name="brian", @passwd="$5$a8s76df5$0tgaFba/asd8f9a096asdtasd/a67s5ah475s876a8s7f8a65s87658a76s4d8h48asdg/">

Note that you'll get EACCES exceptions if you're not root.

As with User, the find method has two versions: one that raises an exception and the other that returns nil.