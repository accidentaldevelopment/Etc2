require 'etc2'
require 'etc'

orig_verbose = $VERBOSE
$VERBOSE = nil
BuiltinEtc = Etc
Etc = Etc2
$VERBOSE = orig_verbose