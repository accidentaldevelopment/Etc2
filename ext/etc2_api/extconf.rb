require 'mkmf'

if have_header('pwd.h') && have_header('grp.h')
  have_header('shadow.h')
  have_func('crypt')
  have_func('getlogin')
  
  create_makefile 'etc2_api'
end