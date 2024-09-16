# Copyright (C) 2024  Ruby-GNOME Project Team
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

# Original:
#   https://gitlab.gnome.org/GNOME/gtk/-/blob/main/demos/gtk-demo/main.c
#
# See the following for license information:
#   * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/AUTHORS
#   * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/COPYING

# We need to use "GtkDemo" as type name because "menu-listitem.ui"
# uses "GtkDemo" as type name.
class GtkDemo < GLib::Object
  @demos = []
  class << self
    def register(demo)
      @demos << demo
    end

    def all
      @demos
    end

    def each(&block)
      @demos.each do |demo|
        each_recursive(demo, &block)
      end
    end

    private
    def each_recursive(demo, &block)
      yield(demo)
      demo.children.each do |child_demo|
        each_recursive(child_demo, &block)
      end
    end
  end

  type_register

  install_property(GLib::Param::String.new("name", nil, nil, nil, :readable))
  attr_reader :name

  install_property(GLib::Param::String.new("title", nil, nil, nil, :readable))
  attr_reader :title

  install_property(GLib::Param::String.new("keywords", nil, nil, nil, :readable))
  attr_reader :keywords

  install_property(GLib::Param::String.new("filename", nil, nil, nil, :readable))
  attr_reader :filename

  attr_accessor :children_model

  def initialize(filename, title, keywords)
    super()
    @name = File.basename(filename, ".rb")
    @title = title
    @keywords = keywords
    @filename = filename
    @children_model = nil
  end

  def run(parent)
    raise NotImplemented
  end
end
