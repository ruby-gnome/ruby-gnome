=begin header

  notebook.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  $Id: notebook.rb,v 1.11 2005/07/18 17:13:32 mutoh Exp $

  Rewritten by TAKAHASHI Hitoshi <thitoshi@ne.scphys.kyoto-u.ac.jp>

Original Copyright:
 
  GTK - The GIMP Toolkit
  Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.
 
  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.

=end

require 'sample'

$book_open_xpm = [
"16 16 4 1",
"       c None s None",
".      c black",
"X      c #808080",
"o      c white",
"                ",
"  ..            ",
" .Xo.    ...    ",
" .Xoo. ..oo.    ",
" .Xooo.Xooo...  ",
" .Xooo.oooo.X.  ",
" .Xooo.Xooo.X.  ",
" .Xooo.oooo.X.  ",
" .Xooo.Xooo.X.  ",
" .Xooo.oooo.X.  ",
"  .Xoo.Xoo..X.  ",
"   .Xo.o..ooX.  ",
"    .X..XXXXX.  ",
"    ..X.......  ",
"     ..         ",
"                "
]

$book_closed_xpm = [
"16 16 6 1",
"       c None s None",
".      c black",
"X      c red",
"o      c yellow",
"O      c #808080",
"#      c white",
"                ",
"       ..       ",
"     ..XX.      ",
"   ..XXXXX.     ",
" ..XXXXXXXX.    ",
".ooXXXXXXXXX.   ",
"..ooXXXXXXXXX.  ",
".X.ooXXXXXXXXX. ",
".XX.ooXXXXXX..  ",
" .XX.ooXXX..#O  ",
"  .XX.oo..##OO. ",
"   .XX..##OO..  ",
"    .X.#OO..    ",
"     ..O..      ",
"      ..        ",
"                "
]

class NotebookSample < SampleWindow
  def initialize
    super("notebook")

    vbox = Gtk::VBox.new(false, 0)
    add(vbox)

    @notebook = Gtk::Notebook.new
    @notebook.signal_connect("switch_page") do |widget, page, num_page|
      unless destroyed?
	page_switch(widget, page, num_page)
      end
    end
    @notebook.tab_pos = Gtk::POS_TOP
    vbox.add(@notebook)
    @notebook.border_width = 10

    @notebook.realize

    @book_open, @book_open_mask =
      Gdk::Pixmap::create_from_xpm_d(@notebook.window, nil, $book_open_xpm)
    @book_closed, @book_closed_mask =
      Gdk::Pixmap::create_from_xpm_d(@notebook.window, nil, $book_closed_xpm)

    create_pages(1, 5)

    vbox.add(Gtk::HSeparator.new)

    cbutton1 = Gtk::CheckButton.new("popup menu")
    vbox.add(cbutton1)
    cbutton1.signal_connect("clicked"){
      @notebook.enable_popup = cbutton1.active?
    }

    hbox = Gtk::HBox.new(false, 5)
    hbox.border_width = 10
    vbox.pack_start(hbox, false, false, 0)

    label = Gtk::Label.new("Notebook Style :")
    hbox.add(label)

    omenu = build_option_menu([
      OptionMenuItem.new("Standard", proc { standard_notebook }),
      OptionMenuItem.new("No tabs",  proc { notabs_notebook }),
      OptionMenuItem.new("Scrollable", proc { scrollable_notebook }) ], 0)

    hbox.add(omenu)

    button = Gtk::Button.new("Show all Pages")

    hbox.add(button)
    button.signal_connect('clicked'){
      @notebook.each do |w|
	w.show
      end
    }

    hbox = Gtk::HBox.new(true, 10)
    hbox.set_border_width(10)
    vbox.pack_start(hbox, false, true, 0)

    button = Gtk::Button.new("prev")
    button.signal_connect("clicked"){
      @notebook.prev_page
    }
    hbox.pack_start(button, true, true, 0)

    button = Gtk::Button.new("next")
    button.signal_connect("clicked"){
      @notebook.next_page
    }
    hbox.pack_start(button, true, true, 0)

    button = Gtk::Button.new("rotate")
    button.signal_connect("clicked"){
      @notebook.tab_pos = (@notebook.tab_pos.to_i + 1) % 4
    }
    hbox.pack_start(button, true, true, 0)

    vbox.add(Gtk::HSeparator.new)

    button = Gtk::Button.new("close")
    button.signal_connect("clicked"){destroy}

    vbox.pack_start(button, false, false, 5)

    button.can_default = true
    button.grab_default
  end

  def standard_notebook
    @notebook.set_show_tabs(true)
    @notebook.set_scrollable(false)
    if @notebook.children.size == 15
      10.times do
	@notebook.remove_page(5)
      end
    end
  end

  def notabs_notebook
    @notebook.set_show_tabs(false)
    if @notebook.children.size == 15
      10.times do
	@notebook.remove_page(5)
      end
    end
  end

  def scrollable_notebook
    @notebook.set_show_tabs(true)
    @notebook.set_scrollable(true)
    if @notebook.children.size == 5
      create_pages(6, 15)
    end
  end

  def create_pages(start_page, end_page)
    start_page.upto(end_page) do |i|
      buffer = "Page #{i}"

      child = Gtk::Frame.new(buffer)
      child.border_width = 10

      vbox = Gtk::VBox.new(true, 0)
      vbox.border_width = 10
      child.add(vbox)

      hbox = Gtk::HBox.new(true, 0)
      vbox.pack_start(hbox, false, true, 5)

      button1 = Gtk::CheckButton.new("Fill Tab")
      hbox.pack_start(button1, true, true, 5)
      button1.active = true
      button1.signal_connect('toggled'){
	expand, fill, pack = @notebook.query_tab_label_packing(child)
	@notebook.set_tab_label_packing(child, expand, button1.active?, pack)
      }

      button2 = Gtk::CheckButton.new("Expand Tab")
      hbox.pack_start(button2, true, true, 5)
      button2.signal_connect('toggled'){
	expand, fill, pack = @notebook.query_tab_label_packing(child)
	@notebook.set_tab_label_packing(child, button2.active?, fill, pack)
      }

      button3 = Gtk::CheckButton.new("Pack end")
      hbox.pack_start(button3, true, true, 5)
      button3.signal_connect('toggled'){
	expand, fill, pack = @notebook.query_tab_label_packing(child)
	@notebook.set_tab_label_packing(child, expand, fill,
				       if button3.active? then Gtk::PACK_END else Gtk::PACK_START end)
      }

      button = Gtk::Button.new("Hide Page")
      vbox.pack_end(button, false, false, 5)
      button.signal_connect('clicked'){
	child.hide
      }

      child.show_all

      label_box = Gtk::HBox.new(false, 0)
      pixwid = Gtk::Image.new(@book_closed, @book_closed_mask)
      label_box.pack_start(pixwid, false, true, 0)
      pixwid.set_padding(3, 1)
      label = Gtk::Label.new(buffer)
      label_box.pack_start(label, false, true, 0)
      label_box.show_all
      
      menu_box = Gtk::HBox.new(false, 0)
      pixwid = Gtk::Image.new(@book_closed, @book_closed_mask)
      menu_box.pack_start(pixwid, false, true, 0)
      pixwid.set_padding(3, 1)
      label = Gtk::Label.new(buffer)
      menu_box.pack_start(label, false, true, 0)
      menu_box.show_all

      @notebook.append_page_menu(child, label_box, menu_box)
    end
  end

  def set_page_pixmaps(notebook, page_num, pix, mask)
    child = notebook.get_nth_page(page_num)
    label = notebook.get_tab_label(child).children[0].set(pix, mask)
  end

  def page_switch(notebook, page, page_num)
    old_page_num = notebook.page
    return if (page_num == old_page_num)
    set_page_pixmaps(notebook, page_num, @book_open, @book_open_mask)
    if (old_page_num != -1)
      set_page_pixmaps(notebook, old_page_num, @book_closed, @book_closed_mask)
    end
  end
end

