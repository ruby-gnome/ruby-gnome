=begin
  dnd.rb - Drag and Drop sample script.

  Copyright (C) 2002-2015 Masao Mutoh
  Copyright (c) 2002-2016 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

class SrcWindow < Gtk::Window
  def initialize
    super("Source Window")
    @label = Gtk::Label.new("Drag here!")
    add(@label)
    set_default_size(100, 100)
    drag_source_set(Gdk::ModifierType::BUTTON1_MASK |
                    Gdk::ModifierType::BUTTON2_MASK,
                    [["test", Gtk::TargetFlags::SAME_APP, 12_345]],
                    Gdk::DragAction::COPY |
                    Gdk::DragAction::MOVE)
    signal_connect("drag-data-get") do |_widget, _context, selection_data, _info, _time|
      selection_data.set(Gdk::Selection::TYPE_STRING, "this is a test")
      puts "drag-data-get signal:"
      puts selection_data.text
    end
  end
end

class DestWindow < Gtk::Window
  def initialize
    super("Dest Window")

    @label = Gtk::Label.new("Drop here!")
    add(@label)
    set_default_size(100, 100)
    drag_dest_set(Gtk::DestDefaults::MOTION |
                  Gtk::DestDefaults::HIGHLIGHT,
                  [["test", :same_app, 12_345]],
                  Gdk::DragAction::COPY |
                  Gdk::DragAction::MOVE)

    signal_connect("drag-data-received") do |_widget, context, _x, _y, selection_data, _info, _time|
      context.targets.each do |target|
        if target.name == "test" ||
           selection_data.type == Gdk::Selection::TYPE_STRING
          data_len = selection_data.data.size
          puts "drag-data-received signal:"
          puts selection_data.data.pack("C#{data_len}")
          puts selection_data.text
        else
          next
        end
      end
    end
    signal_connect("drag-drop") do |widget, context, _x, _y, time|
      widget.drag_get_data(context, context.targets[0], time)
    end
  end
end

win1 = SrcWindow.new
win2 = DestWindow.new

win1.show_all.signal_connect("destroy") { Gtk.main_quit }
win2.show_all.signal_connect("destroy") { Gtk.main_quit }

Gtk.main
