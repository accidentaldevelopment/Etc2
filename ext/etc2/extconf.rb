require 'mkmf'

passwd = have_header('pwd.h')
group  = have_header('grp.h')
shadow = have_header('shadow.h')

if passwd || group || shadow
  create_makefile 'etc2'
end