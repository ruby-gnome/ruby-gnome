=begin
  dnd.rb - Drag and Drop sample script.

  Copyright (C) 2002,2003 Masao Mutoh<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME2.

  $Date: 2003/01/25 18:02:21 $
  $Id: dnd.rb,v 1.5 2003/01/25 18:02:21 mutoh Exp $
=end

require 'gtk2'

Gtk.init

class CommonWindow < Gtk::Window
  attr_reader :list

  def initialize(title, data)
    super()
    set_default_size(300,128)
    set_title(title)
    signal_connect("destroy") { Gtk.main_quit }

    box = Gtk::VBox.new(false, 0)
    add(box)

    sw = Gtk::ScrolledWindow.new
    sw.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
    box.pack_start(sw, true, true, 0)

    model = Gtk::ListStore.new(String, String)
    @list = Gtk::TreeView.new(model)
    renderer = Gtk::CellRendererText.new
    col1 = Gtk::TreeViewColumn.new("Data", renderer, {:text => 0})
    col2 = Gtk::TreeViewColumn.new("Data", renderer, {:text => 1})
    @list.append_column(col1)
    @list.append_column(col2)

    sw.add_with_viewport(@list)

    data.each do |v1, v2|
      iter = model.append
      iter.set_value(0, v1)
      iter.set_value(1, v2)
    end

    @list.selection.signal_connect("changed") do |selection|
      @selected_row = selection.selected 
    end
  end
end

class SrcWindow < CommonWindow
  def initialize
    super("Source Window", [["Hello", "KON-NI-CHIWA"],
            ["Goodbye", "SAYO-NARA"],
            ["Good morning", "OHA-YO-GOZAI-MASU"]])
    
    Gtk::Drag.source_set(@list, Gdk::Window::BUTTON1_MASK | 
                         Gdk::Window::BUTTON2_MASK,
                         [["test", Gtk::Drag::TARGET_SAME_APP, 12345]], 
                         Gdk::DragContext::ACTION_COPY | 
                         Gdk::DragContext::ACTION_MOVE)
    
=begin
    @list.signal_connect("drag-data-get") do |w, dc, selectiondata, info, time|
p title, w, dc, selectiondata, info, time
      data = @selected_row.get_value(0) + "," +
             @selected_row.get_value(1)
p data
      selectiondata.set(Gdk::Selection::TYPE_STRING, data)

    end
    ["drag-begin", "drag_motion", "drag_data_get", "drag_data_delete", "drag_end"].each do |v|
      @list.signal_connect(v) do |w|
p "-----"
        p title, v, w
      end
    end
=end
  end
end

class SrcWindow2 < Gtk::Window
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
#                       [["test", Gtk::Drag::TARGET_SAME_APP, 12345]], 
                       [["test", 0, 12345]], 
 #                      Gdk::DragContext::ACTION_COPY)
                       Gdk::DragContext::ACTION_COPY|Gdk::DragContext::ACTION_MOVE)
    
=begin    
    @list.signal_connect("drag-data-get") do |w, dc, selectiondata, info, time|
p title, w, dc, selectiondata, info, time
      data = @selected_row.get_value(0) + "," +
             @selected_row.get_value(1)
p data
      selectiondata.set(Gdk::Selection::TYPE_STRING, data)

    end
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
=begin
    ["drag-data-get", "drag-data-received", "drag-begin", "drag_motion", "drag_data_get", "drag_data_delete", "drag_end"].each do |v|
      @list.signal_connect(v) do |w|
p "-----"
        p title, v, w
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


win1 = SrcWindow2.new
win2 = DestWindow.new

win1.show_all
win2.show_all

Gtk.main

