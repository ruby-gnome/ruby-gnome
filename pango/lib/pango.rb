# Copyright (C) 2017-2021  Ruby-GNOME Project Team
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

require "gobject-introspection"
require "cairo"
require "cairo-gobject"

require "pango/loader"
require "pango/cairo-loader"
require "pango/fc-loader"
require "pango/ft2-loader"
require "pango/ot-loader"

module Pango
  LOG_DOMAIN = "Pango"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  loader = Loader.new(self)
  loader.load("Pango")
end

module PangoCairo
  loader = Loader.new(self)
  loader.load("PangoCairo")
end

module PangoFc
  loader = Loader.new(self)
  begin
    loader.load("PangoFc")
  rescue GObjectIntrospection::RepositoryError::TypelibNotFound
  end
end

module PangoFT2
  loader = Loader.new(self)
  loader.load("PangoFT2")
end

module PangoOT
  loader = Loader.new(self)
  begin
    loader.load("PangoOT")
  rescue GObjectIntrospection::RepositoryError::TypelibNotFound
  end
end
