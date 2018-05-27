# -*- ruby -*-
#
# Copyright (C) 2018  Ruby-GNOME2 Project Team
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

require_relative "version"

Gem::Specification.new do |s|
  s.name          = "glib2"
  s.summary       =
    "Ruby/GObjectIntrospection is a " +
    "Ruby binding of GObject Introspection."
  s.description   =
    "Ruby/GObjectIntrospection provides bindings of GObject Introspection " +
    "and a Loader module that can generate dynamically ruby bindings " +
    "of any GObject C libraries"
  s.author        = "The Ruby-GNOME2 Project Team"
  s.email         = "ruby-gnome2-devel-en@lists.sourceforge.net"
  s.homepage      = "https://ruby-gnome2.osdn.jp/"
  s.licenses      = ["LGPL-2.1+"]
  s.version       = ruby_glib2_version
  s.extensions    = ["ext/#{s.name}/extconf.rb"]
  s.require_paths = ["lib"]
  s.files = FileList["COPYING.LIB",
                     "README.md",
                     "Rakefile",
                     "#{s.name}.gemspec",
                     "extconf.rb",
                     "version.rb",
                     "lib/**/*.rb",
                     "ext/#{s.name}/depend",
                     "ext/#{s.name}/*.{c,h,def,rb}",
                     "sample/**/*",
                     "test/**/*"]
end
