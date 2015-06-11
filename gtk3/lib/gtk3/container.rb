# Copyright (C) 2014-2015  Ruby-GNOME2 Project Team
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
  class Container
    class << self
      def init
      end

      alias_method :child_properties_raw, :child_properties
      def child_properties
        child_properties_raw[0]
      end

      @have_template = false
      def have_template?
        @have_template
      end

      if method_defined?(:set_template)
        alias_method :set_template_raw, :set_template
      end
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
        define_method(name) do
          get_template_child(self.class.gtype, name)
        end
      end
    end

    include Enumerable

    def <<(widget)
      add(widget)
      self
    end

    alias_method :focus_chain_raw, :focus_chain
    def focus_chain
      set_explicitly, widgets = focus_chain_raw
      if set_explicitly
        widgets
      else
        nil
      end
    end

    alias_method :child_get_property_raw, :child_get_property
    def child_get_property(child, name)
      property = self.class.find_child_property(name)
      value = GLib::Value.new(property.value_type)
      child_get_property_raw(child, name, value)
      value.value
    end

    private
    def initialize_post
      return unless self.class.have_template?
      return unless respond_to?(:init_template)
      init_template
    end
  end
end
