$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/..')
require 'mkmf'
require 'mkmf-gnome2'


PKGConfig.have_package('gobject-2.0') or exit
check_win32

STDOUT.print("checking for new allocation framework... ") # for ruby-1.7
if Object.respond_to? :allocate
  STDOUT.print "yes\n"
  $defs << "-DHAVE_OBJECT_ALLOCATE"
else
  STDOUT.print "no\n"
end

have_func("rb_define_alloc_func") # for ruby-1.8

STDOUT.print("checking for attribute assignment... ") # for ruby-1.7
STDOUT.flush
if defined? try_compile and try_compile <<SRC
#include "ruby.h"
#include "node.h"
int node_attrasgn = (int)NODE_ATTRASGN;
SRC
  STDOUT.print "yes\n"
  $defs << "-DHAVE_NODE_ATTRASGN"
else
  STDOUT.print "no\n"
end

set_output_lib('libruby-glib2.a')

srcdir = File.dirname($0) == "." ? "." :
  File.expand_path(File.dirname($0) + "/src")

Dir.mkdir('src') unless File.exist? 'src'
Dir.chdir "src"
begin
  $defs << "-DRUBY_GLIB2_COMPILATION"
  create_makefile("glib2", srcdir)
ensure
  Dir.chdir('..')
end

create_top_makefile

