=begin

  liststore.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: liststore.rb,v 1.3 2003/02/01 16:46:23 mutoh Exp $

=end

require 'sample'

$gtk_mini_xpm = [
  "15 20 17 1",
  "       c None",
  ".      c #14121F",
  "+      c #278828",
  "@      c #9B3334",
  "#      c #284C72",
  "$      c #24692A",
  "%      c #69282E",
  "&      c #37C539",
  "*      c #1D2F4D",
  "=      c #6D7076",
  "-      c #7D8482",
  ";      c #E24A49",
  ">      c #515357",
  ",      c #9B9C9B",
  "'      c #2FA232",
  ")      c #3CE23D",
  "!      c #3B6CCB",
  "               ",
  "      ***>     ",
  "    >.*!!!*    ",
  "   ***....#*=  ",
  "  *!*.!!!**!!# ",
  " .!!#*!#*!!!!# ",
  " @%#!.##.*!!$& ",
  " @;%*!*.#!#')) ",
  " @;;@%!!*$&)'' ",
  " @%.%@%$'&)$+' ",
  " @;...@$'*'*)+ ",
  " @;%..@$+*.')$ ",
  " @;%%;;$+..$)# ",
  " @;%%;@$$$'.$# ",
  " %;@@;;$$+))&* ",
  "  %;;;@+$&)&*  ",
  "   %;;@'))+>   ",
  "    %;@'&#     ",
  "     >%$$      ",
  "      >=       " ]

class ListStoreSample < SampleWindow

  def initialize
    super("Gtk::ListStore")

    vbox = Gtk::VBox.new
    self.add(vbox)

    scrolled_win = Gtk::ScrolledWindow.new
    scrolled_win.border_width = 5
    scrolled_win.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)

    data = ["auto resize", "not resizeable", "max width 100", "min width 50",
	"hide column", "Title 5", "Title 6", "Title 7",
	"Title 8",  "Title 9",  "Title 10", "Title 11" ]
    @model = Gtk::ListStore.new(*data.collect{|label| label.class})
    treeview = Gtk::TreeView.new(@model)
    renderer = Gtk::CellRendererText.new
    data.each_index do |cnt|
      column = Gtk::TreeViewColumn.new(data[cnt], renderer, {:text => cnt})
      column.sort_column_id = cnt
      append_column(column)
    end
    scrolled_win.add(treeview)
    treeview.selection.signal_connect("changed") do |w, c| click_column(c) end

    # control buttons
    hbox = Gtk::HBox::new(false, 5)
    hbox.border_width = 5
    vbox.pack_start(hbox, false, false, 0)

    button = Gtk::Button::new("Insert Row")
    hbox.pack_start(button)
    button.signal_connect("clicked") do insert_row end

    button = Gtk::Button::new("Add 1,000 Rows With Pixmaps")
    hbox.pack_start(button)
    button.signal_connect("clicked") do add1000 end

    button = Gtk::Button::new("Add 10,000 Rows");
    hbox.pack_start(button)
    button.signal_connect("clicked") do add10000 end

    # second layer of buttons
    hbox = Gtk::HBox::new(false, 5)
    hbox.border_width = 5
    vbox.pack_start(hbox, false, false, 0);

    button = Gtk::Button::new("Clear List");
    hbox.pack_start(button);
    button.signal_connect("clicked") do clear end

    button = Gtk::Button::new("Remove Selection")
    hbox.pack_start(button)
    button.signal_connect("clicked") do remove_selection end

    undo_button = Gtk::Button::new("Undo Selection")
    hbox.pack_start(undo_button)
    undo_button.signal_connect("clicked") do undo_selection end

    button = Gtk::Button::new("Warning Test")
    hbox.pack_start(button)
    button.signal_connect("clicked") do warning_test end

    # third layer of buttons
    hbox = Gtk::HBox::new(false, 5)
    hbox.border_width = 5
    vbox.pack_start(hbox, false, false, 0);

    check = Gtk::CheckButton::new("Show Title Buttons")
    hbox.pack_start(check)
    check.signal_connect("clicked") do |w| toggle_title_buttons(w) end
    check.set_active(true);

    check = Gtk::CheckButton::new("Reorderable")
    hbox.pack_start(check)
    check.signal_connect("clicked") do |w| toggle_reorderable(w) end
    check.set_active(true)

    label = Gtk::Label::new("Selection Mode :")
    hbox.pack_start(label, false, true, 0);

    clist_omenu = build_option_menu(
      [ OptionMenuItem.new("Single",   proc { |w| toggle_sel_mode(w) }),
        OptionMenuItem.new("Browse",   proc { |w| toggle_sel_mode(w) }),
        OptionMenuItem.new("Multiple", proc { |w| toggle_sel_mode(w) }),
        OptionMenuItem.new("Extended", proc { |w| toggle_sel_mode(w) }) ],
	3)

    hbox.pack_start(clist_omenu, false, true, 0);

    vbox.pack_start(scrolled_win)
    @clist.set_row_height(18)
    @clist.set_usize(-1, 300)

    for i in 1 .. 11 do
      @clist.set_column_width(i, 80)
    end

    @clist.set_column_auto_resize(0, true)
    @clist.set_column_resizeable(1, false)
    @clist.set_column_max_width(2, 100)
    @clist.set_column_min_width(3, 50)
    @clist.set_selection_mode(Gtk::SELECTION_EXTENDED)
    @clist.set_column_justification(1, Gtk::JUSTIFY_RIGHT)
    @clist.set_column_justification(2, Gtk::JUSTIFY_CENTER)

    texts = []
    for i in 0 .. 11 do
      texts[i] = sprintf("Column %d", i)
    end
    texts[1, 2] = [ "Right", "Center" ]

    style = Gtk::Style::new()
    style.set_fg(Gtk::STATE_NORMAL, 56000, 0, 0);
    style.set_base(Gtk::STATE_NORMAL, 0, 56000, 32000);

    for i in 0 .. 9 do
      texts[0] = sprintf("CListRow %d", i)
      @clist.append(texts)

      case i % 4
      when 2 then
	@clist.set_row_style(i, style);
      else
        @clist.set_cell_style(i, i%4, style);
      end
    end

    separator = Gtk::HSeparator::new()
    vbox.pack_start(separator, false, true, 0)

    hbox = Gtk::HBox::new()
    vbox.pack_start(hbox, false, true, 0)

    button = Gtk::Button::new("close")
    button.border_width = 10
    hbox.pack_start(button)
    button.signal_connect("clicked") do destroy end

    button.set_flags(Gtk::Widget::CAN_DEFAULT)
    button.grab_default

    @add_remove = false
    @style1 = nil
    @style2 = nil
    @style3 = nil
  end

  private
  def toggle_sel_mode(rmitem)
    return unless rmitem.mapped?

    i = 3
    group = rmitem.group
    group.each do |g|
      break if g.active?
      i -= 1
    end
    @clist.set_selection_mode(i)
  end

  private
  def click_column(column)
    if column == 4 then
      @clist.set_column_visibility(column, false)
    elsif column == @clist.sort_column then
      if @clist.sort_type == Gtk::SORT_ASCENDING
        @clist.sort_type = Gtk::SORT_DESCENDING
      else
        @clist.sort_type = Gtk::SORT_ASCENDING
      end
    else
      @clist.sort_column = column
    end
    @clist.sort
  end

  private
  def add1000
    pixmap, mask = Gdk::Pixmap::create_from_xpm_d(window,
					    style.white,
					    $gtk_mini_xpm)
    texts = []
    for i in 0 .. 11 do
      texts[i] = sprintf("Column %d", i)
    end
    texts[3] = nil
    texts[1] = "Right"
    texts[2] = "Center"
    @clist.freeze
    for i in 0..999 do
      texts[0] = sprintf("CListRow %d", rand(9999))
      row = @clist.append(texts)
      @clist.set_pixtext(row, 3, "gtk+", 5, pixmap, mask)
    end
    @clist.thaw
  end

  private
  def add10000
    texts = []
    for i in 0 .. 11 do
      texts[i] = sprintf("Column %d", i)
    end
    texts[1] = "Right"
    texts[2] = "Center"
    @clist.freeze
    for i in 0..9999 do
      texts[0] = sprintf("CListRow %d", rand(9999))
      row = @clist.append(texts)
    end
    @clist.thaw
  end

  private
  def clear
    @clist.clear
  end

  private
  def remove_selection
    @clist.freeze
    @clist.each_selection do
      | row |
      @clist.remove_row(row)
    end
    if @clist.selection_mode == Gtk::SELECTION_EXTENDED and
       @clist.focus_row >= 0 then
      @clist.select_row(@clist.focus_row, -1)
    end
    @clist.thaw
  end

  private
  def toggle_title_buttons(cbutton)
    if cbutton.active? then
      @clist.column_titles_show
    else
      @clist.column_titles_hide
    end
  end

  private
  def toggle_reorderable(cbutton)
    @clist.set_reorderable(cbutton.active?)
  end

  private
  def insert_row
    texts = [ "This", "is an", "inserted", "row.",
	      "This", "is an", "inserted", "row.",
	      "This", "is an", "inserted", "row." ]

    if @clist.focus_row >= 0 then
      row = @clist.insert(@clist.focus_row, texts)
    else
      row = @clist.prepend(texts)
    end

    if @style1 == nil then

      @style1 = @clist.style.copy
      @style1.set_base(Gtk::STATE_NORMAL, 0, 56000, 0)
      @style1.set_base(Gtk::STATE_SELECTED, 32000, 0, 56000)

      @style2 = @clist.style.copy
      @style2.set_fg(Gtk::STATE_NORMAL, 0, 56000, 0)
      @style2.set_fg(Gtk::STATE_SELECTED, 32000, 0, 56000)

      @style3 = @clist.style.copy
      @style3.set_fg(Gtk::STATE_NORMAL, 0, 56000, 0)
      @style3.set_base(Gtk::STATE_NORMAL, 32000, 0, 56000)
    end

    @clist.set_cell_style(row, 3, @style1)
    @clist.set_cell_style(row, 4, @style2)
    @clist.set_cell_style(row, 0, @style3)
  end

  private
  def warning_test
    @add_remove = !@add_remove
    child = Gtk::Label::new("Test");
    if @add_remove then
      @clist.add(child)
    else
      child.set_parent(child)
      @clist.remove(child)
      child.set_parent(nil)
    end
  end

  private
  def undo_selection
    @clist.undo_selection
  end

end
