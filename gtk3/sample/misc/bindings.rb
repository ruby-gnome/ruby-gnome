#!/usr/bin/env ruby
=begin
  bindings.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

=begin
Usage:
  bindings.rb <filename>

  Following key bindings are effective in the TextView area.

    <Control> + l     : scroll down by one page
    <Control> + m     : scroll up by one page
    <Control> + j     : move cursor donw by one line
    <Control> + k     : move cursor up by one line

  clicking buttons cause following effect

    "Ctrl + l"          : same as pressing <Control> + l in text view area.
    "Ctrl + m"          : same as pressing <Control> + m in text view area.
    "cancel Ctrl +j/k"  : disable <Control> + j and <Control> + k bindings.
=end

require "gtk3"

class Pager < Gtk::TextView
  def initialize(path)
    @path = path
    super()
    load
    set_editable(false)
    set_size_request(400, 400)
  end

  private
  def load
    open(@path).read.each_line do |line|
      buffer.insert_at_cursor(line)
    end
    buffer.place_cursor(buffer.start_iter)
  end
end

file_path = ARGV[0] || __FILE__

window = Gtk::Window.new
window.name = "pager_window"

css_provider = Gtk::CssProvider.new

if Gtk::Version.or_later?(3, 20, 0)
  css_provider.load(:data => <<CSS)
  @binding-set MoveCursor {
    bind "<Control>j" { "move-cursor" (display-lines, 1, 0) };
    bind "<Control>k" { "move-cursor" (display-lines, -1, 0) };
    bind "<Control>l" { "move-cursor" (buffer-ends, 1, 0) };
    bind "<Control>m" { "move-cursor" (buffer-ends, -1, 0) };
  }
  textview {
    caret-color: green;
    -GtkWidget-aspect-ratio: 1.0;
    font: 20 Sans;
    color: #aaa;
    background-color: #333 ;
    -gtk-key-bindings: MoveCursor;
  }
CSS
else
  css_provider.load(:data => <<CSS)
  @binding-set MoveCursor {
    bind "<Control>j" { "move-cursor" (display-lines, 1, 0) };
    bind "<Control>k" { "move-cursor" (display-lines, -1, 0) };
    bind "<Control>l" { "move-cursor" (buffer-ends, 1, 0) };
    bind "<Control>m" { "move-cursor" (buffer-ends, -1, 0) };
  }
  GtkTextView {
    -GtkWidget-cursor-color: green;
    -GtkWidget-aspect-ratio: 1.0;
    font: 20 Sans;
    color: #aaa;
    background-color: #333 ;
    gtk-key-bindings: MoveCursor;
  }
CSS
end

hbox = Gtk::Box.new(:horizontal)

hbox.add(button1 = Gtk::Button.new(:label => "Ctrl + l"))
hbox.add(button2 = Gtk::Button.new(:label => "Ctrl + m"))
hbox.add(button3 = Gtk::Button.new(:label => "Cancel Ctrl + j/k"))

vbox = Gtk::Box.new(:vertical)
sw = Gtk::ScrolledWindow.new
sw.set_size_request(500, 500)

pager = Pager.new(file_path)
pager.style_context.add_provider(css_provider, Gtk::StyleProvider::PRIORITY_USER)
sw.add(pager)

vbox.add(hbox)
vbox.add(sw)

window.add(vbox)
window.show_all

window.signal_connect("destroy") { Gtk.main_quit }

binding_set = Gtk::BindingSet.find("MoveCursor")

button1.signal_connect "clicked" do
  binding_set.activate(Gdk::Keyval::KEY_l, Gdk::ModifierType::CONTROL_MASK, pager)
  pager.grab_focus
end

button2.signal_connect "clicked" do
  binding_set.activate(Gdk::Keyval::KEY_m, Gdk::ModifierType::CONTROL_MASK, pager)
  pager.grab_focus
end

button3.signal_connect "clicked" do
  binding_set.remove(Gdk::Keyval::KEY_j, Gdk::ModifierType::CONTROL_MASK)
  binding_set.remove(Gdk::Keyval::KEY_k, Gdk::ModifierType::CONTROL_MASK)
  pager.grab_focus
end

pager.grab_focus

Gtk.main
