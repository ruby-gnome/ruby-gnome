# -*- ruby -*-
#
# Copyright (C) 2018-2022  Ruby-GNOME Project Team
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

require_relative "../glib2/version"

Gem::Specification.new do |s|
  s.name          = "gtk4"
  s.summary       = "Ruby/GTK4 is a Ruby binding of GTK+-4.x."
  s.description = "Ruby/GTK4 is a Ruby binding of GTK 4.x. It allows Ruby " +
                  "programmers to use the GTK graphics toolkit to make " +
                  "graphical user interfaces for their Ruby scripts. " +
                  "Many of the programs you use like file explorers, " +
                  "browsers, graphics programs etc. use GTK to make their " +
                  "GUI. In fact, there's a good chance that you're looking at " +
                  "a GTK window right now. All the GTK commands and widgets " +
                  "have been translated into Ruby, so Ruby programmers can " +
                  "write scripts that create windows instead of only "  +
                  "using the command line. Visit our homepage for install " +
                  "instructions and tutorials."
  s.author        = "The Ruby-GNOME Project Team"
  s.email         = "ruby-gnome2-devel-en@lists.sourceforge.net"
  s.homepage      = "https://ruby-gnome.github.io/"
  s.licenses      = ["LGPL-2.1-or-later"]
  s.version       = ruby_glib2_version
  s.extensions    = ["ext/#{s.name}/extconf.rb"]
  s.require_paths = ["lib"]
  s.files = [
    "COPYING.LIB",
    "README.md",
    "Rakefile",
    "#{s.name}.gemspec",
  ]
  s.files += Dir.glob("lib/**/*.rb")
  s.files += Dir.glob("ext/#{s.name}/*.{c,h,def,rb}")
  s.files += Dir.glob("sample/**/*")

  s.add_runtime_dependency("atk", "= #{s.version}")
  s.add_runtime_dependency("gdk4", "= #{s.version}")
end
