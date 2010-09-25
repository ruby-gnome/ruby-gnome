#!/usr/bin/env ruby

require 'pathname'
require 'mkmf'
require 'rbconfig'
require 'fileutils'

package = "glib2"

base_dir = Pathname(__FILE__).dirname.expand_path
ext_dir = base_dir + "ext" + package
mkmf_gnome2_dir = base_dir + 'lib'

ruby = File.join(RbConfig::CONFIG['bindir'],
                 RbConfig::CONFIG['ruby_install_name'] +
                 RbConfig::CONFIG["EXEEXT"])

build_dir = Pathname("ext") + package
FileUtils.mkdir_p(build_dir.to_s) unless build_dir.exist?
extconf_rb_path = ext_dir + "extconf.rb"
system(ruby, "-C", build_dir.to_s, extconf_rb_path.to_s, *ARGV) || exit(false)

create_makefile(package)
FileUtils.mv("Makefile", "Makefile.lib")

File.open("Makefile", "w") do |makefile|
  makefile.puts(<<-EOM)
all:
	(cd ext/#{package} && $(MAKE))
	$(MAKE) -f Makefile.lib

install:
	(cd ext/#{package} && $(MAKE) install)
	$(MAKE) -f Makefile.lib install

site-install:
	(cd ext/#{package} && $(MAKE) site-install)
	$(MAKE) -f Makefile.lib site-install

clean:
	(cd ext/#{package} && $(MAKE) clean)
	$(MAKE) -f Makefile.lib clean

distclean:
	(cd ext/#{package} && $(MAKE) distclean)
	$(MAKE) -f Makefile.lib distclean
	@rm -f Makefile.lib
EOM
end
