# Copyright(C) 2012 Ruby-GNOME2 Project.
#
# This program is licenced under the same license of Ruby-GNOME2.

require "open-uri"
require "pathname"

class GNOME2Win32BinaryBuildTask
  include Rake::DSL

  def initialize(configuration)
    @configuration = configuration
    @configuration.build_packages.each do |package|
      package[:compression_method] ||= "gz"
    end
    define
  end

  private
  def define
    namespace :win32 do
      namespace :builder do
        task :before
        define_download_tasks
        define_build_tasks
        build_tasks = build_packages.collect do |package|
          "win32:builder:build:#{package[:name]}"
        end
        desc "Build Windows binaries"
        task :build => build_tasks
        task :after
      end
      desc "download source and build Windows binaries"
      task :build => ["win32:builder:before",
                      "win32:builder:build",
                      "win32:builder:after"]
    end
  end

  def define_download_tasks
    namespace :download do
      build_packages.each do |package|
        base = "#{package[:name]}-#{package[:version]}"
        tar = "#{base}.tar.#{package[:compression_method]}"
        tar_url = "#{download_base_url(package)}/#{tar}"
        tar_full_path = download_dir + tar

        desc "Download #{package[:label]} into #{download_dir}."
        task package[:name] => tar_full_path.to_s

        directory_path = tar_full_path.dirname
        directory directory_path.to_s
        file tar_full_path.to_s => directory_path.to_s do
          rake_output_message "downloading... #{tar_url}"
          open(tar_url) do |downloaded_tar|
            tar_full_path.open("wb") do |tar_file|
              tar_file.print(downloaded_tar.read)
            end
          end
        end
      end
    end
  end

  def define_build_tasks
    namespace :build do
      prepare_task_names = []
      namespace :prepare do
        prepare_task_names << "pkg_config"
        task :pkg_config do
          depended_packages = @configuration.build_dependencies
          use_packages = [@configuration.package.name] + depended_packages
          pkg_config_path = use_packages.collect do |package|
            "../#{package}/#{@configuration.relative_binary_dir}/lib/pkgconfig"
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
        download_task = "win32:builder:download:#{package[:name]}"
        desc "Build #{package[:label]} and install it into #{dist_dir}."
        task package[:name] => [:prepare, download_task] do
          package_tmp_dir = tmp_dir + package[:name]
          rm_rf(package_tmp_dir)
          mkdir_p(package_tmp_dir)

          base = "#{package[:name]}-#{package[:version]}"
          tar = "#{base}.tar.#{package[:compression_method]}"
          tar_full_path = download_dir + tar
          Dir.chdir(package_tmp_dir.to_s) do
            sh("tar", "xf", tar_full_path.to_s) or exit(false)
          end

          Dir.chdir((package_tmp_dir + base).to_s) do
            (package[:patches] || []).each do |patch|
              sh("patch -p1 < #{patches_dir}/#{patch}")
            end
            (package[:remove_paths] || []).each do |path|
              rm_rf(path)
            end
            sh("./autogen.sh") if package[:need_autogen]
            sh("autoreconf --install") if package[:need_autoreconf]
            sh("./configure",
               "CPPFLAGS=#{cppflags(package)}",
               "LDFLAGS=#{ldflags(package)}",
               "--prefix=#{dist_dir}",
               "--host=#{@configuration.build_host}",
               *package[:configure_args]) or exit(false)
            common_make_args = []
            common_make_args << "GLIB_COMPILE_SCHEMAS=glib-compile-schemas"
            build_make_args = common_make_args.dup
            install_make_args = common_make_args.dup
            make_n_jobs = ENV["MAKE_N_JOBS"]
            build_make_args << "-j#{make_n_jobs}" if make_n_jobs
            ENV["GREP_OPTIONS"] = "--text"
            sh("nice", "make", *build_make_args) or exit(false)
            sh("make", "install", *install_make_args) or exit(false)

            package_license_dir = license_dir + package[:name]
            mkdir_p(package_license_dir)
            package_license_files = ["AUTHORS", "COPYING", "COPYING.LIB"]
            package_license_files = package_license_files.reject do |file|
              not File.exist?(file)
            end
            cp(package_license_files, package_license_dir)
            bundled_packages = package[:bundled_packages] || []
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
    @configuration.build_packages
  end

  def dist_dir
    Pathname.new(@configuration.absolute_binary_dir)
  end

  def license_dir
    dist_dir + "share" + "license"
  end

  def package_root_dir
    Pathname.new(@configuration.package.root_dir)
  end

  def tmp_dir
    package_root_dir + "tmp"
  end

  def download_dir
    tmp_dir + "download"
  end

  def patches_dir
    package_root_dir + "patches"
  end

  def glib2_dir
    package_root_dir.parent + "glib2"
  end

  def glib2_include_path
    "#{glib2_dir}/vendor/local/include"
  end

  def glib2_lib_path
    "#{glib2_dir}/vendor/local/lib"
  end

  def rcairo_win32_dir
    package_root_dir.parent.parent + "rcairo.win32"
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

  def download_base_url(package)
    download_base_url = package[:download_base_url]
    return download_base_url if download_base_url

    case package[:download_site]
    when :gnome
      download_base_url = "http://ftp.gnome.org/pub/gnome/sources"
      release_series = package[:version].gsub(/\A(\d+\.\d+).+\z/, '\1')
      download_base_url << "/#{package[:name]}/#{release_series}"
    end
    download_base_url
  end

  def cppflags(package)
    include_paths = package[:include_paths] || []
    if @configuration.build_dependencies.include?("glib2")
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
    library_paths = package[:library_paths] || []
    if @configuration.build_dependencies.include?("glib2")
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
end
