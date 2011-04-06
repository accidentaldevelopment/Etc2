require 'mkmf'

required = true
# These headers are required, no point in building this without them
required &= have_header('pwd.h')
required &= have_header('grp.h')

# These aren't required, but fun
have_header('shadow.h')
have_func('crypt')

# Create Makefile as long as the required stuff is available
create_makefile 'etc2' if required