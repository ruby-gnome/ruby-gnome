# Copyright(C) 2012-2015 Ruby-GNOME2 Project.
#
# This program is licenced under the same license of Ruby-GNOME2.

require "open-uri"
require "pathname"
require "tempfile"
require "tmpdir"

module GNOME2
  module Rake
    class WindowsBinaryBuildTask
      include ::Rake::DSL

      def initialize(package)
        @package = package
      end

      def define
        namespace :windows do
          namespace :builder do
            task :before
            define_build_tasks
            build_tasks = build_packages.collect do |package|
              "windows:builder:build:#{package.name}"
            end
            task :build => build_tasks
            task :after
          end
          desc "Build Windows binaries"
          task :build => ["windows:builder:before",
            "windows:builder:build",
            "windows:builder:after"]
        end
      end

      def rcairo_binary_base_dir
        rcairo_dir + "vendor" + "local"
      end

      def glib2_binary_base_dir
        @package.glib2_root_dir + "vendor" + "local"
      end

      def binary_base_dir(package)
        @package.project_root_dir + package + "vendor" + "local"
      end

      private
      def define_build_tasks
        namespace :build do
          task :prepare

          build_packages.each do |package|
            namespace package.name do
              task :before
              download_task = "source:downloader:download:#{package.name}"
              built_file = package.windows.built_file
              if built_file
                built_file = dist_dir + built_file
                file built_file.to_s do
                  ::Rake::Task["windows:builder:build:prepare"].invoke
                  ::Rake::Task[download_task].invoke
                  build_package_task_body(package)
                end
                task :build => built_file.to_s
              else
                task :build => [:prepare, download_task] do
                  build_package_task_body(package)
                end
              end
              task :after
            end

            prefix = "windows:builder:build:#{package.name}"
            desc "Build #{package.label} and install it into #{dist_dir}."
            task package.name => [
              "#{prefix}:before",
              "#{prefix}:build",
              "#{prefix}:after",
            ]
          end
        end
      end

      def build_package_task_body(package)
        package_tmp_dir = @package.tmp_dir + "windows" + package.name
        rm_rf(package_tmp_dir)
        mkdir_p(package_tmp_dir)

        tar_full_path = @package.download_dir + package.archive_base_name
        Dir.chdir(package_tmp_dir.to_s) do
          sh("tar", "xf", tar_full_path.to_s)
        end

        env = build_env
        package_dir_path =
          package_tmp_dir + package.base_name + package.base_dir_in_package
        Dir.chdir(package_dir_path.to_s) do
          package.windows.patches.each do |patch|
            sh("patch -p1 < #{@package.patches_dir}/#{patch}")
          end
          common_make_args = []
          common_make_args << "MAKE=make"
          common_make_args << "GLIB_COMPILE_SCHEMAS=glib-compile-schemas"
          if package.windows.use_cc_environment_variable?
            common_make_args << cc_env(package)
          end
          add_gobject_introspection_make_args(package, common_make_args)
          build_make_args = common_make_args.dup
          install_make_args = common_make_args.dup
          if package.windows.build_concurrently?
            make_n_jobs = ENV["MAKE_N_JOBS"]
            build_make_args << "-j#{make_n_jobs}" if make_n_jobs
          end
          # build_make_args << "--debug"
          # build_make_args << "V=1"
          # build_make_args << "VERBOSE=1"
          # env["GI_SCANNER_DEBUG"] = "save-temps"
          if File.exist?("meson.build") and not File.exist?("Makefile.am")
            source_dir = Dir.pwd
            build_dir = "build"
            mkdir_p(build_dir)
            Dir.chdir(build_dir) do
              meson(env, package, source_dir)
              sh(env.merge(make_args_to_env(build_make_args)),
                 "nice", "ninja") or exit(false)
              sh(env.merge(make_args_to_env(install_make_args)),
                 "ninja", "install") or exit(false)
            end
          else
            if File.exist?("Makefile.in")
              configure(env, package)
            else
              cmake(env, package)
            end
            sh(env, "nice", "make", *build_make_args) or exit(false)
            sh(env, "make", "install", *install_make_args) or exit(false)
          end

          package_license_dir = license_dir + package.name
          mkdir_p(package_license_dir)
          package_license_files = ["AUTHORS", "COPYING", "COPYING.LIB"]
          package_license_files = package_license_files.reject do |file|
            not File.exist?(file)
          end
          cp(package_license_files, package_license_dir)
          bundled_packages = package.bundled_packages
          bundled_packages.each do |bundled_package|
            bundled_package_license_dir = license_dir + bundled_package[:name]
            mkdir_p(bundled_package_license_dir)
            license_files = bundled_package[:license_files].collect do |file|
              File.join(bundled_package[:path], file)
            end
            cp(license_files, bundled_package_license_dir)
          end
        end
      end

      def build_env
        env = {
          "GI_HOST_OS" => "nt",
          "MSYSTEM" => "MINGW64",
          "GREP_OPTIONS" => "--text",
        }
        depended_packages = @package.windows.build_dependencies
        use_packages = [@package.name] + depended_packages
        pkg_config_path = use_packages.collect do |package|
          "../#{package}/#{@package.windows.relative_binary_dir}/lib/pkgconfig"
        end
        env["PKG_CONFIG_PATH"] = pkg_config_path.collect do |path|
          File.expand_path(path)
        end.join(":")
        env["PKG_CONFIG_LIBDIR"] = rcairo_pkgconfig_path
        env
      end

      def configure(env, package)
        sh(env, "./autogen.sh") if package.windows.need_autogen?
        if package.windows.need_autoreconf?
          sh(env, "autoreconf", "--install", "--force")
        end
        sh(env, "env")
        sh(env,
           "./configure",
           cc_env(package),
           dlltool_env,
           "CPPFLAGS=#{cppflags(package)}",
           "LDFLAGS=#{ldflags(package)}",
           "--prefix=#{dist_dir}",
           "--host=#{@package.windows.build_host}",
           *package.windows.configure_args) or exit(false)
      end

      def meson(env, package, source_dir)
        cross_file = Tempfile.new("meson-cross-file")
        cross_file.puts(<<-CROSS_FILE)
[host_machine]
system = 'windows'
cpu_family = 'x86_64'
cpu = 'x86_64'
endian = 'littel'

[binaries]
c = '/usr/bin/#{cc(package)}'
cpp = '/usr/bin/#{cxx(package)}'
ar = '/usr/bin/#{ar}'
strip = '/usr/bin/#{strip}'
dlltool = '/usr/bin/#{dlltool}'
pkgconfig = '/usr/bin/pkg-config'
        CROSS_FILE
        cross_file.close
        sh(env.merge({
                       "PATH" => "#{g_ir_scanner_bin_dir}:#{ENV["PATH"]}",
                       "CPPFLAGS" => cppflags(package),
                       "LDFLAGS" => ldflags(package),
                     }),
           "meson",
           source_dir,
           ".",
           "--prefix=#{dist_dir}",
           "--cross-file=#{cross_file.path}",
           *package.windows.meson_args) or exit(false)
      end

      def cmake(env, package)
        sh(env,
           "cmake",
           ".",
           "-DCMAKE_INSTALL_PREFIX=#{dist_dir}",
           "-DCMAKE_SYSTEM_NAME=Windows",
           "-DCMAKE_SYSTEM_PROCESSOR=#{@package.windows.build_architecture}",
           "-DCMAKE_C_COMPILER=#{cc(package)}",
           "-DCMAKE_CXX_COMPILER=#{cxx(package)}",
           *package.windows.cmake_args) or exit(false)
      end

      def cc_env(package)
        "CC=#{cc(package)}"
      end

      def dlltool_env
        "DLLTOOL=#{dlltool}"
      end

      def build_packages
        packages = @package.external_packages.select do |package|
          package.windows.build?
        end
        # For backward compatibility
        packages + @package.windows.build_packages
      end

      def dist_dir
        @package.windows.absolute_binary_dir
      end

      def license_dir
        dist_dir + "share" + "license"
      end

      def glib2_include_path
        "#{glib2_binary_base_dir}/include"
      end

      def glib2_lib_path
        "#{glib2_binary_base_dir}/lib"
      end

      def rcairo_dir
        suffix = @package.windows.build_architecture_suffix
        @package.project_root_dir.parent + "rcairo.#{suffix}"
      end

      def rcairo_pkgconfig_path
        "#{rcairo_binary_base_dir}/lib/pkgconfig"
      end

      def rcairo_include_path
        "#{rcairo_binary_base_dir}/include"
      end

      def rcairo_lib_path
        "#{rcairo_binary_base_dir}/lib"
      end

      def cc(package)
        cc_command_line = [
          "#{@package.windows.build_host}-gcc",
          *package.windows.cc_args,
        ]
        cc_command_line.compact.join(" ")
      end

      def cxx(package)
        cxx_command_line = [
          "#{@package.windows.build_host}-g++",
        ]
        cxx_command_line.compact.join(" ")
      end

      def ar
        "#{@package.windows.build_host}-ar"
      end

      def strip
        "#{@package.windows.build_host}-strip"
      end

      def dlltool
        "#{@package.windows.build_host}-dlltool"
      end

      def g_ir_scanner_bin_dir
        "#{@package.project_root_dir}/gobject-introspection/" +
          "#{@package.native.relative_binary_dir}/bin"
      end

      def g_ir_scanner
        "#{g_ir_scanner_bin_dir}/g-ir-scanner"
      end

      def cppflags(package)
        include_paths = package.windows.include_paths
        if @package.windows.build_dependencies.include?("glib2")
          include_paths += [glib2_include_path]
        end
        include_paths += [
          rcairo_include_path,
          dist_dir + 'include',
        ]
        cppflags = include_paths.collect do |path|
          "-I#{path}"
        end
        cppflags.join(" ")
      end

      def ldflags(package)
        library_paths = package.windows.library_paths
        if @package.windows.build_dependencies.include?("glib2")
          library_paths += [glib2_lib_path]
        end
        library_paths += [
          rcairo_lib_path,
          dist_dir + 'lib',
        ]
        ldflags = library_paths.collect do |path|
          "-L#{path}"
        end
        ldflags.join(" ")
      end

      def cmake_root_paths
        paths = [
          "/usr/#{@package.windows.build_host}",
          rcairo_binary_base_dir.to_path,
        ]
        @package.windows.build_dependencies.each do |package|
          paths << binary_base_dir(package).to_path
        end
        paths
      end

      def make_args_to_env(make_args)
        env = {}
        make_args.each do |arg|
          key, value = arg.split("=", 2)
          env[key] = value
        end
        env
      end

      def add_gobject_introspection_make_args(package, common_make_args)
        return unless use_gobject_introspection?(package)

        introspection_scanner = "INTROSPECTION_SCANNER=#{g_ir_scanner}"
        common_make_args << introspection_scanner


        dependencies = [
          "gobject-introspection",
          @package.name,
        ]
        dependencies += @package.windows.gobject_introspection_dependencies

        gi_base_dir = binary_base_dir("gobject-introspection")
        introspection_compiler = "INTROSPECTION_COMPILER="
        introspection_compiler << "#{gi_base_dir}/bin/g-ir-compiler.exe"
        introspection_compiler_args = ""
        dependencies.each do |dependent_package|
          gir_dir = "#{binary_base_dir(dependent_package)}/share/gir-1.0"
          introspection_compiler_args << " --includedir=#{gir_dir}"
        end
        if package.windows.gobject_introspection_compiler_split_args?
          common_make_args << introspection_compiler
          common_make_args <<
            "INTROSPECTION_COMPILER_ARGS=#{introspection_compiler_args}"
          common_make_args <<
            "INTROSPECTION_COMPILER_OPTS=#{introspection_compiler_args}"
        else
          introspection_compiler << " #{introspection_compiler_args}"
          common_make_args << introspection_compiler
        end

        common_make_args << dlltool_env

        data_dirs = dependencies.collect do |dependent_package|
          "#{binary_base_dir(dependent_package)}/share"
        end
        common_make_args << "XDG_DATA_DIRS=#{data_dirs.join(File::PATH_SEPARATOR)}"
      end

      def use_gobject_introspection?(package)
        return false unless package.windows.use_gobject_introspection?
        @package.windows.build_dependencies.include?("gobject-introspection")
      end
    end
  end
end
