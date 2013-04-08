# -*- ruby -*-
#
# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

module GNOME2
  module Rake
    class ExternalPackage < Struct.new(:name,
                                       :label,
                                       :version,
                                       :download_site,
                                       :download_base_url,
                                       :compression_method,
                                       :windows,
                                       :patches,
                                       :need_autogen,
                                       :need_autoreconf,
                                       :build_concurrently,
                                       :bundled_packages)
      def initialize(properties)
        super()
        properties.each do |key, value|
          send("#{key}=", value)
        end
      end

      def compression_method
        super || "gz"
      end

      def base_name
        "#{name}-#{version}"
      end

      def archive_base_name
        "#{base_name}.tar.#{compression_method}"
      end

      def archive_url
        "#{download_base_url}/#{archive_base_name}"
      end

      def download_base_url
        super || download_site_base_url
      end

      def patches
        super || []
      end

      def need_autogen?
        need_autogen
      end

      def need_autoreconf?
        need_autoreconf
      end

      def windows
        super || WindowsConfiguration.new({})
      end

      def windows=(properties)
        super(WindowsConfiguration.new(properties))
      end

      def build_concurrently?
        build_concurrently
      end

      def bundled_packages
        super || []
      end

      private
      def download_site_base_url
        case download_site
        when :gnome
          base_url = "http://ftp.gnome.org/pub/gnome/sources"
          release_series = version.gsub(/\A(\d+\.\d+).+\z/, '\1')
          base_url << "/#{name}/#{release_series}"
        else
          base_url = nil
        end
        base_url
      end

      class WindowsConfiguration < Struct.new(:build,
                                              :include_paths,
                                              :library_paths,
                                              :configure_args)
        def initialize(properties)
          super()
          properties.each do |key, value|
            send("#{key}=", value)
          end
        end

        def build?
          build.nil? ? true : build
        end

        def include_paths
          super || []
        end

        def library_paths
          super || []
        end

        def configure_args
          super || []
        end
      end
    end
  end
end
