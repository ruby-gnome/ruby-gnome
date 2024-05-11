# -*- ruby -*-
#
# Copyright (C) 2018  Ruby-GNOME Project Team
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
  s.name          = "gvlc"
  s.summary       = "Ruby/VLC is a Ruby binding of libVLC for Ruby/GTK."
  s.description   = "Ruby/VLC is a Ruby binding of libVLC for Ruby/GTK."
  s.author        = "The Ruby-GNOME Project Team"
  s.email         = "ruby-gnome2-devel-en@lists.sourceforge.net"
  s.homepage      = "https://ruby-gnome.github.io/"
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
    "ext/#{s.name}/depend",
  ]
  s.files += Dir.glob("lib/**/*.rb")
  s.files += Dir.glob("sample/**/*")
  s.files += Dir.glob("ext/#{s.name}/*.{c,h,def,rb}")
  s.files += Dir.glob("test/**/*")

  s.add_runtime_dependency("glib2", "= #{s.version}")

  s.post_install_message = "This library is experimental."

  s.metadata["msys2_mingw_dependencies"] = "vlc"
end
