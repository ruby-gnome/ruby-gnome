module Gtk
  extend GLib::Deprecatable
  define_deprecated_const :Curve, :raise => "Don't use this widget anymore."
  define_deprecated_const :GammaCurve, :raise => "Don't use this widget anymore."
  define_deprecated_const :Ruler, :raise => "Don't use this widget anymore."
  define_deprecated_const :HRuler, :raise => "Don't use this widget anymore."
  define_deprecated_const :VRuler, :raise => "Don't use this widget anymore."

  class BindingSet
    extend GLib::Deprecatable
    define_deprecated_method :entry_clear, :entry_remove
  end

  class Dialog
    extend GLib::Deprecatable
    define_deprecated_method :vbox, :child
  end
end

