# Copyright(C) 2011-2019 Ruby-GNOME2 Project.
#
# This program is licenced under the same license of Ruby-GNOME2.

require "find"
require "pathname"

require "rubygems"
require "rubygems/package_task"
require "gnome2/rake/package"
require "gnome2/rake/external-package"
require "gnome2/rake/source-download-task"
require "gnome2/rake/native-binary-build-task"
require "gnome2/rake/windows-binary-download-task"
require "gnome2/rake/windows-binary-build-task"

module GNOME2
  module Rake
    class PackageTask
      class << self
        def define(spec, root_dir, &block)
          new(spec, root_dir, &block).define
        end
      end

      include ::Rake::DSL

      attr_accessor :name, :summary, :description, :author, :email, :homepage, :required_ruby_version, :post_install_message
      attr_reader :root_dir
      def initialize(spec=nil, root_dir=nil)
        @spec = spec
        initialize_variables
        initialize_configurations
        if @spec
          @package = Package.new(@spec.name, root_dir)
        else
          file, line, method = caller[1].scan(/^(.*):(\d+)(?::.*`(.*)')?\Z/).first
          root_dir = File.dirname(file)
          @package = Package.new(File.basename(root_dir), root_dir)
        end
        @packages = FileList["#{@package.root_dir.parent}/*"].map{|f| File.directory?(f) ? File.basename(f) : nil}.compact
        @name = @package.name
        @cross_compiling_hooks = []
        yield(self) if block_given?
      end

      def cross_compiling(&block)
        warn("cross compile support is dropped.")
        @cross_compiling_hooks << block
      end

      def define
        task :default => :build
        define_spec
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

      def external_packages
        @package.external_packages
      end

      def external_packages=(packages)
        @package.external_packages = packages
      end

      def windows_binary_build_task
        @windows_binary_build_task ||= WindowsBinaryBuildTask.new(@package)
      end

      private
      def initialize_variables
        @summary = ""
        @description = ""
        @author = "The Ruby-GNOME2 Project Team"
        @email = "ruby-gnome2-devel-en@lists.sourceforge.net"
        @homepage = "https://ruby-gnome2.osdn.jp/"
        @external_packages = []
      end

      def initialize_configurations
        @dependency_configuration = DependencyConfiguration.new(self)
      end

      def define_spec
        @spec ||= Gem::Specification.new do |s|
          warn("spec generation is deprecated. Use .gemspec instead.")
          s.name                  = @name
          s.summary               = @summary
          s.description           = @description
          s.author                = @author
          s.email                 = @email
          s.homepage              = @homepage
          s.licenses              = ["LGPL-2.1+"]
          s.version               = version
          extensions              = FileList["ext/#{@name}/extconf.rb",
                                             "dependency-check/Rakefile"]
          extensions.existing!
          s.extensions            = extensions
          s.require_paths         = ["lib"]
          files                   = FileList["ChangeLog", "README",
                                             "Rakefile", "extconf.rb",
                                             "dependency-check/Rakefile",
                                             "lib/**/*.rb",
                                             "ext/**/depend",
                                             "ext/**/*.{c,h,def,rb}",
                                             "{sample,test}/**/*"]
          files.existing!
          s.files                 = files
          s.required_ruby_version = @required_ruby_version || ">= 2.1.0"
          s.post_install_message  = @post_install_message
          @dependency_configuration.apply(s)
        end
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
