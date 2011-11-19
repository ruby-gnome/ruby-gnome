module Gtk
  extend GLib::Deprecatable
  define_deprecated_const :Curve, :raise => "Don't use this widget anymore."
  define_deprecated_const :GammaCurve, :raise => "Don't use this widget anymore."
  define_deprecated_const :Ruler, :raise => "Don't use this widget anymore."
  define_deprecated_const :HRuler, :raise => "Don't use this widget anymore."
  define_deprecated_const :VRuler, :raise => "Don't use this widget anymore."
  define_deprecated_const :InputDialog, :raise => "Don't use this widget anymore."
  define_deprecated_const :Combo, :raise => "Use Gtk::ComboBox instead."
  define_deprecated_const :OptionMenu, :raise => "Use Gtk::ComboBox instead."
  define_deprecated_const :ItemFactory, :raise => "Use Gtk::UIManager instead."

  class BindingSet
    extend GLib::Deprecatable
    define_deprecated_method :entry_clear, :entry_remove
  end

  class Dialog
    extend GLib::Deprecatable
    define_deprecated_method :vbox, :child
  end
end

