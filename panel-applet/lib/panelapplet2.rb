require 'gtk2'
require 'panelapplet2.so'

if PanelApplet.need_gnome?
  require 'gnome2'
end

require 'panelapplet2_main.so'
