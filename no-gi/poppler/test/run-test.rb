#!/usr/bin/env ruby

base_dir = File.expand_path(File.join(File.dirname(__FILE__), ".."))

have_make = system("which make > /dev/null")

glib_dir = File.expand_path(File.join(base_dir, "..", "glib2"))
gdk_pixbuf_dir = File.expand_path(File.join(base_dir, "..", "gdk_pixbuf2"))
pango_dir = File.join(File.join(base_dir, ".." , "pango"))

$LOAD_PATH.unshift(File.join(glib_dir, "test"))
require 'glib-test-init'

[[gdk_pixbuf_dir, "gdk_pixbuf2"],
 [pango_dir, "pango"],
 [glib_dir, "glib2"],
 [base_dir, "poppler"]].each do |dir, module_name|
  if File.exist?(File.join(dir, "Makefile"))
    if have_make
      system("cd #{dir.dump} && make > /dev/null") or exit(false)
    end
    $LOAD_PATH.unshift(File.join(dir, "ext", module_name))
  end
  $LOAD_PATH.unshift(File.join(dir, "lib"))
end
require "poppler"

$LOAD_PATH.unshift(File.join(base_dir, "test"))
require 'poppler-test-utils'
class Test::Unit::TestCase
  include PopplerTestUtils
end

exit Test::Unit::AutoRunner.run(true, File.join(base_dir, "test"))
