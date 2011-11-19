module Gtk
  class BindingSet
    extend GLib::Deprecatable
    define_deprecated_method :entry_clear, :entry_remove
  end

  class Dialog
    extend GLib::Deprecatable
    define_deprecated_method :vbox, :child
  end
end

