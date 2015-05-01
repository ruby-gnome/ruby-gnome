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
      define_version_module
      setup_tree_model(repository, namespace)
      setup_pending_constants
    end

    def call_init_function(repository, namespace)
      init_check = repository.find(namespace, "init_check")
      arguments = [
        [$0] + @init_arguments,
      ]
      succeeded, argv = init_check.invoke(:arguments => arguments)
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

    def setup_tree_model(repository, namespace)
      repository.each(namespace) do |info|
        case info
        when GObjectIntrospection::InterfaceInfo
          case info.name
          when "TreeModel"
            info.gtype.to_class.__send__(:include, Enumerable)
            return
          end
        end
      end
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
      require "gtk3/about-dialog"
      require "gtk3/action"
      require "gtk3/action-group"
      require "gtk3/box"
      require "gtk3/button"
      require "gtk3/border"
      require "gtk3/builder"
      require "gtk3/calendar"
      require "gtk3/cell-layout"
      require "gtk3/color-chooser-dialog"
      require "gtk3/combo-box"
      require "gtk3/combo-box-text"
      require "gtk3/container"
      require "gtk3/css-provider"
      require "gtk3/dialog"
      require "gtk3/file-chooser-dialog"
      require "gtk3/font-chooser-dialog"
      require "gtk3/gesture-multi-press"
      require "gtk3/icon-theme"
      require "gtk3/icon-view"
      require "gtk3/image"
      require "gtk3/label"
      require "gtk3/level-bar"
      require "gtk3/list-store"
      require "gtk3/menu-item"
      require "gtk3/message-dialog"
      require "gtk3/radio-action"
      require "gtk3/radio-button"
      require "gtk3/scrolled-window"
      require "gtk3/search-bar"
      require "gtk3/spin-button"
      require "gtk3/stack"
      require "gtk3/stock"
      require "gtk3/style-context"
      require "gtk3/style-properties"
      require "gtk3/target-list"
      require "gtk3/text-buffer"
      require "gtk3/text-tag-table"
      require "gtk3/toggle-action"
      require "gtk3/toggle-button"
      require "gtk3/tool-button"
      require "gtk3/tree-iter"
      require "gtk3/tree-model"
      require "gtk3/tree-path"
      require "gtk3/tree-selection"
      require "gtk3/tree-store"
      require "gtk3/tree-view"
      require "gtk3/tree-view-column"
      require "gtk3/ui-manager"
      require "gtk3/widget"
      require "gtk3/window"
      require "gtk3/version"

      require "gtk3/deprecated"
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

    def enum_class_name(info)
      case info.name
      when /\ARc/
        "RC#{$POSTMATCH}"
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
        when "compare"
          method_name = "<=>"
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
      else
        super
      end
    end

    def initialize_post(object)
      super
      return unless object.is_a?(GLib::Object)
      self.class.reference_gobject(object, :sink => true)
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
