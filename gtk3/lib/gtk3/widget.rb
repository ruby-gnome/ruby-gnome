# Copyright (C) 2015-2020  Ruby-GNOME Project Team
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

      alias_method :set_connect_func_raw, :set_connect_func
      def set_connect_func(&block)
        set_connect_func_raw do |*args|
          Builder.connect_signal(*args, &block)
        end
      end
    end

    alias_method :events_raw, :events
    def events
      Gdk::EventMask.new(events_raw)
    end

    alias_method :add_events_raw, :add_events
    def add_events(new_events)
      unless new_events.is_a?(Gdk::EventMask)
        new_events = Gdk::EventMask.new(new_events)
      end
      add_events_raw(new_events.to_i)
    end

    alias_method :set_events_raw, :set_events
    def set_events(new_events)
      unless new_events.is_a?(Gdk::EventMask)
        new_events = Gdk::EventMask.new(new_events)
      end
      set_events_raw(new_events.to_i)
    end

    alias_method :events_raw=, :events=
    alias_method :events=, :set_events

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

    alias_method :drag_source_set_raw, :drag_source_set
    def drag_source_set(flags, targets, actions)
      targets = ensure_drag_targets(targets)
      drag_source_set_raw(flags, targets, actions)
    end

    alias_method :drag_dest_set_raw, :drag_dest_set
    def drag_dest_set(flags, targets, actions)
      targets = ensure_drag_targets(targets)
      drag_dest_set_raw(flags, targets, actions)
    end

    alias_method :style_get_property_raw, :style_get_property
    def style_get_property(name)
      property = self.class.find_style_property(name)
      if property.nil?
        raise ArgumentError, "unknown style: #{name.inspect}"
      end
      value = GLib::Value.new(property.value_type)
      style_get_property_raw(name, value)
      value.value
    end

    alias_method :render_icon_pixbuf_raw, :render_icon_pixbuf
    def render_icon_pixbuf(stock_id, size)
      size = IconSize.new(size) unless size.is_a?(IconSize)
      render_icon_pixbuf_raw(stock_id, size)
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
