=begin
  dnd.rb - Drag and Drop sample script.

  Copyright (C) 2002,2003 Masao Mutoh<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME2.

  $Date: 2003/01/30 16:16:57 $
  $Id: dnd.rb,v 1.7 2003/01/30 16:16:57 mutoh Exp $
=end

require 'gtk2'

Gtk.init

class SrcWindow < Gtk::Window
  def initialize
    super
    set_title("Source Window")
    @label = Gtk::Label.new("Drag here!")
    add(@label)
    set_default_size(100, 100)
    Gtk::Drag.source_set(self, Gdk::Window::BUTTON1_MASK | 
                         Gdk::Window::BUTTON2_MASK,
                         [["test", Gtk::Drag::TARGET_SAME_APP, 12345]], 
                         Gdk::DragContext::ACTION_COPY | 
                         Gdk::DragContext::ACTION_MOVE)
    signal_connect("drag_data_get") do |widget, context, selection_data, info, time|
#      selection_data.set("text/uri-list", 8, "hoge.txt") 
      selection_data.set(Gdk::Selection::TYPE_STRING, "hoge.txt")  
    end
 end
end

    
class DestWindow < Gtk::Window
  def initialize
    super
    set_title("Dest Window")

    @label = Gtk::Label.new("Drop here!")
    add(@label)
    set_default_size(100, 100)
    Gtk::Drag.dest_set(self, Gtk::Drag::DEST_DEFAULT_MOTION | 
                       Gtk::Drag::DEST_DEFAULT_HIGHLIGHT,
                       [["test", Gtk::Drag::TARGET_SAME_APP, 12345]], 
                       Gdk::DragContext::ACTION_COPY|Gdk::DragContext::ACTION_MOVE)
    
    signal_connect("drag-data-received") do |w, dc, x, y, selectiondata, info, time|
      dc.targets.each do |target|
        if target.name == "test" ||
            selectiondata.type == Gdk::Selection::TYPE_STRING
          puts selectiondata.data
        end
      end
    end
    signal_connect("drag-drop") do |w, dc, x, y, time|
      Gtk::Drag.get_data(w, dc, dc.targets[0], time)
    end
  end
end

win1 = SrcWindow.new
win2 = DestWindow.new

win1.show_all
win2.show_all

Gtk.main

