# Copyright (C) 2017  Ruby-GNOME2 Project Team
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

module Vte
  if const_defined?(:Regex)
    class Regex
      alias_method :initialize_raw, :initialize
      def initialize(pattern, compile_flags, options)
        flags = parse_compile_flags(compile_flags)
        if options[:for_match]
          initialize_new_for_match(pattern, pattern.length, flags)
        elsif options[:for_search]
          initialize_new_for_search(pattern, pattern.length, flags)
        else
          raise(ArgumentError,
                "must specify usage :for_match or :for_search : #{options.inspect}")
        end
      end

      private

      def parse_compile_flags(compile_flags)
        return compile_flags if compile_flags.class == Integer

        flags = 0
        return flags unless compile_flags.class == Array

        compile_flags.each do |flag|
          flags |= GLib::RegexCompileFlags.new(flag).to_i
        end

        flags
      end
    end
  end
end
