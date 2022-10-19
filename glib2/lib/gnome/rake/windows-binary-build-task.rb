# Copyright(C) 2012-2022  Ruby-GNOME Project.
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

module GNOME
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
