module Gtk
  extend GLib::Deprecatable
  define_deprecated_const :Curve, :raise => "Don't use this widget anymore."
  define_deprecated_const :GammaCurve, :raise => "Don't use this widget anymore."
  define_deprecated_const :Ruler, :raise => "Don't use this widget anymore."
  define_deprecated_const :HRuler, :raise => "Don't use this widget anymore."
  define_deprecated_const :VRuler, :raise => "Don't use this widget anymore."
  define_deprecated_const :InputDialog, :raise => "Don't use this widget anymore."
  define_deprecated_const :Combo, :raise => "Use 'Gtk::ComboBox' instead."
  define_deprecated_const :OptionMenu, :raise => "Use 'Gtk::ComboBox' instead."
  define_deprecated_const :ItemFactory, :raise => "Use 'Gtk::UIManager' instead."
  define_deprecated_const :Tooltips, :raise => "Use 'Gtk::Tooltip' API."

  class BindingSet
    extend GLib::Deprecatable
    define_deprecated_method :entry_clear, :entry_remove
  end

  class Box
    extend GLib::Deprecatable
    define_deprecated_method :pack_start_defaults, :pack_start
    define_deprecated_method :pack_end_defaults, :pack_end
  end

  class Calendar
    extend GLib::Deprecatable
    define_deprecated_method :freeze, :warn => "Don't use this method."
    define_deprecated_method :thaw, :warn => "Don't use this method."
  end

  class CellRenderer
    extend GLib::Deprecatable
    define_deprecated_method :editing_canceled, :warn => "Use '#{self}#stop_editing'." do |_self|
      _self.stop_editing(true)
    end
  end

  class CellView
    extend GLib::Deprecatable
    define_deprecated_method :cell_renderers, :warn => "Use 'Gtk::CellLayout#cells'." do |_self|
      _self.cells
    end
  end

  class ColorSelectionDialog
    alias :__initialize__ :initialize
    def initialize(*args)
      if args.size == 1 && args.first.is_a?(Hash)
        params = args.first
      else
        warn "#{caller[0]}: '#{self.class}#initialize(title)' style has been deprecated. Use '#{self.class}#initialize(options = {})' style."
        params = {:title => args.first}
      end
      __initialize__(params)
    end
  end

  class Dialog
    extend GLib::Deprecatable
    define_deprecated_method :vbox, :child

    alias :__initialize__ :initialize
    def initialize(*args)
      if args.size == 1 && args.first.is_a?(Hash)
        params = args.first
      else
        warn "#{caller[0]}: '#{self.class}#initialize(title, parent, flags, *buttons)' style has been deprecated. Use '#{self.class}#initialize(options = {})' style."
        title, parent, flags, *buttons = args
        params = {:title => title, :parent => parent, :flags => flags, :buttons => buttons}
      end
      __initialize__(params)
    end
  end

  class FileChooserDialog
    alias :__initialize__ :initialize
    def initialize(*args)
      if args.size == 1 && args.first.is_a?(Hash)
        params = args.first
      else
        warn "#{caller[0]}: '#{self.class}#initialize(title, parent, action, back, buttons)' style has been deprecated. Use '#{self.class}#initialize(options = {})' style."
        title, parent, action, back, buttons = args
        params = {:title => title, :parent => parent, :action => action, :buttons => buttons}
      end
      __initialize__(params)
    end
  end

  class FontSelectionDialog
    extend GLib::Deprecatable
    define_deprecated_method :apply_button, :raise => "Don't use this method."

    alias :__initialize__ :initialize
    def initialize(*args)
      if args.size == 1 && args.first.is_a?(Hash)
        params = args.first
      else
        warn "#{caller[0]}: '#{self.class}#initialize(title)' style has been deprecated. Use '#{self.class}#initialize(options = {})' style."
        params = {:title => args.first}
      end
      __initialize__(params)
    end
  end

  class MessageDialog
    alias :__initialize__ :initialize
    def initialize(*args)
      if args.size == 1 && args.first.is_a?(Hash)
        params = args.first
      else
        warn "#{caller[0]}: '#{self.class}#initialize(parent, flags, type, buttons_type, message)' style has been deprecated. Use '#{self.class}#initialize(options = {})' style."
        parent, flags, type, buttons_type, message = args
        params = {:parent => parent, :flags => flags, :type => type, :buttons_type => buttons_type, :message => message}
      end
      __initialize__(params)
    end
  end

  class PageSetupUnixDialog
    alias :__initialize__ :initialize
    def initialize(*args)
      if args.size == 1 && args.first.is_a?(Hash)
        params = args.first
      else
        warn "#{caller[0]}: '#{self.class}#initialize(title, parent)' style has been deprecated. Use '#{self.class}#initialize(options = {})' style."
        title, parent = args
        params = {:title => title, :parent => parent}
      end
      __initialize__(params)
    end
  end

  class Paned
    extend GLib::Deprecatable
    %w(child1 child2).product(%w(resize shrink)).each do |child, prop|
      define_deprecated_method "#{child}_#{prop}?", :warn => "Use '#{prop}' child property." do |_self|
        _self.child_get_property(_self.send(child), prop)
      end
    end
  end

  class RecentManager
    extend GLib::Deprecatable
    define_deprecated_method :set_screen, :warn => "Don't use this method."
    alias :screen= :set_screen
  end

  class TreeView
    extend GLib::Deprecatable
    define_deprecated_method :widget_to_tree_coords, :convert_widget_to_bin_window_coords
    define_deprecated_method :tree_to_widget_coords, :convert_bin_window_to_widget_coords
  end

  class TreeViewColumn
    extend GLib::Deprecatable
    define_deprecated_method :cell_renderers, :warn => "Use 'Gtk::CellLayout#cells'." do |_self|
      _self.cells
    end
  end

  class Widget
    extend GLib::Deprecatable
    define_deprecated_const :Flags, :raise => "Don't use this flags anymore."
    define_deprecated_method :flags, :raise => "Use the proper method."
    define_deprecated_method :set_flags, :warn => "Use the proper method."
    alias :flags= :set_flags
    define_deprecated_method :unset_flags, :warn => "Use the proper method."
    define_deprecated_method :no_window?, :warn => "Use '#{self}#has_window?'." do |_self|
      !_self.has_window?
    end
    define_deprecated_method :rc_style?, :has_rc_style?
    define_deprecated_method :parent_sensitive?, :warn => "Use '#{self}#sensitive?' on the parent widget." do |_self|
      _self.parent.sensitive?
    end
    define_deprecated_method :hide_all, :hide
    define_deprecated_method :colormap, :raise => "Use '#{self}#visual'."
    define_deprecated_method :set_colormap, :warn => "Use '#{self}#set_visual'."
    alias :colormap= :set_colormap
    define_deprecated_method :shape_combine_mask, :warn => "Don't use this method."
    define_deprecated_method :input_shape_combine_mask, :warn => "Don't use this method."
    define_deprecated_method :reset_shapes, :warn => "Don't use this method."
    define_deprecated_method :set_scroll_adjustments, :warn => "Use 'Gtk::Scrollable#set_hadjustment' and 'Gtk::Scrollable#set_vadjustment'." do |_self, hadj, vadj|
      _self.set_hadjustment(hadj)
      _self.set_vadjustment(vadj)
      true
    end
    alias :set_scroll_adjustment :set_scroll_adjustments
    define_deprecated_method :action, :warn => "Use 'Gtk::Activatable#related_action'." do |_self|
      _self.related_action
    end

    alias :__set_allocation__ :set_allocation
    private :__set_allocation__
    def set_allocation(*args)
      case args.size
      when 1
        __set_allocation__(args.first)
      when 4
        warn "#{caller[0]}: '#{self.class}#set_allocation(x, y, width, height)' style has been deprecated. Use '#{self.class}#set_allocation(alloc)' style."
        __set_allocation__(Gtk::Allocation.new(*args))
      else
        raise ArgumentError.new("need 1 or 4 arguments.")
      end
    end
  end
end

