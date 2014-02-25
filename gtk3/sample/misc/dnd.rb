=begin
  dnd.rb - Drag and Drop sample script.

  Copyright (C) 2002-2006 Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.

  $Date: 2006/06/17 13:18:12 $
  $Id: dnd.rb,v 1.9 2006/06/17 13:18:12 mutoh Exp $
=end

require "gtk3"

class SrcWindow < Gtk::Window
  def initialize
    super("Source Window")
    @label = Gtk::Label.new("Drag here!")
    add(@label)
    set_default_size(100, 100)
    drag_source_set(Gdk::Window::ModifierType::BUTTON1_MASK |
                    Gdk::Window::ModifierType::BUTTON2_MASK,
                    [["test", Gtk::Drag::TargetFlags::SAME_APP, 12345]],
                    Gdk::DragContext::Action::COPY |
                    Gdk::DragContext::Action::MOVE)
    signal_connect("drag_data_get") do |widget, context, selection_data, info, time|
#      selection_data.set("text/uri-list", 8, "hoge.txt")
      selection_data.set(Gdk::Selection::TYPE_STRING, "hoge.txt")
    end
 end
end

class DestWindow < Gtk::Window
  def initialize
    super("Dest Window")

    @label = Gtk::Label.new("Drop here!")
    add(@label)
    set_default_size(100, 100)
    drag_dest_set(Gtk::Drag::DestDefaults::MOTION |
                  Gtk::Drag::DestDefaults::HIGHLIGHT,
                  [["test", :same_app, 12345]],
                  Gdk::DragContext::Action::COPY |
                  Gdk::DragContext::Action::MOVE)

    signal_connect("drag-data-received") do |w, dc, x, y, selectiondata, info, time|
      dc.targets.each do |target|
        if target.name == "test" ||
           selectiondata.type == Gdk::Selection::TYPE_STRING
          puts selectiondata.data
        end
      end
    end
    signal_connect("drag-drop") do |w, dc, x, y, time|
      w.drag_get_data(dc, dc.targets[0], time)
    end
  end
end

win1 = SrcWindow.new
win2 = DestWindow.new

win1.show_all.signal_connect("destroy") {Gtk.main_quit}
win2.show_all.signal_connect("destroy") {Gtk.main_quit}

Gtk.main
