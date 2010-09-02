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

Dir.chdir(ext_dir.to_s) do
  system(ruby, "extconf.rb", *ARGV) || exit(false)
end

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
