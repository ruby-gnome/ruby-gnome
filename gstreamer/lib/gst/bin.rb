# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

module Gst
  class Bin
    include Enumerable

    alias_method :add_element, :add
    def add_elements(*elements)
      elements.each do |element|
        add_element(element)
      end
    end
    alias_method :add, :add_elements

    def <<(element)
      add_element(element)
      self
    end

    def each(options={})
      return to_enum(:each, options) unless block_given?

      if options[:recurse]
        iterator = iterate_recurse
      elsif options[:sink]
        iterator = iterate_sinks
      elsif options[:sorted]
        iterator = iterate_sorted
      elsif options[:sources]
        iterator = iterate_sources
      elsif options[:interface]
        iterator = iterate_all_by_interface(options[:interface])
      else
        iterator = iterate_elements
      end

      loop do
        result, element = iterator.next
        case result
        when IteratorResult::DONE
          break
        when IteratorResult::OK
          yield(element.value)
        when IteratorResult::RESYNC
          iterator.resync
        when IteratorResult::ERROR
          raise "failed to iterate"
        end
      end
    end
  end
end
