=begin
  dnd.rb - Drag and Drop sample script.

  Copyright (C) 2002,2003 Masao Mutoh<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME2.

  $Date: 2003/01/25 18:05:53 $
  $Id: dnd.rb,v 1.6 2003/01/25 18:05:53 mutoh Exp $
=end

require 'gtk2'

Gtk.init

class SrcWindow < Gtk::Window
  def initialize
    super
    set_title("Source Window")
    @label = Gtk::Label.new("Drop this!")
    add(@label)
    set_default_size(100, 100)
    Gtk::Drag.source_set(self, Gdk::Window::BUTTON1_MASK | 
                         Gdk::Window::BUTTON2_MASK,
                         [["test", Gtk::Drag::TARGET_SAME_APP, 12345]], 
                         Gdk::DragContext::ACTION_COPY | 
                         Gdk::DragContext::ACTION_MOVE)
  end
end

    
class DestWindow < Gtk::Window
  def initialize
    super
    set_title("Dest Window")

    @label = Gtk::Label.new("Drag here!")
    add(@label)
    set_default_size(100, 100)
    Gtk::Drag.dest_set(self, Gtk::Drag::DEST_DEFAULT_MOTION | 
                       Gtk::Drag::DEST_DEFAULT_HIGHLIGHT |
                       Gtk::Drag::DEST_DEFAULT_DROP, 
                       [["test", Gtk::Drag::TARGET_SAME_APP, 12345]], 
                       Gdk::DragContext::ACTION_COPY|Gdk::DragContext::ACTION_MOVE)
    
=begin    
    @list.signal_connect("drag-data-received") do |w, dc, x, y, selectiondata, info, time|
p w,dc,x,y,selectiondata, info, time
      dc.targets.each do |target|
p target.name
p selectiondata.data
        if target.name == "test" ||
            selectiondata.type == Gdk::Selection::TYPE_STRING
          w.append(selectiondata.data.split(","))
        end
      end
    end
=end
    ["drag-data-get", "drag-data-received", "drag-begin", "drag_motion", "drag_data_get", "drag_data_delete", "drag_end"].each do |v|
      signal_connect(v) do |w|
p "-----"
        p title, v, w
      end
    end
    
  end
end

win1 = SrcWindow.new
win2 = DestWindow.new

win1.show_all
win2.show_all

Gtk.main

