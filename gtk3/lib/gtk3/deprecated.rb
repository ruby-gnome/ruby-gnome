module Gtk
  extend GLib::Deprecatable
  define_deprecated_const :Curve, :raise => "Don't use this widget anymore."
  define_deprecated_const :GammaCurve, :raise => "Don't use this widget anymore."
  define_deprecated_const :Ruler, :raise => "Don't use this widget anymore."
  define_deprecated_const :HRuler, :raise => "Don't use this widget anymore."
  define_deprecated_const :VRuler, :raise => "Don't use this widget anymore."
  define_deprecated_const :InputDialog, :raise => "Don't use this widget anymore."
  define_deprecated_const :Combo, :raise => "Use 'Gtk::ComboBoxText' instead."
  define_deprecated_const :ComboBoxEntry, :raise => "Use 'Gtk::ComboBox' instead."
  define_deprecated_const :OptionMenu, :raise => "Use 'Gtk::ComboBox' instead."
  define_deprecated_const :ItemFactory, :raise => "Use 'Gtk::UIManager' instead."
  define_deprecated_const :Tooltips, :raise => "Use 'Gtk::Tooltip' API."
  define_deprecated_const :FileSelection, :raise => "Use 'Gtk::FileChooserDialog' instead."
  define_deprecated_const :UPDATE_CONTINUOUS, :warn => "Don't use this constant anymore."
  define_deprecated_const :UPDATE_DISCONTINUOUS, :warn => "Don't use this constant anymore."
  define_deprecated_const :UPDATE_DELAYED, :warn => "Don't use this constant anymore."

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
    extend GLib::Deprecatable
    define_deprecated_method :colorsel, :color_selection
    define_deprecated_method_by_hash_args :initialize, 'title' do |_self, title|
      {:title => title}
    end
  end

  class ComboBox
    extend GLib::Deprecatable
    define_deprecated_method :append_text, :raise => "Use 'Gtk::ComboBoxText#append_text'."
    define_deprecated_method :insert_text, :raise => "Use 'Gtk::ComboBoxText#insert_text'."
    define_deprecated_method :prepend_text, :raise => "Use 'Gtk::ComboBoxText#prepend_text'."
    define_deprecated_method :remove_text, :raise => "Use 'Gtk::ComboBoxText#remove'."
    define_deprecated_method :active_text, :raise => "Use 'Gtk::ComboBoxText#active_text'."
    define_deprecated_method_by_hash_args :initialize, 'model' do |_self, model|
      case model
      when TreeModel
        {:model => model}
      when true, false
        raise GLib::DeprecatedError.new("#{caller[0]}: '#{self}#initialize(is_text_only)' style has been deprecated. Use 'Gtk::ComboBoxText'.")
      end
    end
  end

  class Dialog
    extend GLib::Deprecatable
    define_deprecated_method :vbox, :child
    define_deprecated_method_by_hash_args :initialize, 'title, parent, flags, *buttons' do |_self, title, parent, flags, *buttons|
      {:title => title, :parent => parent, :flags => flags, :buttons => buttons}
    end
  end

  class FileChooserDialog
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize, 'title, parent, action, back, *buttons' do |_self, title, parent, action, back, *buttons|
      {:title => title, :parent => parent, :action => action, :buttons => buttons}
    end
  end

  class FontSelectionDialog
    extend GLib::Deprecatable
    define_deprecated_method :apply_button, :raise => "Don't use this method."
    define_deprecated_method_by_hash_args :initialize, 'title' do |_self, title|
      {:title => title}
    end
  end

  class MessageDialog
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize, 'parent, flags, type, buttons_type, message' do |_self, parent, flags, type, buttons_type, message|
      {:parent => parent, :flags => flags, :type => type, :buttons_type => buttons_type, :message => message}
    end
  end

  class Notebook
    extend GLib::Deprecatable
    define_deprecated_singleton_method :set_window_creation_hook, :warn => "Use 'create-window' signal."
    define_deprecated_method :query_tab_label_packing, :warn => "Use 'tab-expand' and 'tab-fill' child property." do |_self, child|
      [_self.child_get_property(child, 'tab-expand'), _self.child_get_property(child, 'tab-fill')]
    end
    define_deprecated_method :set_tab_label_packing, :warn => "Use 'tab-expand' and 'tab-fill' child property." do |_self, child, expand, fill, pack_type|
      _self.child_set_property(child, 'tab-expand', expand)
      _self.child_set_property(child, 'tab-fill', fill)
    end
  end

  class PageSetupUnixDialog
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize, 'title, parent' do |_self, title, parent|
      {:title => title, :parent => parent}
    end
  end

  class PrintUnixDialog
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize, 'title, parent' do |_self, title, parent|
      {:title => title, :parent => parent}
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

  class Range
    extend GLib::Deprecatable
    define_deprecated_method :update_policy, :raise => "Don't use this method."
    define_deprecated_method :set_update_policy, :warn => "Don't use this method."
    alias :update_policy= :set_update_policy
  end

  class RecentChooserDialog
    extend GLib::Deprecatable
    define_deprecated_method_by_hash_args :initialize, 'title, parent, manager, *buttons' do |_self, title, parent, *buttons|
      manager = buttons.first.is_a?(RecentManager) ? buttons.shift : nil
      {:title => title, :parent => parent, :manager => manager, :buttons => buttons}
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

