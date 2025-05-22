# Copyright (C) 2014-2025  Ruby-GNOME Project Team
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

    def load
      self.version = "3.0"
      super("Gtk")
    end

    private
    def pre_load(repository, namespace)
      call_init_function(repository, namespace)
      define_stock_module
      define_version_module
      define_methods_modules
      setup_pending_constants
    end

    def call_init_function(repository, namespace)
      init_check = repository.find(namespace, "init_check")
      arguments = [
        [$0] + @init_arguments,
      ]
      succeeded, argv = init_check.invoke(arguments)
      @init_arguments.replace(argv[1..-1])
      raise InitError, "failed to initialize GTK+" unless succeeded
    end

    def define_stock_module
      @stock_module = Module.new
      @base_module.const_set("Stock", @stock_module)
    end

    def define_version_module
      @version_module = Module.new
      @base_module.const_set("Version", @version_module)
    end

    def define_methods_modules
      @stock_singleton_methods_module =
        define_methods_module(:StockSingletonMethods)
      @widget_methods_module =
        define_methods_module(:WidgetMethods)
      @gdk_drag_context_methods_module =
        define_methods_module(:GdkDragContextMethods)
      @icon_size_class_methods_module =
        define_methods_module(:IconSizeClassMethods)
      @accel_group_class_methods_module =
        define_methods_module(:AccelGroupClassMethods)
      @gdk_event_methods_module =
        define_methods_module(:GdkEventMethods)
    end

    def apply_methods_modules
      apply_methods_module(@stock_singleton_methods_module,
                           @base_module::Stock.singleton_class)
      apply_methods_module(@widget_methods_module,
                           @base_module::Widget)
      apply_methods_module(@gdk_drag_context_methods_module,
                           Gdk::DragContext)
      apply_methods_module(@icon_size_class_methods_module,
                           @base_module::IconSize.singleton_class)
      apply_methods_module(@accel_group_class_methods_module,
                           @base_module::AccelGroup.singleton_class)
      apply_methods_module(@gdk_event_methods_module,
                           Gdk::Event)
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
      require "gtk3.so"
    end

    def require_libraries
      require_relative "gdk-drag-context"
      require_relative "gdk-screen"

      require_relative "about-dialog"
      require_relative "action"
      require_relative "action-group"
      require_relative "application"
      require_relative "binding-set"
      require_relative "border"
      require_relative "box"
      require_relative "button"
      require_relative "builder"
      require_relative "calendar"
      require_relative "cell-layout"
      require_relative "check-menu-item"
      require_relative "clipboard"
      require_relative "color-chooser-dialog"
      require_relative "combo-box"
      require_relative "combo-box-text"
      require_relative "container"
      require_relative "css-provider"
      require_relative "dialog"
      require_relative "entry"
      require_relative "entry-buffer"
      require_relative "file-chooser-dialog"
      require_relative "font-chooser-dialog"
      require_relative "gesture-multi-press"
      require_relative "icon-size"
      require_relative "icon-theme"
      require_relative "icon-view"
      require_relative "image"
      require_relative "image-menu-item"
      require_relative "label"
      require_relative "level-bar"
      require_relative "list-store"
      require_relative "menu-item"
      require_relative "message-dialog"
      require_relative "paned"
      require_relative "popover"
      require_relative "radio-action"
      require_relative "radio-button"
      require_relative "recent-chooser-dialog"
      require_relative "response-type"
      require_relative "scale-button"
      require_relative "scrolled-window"
      require_relative "search-bar"
      require_relative "selection-data"
      require_relative "spin-button"
      require_relative "show-uri"
      require_relative "stack"
      require_relative "stock"
      require_relative "stock-item"
      require_relative "style-context"
      require_relative "style-properties"
      require_relative "table"
      require_relative "target-list"
      require_relative "target-entry"
      require_relative "text-buffer"
      require_relative "text-iter"
      require_relative "text-tag"
      require_relative "text-tag-table"
      require_relative "text-view"
      require_relative "toggle-action"
      require_relative "toggle-button"
      require_relative "tool-button"
      require_relative "tree-iter"
      require_relative "tree-model"
      require_relative "tree-model-filter"
      require_relative "tree-model-sort"
      require_relative "tree-path"
      require_relative "tree-selection"
      require_relative "tree-store"
      require_relative "tree-view"
      require_relative "tree-view-column"
      require_relative "ui-manager"
      require_relative "version"
      require_relative "widget"
      require_relative "window"

      require_relative "deprecated"
    end

    def rubyish_method_name(function_info, options={})
      name = function_info.name
      case name
      when /\Alist_(child_properties|style_properties)\z/
        $1
      else
        super
      end
    end

    def load_function_info(info)
      name = info.name
      case name
      when "init", /_get_type\z/
        # ignore
      when /\Astock_/
        method_name = rubyish_method_name(info, :prefix => "stock_")
        define_module_function(info,
                               @stock_singleton_methods_module,
                               method_name)
      when /\Adrag_(?:source_|dest_|get_data\z|(?:un)highlight\z|begin|check_threshold\z)/
        # For OS X. It may be broken. It's not tested.
        method_name = rubyish_method_name(info)
        define_method(info, @widget_methods_module, method_name)
      when /\Adrag_/
        method_name = rubyish_method_name(info,
                                          :prefix => "drag_",
                                          :n_in_args_offset => -1)
        define_method(info, @gdk_drag_context_methods_module, method_name)
      when /\Abinding_/
        # Ignore because singleton methods are defined.
      when /\Aicon_size_/
        method_name = rubyish_method_name(info, :prefix => "icon_size_")
        define_module_function(info,
                               @icon_size_class_methods_module,
                               method_name)
      when /\Atest_widget_/
        name = $POSTMATCH
        define_method(info, @widget_methods_module, name)
      when /\Aaccel_groups_/
        method_name = rubyish_method_name(info, :prefix => "accel_groups_")
        define_module_function(info,
                               @accel_group_class_methods_module,
                               method_name)
      else
        super
      end
    end

    def define_singleton_method(info, klass, name)
      case klass.name
      when "Gtk"
        case name
        when "get_event_widget"
          define_method(info, @gdk_event_methods_module, "widget")
          return
        when "events_pending"
          name = "events_pending?"
        end
      when "Gtk::AccelMap"
        case name
        when "each", "foreach_unfiltered"
          # Implemented in C
          return
        end
      when "Gtk::Widget"
        case name
        when "default_style"
          # Ignore deprecated method
          return
        end
      end

      super(info, klass, name)
    end

    def define_enum(info)
      case info.name
      when /\AArrow/
        self.class.define_class(info.gtype, $POSTMATCH, Arrow)
      when /\ALevelBar/
        self.class.define_class(info.gtype, $POSTMATCH, LevelBar)
      when /\ARevealer/
        self.class.define_class(info.gtype, $POSTMATCH, Revealer)
      when /\AStack/
        self.class.define_class(info.gtype, $POSTMATCH, Stack)
      else
        super
      end
    end

    def rubyish_class_name(info)
      case info
      when GObjectIntrospection::EnumInfo
        case info.name
        when /\ARc/
          "RC#{$POSTMATCH}"
        else
          super
        end
      else
        super
      end
    end

    def flags_class_name(info)
      case info.name
      when /\ARc/
        "RC#{$POSTMATCH}"
      else
        super
      end
    end

    def field_name(field_info, klass)
      case klass.name
      when "Gtk::AccelKey"
        super.gsub(/\Aaccel_/, "")
      else
        super
      end
    end

    def load_method_info(info, klass, method_name)
      case klass.name
      when "Gtk::Assistant"
        case method_name
        when /\A(?:set|get)_page_(?:header|side)_image\z/
          # Ignore deprecated methods
          return
        end
      when "Gtk::Buildable"
        case method_name
        when "name"
          method_name = "builder_name"
        when "set_name"
          method_name = "set_builder_name"
        end
      when "Gtk::Builder"
        case method_name
        when "connect_signals"
          return
        when "connect_signals_full"
          method_name = "connect_signals"
        when "get_type_from_name"
          method_name = "get_type"
        end
      when "Gtk::BindingSet"
        case method_name
        when "add_path"
          # Ignore deprecated methods
          return
        end
      when "Gtk::Button"
        case method_name
        when "enter", "leave", "pressed", "released"
          # Ignore deprecated methods
          return
        end
      when "Gtk::CellRenderer"
        case method_name
        when "get_size"
          # Ignore deprecated methods
          return
        end
      when "Gtk::CellView"
        case method_name
        when "get_size_of_row"
          # Ignore deprecated methods
          return
        end
      when "Gtk::Container"
        case method_name
        when "forall"
          method_name = "each_all"
        end
      when "Gtk::IconSet"
        case method_name
        when "render_icon"
          # Ignore deprecated methods
          return
        end
      when "Gtk::IconTheme"
        case method_name
        when "list_icons"
          method_name = "icons"
        end
      when "Gtk::Image"
        method_name = method_name.gsub(/\Agicon/, "icon")
      when "Gtk::Tooltip"
        case method_name
        when "set_icon_from_stock"
          # Ignore deprecated methods
          return
        end
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
      when "Gtk::Widget"
        case info.name
        when "size_request",
             "get_child_requisition",
             "ensure_style",
             "reset_rc_styles",
             "class_path",
             "modify_style",
             "get_modifier_style",
             "modify_fg",
             "modify_bg",
             "modify_text",
             "modify_base",
             "modify_font",
             "modify_cursor",
             "render_icon",
             "get_state",
             "has_rc_style",
             "get_requisition",
             "get_pointer"
          # Ignore deprecated method
          return
        end
      end
      super(info, klass, method_name)
    end

    def load_constant_info(info)
      case info.name
      when /\ASTOCK_/
        @stock_module.const_set($POSTMATCH, info.value)
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
