# coding: utf-8

# Copyright(C) 2011 Ruby-GNOME2 Project.
#
# This program is licenced under the same license of Ruby-GNOME2.

require 'find'

require 'rubygems'
require 'rake/extensiontask'

class GNOME2Package
  attr_accessor :name, :summary, :description, :author, :email, :homepage, :post_install_message
  def initialize
    initialize_variables
    initialize_configurations
    file, line, method = caller[1].scan(/^(.*):(\d+)(?::.*`(.*)')?\Z/).first
    @package_root = File.dirname(file)
    @glib2_root = File.expand_path("#{@package_root}/../glib2")
    @packages = FileList["#{File.dirname(@package_root)}/*"].map{|f| File.directory?(f) ? File.basename(f) : nil}.compact
    @name = File.basename(@package_root)
    yield(self) if block_given?
  end

  def define_tasks
    task :default => :build
    define_gem_tasks
    define_win32_tasks
  end

  def ruby_gnome2_package?(name)
    @packages.include?(name)
  end

  def dependency
    @dependency_configuration
  end

  def win32
    @win32_configuration
  end

  def version
    ENV["VERSION"] || guess_version
  end

  def guess_version
    versions = {}
    File.open("#{@glib2_root}/ext/glib2/rbglib.h") do |rbglib_h|
      rbglib_h.each_line do |line|
        if /#define\s+RBGLIB_([A-Z]+)_VERSION\s+(\d+)/ =~ line
          versions[$1.downcase] = $2.to_i
        end
      end
    end
    ["major", "minor", "micro"].collect {|type| versions[type]}.compact.join(".")
  end

  private
  def initialize_variables
    @name = ""
    @summary = ""
    @description = ""
    @author = "The Ruby-GNOME2 Proejct Team"
    @email = "ruby-gnome2-devel-en@lists.sourceforge.net"
    @homepage = "http://ruby-gnome2.sourceforge.jp/"
  end

  def initialize_configurations
    @dependency_configuration = DependencyConfiguration.new(self)
    @win32_configuration = Win32Configuration.new(self)
  end

  def define_gem_tasks
    @spec = Gem::Specification.new do |s|
      s.name                  = @name
      s.summary               = @summary
      s.description           = @description
      s.author                = @author
      s.email                 = @email
      s.homepage              = @homepage
      s.version               = version
      s.extensions            = FileList["ext/#{@name}/extconf.rb"]
      s.require_paths         = ["lib"]
      s.files                 = FileList[
          "ChangeLog", "README", "Rakefile", "extconf.rb",
          "lib/**/*.rb", "{ext,sample,test,test-unit}/**/*"]
      s.post_install_message  = @post_install_message
      @dependency_configuration.apply(s)
    end

    Rake::GemPackageTask.new(@spec) do |pkg|
    end
  end

  def define_win32_tasks
    Rake::ExtensionTask.new(@name, @spec) do |ext|
      ext.cross_compile = true
      ext.cross_compiling do |spec|
        if /mingw|mswin/ =~ spec.platform.to_s
          win32_dir = File.join("vendor", "local")
          win32_files = []
          Find.find(win32_dir) do |file|
            next if /\.zip\z/ =~ file
            win32_files << file
          end
          spec.files += win32_files
        end
      end
    end

    namespace :win32 do
      desc "download Windows binaries"
      task :download do
        $LOAD_PATH.unshift("#{@glib2_root}/lib")
        require 'gnome2-win32-binary-downloader'
        GNOME2Win32BinaryDownloader.download(@win32_configuration.to_hash)
      end
    end
  end

  class DependencyConfiguration
    attr_accessor :platform, :ruby
    def initialize(package)
      @package = package
      @platform = Gem::Platform::RUBY
      @ruby = ">=1.8.5"
      @gem_configuration = GemConfiguration.new(@package)
    end

    def gem
      @gem_configuration
    end

    def apply(spec)
      spec.platform = @platform
      spec.required_ruby_version = @ruby
      @gem_configuration.apply(spec)
    end

    class GemConfiguration
      attr_accessor :runtime, :development
      def initialize(package)
        @package = package
        @runtime = []
        @development = []
      end

      def apply(spec)
        @runtime.each do |dependency|
          spec.add_runtime_dependency(*append_version(dependency))
        end

        @development.each do |dependency|
          spec.add_development_dependency(*append_version(dependency))
        end
      end

      def append_version(dependency)
        name, *ver = dependency.is_a?(Array) ? dependency : [dependency]
        ver << ">= #{@package.version}" if @package.ruby_gnome2_package?(name)
        [name, *ver]
      end
    end
  end

  class Win32Configuration
    attr_accessor :packages, :dependencies
    def initialize(package)
      @package = package
      @packages = []
      @dependencies = []
    end

    def to_hash
      {
        :packages => @packages,
        :dependencies => @dependencies,
      }
    end
  end
end

