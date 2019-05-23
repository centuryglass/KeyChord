# Here's the license text for this file:
#
# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# For more information, please refer to <http://unlicense.org/>

import os
import ycm_core
import pkgconfig

flags = [
'-std=gnu++14',
# ...and the same thing goes for the magic -x option which specifies the
# language that the files to be compiled are written in. This is mostly
# relevant for c++ headers.
# For a C project, you would set this to 'c' instead of 'c++'.
'-x',
'c++',
'./ClangCompleter',
'-pthread',
'-DLINUX=1',
'-DDEBUG=1',
'-D_DEBUG=1',
'-DDONT_SET_USING_JUCE_NAMESPACE=1',
'-DJUCER_LINUX_MAKE_6D53C8B4=1',
'-DJUCE_APP_VERSION=0.0.8.10',
'-DJUCE_APP_VERSION_HEX=0x80a',
'-lcrypto',
'-ldl',
'-lpthread',
'-lrt',
'-I./JuceLibraryCode',
'-I./deps/JUCE/modules',
 ]

pkgConfigFlags = [
'NetworkManager',
'libnm-glib',
'alsa',
'freetype2',
'libssl',
'gio-2.0',
'x11',
'xext',
'xinerama'
]

for pflag in pkgConfigFlags:
  flagList = pkgconfig.cflags(pflag).split(' ')
  for f in flagList:
    flags.append(f)

for dirname, subdirList, fileList in os.walk('./Source'):
  flags.append('-I'+dirname)
for dirname, subdirList, fileList in os.walk('./Tests'):
  flags.append('-I'+dirname)

def DirectoryOfThisScript():
  return os.path.dirname( os.path.abspath( __file__ ) )

def FlagsForFile(filename, **kwargs):
  return {
    'flags': flags,
    'include_paths_relative_to_dir': DirectoryOfThisScript()
  }
