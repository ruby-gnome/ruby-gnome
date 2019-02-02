# Copyright(C) 2012-2019 Ruby-GNOME2 Project.
#
# This program is licenced under the same license of Ruby-GNOME2.

module GNOME2
  module Rake
    class WindowsBinaryBuildTask
      def initialize(package)
        @package = package
      end

      def define
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
      def rcairo_dir
        @package.project_root_dir.parent + "rcairo"
      end
    end
  end
end
