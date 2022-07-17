# Copyright (C) 2015-2022  Ruby-GNOME Project Team
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

module Gtk
  class Widget
    class << self
      def init
      end

      def have_template?
        @have_template ||= false
      end

      def template_children
        @template_children ||= []
      end

      if method_defined?(:set_template)
        alias_method :set_template_raw, :set_template
        def set_template(template)
          resource = template[:resource]
          data = template[:data]
          if resource
            set_template_from_resource(resource)
          else
            set_template_raw(data)
          end
          @have_template = true
        end

        def bind_template_child(name, options={})
          internal_child = options[:internal_child]
          internal_child = false if internal_child.nil?
          bind_template_child_full(name, internal_child, 0)
          template_children << name
          attr_reader(name)
        end
      end
    end

    alias_method :set_size_request_raw, :set_size_request
    def set_size_request(*args)
      case args.size
      when 1
        options = args[0]
        raise ArgumentError, ":width is missing" unless options.key?(:width)
        width = options[:width]
        raise ArgumentError, ":height is missing" unless options.key?(:height)
        height = options[:height]
      when 2
        width, height = args
      else
        message = "wrong number of arguments (given #{args.size}, expected 1..2)"
        raise ArgumentError, message
      end
      set_size_request_raw(width, height)
    end

    alias_method :translate_coordinates_raw, :translate_coordinates
    def translate_coordinates(widget, x, y)
      translated, x, y = translate_coordinates_raw(widget, x, y)
      if translated
        [x, y]
      else
        nil
      end
    end

    alias_method :style_context_raw, :style_context
    def style_context
      @style_context ||= style_context_raw
    end

    def children
      _children = []
      child = first_child
      while child
        _children << child
        child = child.next_sibling
      end
      _children
    end

    private
    def initialize_post
      klass = self.class
      return unless klass.have_template?
      return unless respond_to?(:init_template)

      init_template
      gtype = klass.gtype
      klass.template_children.each do |name|
        instance_variable_set("@#{name}", get_template_child(gtype, name))
      end
    end

    def ensure_drag_targets(targets)
      return targets unless targets.is_a?(Array)

      targets.collect do |target|
        case target
        when Array
          TargetEntry.new(*target)
        else
          target
        end
      end
    end
  end
end
