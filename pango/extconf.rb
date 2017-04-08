#!/usr/bin/env ruby
#
# Copyright (C) 2017  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

require "pathname"
require "mkmf"
require "rbconfig"
require "fileutils"

package = "pango"

base_dir = Pathname(__FILE__).dirname.expand_path
ext_dir = base_dir + "ext" + package
mkmf_gnome2_dir = base_dir + "lib"

ruby = File.join(RbConfig::CONFIG["bindir"],
                 RbConfig::CONFIG["ruby_install_name"] +
                 RbConfig::CONFIG["EXEEXT"])

build_dir = Pathname("ext") + package
FileUtils.mkdir_p(build_dir.to_s) unless build_dir.exist?
extconf_rb_path = ext_dir + "extconf.rb"
system(ruby, "-C", build_dir.to_s, extconf_rb_path.to_s, *ARGV) || exit(false)

create_makefile(package)
FileUtils.mv("Makefile", "Makefile.lib")

File.open("Makefile", "w") do |makefile|
  makefile.puts(<<-MAKEFILE)
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
  MAKEFILE
end
