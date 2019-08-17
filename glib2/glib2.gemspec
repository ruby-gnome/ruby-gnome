# -*- ruby -*-
#
# Copyright (C) 2018-2019  Ruby-GNOME Project Team
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
  s.summary       = "Ruby/GLib2 is a Ruby binding of GLib-2.x."
  s.description   =
    "Ruby/GLib2 provides " +
    "base features for GLib2 based bindings and " +
    "many useful utility features."
  s.author        = "The Ruby-GNOME Project Team"
  s.email         = "ruby-gnome2-devel-en@lists.sourceforge.net"
  s.homepage      = "https://ruby-gnome2.osdn.jp/"
  s.licenses      = ["LGPL-2.1+"]
  s.version       = ruby_glib2_version
  s.extensions    = ["ext/#{s.name}/extconf.rb"]
  s.require_paths = ["lib"]
  s.files = [
    "COPYING.LIB",
    "README.md",
    "Rakefile",
    "#{s.name}.gemspec",
    "extconf.rb",
    "version.rb",
    "ext/#{s.name}/depend",
  ]
  s.files += Dir.glob("lib/**/*.rb")
  s.files += Dir.glob("ext/#{s.name}/*.{c,h,def,rb}")
  s.files += Dir.glob("sample/**/*")
  s.files += Dir.glob("test/**/*")

  s.add_runtime_dependency("pkg-config", ">= 1.3.5")
  s.add_runtime_dependency("native-package-installer", ">= 1.0.3")
  s.add_development_dependency("test-unit", ">= 2")

  s.metadata["msys2_mingw_dependencies"] = "glib2"
end
