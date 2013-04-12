# Copyright(C) 2012 Ruby-GNOME2 Project.
#
# This program is licenced under the same license of Ruby-GNOME2.

require "open-uri"
require "pathname"

class GNOME2Win32BinaryBuildTask
  include Rake::DSL

  def initialize(package)
    @package = package
    define
  end

  private
  def define
    namespace :win32 do
      namespace :builder do
        task :before
        define_build_tasks
        build_tasks = build_packages.collect do |package|
          "win32:builder:build:#{package.name}"
        end
        task :build => build_tasks
        task :after
      end
      desc "Build Windows binaries"
      task :build => ["win32:builder:before",
                      "win32:builder:build",
                      "win32:builder:after"]
    end
  end

  def define_build_tasks
    namespace :build do
      prepare_task_names = []
      namespace :prepare do
        prepare_task_names << "pkg_config"
        task :pkg_config do
          depended_packages = @package.windows.build_dependencies
          use_packages = [@package.name] + depended_packages
          pkg_config_path = use_packages.collect do |package|
            "../#{package}/#{@package.windows.relative_binary_dir}/lib/pkgconfig"
          end
          ENV["PKG_CONFIG_PATH"] = pkg_config_path.collect do |path|
            File.expand_path(path)
          end.join(":")
          ENV["PKG_CONFIG_LIBDIR"] = rcairo_win32_pkgconfig_path
        end
      end

      full_prepare_task_names = prepare_task_names.collect do |name|
        "win32:builder:build:prepare:#{name}"
      end
      task :prepare => full_prepare_task_names

      build_packages.each do |package|
        download_task = "source:downloader:download:#{package.name}"
        desc "Build #{package.label} and install it into #{dist_dir}."
        task package.name => [:prepare, download_task] do
          package_tmp_dir = @package.tmp_dir + package.name
          rm_rf(package_tmp_dir)
          mkdir_p(package_tmp_dir)

          tar_full_path = @package.download_dir + package.archive_base_name
          Dir.chdir(package_tmp_dir.to_s) do
            sh("tar", "xf", tar_full_path.to_s) or exit(false)
          end

          Dir.chdir((package_tmp_dir + package.base_name).to_s) do
            package.windows.patches.each do |patch|
              sh("patch -p1 < #{@package.patches_dir}/#{patch}")
            end
            sh("./autogen.sh") if package.windows.need_autogen?
            sh("autoreconf --install") if package.windows.need_autoreconf?
            cc_env = "CC=#{@package.windows.build_host}-gcc"
            sh("./configure",
               cc_env,
               "CPPFLAGS=#{cppflags(package)}",
               "LDFLAGS=#{ldflags(package)}",
               "--prefix=#{dist_dir}",
               "--host=#{@package.windows.build_host}",
               *package.windows.configure_args) or exit(false)
            common_make_args = []
            common_make_args << "GLIB_COMPILE_SCHEMAS=glib-compile-schemas"
            common_make_args << cc_env
            add_gobject_introspection_make_args(common_make_args)
            build_make_args = common_make_args.dup
            install_make_args = common_make_args.dup
            if package.windows.build_concurrently?
              make_n_jobs = ENV["MAKE_N_JOBS"]
              build_make_args << "-j#{make_n_jobs}" if make_n_jobs
            end
            ENV["GREP_OPTIONS"] = "--text"
            sh("nice", "make", *build_make_args) or exit(false)
            sh("make", "install", *install_make_args) or exit(false)

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
      end
    end
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
    "#{@package.glib2_root_dir}/vendor/local/include"
  end

  def glib2_lib_path
    "#{@package.glib2_root_dir}/vendor/local/lib"
  end

  def gobject_introspection_data_path
    "#{@package.root_dir}/gobject-introspection/vendor/local/share"
  end

  def rcairo_win32_dir
    @package.project_root_dir.parent + "rcairo.win32"
  end

  def rcairo_win32_pkgconfig_path
    "#{rcairo_win32_dir}/vendor/local/lib/pkgconfig"
  end

  def rcairo_win32_include_path
    "#{rcairo_win32_dir}/vendor/local/include"
  end

  def rcairo_win32_lib_path
    "#{rcairo_win32_dir}/vendor/local/lib"
  end

  def cppflags(package)
    include_paths = package.windows.include_paths
    if @package.windows.build_dependencies.include?("glib2")
      include_paths += [glib2_include_path]
    end
    include_paths += [
      rcairo_win32_include_path,
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
      rcairo_win32_lib_path,
      dist_dir + 'lib',
    ]
    ldflags = library_paths.collect do |path|
      "-L#{path}"
    end
    ldflags.join(" ")
  end

  def add_gobject_introspection_make_args(common_make_args)
    unless @package.windows.build_dependencies.include?("gobject-introspection")
      return
    end
    common_make_args << "XDG_DATA_DIRS=#{gobject_introspection_data_path}"
  end
end
