require 'mkmf'

if have_header('pwd.h') && have_header('grp.h')
  have_header('shadow.h')
  have_func('crypt')
  have_func('getlogin')
  
  # $CFLAGS << ' -Wall -std=c99'
  $CFLAGS << ' -Wall'
  create_makefile 'etc2'
end