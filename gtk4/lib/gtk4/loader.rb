# Copyright (C) 2014-2022  Ruby-GNOME Project Team
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
    def load
      self.version = "4.0"
      super("Gtk")
    end

    private
    def pre_load(repository, namespace)
      call_init_function(repository, namespace)
      define_version_module
      define_methods_modules
      setup_pending_constants
    end

    def call_init_function(repository, namespace)
      init_check = repository.find(namespace, "init_check")
      succeeded = init_check.invoke([])
      raise InitError, "failed to initialize GTK" unless succeeded
    end

    def define_version_module
      @version_module = Module.new
      @base_module.const_set("Version", @version_module)
    end

    def define_methods_modules
      @widget_methods_module =
        define_methods_module(:WidgetMethods)
    end

    def apply_methods_modules
      apply_methods_module(@widget_methods_module,
                           @base_module::Widget)
    end

    def level_bar_class
      @level_bar_class ||= @base_module.const_get(:LevelBar)
    end

    def style_provider_module
      @style_provider_module ||= @base_module.const_get(:StyleProvider)
    end

    def post_load(repository, namespace)
      apply_pending_constants
      apply_methods_modules
      require_extension
      require_libraries
    end

    def require_extension
      require "gtk4.so"
    end

    def require_libraries
      require_relative "gdk-display"

      require_relative "about-dialog"
      require_relative "application"
      require_relative "binding-set"
      require_relative "border"
      require_relative "box"
      require_relative "button"
      require_relative "builder"
      require_relative "cell-layout"
      require_relative "color-chooser-dialog"
      require_relative "combo-box"
      require_relative "combo-box-text"
      require_relative "css-provider"
      require_relative "dialog"
      require_relative "entry"
      require_relative "entry-buffer"
      require_relative "file-chooser-dialog"
      require_relative "font-chooser-dialog"
      require_relative "gesture-multi-press"
      require_relative "icon-theme"
      require_relative "icon-view"
      require_relative "image"
      require_relative "label"
      require_relative "level-bar"
      require_relative "list-store"
      require_relative "message-dialog"
      require_relative "paned"
      require_relative "response-type"
      require_relative "scale-button"
      require_relative "scrolled-window"
      require_relative "search-bar"
      require_relative "spin-button"
      require_relative "show-uri"
      require_relative "stack"
      require_relative "style-context"
      require_relative "text-buffer"
      require_relative "text-iter"
      require_relative "text-tag"
      require_relative "text-tag-table"
      require_relative "text-view"
      require_relative "toggle-button"
      require_relative "tree-iter"
      require_relative "tree-model"
      require_relative "tree-model-filter"
      require_relative "tree-model-sort"
      require_relative "tree-path"
      require_relative "tree-selection"
      require_relative "tree-sortable"
      require_relative "tree-store"
      require_relative "tree-view"
      require_relative "tree-view-column"
      require_relative "version"
      require_relative "widget"

      require_relative "deprecated"
    end

    def load_function_info(info)
      name = info.name
      case name
      when "init", "init_check"
        # ignore
      when /\Atest_widget_/
        name = $POSTMATCH
        define_method(info, @widget_methods_module, name)
      else
        super
      end
    end

    def rubyish_method_name(function_info, options={})
      name = function_info.name
      case name
      when "list_toplevels"
        # To avoid conflict with gtk_window_get_toplevels()
        name
      else
        super
      end
    end

    def load_method_info(info, klass, method_name)
      case klass.name
      when "Gtk::Builder"
        case method_name
        when "get_type_from_name"
          method_name = "get_type"
        end
      when "Gtk::Image"
        method_name = method_name.gsub(/\Agicon/, "icon")
      when "Gtk::TreePath"
        case method_name
        when "next", "prev", "up", "down"
          method_name += "!"
        end
      when "Gtk::TreeSelection"
        case method_name
        when "selected_foreach"
          method_name = "each"
        end
      when "Gtk::TreeView"
        case method_name
        when "row_expanded"
          method_name += "?"
        end
      end
      super(info, klass, method_name)
    end

    def load_constant_info(info)
      case info.name
      when /_VERSION\z/
        @version_module.const_set($PREMATCH, info.value)
      when /\ALEVEL_BAR_/
        @pending_constants << info
      when /\ASTYLE_PROVIDER_/
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
        when /\ASTYLE_PROVIDER_/
          style_provider_module.const_set($POSTMATCH, info.value)
        end
      end
    end
  end
end
