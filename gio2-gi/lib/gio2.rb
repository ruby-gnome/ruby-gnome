# Copyright (C) 2013-2014  Ruby-GNOME2 Project Team
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

require "gobject-introspection"
require "glib2"

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)

require "gio2/deprecated"

module Gio
  LOG_DOMAIN = "Gio"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  class << self
    def const_missing(name)
      init
      if const_defined?(name)
        const_get(name)
      else
        super
      end
    end

    def init
      loader = Loader.new(self)
      loader.load("Gio")
      class << self
        remove_method(:init)
        remove_method(:const_missing)
      end
    end
  end

  class Loader < GObjectIntrospection::Loader
    private
    def pre_load(repository, namespace)
      define_content_type_class
      define_mime_type_class
      @content_type_guess_for_tree_info = nil
    end

    def post_load(repository, namespace)
      if @content_type_guess_for_tree_info
        content_type_class = @content_type_class
        info = @content_type_guess_for_tree_info
        file_module = @base_module.const_get("File")
        file_module.__send__(:define_method, "guess_content_types") do
          info.invoke(:arguments => [self]).collect do |type|
            content_type_class.new(type)
          end
        end
      end
    end

    def define_content_type_class
      @content_type_class = Class.new do
        def initialize(type)
          @type = type
        end

        def to_s
          @type
        end
      end
      @base_module.const_set("ContentType", @content_type_class)
    end

    def define_mime_type_class
      @mime_type_class = Class.new do
        def initialize(type)
          @type = type
        end

        def to_s
          @type
        end
      end
      @base_module.const_set("MimeType", @mime_type_class)
    end

    def load_function_info(info)
      name = info.name
      case name
      when "init"
        # ignore
      when /\Acontent_type_/
        load_function_info_content_type(info)
      when "content_types_get_registered"
        @content_type_class.define_singleton_method(:registered) do
          info.invoke(:arguments => []).collect do |type|
            new(type)
          end
        end
      else
        super
      end
    end

    def load_function_info_content_type(info)
      name = info.name.gsub(/\Acontent_type_/, "")
      case name
      when "equals", "is_a"
        case name
        when "equals"
          method_name = "=="
        when "is_a"
          method_name = "#{name}?"
        end
        @content_type_class.__send__(:define_method, method_name) do |other|
          if other.is_a?(self.class)
            other = other.to_s
          end
          if other.is_a?(String)
            info.invoke(:arguments => [to_s, other])
          else
            false
          end
        end
      when "from_mime_type"
        @mime_type_class.__send__(:define_method, "content_type") do
          info.invoke(:arguments => [to_s])
        end
      when "get_mime_type"
        mime_type_class = @mime_type_class
        @content_type_class.__send__(:define_method, "mime_type") do
          mime_type = info.invoke(:arguments => [to_s])
          if mime_type
            mime_type_class.new(mime_type)
          else
            nil
          end
        end
      when "guess_for_tree"
        @content_type_guess_for_tree_info = info
      when "guess"
        vlaidate = lambda do |arguments|
          method_name = "#{@content_type_class}.#{name}"
          validate_arguments(info, method_name, arguments)
        end
        @content_type_class.define_singleton_method(:guess) do |*arguments|
          validate.call(arguments)
          info.invoke(:arguments => arguments)
        end
      else
        case name
        when /\Acan_be_/
          method_name = "#{$1}?"
        when /\Ais_/
          method_name = "#{$POSTMATCH}?"
        when /\Aget_/
          method_name = $POSTMATCH
        else
          method_name = name
        end
        @content_type_class.__send__(:define_method, method_name) do
          info.invoke(:arguments => [to_s])
        end
      end
    end

    def load_function_infos(infos, klass)
      case klass.name
      when "Gio::File"
        load_function_infos_file(infos, klass)
        super
      else
        super
      end
    end

    def load_function_infos_file(infos, klass)
      new_for_commandline_arg_infos = []

      infos.each do |info|
        name = info.name
        case name
        when /\Anew_for_commandline_arg/
          new_for_commandline_arg_infos << info
        when /\Anew_(?:for_)?/
          method_name = $POSTMATCH
          define_singleton_method(klass, method_name, info)
        else
          define_singleton_method(klass, name, info)
        end
      end

      define_file_commandline_arg(new_for_commandline_arg_infos, klass)
    end

    def define_file_commandline_arg(infos, klass)
      method_name = "commandline_arg"
      find_info = lambda do |arguments|
        find_suitable_callable_info(infos, arguments)
      end
      validate = lambda do |info, arguments|
        validate_arguments(info, "#{klass}.#{method_name}", arguments)
      end
      klass.__send__(:define_method, method_name) do |*arguments|
        info = find_info.call(arguments)
        validate.call(info, arguments)
        info.invoke(:arguments => arguments)
      end
    end
  end
end
