# Copyright(C) 2010-2019 Ruby-GNOME2 Project.
#
# This program is licenced under the same license of Ruby-GNOME2.

module GNOME2
  module Rake
    class WindowsBinaryDownloadTask
      URL_BASE = "http://ftp.gnome.org/pub/gnome/binaries"
      def initialize(package)
        @package = package
      end

      def define
      end
    end
  end
end
