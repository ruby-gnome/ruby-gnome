=begin header

  notebook.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team

  $Id: notebook.rb,v 1.10 2004/03/05 16:24:30 mutoh Exp $

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

    box1 = Gtk::VBox.new(false, 0)
    add(box1)

    @notebook = Gtk::Notebook.new()
    @notebook.signal_connect("switch_page") do |widget, page, num_page|
      unless destroyed?
	page_switch(widget, page, num_page)
      end
    end
    @notebook.set_tab_pos(Gtk::POS_TOP)
    box1.pack_start(@notebook, true, true, 0)
    @notebook.set_border_width(10)

    @notebook.realize
    @book_open, @book_open_mask =
      Gdk::Pixmap::create_from_xpm_d(@notebook.window, nil, $book_open_xpm)
    @book_closed, @book_closed_mask =
      Gdk::Pixmap::create_from_xpm_d(@notebook.window, nil, $book_closed_xpm)

    create_pages(1, 5)

    separator = Gtk::HSeparator.new()
    box1.pack_start(separator, false, true, 10)

    box2 = Gtk::HBox.new(false, 5)
    box2.set_border_width(10)
    box1.pack_start(box2, false, true, 0)

    cbutton1 = Gtk::CheckButton.new("popup menu")
    box2.pack_start(cbutton1, true, false, 0)
    cbutton1.signal_connect("clicked") do
      @notebook.set_enable_popup(cbutton1.active?)
    end

    box2 = Gtk::HBox.new(false, 5)
    box2.set_border_width(10)
    box1.pack_start(box2, false, true, 0)

    label = Gtk::Label.new("Notebook Style :")
    box2.pack_start(label, false, true, 0)

    omenu = build_option_menu([
      OptionMenuItem.new("Standard", proc { standard_notebook }),
      OptionMenuItem.new("No tabs",  proc { notabs_notebook }),
      OptionMenuItem.new("Scrollable", proc { scrollable_notebook }) ], 0)
    box2.pack_start(omenu, false, true, 0)

    button = Gtk::Button.new("Show all Pages")
    box2.pack_start(button, false, true, 0)
    button.signal_connect('clicked') do
      @notebook.each do |w|
	w.show
      end
    end

    box2 = Gtk::HBox.new(true, 10)
    box2.set_border_width(10)
    box1.pack_start(box2, false, true, 0)

    button = Gtk::Button.new("prev")
    button.signal_connect("clicked") do
      @notebook.prev_page
    end
    box2.pack_start(button, true, true, 0)

    button = Gtk::Button.new("next")
    button.signal_connect("clicked") do
      @notebook.next_page
    end
    box2.pack_start(button, true, true, 0)

    button = Gtk::Button.new("rotate")
    button.signal_connect("clicked") do
      @notebook.set_tab_pos((@notebook.tab_pos.to_i + 1) % 4)
    end
    box2.pack_start(button, true, true, 0)

    separator = Gtk::HSeparator.new()
    box1.pack_start(separator, false, true, 5)

    button = Gtk::Button.new("close")
    button.set_border_width(5)
    button.signal_connect("clicked") do
      destroy
    end
    box1.pack_start(button, false, false, 0)
    button.set_flags(Gtk::Widget::CAN_DEFAULT)
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
      child.set_border_width(10)

      vbox = Gtk::VBox.new(true, 0)
      vbox.set_border_width(10)
      child.add(vbox)

      hbox = Gtk::HBox.new(true, 0)
      vbox.pack_start(hbox, false, true, 5)

      button1 = Gtk::CheckButton.new("Fill Tab")
      hbox.pack_start(button1, true, true, 5)
      button1.set_active(true)
      button1.signal_connect('toggled') do
	expand, fill, pack = @notebook.query_tab_label_packing(child)
	@notebook.set_tab_label_packing(child, expand, button1.active?, pack)
      end

      button2 = Gtk::CheckButton.new("Expand Tab")
      hbox.pack_start(button2, true, true, 5)
      button2.signal_connect('toggled') do
	expand, fill, pack = @notebook.query_tab_label_packing(child)
	@notebook.set_tab_label_packing(child, button2.active?, fill, pack)
      end

      button3 = Gtk::CheckButton.new("Pack end")
      hbox.pack_start(button3, true, true, 5)
      button3.signal_connect('toggled') do
	expand, fill, pack = @notebook.query_tab_label_packing(child)
	@notebook.set_tab_label_packing(child, expand, fill,
				       if button3.active? then Gtk::PACK_END else Gtk::PACK_START end)
      end

      button = Gtk::Button.new("Hide Page")
      vbox.pack_end(button, false, false, 5)
      button.signal_connect('clicked') do
	child.hide
      end

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

