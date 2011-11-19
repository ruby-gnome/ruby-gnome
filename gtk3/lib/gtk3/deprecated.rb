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

  class Dialog
    extend GLib::Deprecatable
    define_deprecated_method :vbox, :child
  end
end

