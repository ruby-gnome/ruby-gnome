module Gdk
  extend GLib::Deprecatable
  define_deprecated_const :Pixmap, :raise => "Use Cairo::Surface instead."

  module Keyval
    extend GLib::Deprecatable
    constants.each do |key|
      define_deprecated_const key.to_s.sub(/^GDK_KEY_/, 'GDK_'), [self, key].join('::')
    end
  end
end

