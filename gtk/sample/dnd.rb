=begin
  dnd.rb - Drag and Drop sample script.

  Copyright (C) 2002 MUTOH Masao<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME.

  $Date: 2002/05/21 17:32:21 $
  $Id: dnd.rb,v 1.2 2002/05/21 17:32:21 mutoh Exp $
=end

require 'gtk2'

class TestWindow < Gtk::Window
  attr_reader :clist

  DRAG_TEST = "test"

  def initialize(title)
    super()
    set_usize(300,128)
    set_title(title)
    signal_connect(Gtk::Window::SIGNAL_DESTROY) { Gtk.main_quit }

    box = Gtk::VBox.new(false, 0)
    add(box)

    sw = Gtk::ScrolledWindow.new
    sw.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
    box.pack_start(sw, true, true, 0)

    @clist = Gtk::CList.new(["English", "Japanese"])
    @clist.set_selection_mode(Gtk::SELECTION_BROWSE)
    @clist.set_column_width(0, 120)
    @clist.set_column_width(1, 120)
    @clist.signal_connect(Gtk::CList::SIGNAL_SELECT_ROW) do 
      |w, row, column, event|
      @selected_row = row
    end
    sw.add(@clist)

    @clist.drag_dest_set(Gtk::DEST_DEFAULT_MOTION | 
                         Gtk::DEST_DEFAULT_HIGHLIGHT |
                         Gtk::DEST_DEFAULT_DROP, 
                         [[DRAG_TEST, Gtk::TARGET_SAME_APP, 12345]], 
                         Gdk::DragContext::ACTION_COPY | 
                         Gdk::DragContext::ACTION_MOVE)
    
    @clist.drag_source_set(Gdk::BUTTON1_MASK | 
                           Gdk::BUTTON2_MASK,
                          [[DRAG_TEST, Gtk::TARGET_SAME_APP, 12345]], 
                           Gdk::DragContext::ACTION_COPY | 
                           Gdk::DragContext::ACTION_MOVE)
    
    @clist.signal_connect(Gtk::Widget::SIGNAL_DRAG_DATA_GET) do 
      |w, dc, selectiondata, info, time|
      data = @clist.get_text(@selected_row, 0) + "," +
             @clist.get_text(@selected_row, 1)

      selectiondata.set(Gdk::Selection::TYPE_STRING, 
                             8, # 8 bits per character
                             data)
    end
    @clist.signal_connect(Gtk::Widget::SIGNAL_DRAG_DATA_RECEIVED) do 
      |w, dc, x, y, selectiondata, info, time|
      dc.targets.each do |target|
        if target.name == DRAG_TEST ||
            selectiondata.type == Gdk::Selection::TYPE_STRING
          w.append(selectiondata.data.split(","))
        end
      end
    end
  end
end

win1 = TestWindow.new("List 1")
win2 = TestWindow.new("List 2")

win1.clist.append(["Hello", "KON-NI-CHIWA"])
win1.clist.append(["Goodbye", "SAYO-NARA"])
win1.clist.append(["Good morning", "OHA-YO-GOZAI-MASU"])

win1.show_all
win2.show_all

Gtk.main

