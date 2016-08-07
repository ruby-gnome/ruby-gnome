# -*- ruby -*-
#
# Copyright (C) 2013-2015  Ruby-GNOME2 Project Team
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

require "open-uri"

module GNOME2
  module Rake
    class ExternalPackage < Struct.new(:name,
                                       :base_name,
                                       :archive_base_name,
                                       :label,
                                       :version,
                                       :download_site,
                                       :download_base_url,
                                       :compression_method,
                                       :base_dir_in_package,
                                       :windows,
                                       :native,
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
        super || "#{name}-#{version}"
      end

      def archive_base_name
        super || "#{base_name}.tar.#{compression_method}"
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

      def base_dir_in_package
        super || "."
      end

      def windows
        super || WindowsConfiguration.new({})
      end

      def windows=(properties)
        super(WindowsConfiguration.new(properties))
      end

      def native
        super || NativeConfiguration.new({})
      end

      def native=(properties)
        super(NativeConfiguration.new(properties))
      end

      def bundled_packages
        super || []
      end

      def latest_version
        case download_site
        when :gnome
          latest_version_gnome
        when :freedesktop
          latest_version_freedesktop
        when :freedesktop_gstreamer
          latest_version_freedesktop_gstreamer
        else
          nil
        end
      end

      private
      def download_site_base_url
        case download_site
        when :gnome
          base_url = gnome_base_url
          release_series = version.gsub(/\A(\d+\.\d+)(?:[^\d].*)?\z/, '\1')
          base_url << "/#{name}/#{release_series}"
        when :freedesktop
          base_url = freedesktop_base_url
          base_url << "/#{name}/release"
        when :freedesktop_gstreamer
          base_url = freedesktop_gstreamer_base_url
          base_url << "/#{name}"
        when :gnu
          base_url = "http://ftp.gnu.org/pub/gnu/#{name}"
        else
          base_url = nil
        end
        base_url
      end

      def gnome_base_url
        "http://ftp.gnome.org/pub/gnome/sources"
      end

      def freedesktop_base_url
        "https://www.freedesktop.org/software"
      end

      def freedesktop_gstreamer_base_url
        "https://gstreamer.freedesktop.org/src"
      end

      def sort_versions(versions)
        versions.sort_by do |version|
          version.split(".").collect(&:to_i)
        end
      end

      def latest_version_gnome
        base_url = "#{gnome_base_url}/#{name}"
        minor_versions = []
        open(base_url) do |index|
          index.read.scan(/<a (.+?)>/) do |content,|
            case content
            when /href="(\d+(?:\.\d+)*)\/?"/
              minor_version = $1
              next if development_minor_version_gnome?(minor_version)
              minor_versions << minor_version
            end
          end
        end
        return nil if minor_versions.empty?

        latest_minor_version = sort_versions(minor_versions).last
        versions = []
        open("#{base_url}/#{latest_minor_version}") do |index|
          index.read.scan(/<a (.+?)>/) do |content,|
            case content
            when /href="#{Regexp.escape(name)}-
                        (\d+(?:\.\d+)*)
                        \.tar\.#{Regexp.escape(compression_method)}"/x
              versions << $1
            end
          end
        end
        sort_versions(versions).last
      end

      def development_minor_version_gnome?(minor_version)
        minor_version.split(".").last.to_i.odd?
      end

      def latest_version_freedesktop
        base_url = "#{freedesktop_base_url}/#{name}/release"
        versions = []
        open(base_url) do |index|
          index.read.scan(/<a (.+?)>/) do |content,|
            case content
            when /href="#{Regexp.escape(name)}-
                        (\d+(?:\.\d+)*)
                        \.tar\.#{Regexp.escape(compression_method)}"/x
              versions << $1
            end
          end
        end
        sort_versions(versions).last
      end

      def latest_version_freedesktop_gstreamer
        base_url = "#{freedesktop_gstreamer_base_url}/#{name}"
        versions = []
        open(base_url) do |index|
          index.read.scan(/<a (.+?)>/) do |content,|
            case content
            when /href="#{Regexp.escape(name)}-
                        (\d+(?:\.\d+)*)
                        \.tar\.#{Regexp.escape(compression_method)}"/x
              version = $1
              next if development_version_freedesktop_gstreamer?(version)
              versions << version
            end
          end
        end
        sort_versions(versions).last
      end

      def development_version_freedesktop_gstreamer?(version)
        version.split(".")[1].to_i.odd?
      end

      class WindowsConfiguration < Struct.new(:build,
                                              :include_paths,
                                              :library_paths,
                                              :configure_args,
                                              :cmake_args,
                                              :cc_args,
                                              :patches,
                                              :built_file,
                                              :need_autogen,
                                              :need_autoreconf,
                                              :build_concurrently,
                                              :use_cc_environment_variable,
                                              :gobject_introspection_compiler_split_args)
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

        def cmake_args
          super || []
        end

        def cc_args
          super || []
        end

        def patches
          super || []
        end

        def built_file
          super || nil
        end

        def need_autogen?
          need_autogen.nil? ? false : need_autogen
        end

        def need_autoreconf?
          need_autoreconf.nil? ? false : need_autoreconf
        end

        def build_concurrently?
          build_concurrently.nil? ? true : build_concurrently
        end

        def use_cc_environment_variable?
          use_cc_environment_variable.nil? ? true : use_cc_environment_variable
        end

        def gobject_introspection_compiler_split_args?
          gobject_introspection_compiler_split_args
        end
      end

      class NativeConfiguration < Struct.new(:build,
                                             :configure_args,
                                             :patches,
                                             :built_file,
                                             :need_autogen,
                                             :need_autoreconf,
                                             :build_concurrently)
        def initialize(properties)
          super()
          properties.each do |key, value|
            send("#{key}=", value)
          end
        end

        def build?
          build.nil? ? false : build
        end

        def configure_args
          super || []
        end

        def patches
          super || []
        end

        def built_file
          super || nil
        end

        def need_autogen?
          need_autogen.nil? ? false : need_autogen
        end

        def need_autoreconf?
          need_autoreconf.nil? ? false : need_autoreconf
        end

        def build_concurrently?
          build_concurrently.nil? ? true : build_concurrently
        end
      end
    end
  end
end
