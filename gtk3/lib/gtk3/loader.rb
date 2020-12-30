# Copyright (C) 2014-2020  Ruby-GNOME Project Team
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

    def level_bar_class
      @level_bar_class ||= @base_module.const_get(:LevelBar)
    end

    def style_provider_module
      @style_provider_module ||= @base_module.const_get(:StyleProvider)
    end

    def post_load(repository, namespace)
      apply_pending_constants
      require_extension
      require_libraries
    end

    def require_extension
      require "gtk3.so"
    end

    def require_libraries
      require "gtk3/gdk-drag-context"
      require "gtk3/gdk-screen"

      require "gtk3/about-dialog"
      require "gtk3/action"
      require "gtk3/action-group"
      require "gtk3/application"
      require "gtk3/binding-set"
      require "gtk3/border"
      require "gtk3/box"
      require "gtk3/button"
      require "gtk3/builder"
      require "gtk3/calendar"
      require "gtk3/cell-layout"
      require "gtk3/check-menu-item"
      require "gtk3/clipboard"
      require "gtk3/color-chooser-dialog"
      require "gtk3/combo-box"
      require "gtk3/combo-box-text"
      require "gtk3/container"
      require "gtk3/css-provider"
      require "gtk3/dialog"
      require "gtk3/entry"
      require "gtk3/entry-buffer"
      require "gtk3/file-chooser-dialog"
      require "gtk3/font-chooser-dialog"
      require "gtk3/gesture-multi-press"
      require "gtk3/icon-size"
      require "gtk3/icon-theme"
      require "gtk3/icon-view"
      require "gtk3/image"
      require "gtk3/image-menu-item"
      require "gtk3/label"
      require "gtk3/level-bar"
      require "gtk3/list-store"
      require "gtk3/menu-item"
      require "gtk3/message-dialog"
      require "gtk3/paned"
      require "gtk3/radio-action"
      require "gtk3/radio-button"
      require "gtk3/recent-chooser-dialog"
      require "gtk3/response-type"
      require "gtk3/scale-button"
      require "gtk3/scrolled-window"
      require "gtk3/search-bar"
      require "gtk3/selection-data"
      require "gtk3/spin-button"
      require "gtk3/show-uri"
      require "gtk3/stack"
      require "gtk3/stock"
      require "gtk3/stock-item"
      require "gtk3/style-context"
      require "gtk3/style-properties"
      require "gtk3/table"
      require "gtk3/target-list"
      require "gtk3/target-entry"
      require "gtk3/text-buffer"
      require "gtk3/text-iter"
      require "gtk3/text-tag"
      require "gtk3/text-tag-table"
      require "gtk3/text-view"
      require "gtk3/toggle-action"
      require "gtk3/toggle-button"
      require "gtk3/tool-button"
      require "gtk3/tree-iter"
      require "gtk3/tree-model"
      require "gtk3/tree-model-filter"
      require "gtk3/tree-model-sort"
      require "gtk3/tree-path"
      require "gtk3/tree-selection"
      require "gtk3/tree-store"
      require "gtk3/tree-view"
      require "gtk3/tree-view-column"
      require "gtk3/ui-manager"
      require "gtk3/version"
      require "gtk3/widget"
      require "gtk3/window"

      require "gtk3/deprecated"
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
        stock_module = @base_module.const_get(:Stock)
        method_name = rubyish_method_name(info, :prefix => "stock_")
        define_singleton_method(stock_module, method_name, info)
      when /\Adrag_(?:source_|dest_|get_data\z|(?:un)highlight\z|begin|check_threshold\z)/
        # For OS X. It may be broken. It's not tested.
        widget_class = @base_module.const_get(:Widget)
        method_name = rubyish_method_name(info)
        define_method(info, widget_class, method_name)
      when /\Adrag_/
        method_name = rubyish_method_name(info,
                                          :prefix => "drag_",
                                          :n_in_args_offset => -1)
        define_method(info, Gdk::DragContext, method_name)
      when /\Abinding_/
        # Ignore because singleton methods are defined.
      when /\Aicon_size_/
        icon_size_class = @base_module.const_get(:IconSize)
        method_name = rubyish_method_name(info, :prefix => "icon_size_")
        define_singleton_method(icon_size_class, method_name, info)
      when /\Atest_widget_/
        name = $POSTMATCH
        define_method(info, Gtk::Widget, name)
      when /\Aaccel_groups_/
        accel_group_class = @base_module.const_get(:AccelGroup)
        method_name = rubyish_method_name(info, :prefix => "accel_groups_")
        define_singleton_method(accel_group_class, method_name, info)
      else
        super
      end
    end

    def define_singleton_method(klass, name, info)
      case klass.name
      when "Gtk"
        case name
        when "get_event_widget"
          define_gdk_event_widget(info)
          return
        when "events_pending"
          name = "events_pending?"
        end
      when "Gtk::Widget"
        case name
        when "default_style"
          # Ignore deprecated method
          return
        end
      end

      super(klass, name, info)
    end

    def define_gdk_event_widget(info)
      define_method(info, Gdk::Event, "widget")
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
