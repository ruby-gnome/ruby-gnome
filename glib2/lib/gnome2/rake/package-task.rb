# coding: utf-8

# Copyright(C) 2011-2014 Ruby-GNOME2 Project.
#
# This program is licenced under the same license of Ruby-GNOME2.

require "find"
require "pathname"

require "rubygems"
require "rubygems/package_task"
require "rake/extensiontask"
require "gnome2/rake/package"
require "gnome2/rake/external-package"
require "gnome2/rake/source-download-task"
require "gnome2/rake/native-binary-build-task"
require "gnome2/rake/windows-binary-download-task"
require "gnome2/rake/windows-binary-build-task"

module GNOME2
  module Rake
    class PackageTask
      include ::Rake::DSL

      attr_accessor :name, :summary, :description, :author, :email, :homepage, :required_ruby_version, :post_install_message
      attr_reader :root_dir
      def initialize
        initialize_variables
        initialize_configurations
        file, line, method = caller[1].scan(/^(.*):(\d+)(?::.*`(.*)')?\Z/).first
        @package = Package.new(File.dirname(file))
        @packages = FileList["#{@package.root_dir.parent}/*"].map{|f| File.directory?(f) ? File.basename(f) : nil}.compact
        @name = @package.name
        @cross_compiling_hooks = []
        yield(self) if block_given?
      end

      def cross_compiling(&block)
        @cross_compiling_hooks << block
      end

      def define
        task :default => :build
        define_spec
        define_source_tasks
        define_native_tasks
        define_windows_tasks
        define_package_tasks
      end

      # Deprecated. Use #define instead.
      def define_tasks
        define
      end

      def ruby_gnome2_package?(name)
        @packages.include?(name)
      end

      def dependency
        @dependency_configuration
      end

      def package
        @package
      end

      def windows
        @package.windows
      end

      def native
        @package.native
      end

      def version
        ENV["VERSION"] || guess_version
      end

      def guess_version
        versions = {}
        File.open("#{@package.glib2_root_dir}/ext/glib2/rbglib.h") do |rbglib_h|
          rbglib_h.each_line do |line|
            if /#define\s+RBGLIB_([A-Z]+)_VERSION\s+(\d+)/ =~ line
              versions[$1.downcase] = $2.to_i
            end
          end
        end
        ["major", "minor", "micro"].collect {|type| versions[type]}.compact.join(".")
      end

      def external_packages=(packages)
        @package.external_packages = packages
      end

      private
      def initialize_variables
        @summary = ""
        @description = ""
        @author = "The Ruby-GNOME2 Project Team"
        @email = "ruby-gnome2-devel-en@lists.sourceforge.net"
        @homepage = "http://ruby-gnome2.sourceforge.jp/"
        @external_packages = []
      end

      def initialize_configurations
        @dependency_configuration = DependencyConfiguration.new(self)
      end

      def cross_platform
        "#{windows.build_architecture}-mingw32"
      end

      def define_spec
        @spec = Gem::Specification.new do |s|
          s.name                  = @name
          s.summary               = @summary
          s.description           = @description
          s.author                = @author
          s.email                 = @email
          s.homepage              = @homepage
          s.licenses              = ["LGPLv2.1 or later"]
          s.version               = version
          extensions              = FileList["ext/#{@name}/extconf.rb"]
          extensions.existing!
          s.extensions            = extensions
          s.require_paths         = ["lib"]
          files                   = FileList["ChangeLog", "README",
                                             "Rakefile", "extconf.rb",
                                             "lib/**/*.rb",
                                             "ext/**/depend",
                                             "ext/**/*.{c,h,def,rb}",
                                             "{sample,test}/**/*"]
          files.existing!
          s.files                 = files
          s.required_ruby_version = @required_ruby_version || ">= 1.9.3"
          s.post_install_message  = @post_install_message
          @dependency_configuration.apply(s)
        end
      end

      def define_source_tasks
        define_source_download_tasks
      end

      def define_source_download_tasks
        task = SourceDownloadTask.new(@package)
        task.define
      end

      def define_native_tasks
        define_native_build_tasks
      end

      def define_native_build_tasks
        task = NativeBinaryBuildTask.new(@package)
        task.define
      end

      def define_windows_tasks
        define_windows_extension_task
        define_windows_download_task
        define_windows_build_task
      end

      def so_base_name
        @name.gsub(/-/, "_")
      end

      def define_windows_extension_task
        ::Rake::ExtensionTask.new(so_base_name, @spec) do |ext|
          ext.cross_platform = cross_platform
          ext.ext_dir = "ext/#{@name}"
          ext.cross_compile = true
          ext.cross_compiling do |spec|
            if /mingw|mswin/ =~ spec.platform.to_s
              windows_binary_dir = @package.windows.relative_binary_dir
              windows_files = []
              if windows_binary_dir.exist?
                Find.find(windows_binary_dir.to_s) do |path|
                  next unless File.file?(path)
                  next if /\.zip\z/ =~ path
                  windows_files << path
                end
              end
              spec.files += windows_files
              stage_path = "#{ext.tmp_dir}/#{ext.cross_platform}/stage"
              windows_files.each do |windows_file|
                stage_windows_file = "#{stage_path}/#{windows_file}"
                stage_windows_binary_dir = File.dirname(stage_windows_file)
                directory stage_windows_binary_dir
                stage_windows_file_dependencies = [
                  stage_windows_binary_dir,
                  windows_file,
                ]
                file stage_windows_file => stage_windows_file_dependencies do
                  cp windows_file, stage_windows_file
                end
              end
            end
            @cross_compiling_hooks.each do |hook|
              hook.call(spec)
            end
          end
        end

        extconf_rb = "ext/#{@name}/extconf.rb"
        unless File.exist?(extconf_rb)
          native_task_name = "native:#{@name}:#{cross_platform}"
          if ::Rake::Task.task_defined?(native_task_name)
            ::Rake::Task[native_task_name].prerequisites.clear
          end
        end
      end

      def define_windows_download_task
        GNOME2WindowsBinaryDownloadTask.new(@package)
      end

      def define_windows_build_task
        GNOME2WindowsBinaryBuildTask.new(@package)
      end

      def define_package_tasks
        Gem::PackageTask.new(@spec) do |pkg|
        end
      end

      class DependencyConfiguration
        attr_accessor :platform, :ruby
        def initialize(package)
          @package = package
          @platform = Gem::Platform::RUBY
          @gem_configuration = GemConfiguration.new(@package)
        end

        def gem
          @gem_configuration
        end

        def apply(spec)
          spec.platform = @platform
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
            ver << "= #{@package.version}" if @package.ruby_gnome2_package?(name)
            [name, *ver]
          end
        end
      end
    end
  end
end
