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
  s.name          = "clutter-gtk"
  s.summary       = "Ruby/ClutterGTK is a Ruby binding of Clutter-GTK."
  s.description   = "Ruby/ClutterGTK is a Ruby binding of Clutter-GTK."
  s.author        = "The Ruby-GNOME Project Team"
  s.email         = "ruby-gnome2-devel-en@lists.sourceforge.net"
  s.homepage      = "https://ruby-gnome.github.io/"
  s.licenses      = ["LGPL-2.1-or-later"]
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
  s.files += Dir.glob("sample/**/*")
  s.files += Dir.glob("test/**/*")

  s.add_runtime_dependency("clutter", "= #{s.version}")
  s.add_runtime_dependency("clutter-gdk", "= #{s.version}")
  s.add_runtime_dependency("gtk3", "= #{s.version}")
  s.add_runtime_dependency("rake")

  [
    ["alpine_linux", "clutter-gtk-dev"],
    ["debian", "libclutter-gtk-1.0-dev"],
    ["rhel", "pkgconfig(clutter-gtk-1.0)"],
  ].each do |platform, package|
    s.requirements << "system: clutter-gtk-1.0: #{platform}: #{package}"
  end
end
