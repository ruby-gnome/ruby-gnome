# Copyright(C) 2012-2015 Ruby-GNOME2 Project.
#
# This program is licenced under the same license of Ruby-GNOME2.

require "open-uri"
require "pathname"

class GNOME2WindowsBinaryBuildTask
  include Rake::DSL

  def initialize(package)
    @package = package
    define
  end

  private
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
          ENV["PKG_CONFIG_LIBDIR"] = rcairo_windows_pkgconfig_path
        end

        prepare_task_names << "pkg_config_for_build"
        task :pkg_config_for_build do
          # XXX: Is it needless?
          # ENV["PKG_CONFIG_FOR_BUILD"] = "env - pkg-config"
        end
      end

      full_prepare_task_names = prepare_task_names.collect do |name|
        "windows:builder:build:prepare:#{name}"
      end
      task :prepare => full_prepare_task_names

      build_packages.each do |package|
        namespace package.name do
          task :before
          download_task = "source:downloader:download:#{package.name}"
          built_file = package.windows.built_file
          if built_file
            built_file = dist_dir + built_file
            file built_file.to_s do
              Rake::Task["windows:builder:build:prepare"].invoke
              Rake::Task[download_task].invoke
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

    package_dir_path =
      package_tmp_dir + package.base_name + package.base_dir_in_package
    Dir.chdir(package_dir_path.to_s) do
      package.windows.patches.each do |patch|
        sh("patch -p1 < #{@package.patches_dir}/#{patch}")
      end
      if File.exist?("configure")
        configure(package)
      else
        cmake(package)
      end
      common_make_args = []
      common_make_args << "MAKE=make"
      common_make_args << "GLIB_COMPILE_SCHEMAS=glib-compile-schemas"
      if package.windows.use_cc_environment_variable?
        common_make_args << cc_env(package)
      end
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

  def configure(package)
    sh("./autogen.sh") if package.windows.need_autogen?
    sh("autoreconf --install") if package.windows.need_autoreconf?
    sh("./configure",
       cc_env(package),
       "CPPFLAGS=#{cppflags(package)}",
       "LDFLAGS=#{ldflags(package)}",
       "--prefix=#{dist_dir}",
       "--host=#{@package.windows.build_host}",
       *package.windows.configure_args) or exit(false)
  end

  def cmake(package)
    sh("cmake",
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

  def glib2_binary_base_dir
    @package.glib2_root_dir + "vendor" + "local"
  end

  def glib2_include_path
    "#{glib2_binary_base_dir}/include"
  end

  def glib2_lib_path
    "#{glib2_binary_base_dir}/lib"
  end

  def rcairo_windows_dir
    suffix = @package.windows.build_architecture_suffix
    @package.project_root_dir.parent + "rcairo.#{suffix}"
  end

  def rcairo_windows_binary_base_dir
    rcairo_windows_dir + "vendor" + "local"
  end

  def rcairo_windows_pkgconfig_path
    "#{rcairo_windows_binary_base_dir}/lib/pkgconfig"
  end

  def rcairo_windows_include_path
    "#{rcairo_windows_binary_base_dir}/include"
  end

  def rcairo_windows_lib_path
    "#{rcairo_windows_binary_base_dir}/lib"
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

  def cppflags(package)
    include_paths = package.windows.include_paths
    if @package.windows.build_dependencies.include?("glib2")
      include_paths += [glib2_include_path]
    end
    include_paths += [
      rcairo_windows_include_path,
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
      rcairo_windows_lib_path,
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
      rcairo_windows_binary_base_dir.to_path,
    ]
    @package.windows.build_dependencies.each do |package|
      paths << "#{@package.project_root_dir}/#{package}/vendor/local"
    end
    paths
  end

  def add_gobject_introspection_make_args(common_make_args)
    unless @package.windows.build_dependencies.include?("gobject-introspection")
      return
    end

    g_ir_scanner = "#{@package.project_root_dir}/gobject-introspection/"
    g_ir_scanner << "#{@package.native.relative_binary_dir}/bin/g-ir-scanner"
    introspection_scanner = "INTROSPECTION_SCANNER=#{g_ir_scanner}"
    common_make_args << introspection_scanner


    dependencies = [
      "gobject-introspection",
      @package.name,
    ]
    dependencies += @package.windows.gobject_introspection_dependencies

    compute_base_dir = lambda do |package|
      "#{@package.project_root_dir}/#{package}/vendor/local"
    end

    gi_base_dir = compute_base_dir.call("gobject-introspection")
    introspection_compiler = "INTROSPECTION_COMPILER="
    introspection_compiler << "#{gi_base_dir}/bin/g-ir-compiler.exe"
    dependencies.each do |package|
      gir_dir = "#{compute_base_dir.call(package)}/share/gir-1.0"
      introspection_compiler << " --includedir=#{gir_dir}"
    end
    common_make_args << introspection_compiler

    data_dirs = dependencies.collect do |package|
      "#{compute_base_dir.call(package)}/share"
    end
    common_make_args << "XDG_DATA_DIRS=#{data_dirs.join(File::PATH_SEPARATOR)}"
  end
end
