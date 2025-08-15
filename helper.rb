# Copyright (C) 2025  Ruby-GNOME Project Team
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

module Helper
  module_function

  ALL_PACKAGES = [
    "adwaita",
    "atk",
    "cairo-gobject",
    "clutter",
    "clutter-gdk",
    "clutter-gstreamer",
    "clutter-gtk",
    "gdk3",
    "gdk4",
    "gdk_pixbuf2",
    "gegl",
    "gio2",
    "glib2",
    "gnumeric",
    "gobject-introspection",
    "goffice",
    "graphene1",
    "gsf",
    "gsk4",
    "gstreamer",
    "gtk3",
    "gtk4",
    "gtksourceview3",
    "gtksourceview4",
    "gtksourceview5",
    "gvlc",
    "libhandy",
    "libsecret",
    "pango",
    "poppler",
    "rsvg2",
    "vte3",
    "vte4",
    "webkit-gtk",
    "webkit2-gtk",
    "wnck3",
  ]

  PRIOR_PACKAGES = [
    "glib2",
    "cairo-gobject",
    "gobject-introspection",
    "gio2",
    "gdk_pixbuf2",
    "pango",
    "atk",
    "gdk3",
    "gtk3",
    "graphene1",
    "gsk4",
    "gdk4",
    "gtk4",
    "gstreamer",
    "clutter",
    "clutter-gdk",
    "gsf",
    "goffice",
  ]

  def all_packages
    ALL_PACKAGES.reject do |package|
      ENV["RUBY_GNOME_#{package.upcase.gsub("-", "_")}_ENABLE"] == "no"
    end
  end

  def all_test_packages
    all_packages.reject do |package|
      ENV["RUBY_GNOME_#{package.upcase.gsub("-", "_")}_TEST_ENABLE"] == "no"
    end
  end

  def prior_packages
    PRIOR_PACKAGES
  end

  def sort_packages(packages)
    packages.sort_by do |package|
      PRIOR_PACKAGES.index(package) ||
        (PRIOR_PACKAGES.size + packages.index(package))
    end
  end

  def sorted_all_packages
    sort_packages(all_packages)
  end
end
