=begin
  cursor.rb - Gdk::Cursor sample script.

  Copyright (C) 2001,2002 Masao Mutoh<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME.

  $Date: 2002/08/18 06:28:32 $
  $Id: cursor.rb,v 1.4 2002/08/18 06:28:32 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new
window.realize

button = Gtk::Button.new("Click!")

cursors = ["X_CURSOR", "ARROW", "BASED_ARROW_DOWN", "BASED_ARROW_UP", 
  "BOAT", "BOGOSITY", "BOTTOM_LEFT_CORNER", "BOTTOM_RIGHT_CORNER", 
  "BOTTOM_SIDE", "BOTTOM_TEE", "BOX_SPIRAL", "CENTER_PTR", "CIRCLE", 
  "CLOCK", "COFFEE_MUG", "CROSS", "CROSSHAIR", "CROSS_REVERSE", 
  "DIAMOND_CROSS", "DOT", "DOTBOX", "DOUBLE_ARROW", "DRAFT_LARGE", 
  "DRAFT_SMALL", "DRAPED_BOX", "EXCHANGE", "FLEUR", "GOBBLER", "GUMBY", 
  "HAND1", "HAND2", "HEART", "ICON", "IRON_CROSS", "LEFTBUTTON", 
  "LEFT_PTR", "LEFT_SIDE", "LEFT_TEE", "LL_ANGLE", "LR_ANGLE", "MAN", 
  "MIDDLEBUTTON", "MOUSE", "PENCIL", "PIRATE", "PLUS", "QUESTION_ARROW", 
  "RIGHTBUTTON", "RIGHT_PTR", "RIGHT_SIDE", "RIGHT_TEE", "RTL_LOGO", 
  "SAILBOAT", "SB_DOWN_ARROW", "SB_H_DOUBLE_ARROW", "SB_LEFT_ARROW", 
  "SB_RIGHT_ARROW", "SB_UP_ARROW", "SB_V_DOUBLE_ARROW", "SHUTTLE", 
  "SIZING", "SPIDER", "SPRAYCAN", "STAR", "TARGET", "TCROSS", 
  "TOP_LEFT_ARROW", "TOP_LEFT_CORNER", "TOP_RIGHT_CORNER", "TOP_SIDE", 
  "TOP_TEE", "TREK", "UL_ANGLE", "UMBRELLA", "UR_ANGLE", "WATCH", "XTERM"]

cnt = 0
button.signal_connect(Gtk::Button::SIGNAL_CLICKED) do
  cursor = eval("Gdk::Cursor::" + cursors[cnt])
  button.child.set_text(cnt.to_s + ":" + cursors[cnt])
  window.window.set_cursor(Gdk::Cursor.new(cursor))
  cnt += 1
  cnt = 0 if cnt == cursors.size
end
window.add(button)
window.set_usize(200,100).show_all

Gtk.main
