# -*- ruby -*-
#
# Copyright (C) 2013-2018  Ruby-GNOME2 Project Team
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

require "rake"

module GNOME2
  module Rake
    class NativeBinaryBuildTask
      include ::Rake::DSL

      def initialize(package)
        @package = package
      end

      def define
        namespace :native do
          namespace :builder do
            task :before
            define_build_tasks
            build_tasks = build_packages.collect do |package|
              "native:builder:build:#{package.name}"
            end
            task :build => build_tasks
            task :after
          end

          desc "Build binaries for build environment"
          task :build => [
            "native:builder:before",
            "native:builder:build",
            "native:builder:after",
          ]
        end
      end

      private
      def define_build_tasks
        namespace :build do
          build_packages.each do |package|
            namespace package.name do
              download_task = "source:downloader:download:#{package.name}"
              built_file = package.native.built_file
              if built_file
                built_file = dist_dir + built_file
                file built_file.to_s do
                  ::Rake::Task[download_task].invoke
                  build_package_task_body(package)
                end
                task :build => built_file.to_s
              else
                task :build => [download_task] do
                  build_package_task_body(package)
                end
              end
            end
            desc "Build #{package.label} and install it into #{dist_dir}."
            task package.name => "native:builder:build:#{package.name}:build"
          end
        end
      end

      def build_package_task_body(package)
        package_tmp_dir = @package.tmp_dir + "native" + package.name
        rm_rf(package_tmp_dir)
        mkdir_p(package_tmp_dir)

        tar_full_path = @package.download_dir + package.archive_base_name
        Dir.chdir(package_tmp_dir.to_s) do
          sh("tar", "xf", tar_full_path.to_s) or exit(false)
        end

        package_dir_path =
          package_tmp_dir + package.base_name + package.base_dir_in_package
        Dir.chdir(package_dir_path.to_s) do
          package.native.patches.each do |patch|
            sh("patch -p1 < #{@package.patches_dir}/#{patch}")
          end
          build_dir = "build"
          env = []
          env << "PKG_CONFIG_PATH=#{pkg_config_path}"
          env << "GLIB_COMPILE_SCHEMAS=glib-compile-schemas"
          sh("env",
             *env,
             "meson",
             build_dir,
             "--prefix=#{dist_dir}",
             "--libdir=lib") or exit(false)
          sh("env", *env, "nice", "ninja", "-C", build_dir) or exit(false)
          sh("env", *env, "ninja", "-C", build_dir, "install") or exit(false)
        end
      end

      def build_packages
        @package.external_packages.select do |package|
          package.native.build?
        end
      end

      def dist_dir
        @package.native.absolute_binary_dir
      end

      def pkg_config_path
        dist_dir + "lib/pkgconfig"
      end
    end
  end
end
