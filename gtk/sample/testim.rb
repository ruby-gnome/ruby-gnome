#
#  testim.rb - a sample script for input method
#    written by TAKAHASHI Hitoshi <thitoshi@ne.scphys.kyoto-u.ac.jp>
#

require 'gtk2'


$text = ""
$pixmap = nil
$ic = nil

DrawWidth = 200
DrawHeight = 100
TextBase = 57


def create_ic(widget)
  if ! Gdk::IM.ready? || ! (attr = Gdk::ICAttr.new)
    STDERR << "Cannot use Input Method!\n"
    return
  end
  attrmask = Gdk::IC::ALL_REQ
  supported_style = Gdk::IM::PREEDIT_NONE | Gdk::IM::PREEDIT_NOTHING |
    Gdk::IM::PREEDIT_POSITION | Gdk::IM::STATUS_NONE |
    Gdk::IM::STATUS_NOTHING
  if widget.style.font.font_type != Gdk::FONT_FONTSET
    supported_style &= ~Gdk::IM::PREEDIT_POSITION
  end
  attr.style = Gdk::IM.decide_style(supported_style)
  attr.client_window = widget.window
  colormap = widget.get_colormap
  if colormap != Gtk::Widget.get_default_colormap
    attrmask |= Gdk::IC::PREEDIT_COLORMAP
    attr.preedit_colormap = colormap
  end
  if attr.style & Gdk::IM::PREEDIT_POSITION
    attrmask |= Gdk::IC::PREEDIT_POSITION_REQ
    attr.preedit_area = Gdk::Rectangle.new(0, 0, DrawWidth, DrawHeight)
    attr.preedit_fontset = widget.style.font
  end
  $ic = Gdk::IC.new(attr, attrmask)
  if ! $ic
    STDERR << "Cannot create Input Context!\n"
  else
    mask = widget.window.get_events
    mask |= $ic.get_events
    widget.window.set_events mask
  end
end


def set_preedit_area(widget)
  if $ic && ($ic.get_style & Gdk::IM::PREEDIT_POSITION)
    attr = $ic.get_attr(Gdk::IC::PREEDIT_AREA_REQ)
    width, height = widget.window.get_size
    attr.preedit_area = Gdk::Rectangle.new(0, 0, width, height)
    attr.preedit_fontset = widget.style.font
    $ic.set_attr attr, Gdk::IC::PREEDIT_AREA_REQ
  end
end


def set_spot_location(widget)
  if $ic && ($ic.get_style & Gdk::IM::PREEDIT_POSITION)
    attr = $ic.get_attr(Gdk::IC::SPOT_LOCATION)
    attr.spot_location.x = widget.style.font.string_width($text)
    attr.spot_location.y = TextBase
    $ic.set_attr attr, Gdk::IC::SPOT_LOCATION
  end
end


def input_character(widget, event)
  if event.length > 0
    $text << event.string
    draw_pixmap widget
    draw_window widget, 0, 0, DrawWidth, DrawHeight
    set_spot_location widget
  end
end


def draw_pixmap(widget)
  $pixmap.draw_rectangle widget.style.white_gc, true,
    0, 0, DrawWidth, DrawHeight
  $pixmap.draw_text widget.style.font, widget.style.black_gc,
    0, TextBase, $text
end


def draw_window(widget, x, y, width, height)
  widget.window.draw_pixmap widget.style.fg_gc(widget.state), $pixmap,
    x, y, x, y, width, height
end


window = Gtk::Window.new(Gtk::WINDOW_TOPLEVEL)
window.set_title "IM test"

vbox = Gtk::VBox.new(false, 0)
vbox.show
window.add vbox

draw = Gtk::DrawingArea.new
draw.size DrawWidth, DrawHeight
draw.set_flags Gtk::Widget::CAN_FOCUS
vbox.pack_start draw, false, false, 0
draw.show
draw.grab_focus
draw.set_events Gdk::EXPOSURE_MASK | Gdk::LEAVE_NOTIFY_MASK |
  Gdk::ENTER_NOTIFY_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::KEY_PRESS_MASK |
  Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK

draw.realize
create_ic draw
$pixmap = Gdk::Pixmap.new(draw.window, DrawWidth, DrawHeight, -1)
$pixmap.draw_rectangle draw.style.white_gc, true, 0, 0, DrawWidth, DrawHeight

draw.signal_connect("expose_event") do |widget, event|
  area = event.area
  draw_window widget, area.x, area.y, area.width, area.height
end
draw.signal_connect("configure_event") do |widget, event|
  set_preedit_area widget
  set_spot_location widget
#  Gdk::IM.begin $ic, widget.window
end
draw.signal_connect("key_press_event") do |widget, event|
  input_character widget, event
end
draw.signal_connect("enter_notify_event") do |widget, event|
  if Gdk::IM.ready? && $ic
    Gdk::IM.begin $ic, widget.window
  end
  widget.grab_focus
end

button = Gtk::Button.new("quit")
button.signal_connect("clicked") do
  exit 0
end
button.show
vbox.pack_start button, false, false, 0

window.show
Gtk.main
