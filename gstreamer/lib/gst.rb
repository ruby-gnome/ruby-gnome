# Copyright (C) 2013-2021  Ruby-GNOME Project Team
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

require "English"

require "gobject-introspection"

require "gst/loader"
require "gst/base-loader"
require "gst/controller-loader"
require "gst/audio-loader"

module Gst
  LOG_DOMAIN = "GStreamer"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  class << self
    def const_missing(name)
      init
      if const_defined?(name)
        const_get(name)
      else
        super
      end
    end

    def method_missing(name, *args, &block)
      init
      if respond_to?(name)
        __send__(name, *args, &block)
      else
        super
      end
    end

    def init(*argv)
      class << self
        remove_method(:init)
        remove_method(:const_missing)
        remove_method(:method_missing)
      end
      loader = Loader.new(self, argv)
      loader.load
      init_base
      init_controller
      init_audio
    end

    private
    def init_base
      loader = GstBase::Loader.new(GstBase)
      loader.load
      Gst.include(GstBase)
    end

    def init_controller
      loader = GstController::Loader.new(GstController)
      loader.load
      Gst.include(GstController)
    end

    def init_audio
      loader = GstAudio::Loader.new(GstAudio)
      loader.load
      Gst.include(GstAudio)
    end
  end
end
