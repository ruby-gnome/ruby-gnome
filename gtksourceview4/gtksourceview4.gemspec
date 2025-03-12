# -*- ruby -*-
#
# Copyright (C) 2018-2025  Ruby-GNOME Project Team
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
  s.name          = "gtksourceview4"
  s.summary       = "Ruby/GtkSourceView4 is a Ruby binding of gtksourceview-4.x."
  s.description   = "Ruby/GtkSourceView4 is a Ruby binding of gtksourceview-4.x."
  s.author        = "The Ruby-GNOME Project Team"
  s.email         = "ruby-gnome2-devel-en@lists.sourceforge.net"
  s.homepage      = "https://ruby-gnome.github.io/"
  s.licenses      = ["LGPL-2.1+"]
  s.version       = ruby_glib2_version
  s.extensions    = ["dependency-check/Rakefile"]
  s.require_paths = ["lib"]
  s.files = [
    "COPYING.LIB",
    "README.md",
    "Rakefile",
    "#{s.name}.gemspec",
    "dependency-check/Rakefile",
  ]
  s.files += Dir.glob("lib/**/*.rb")
  s.files += Dir.glob("test/**/*")

  s.add_runtime_dependency("gtk3", "= #{s.version}")

  [
    ["alpine_linux", "gtksourceview4-dev"],
  ].each do |platform, package|
    s.requirements << "system: gtksourceview-4: #{platform}: #{package}"
  end

  s.metadata["msys2_mingw_dependencies"] = "gtksourceview4"
end
