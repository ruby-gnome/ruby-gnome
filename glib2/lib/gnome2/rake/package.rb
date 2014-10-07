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

require "pathname"

module GNOME2
  module Rake
    class Package
      attr_reader :name
      attr_reader :root_dir
      attr_reader :windows
      attr_reader :native
      attr_writer :external_packages
      def initialize(root_dir)
        @root_dir = Pathname.new(root_dir).expand_path
        @name = @root_dir.basename.to_s
        @windows = WindowsConfiguration.new
        @native = NativeConfiguration.new
        @external_packages = []
      end

      def project_root_dir
        @root_dir.parent
      end

      def glib2_root_dir
        project_root_dir + "glib2"
      end

      def tmp_dir
        @root_dir + "tmp"
      end

      def download_dir
        tmp_dir + "download"
      end

      def patches_dir
        @root_dir + "patches"
      end

      def external_packages
        @external_packages.collect do |package|
          ExternalPackage.new(package)
        end
      end

      class WindowsConfiguration < Struct.new(:packages,
                                              :dependencies,
                                              :build_dependencies,
                                              :gobject_introspection_dependencies,
                                              :build_packages,
                                              :build_host)

        attr_reader :relative_binary_dir, :absolute_binary_dir
        def initialize
          super
          @relative_binary_dir = Pathname.new("vendor/local")
          @absolute_binary_dir = @relative_binary_dir.expand_path
        end

        def packages
          super || []
        end

        def dependencies
          super || []
        end

        def build_dependencies
          super || []
        end

        def gobject_introspection_dependencies
          super || []
        end

        def build_packages
          (super || []).collect do |package|
            package = package.dup
            package[:windows] = {
              :include_paths   => package.delete(:include_paths),
              :library_paths   => package.delete(:library_paths),
              :configure_args  => package.delete(:configure_args),
              :patches         => package.delete(:patches),
              :need_autogen    => package.delete(:need_autogen),
              :need_autoreconf => package.delete(:need_autoreconf),
            }
            ExternalPackage.new(package)
          end
        end

        def build_host
          super || "i686-w64-mingw32"
        end

        def build_architecture
          case build_host
          when /\Ai\d86-/
            "x86_64"
          else
            "x64"
          end
        end
      end

      class NativeConfiguration
        attr_reader :relative_binary_dir, :absolute_binary_dir
        def initialize
          @relative_binary_dir = Pathname.new("tmp/native/local")
          @absolute_binary_dir = @relative_binary_dir.expand_path
        end
      end
    end
  end
end
