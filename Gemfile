# Copyright (C) 2012-2025  Ruby-GNOME Project Team
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

source "https://rubygems.org/"

plugin "rubygems-requirements-system"

require_relative "helper"
Helper.sorted_all_packages.each do |package|
  gemspec path: File.join(__dir__, package)
end

group :development, :docs, :test do
  gem "bundler"
  gem "erb"
  gem "rake"
end

group :docs do
  gem "commonmarker"
  gem "rdoc"
  gem "yard"
  gem "yard-gobject-introspection",
      github: "ruby-gnome/yard-gobject-introspection"
end

group :test do
  gem "test-unit"
  gem "webrick"
end

local_gemfile = File.join(File.dirname(__FILE__), "Gemfile.local")
if File.exist?(local_gemfile)
  eval_gemfile(local_gemfile)
end
