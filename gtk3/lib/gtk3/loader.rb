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
  class Loader < GObjectIntrospection::Loader
    def initialize(base_module, init_arguments)
      super(base_module)
      @init_arguments = init_arguments
    end

    private
    def pre_load(repository, namespace)
      call_init_function(repository, namespace)
      define_stock_module
      setup_pending_constants
    end

    def call_init_function(repository, namespace)
      init_check = repository.find(namespace, "init_check")
      arguments = [
        [$0] + @init_arguments,
      ]
      succeeded, argv, error = init_check.invoke(:arguments => arguments)
      @init_arguments.replace(argv[1..-1])
      raise error unless succeeded
    end

    def define_stock_module
      @stock_module = Module.new
      @base_module.const_set("Stock", @stock_module)
    end

    def level_bar_class
      @level_bar_class ||= @base_module.const_get(:LevelBar)
    end

    def post_load(repository, namespace)
      apply_pending_constants
      require_extension
      require_libraries
    end

    def require_extension
      begin
        major, minor, _ = RUBY_VERSION.split(/\./)
        require "#{major}.#{minor}/gtk3.so"
      rescue LoadError
        require "gtk3.so"
      end
    end

    def require_libraries
      require "gtk3/box"
      require "gtk3/button"
      require "gtk3/border"
      require "gtk3/builder"
      require "gtk3/container"
      require "gtk3/css-provider"
      require "gtk3/gtk"
      require "gtk3/icon-theme"
      require "gtk3/image"
      require "gtk3/label"
      require "gtk3/level-bar"
      require "gtk3/scrolled-window"
      require "gtk3/search-bar"
      require "gtk3/spin-button"
      require "gtk3/stack"
      require "gtk3/style-properties"
      require "gtk3/text-buffer"
      require "gtk3/tree-iter"
      require "gtk3/tree-selection"
      require "gtk3/tree-store"
      require "gtk3/tree-view-column"
      require "gtk3/window"

      require "gtk3/deprecated"
    end

    def rubyish_method_name(function_info, options={})
      name = super
      case name
      when "forall"
        "each_all"
      else
        name
      end
    end

    def load_function_info(info)
      name = info.name
      case name
      when "init", /_get_type\z/
        # ignore
      else
        super
      end
    end

    def define_enum(info)
      case info.name
      when /\AArrow/
        self.class.define_class(info.gtype, $POSTMATCH, Gtk::Arrow)
      when /\ALevelBar/
        self.class.define_class(info.gtype, $POSTMATCH, Gtk::LevelBar)
      when /\ARevealer/
        self.class.define_class(info.gtype, $POSTMATCH, Gtk::Revealer)
      when /\AStack/
        self.class.define_class(info.gtype, $POSTMATCH, Gtk::Stack)
      else
        super
      end
    end

    def load_method_info(info, klass, method_name)
      if klass.name == "Gtk::Image"
        method_name = method_name.gsub(/\Agicon/, "icon")
      end
      super(info, klass, method_name)
    end

    def load_constant_info(info)
      case info.name
      when /\ASTOCK_/
        @stock_module.const_set($POSTMATCH, info.value)
      when /\ALEVEL_BAR_/
        @pending_constants << info
      else
        super
      end
    end

    def setup_pending_constants
      @pending_constants = []
    end

    def apply_pending_constants
      @pending_constants.each do |info|
        case info.name
        when /\ALEVEL_BAR_/
          level_bar_class.const_set($POSTMATCH, info.value)
        end
      end
    end
  end
end
