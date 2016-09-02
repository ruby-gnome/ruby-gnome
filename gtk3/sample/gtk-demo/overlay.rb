# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Overlay/Interactive Overlay

Shows widgets in static positions over a main widget.

The overlayed widgets can be interactive controls such
as the entry in this example, or just decorative, like
the big blue label.
=end
class OverlayDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.set_default_size(500, 510)
    @window.title = "Interactive Overlay"
    @window.screen = main_window.screen

    overlay = Gtk::Overlay.new
    initialize_grid
    overlay.add(@grid)

    @entry = Gtk::Entry.new
    @entry.placeholder_text = "Your Lucky Number"

    vbox = Gtk::Box.new(:vertical, 10)
    overlay.add_overlay(vbox)
    overlay.set_overlay_pass_through(vbox, true)
    vbox.halign = :center
    vbox.valign = :center

    label = Gtk::Label.new("<span foreground='blue' weight='ultrabold' font='40'>Numbers</span>")
    label.use_markup = true
    vbox.pack_start(label, :expand => false, :fill => false, :padding => 8)

    vbox.pack_start(@entry, :expand => false, :fill => false, :padding => 8)

    @window.add(overlay)
    overlay.show_all
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
  end

  private

  def initialize_grid
    @grid = Gtk::Grid.new
    (0..4).each do |i|
      (0..4).each do |j|
        text = (5 * j + i).to_s
        button = Gtk::Button.new(:label => text)
        button.hexpand = true
        button.vexpand = true
        button.signal_connect "clicked" do |widget|
          @entry.text = widget.label
        end
        @grid.attach(button, i, j, 1, 1)
      end
    end
  end
end
