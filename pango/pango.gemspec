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
  s.name          = "pango"
  s.summary       = "Ruby/Pango is a Ruby binding of pango-1.x."
  s.description   = "Ruby/Pango is a Ruby binding of pango-1.x based on GObject-Introspection."
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
  s.files += Dir.glob("ext/#{s.name}/*.{c,h,def,rb}")

  s.add_runtime_dependency("cairo-gobject", "= #{s.version}")
  s.add_runtime_dependency("gobject-introspection", "= #{s.version}")

  s.metadata = {
    "bug_tracker_uri" => "https://github.com/ruby-gnome/ruby-gnome/issues",
    "changelog_uri" => "https://github.com/ruby-gnome/ruby-gnome/blob/master/NEWS",
    "documentation_uri" => "https://ruby-gnome2.osdn.jp/hiki.cgi?Ruby%2FPango",
    "mailing_list_uri" => "https://sourceforge.net/p/ruby-gnome2/mailman/ruby-gnome2-devel-en",
    "source_code_uri" => "https://github.com/ruby-gnome/ruby-gnome/tree/master/pango",
    "msys2_mingw_dependencies" => "pango"
  }
end
