=begin header

  treestore.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: treestore.rb,v 1.4 2003/02/01 16:46:23 mutoh Exp $

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
  "                " ]

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
  "                " ]

$mini_page_xpm = [
  "16 16 4 1",
  "       c None s None",
  ".      c black",
  "X      c white",
  "o      c #808080",
  "                ",
  "   .......      ",
  "   .XXXXX..     ",
  "   .XoooX.X.    ",
  "   .XXXXX....   ",
  "   .XooooXoo.o  ",
  "   .XXXXXXXX.o  ",
  "   .XooooooX.o  ",
  "   .XXXXXXXX.o  ",
  "   .XooooooX.o  ",
  "   .XXXXXXXX.o  ",
  "   .XooooooX.o  ",
  "   .XXXXXXXX.o  ",
  "   ..........o  ",
  "    oooooooooo  ",
  "                " ]

class TreeStoreSample < SampleWindow

  def initialize
    super("Gtk::CTree")

    @books = 0
    @pages = 0

    @style1 = Gtk::Style::new()
    @style1.set_base(Gtk::STATE_NORMAL, 0, 56000, 0)
    @style1.set_fg(Gtk::STATE_SELECTED, 32000, 0, 56000)

    @style2 = Gtk::Style::new()
    @style2.set_base(Gtk::STATE_SELECTED, 32000, 0, 56000)
    @style2.set_fg(Gtk::STATE_NORMAL, 0, 56000, 0)
    @style2.set_base(Gtk::STATE_NORMAL, 32000, 0, 56000)

    @export_window = nil

    @tooltips = Gtk::Tooltips::new()
      
    vbox = Gtk::VBox::new()
    add(vbox)

    hbox = Gtk::HBox::new(false, 5)
    hbox.border_width = 5
    vbox.pack_start(hbox, false)

    label = Gtk::Label::new("Depth :")
    hbox.pack_start(label, false)

    adj = Gtk::Adjustment::new(4, 1, 10, 1, 5, 0)
    @spin1 = Gtk::SpinButton::new(adj)
    hbox.pack_start(@spin1, false, true, 5)

    label = Gtk::Label::new("Books :")
    hbox.pack_start(label, false)

    adj = Gtk::Adjustment::new(3, 1, 20, 1, 5, 0)
    @spin2 = Gtk::SpinButton::new(adj)
    hbox.pack_start(@spin2, false, true, 5)

    label = Gtk::Label::new("Pages :")
    hbox.pack_start(label, false)

    adj = Gtk::Adjustment::new(5, 1, 20, 1, 5, 0)
    @spin3 = Gtk::SpinButton::new(adj)
    hbox.pack_start(@spin3, false, true, 5)

    button = Gtk::Button::new("Close")
    hbox.pack_end(button)
    button.signal_connect("clicked") do destroy end

    button = Gtk::Button::new("Rebuild Tree")
    hbox.pack_end(button)
    button.signal_connect("clicked") do rebuild end

    scrolled_win = Gtk::ScrolledWindow::new()
    scrolled_win.border_width = 5
    scrolled_win.set_policy(Gtk::POLICY_AUTOMATIC,
			      Gtk::POLICY_ALWAYS)
    vbox.pack_start(scrolled_win)

    @ctree = Gtk::CTree::new([ "Tree", "Info" ], 0)
    scrolled_win.add(@ctree)

    @ctree.set_column_auto_resize(0, true);
    @ctree.set_column_width(1, 200);
    @ctree.set_selection_mode(Gtk::SELECTION_EXTENDED);
    @ctree.line_style = Gtk::CTree::LINES_DOTTED;
    @line_style = Gtk::CTree::LINES_DOTTED;

    @ctree.signal_connect("click_column") do |w,c| click_column(c) end
    @ctree.signal_connect_after("button_press_event") do after_press end
    @ctree.signal_connect_after("button_release_event") do after_press end
    @ctree.signal_connect_after("tree_move") do |w, child, parent, sibling|
      after_move(child, parent, sibling) end
    @ctree.signal_connect_after("end_selection") do after_press end
    @ctree.signal_connect_after("toggle_focus_row") do after_press end
    @ctree.signal_connect_after("select_all") do after_press end
    @ctree.signal_connect_after("unselect_all") do after_press end
    @ctree.signal_connect_after("scroll_vertical") do after_press end

    bbox = Gtk::HBox::new(false, 5)
    bbox.border_width = 5
    vbox.pack_start(bbox, false, true, 0)

    mbox = Gtk::VBox::new(true, 5)
    bbox.pack_start(mbox, false, true, 0)

    label = Gtk::Label::new("Row Height :")
    mbox.pack_start(label, false, false, 0)

    label = Gtk::Label::new("Indent :")
    mbox.pack_start(label, false, false, 0)

    label = Gtk::Label::new("Spacing :")
    mbox.pack_start(label, false, false, 0)

    mbox = Gtk::VBox::new(true, 5)
    bbox.pack_start(mbox, false, true, 0);

    adj = Gtk::Adjustment::new(20, 12, 100, 1, 10, 0)
    spinner = Gtk::SpinButton::new(adj, 0, 0)
    mbox.pack_start(spinner, false, false, 5)
    @tooltips.set_tip(spinner, "Row height of list items", nil)
    adj.signal_connect("value_changed") do |w| change_row_height(w) end
    @ctree.set_row_height(adj.value)

    adj = Gtk::Adjustment::new(20, 0, 60, 1, 10, 0)
    spinner = Gtk::SpinButton::new(adj, 0, 0)
    mbox.pack_start(spinner, false, false, 5)
    @tooltips.set_tip(spinner, "Tree Indentation.", nil)
    adj.signal_connect("value_changed") do |w| change_indent(w) end

    adj = Gtk::Adjustment::new(5, 0, 60, 1, 10, 0)
    spinner = Gtk::SpinButton::new(adj, 0, 0)
    mbox.pack_start(spinner, false, false, 5)
    @tooltips.set_tip(spinner, "Tree Spacing.", nil)
    adj.signal_connect("value_changed") do |w| change_spacing(w) end

    mbox = Gtk::VBox::new(true, 5)
    bbox.pack_start(mbox, false, true, 0)

    hbox = Gtk::HBox::new(false, 5)
    mbox.pack_start(hbox, false, false, 0)

    button = Gtk::Button::new("Expand All")
    hbox.pack_start(button)
    button.signal_connect("clicked") do expand_all end

    button = Gtk::Button::new("Collapse All")
    hbox.pack_start(button)
    button.signal_connect("clicked") do collapse_all end

    button = Gtk::Button::new("Change Style")
    hbox.pack_start(button)
    button.signal_connect("clicked") do change_style end

    button = Gtk::Button::new("Export Tree")
    hbox.pack_start(button)
    button.set_sensitive(false)
#    button.signal_connect("clicked") do export end

    hbox = Gtk::HBox::new(FALSE, 5);
    mbox.pack_start(hbox, false, false, 0)

    button = Gtk::Button::new("Select All")
    hbox.pack_start(button)
    button.signal_connect("clicked") do select_all end

    button = Gtk::Button::new("Unselect All")
    hbox.pack_start(button)
    button.signal_connect("clicked") do unselect_all end

    button = Gtk::Button::new("Remove Selection")
    hbox.pack_start(button)
    button.signal_connect("clicked") do remove_selection end

    check = Gtk::CheckButton::new("Reorderable")
    hbox.pack_start(check, false, true, 0)
    @tooltips.set_tip(check,
		      "Tree items can be reordered by dragging.", nil)
    check.signal_connect("clicked") do |cb| toggle_reorderable(cb) end
    check.set_active(true)

    hbox = Gtk::HBox::new(true, 5)
    mbox.pack_start(hbox, false, false, 0)

    omenu1 = build_option_menu(
      [ OptionMenuItem.new("No lines", proc { |w| toggle_line_style(w) }),
	OptionMenuItem.new("Solid",    proc { |w| toggle_line_style(w) }),
	OptionMenuItem.new("Dotted",   proc { |w| toggle_line_style(w) }),
	OptionMenuItem.new("Tabbed",   proc { |w| toggle_line_style(w) }) ],
	2)
    hbox.pack_start(omenu1, false, true, 0)
    @tooltips.set_tip(omenu1, "The tree's line style.", nil)

    omenu2 = build_option_menu(
      [ OptionMenuItem.new("None",     proc { |w| toggle_expander_style(w) }),
	OptionMenuItem.new("Square",   proc { |w| toggle_expander_style(w) }),
	OptionMenuItem.new("Triangle", proc { |w| toggle_expander_style(w) }),
	OptionMenuItem.new("Circular", proc { |w| toggle_expander_style(w) }) ],
	1)
    hbox.pack_start(omenu2, false, true, 0)
    @tooltips.set_tip(omenu2, "The tree's expander style.", nil)

    omenu3 = build_option_menu(
      [ OptionMenuItem.new("Left",  proc { |w| toggle_justify(w) }),
	OptionMenuItem.new("Right", proc { |w| toggle_justify(w) }) ],
	0)
    hbox.pack_start(omenu3, false, true, 0)
    @tooltips.set_tip(omenu3, "The tree's justification.", nil)

    omenu4 = build_option_menu(
      [ OptionMenuItem.new("Single",   proc { |w| toggle_sel_mode(w) }),
	OptionMenuItem.new("Browse",   proc { |w| toggle_sel_mode(w) }),
	OptionMenuItem.new("Multiple", proc { |w| toggle_sel_mode(w) }),
	OptionMenuItem.new("Extended", proc { |w| toggle_sel_mode(w) }) ],
	3)
    hbox.pack_start(omenu4, false, true, 0)
    @tooltips.set_tip(omenu4, "The list's selection mode.", nil)

    realize

    @pixmap1, @mask1 = Gdk::Pixmap::create_from_xpm_d(window,
					    style.white,
					    $book_closed_xpm)
    @pixmap2, @mask2 = Gdk::Pixmap::create_from_xpm_d(window,
					    style.white,
					    $book_open_xpm)
    @pixmap3, @mask3 = Gdk::Pixmap::create_from_xpm_d(window,
					    style.white,
					    $mini_page_xpm)

    @ctree.set_usize(0, 300)
    frame = Gtk::Frame::new(nil)
    frame.border_width = 0
    frame.set_shadow_type(Gtk::SHADOW_OUT)
    vbox.pack_start(frame, false, true, 0)

    hbox = Gtk::HBox::new(true, 2)
    hbox.border_width = 2
    frame.add(hbox)

    frame = Gtk::Frame::new(nil)
    frame.set_shadow_type(Gtk::SHADOW_IN)
    hbox.pack_start(frame, false, true, 0)

    hbox2 = Gtk::HBox::new()
    hbox2.border_width = 2
    frame.add(hbox2)

    label = Gtk::Label::new("Books :")
    hbox2.pack_start(label, false, true, 0)

    @book_label = Gtk::Label::new(sprintf("%d", @books))
    hbox2.pack_end(@book_label, false, true, 5)

    frame = Gtk::Frame::new(nil)
    frame.set_shadow_type(Gtk::SHADOW_IN)
    hbox.pack_start(frame, false, true, 0)

    hbox2 = Gtk::HBox::new()
    hbox2.border_width = 2
    frame.add(hbox2)

    label = Gtk::Label::new("Pages :")
    hbox2.pack_start(label, false, true, 0)

    @page_label = Gtk::Label::new(sprintf("%d", @pages))
    hbox2.pack_end(@page_label, false, true, 5)

    frame = Gtk::Frame::new(nil)
    frame.set_shadow_type(Gtk::SHADOW_IN)
    hbox.pack_start(frame, false, true, 0)

    hbox2 = Gtk::HBox::new()
    hbox2.border_width = 2
    frame.add(hbox2)

    label = Gtk::Label::new("Selected :")
    hbox2.pack_start(label, false, true, 0)

    @sel_label = Gtk::Label::new("")
    hbox2.pack_start(@sel_label, false, true, 5)

    frame = Gtk::Frame::new(nil)
    frame.set_shadow_type(Gtk::SHADOW_IN)
    hbox.pack_start(frame, false, true, 0)

    hbox2 = Gtk::HBox::new()
    hbox2.border_width = 2
    frame.add(hbox2)

    label = Gtk::Label::new("Visible :")
    hbox2.pack_start(label, false, true, 0)

    @vis_label = Gtk::Label::new("")
    hbox2.pack_start(@vis_label, false, true, 5)

    rebuild

  end

  private
  def rebuild
    d = @spin1.get_value_as_int
    b = @spin2.get_value_as_int
    p = @spin3.get_value_as_int

    n = (b^d-1)/(b-1)*(p+1)
    if n > 100000 then
        printf("%d total items? Try less\n", n)
	return
    end

    @ctree.freeze
    @ctree.clear

    @books = 1
    @pages = 0

    parent = @ctree.insert_node(nil, nil, ["Root", ""], 5,
				@pixmap1, @mask1, @pixmap2, @mask2,
				false, true)

    style = Gtk::Style::new()
    style.set_base(Gtk::STATE_NORMAL, 0, 45000, 55000)
    @ctree.node_set_row_data(parent, style)

    if @ctree.line_style == Gtk::CTree::LINES_TABBED then
      @ctree.node_set_row_style(parent, style)
    end

    build_recursive(1, d, b, p, parent)
    @ctree.thaw
    after_press
  end

  private
  def build_recursive(cur_depth, depth, num_books, num_pages, parent)
    sibling = nil;
    (num_pages + num_books).step(num_books+1, -1) do |i|
      @pages+=1
      sibling = @ctree.insert_node(parent, sibling,
	[sprintf("Page %02d", rand(99)), sprintf("Item %d-%d", cur_depth, i)],
	5, @pixmap3, @mask3, nil, nil, true, false)

      if parent != nil && @ctree.line_style == Gtk::CTree::LINES_TABBED then
	@ctree.node_set_row_style(sibling, @ctree.node_get_row_style(parent))
      end
    end

    return self if cur_depth == depth

    num_books.step(1, -1) do |i|
      @books+=1
      sibling = @ctree.insert_node(parent, sibling,
	[sprintf("Book %02d", rand(99)), sprintf("Item %d-%d", cur_depth, i)],
	5, @pixmap1, @mask1, @pixmap2, @mask2, false, false)

      style = Gtk::Style::new()
      case cur_depth % 3
      when 0 then
	style.set_base(Gtk::STATE_NORMAL,
		       10000 * (cur_depth % 6),
		       0,
		       65535 - ((i * 10000) % 65535))
      when 1 then
	style.set_base(Gtk::STATE_NORMAL,
		       10000 * (cur_depth % 6),
		       65535 - ((i * 10000) % 65535),
		       0)
      else
	style.set_base(Gtk::STATE_NORMAL,
		       65535 - ((i * 10000) % 65535),
		       0,
		       10000 * (cur_depth % 6))
      end
      @ctree.node_set_row_data(sibling, style)
      if @ctree.line_style == Gtk::CTree::LINES_TABBED then
	@ctree.node_set_row_style(sibling, style)
      end
      build_recursive(cur_depth + 1, depth, num_books, num_pages, sibling)
    end
  end

  private
  def click_column(column)
    if column == @ctree.sort_column then
      if @ctree.sort_type == Gtk::SORT_ASCENDING then
	@ctree.sort_type = Gtk::SORT_DESCENDING
      else
	@ctree.sort_type = Gtk::SORT_ASCENDING
      end
    else
      @ctree.sort_column = column
    end
    @ctree.sort_recursive(nil)
  end

  private
  def after_press
    rows = 0
    @ctree.each_selection do rows += 1 end

    @sel_label.set_text(sprintf("%d", rows))
    @vis_label.set_text(sprintf("%d", @ctree.rows))
    @book_label.set_text(sprintf("%d", @books))
    @page_label.set_text(sprintf("%d", @pages))
  end

  private
  def after_move(child, parent, sibling)
    info = @ctree.get_node_info(child)
    source = info[0]

    if parent == nil then
      target1 = nil
    else
      info = @ctree.get_node_info(parent)
      target1 = info[0]
    end
    if sibling == nil then
      target2 = nil
    else
      info = @ctree.get_node_info(sibling)
      target2 = info[0]
    end

    printf("Moving \"%s\" to \"%s\" with sibling \"%s\".\n",
	   source, target1, target2)
  end

  private
  def change_indent(adj)
    @ctree.set_indent(adj.value)
  end

  private
  def change_spacing(adj)
    @ctree.set_spacing(adj.value)
  end

  private
  def change_row_height(adj)
    @ctree.set_row_height(adj.value)
  end

  private
  def expand_all
    @ctree.expand_recursive(nil)
    after_press
  end

  private
  def collapse_all
    @ctree.collapse_recursive(nil)
    after_press
  end

  private
  def change_style
    row = @ctree.focus_row
    row = 0 if row < 0

    node = @ctree.node_nth(row)
    return if node == nil;

    @ctree.node_set_cell_style(node, 1, @style1)
    @ctree.node_set_cell_style(node, 0, @style2)
    
    if node.children != nil then
      @ctree.node_set_row_style(node.children, @style2) 
    end
  end

  def export
    if @export_window == nil then
      @export_window = SampleWindow::new()
      @export_window.signal_connect("destroy") do @export_window.destroy end
      @export_window.set_title("exported ctree")
      @export_window.border_wirth = 5

      vbox = Gtk::VBox::new()
      @export_window.add(vbox)

      button = Gtk::Button::new("Close")
      vbox.pack_end(button, false, true)
      button.signal_connect("clicked") do @export_window.destroy end

      sep = Gtk::HSeparator::new()
      vbox.pack_end(sep, false, true, 10)

      export_ctree = Gtk::CTree::new(["Tree", "Info"], 0)
      export_ctree.line_style = Gtk::CTree::LINES_DOTTED

      scrolled_win = Gtk::ScrolledWindow::new()
      scrolled_win.add(export_ctree)
      scrolled_win.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
      vbox.pack_start(scrolled_win)
      export_ctree.selection_mode = Gtk::SELECTION_EXTENDED
      export_ctree.set_column_width(0, 200)
      export_ctree.set_column_width(1, 200)
      export_ctree.set_usize(300, 200)
    end

    if export_window.visible? == false then
      export_window.show_all
    end
    export_ctree.clear

    node = @ctree.node_nth(@ctree.focus_row)
    return if node == nil

#    gnode = gtk_ctree_export_to_gnode(nil, nil, node,
#				       ctree2gnode, nil);
#    if gnode != nil then
#      @ctree.insert_gnode (export_ctree, NULL, NULL, gnode,
#			      gnode2ctree, NULL);
#      gnode.destroy
#    end
  end

  private
  def select_all
    @ctree.select_recursive(nil)
  end

  private
  def unselect_all
    @ctree.unselect_recursive(nil)
  end

  private
  def remove_selection
    @ctree.freeze
    @ctree.each_selection do |node|
      if node.leaf? then
	@pages -= 1
      else
	@ctree.post_recursive(node) do |n|
	  if n.leaf? then
	    @pages -= 1
	  else
	    @books -= 1
	  end
        end
      end
      @ctree.remove_node(node)
    end
    if @ctree.selection_mode == Gtk::SELECTION_EXTENDED &&
       @ctree.focus_row >= 0 then
      node = @ctree.node_nth(@ctree.focus_row)
      if node != nil then
	@ctree.select(node)
      end
    end
    @ctree.thaw
    after_press
  end

  private
  def toggle_reorderable(cb)
    @ctree.set_reorderable(cb.active?)
  end

  private
  def toggle_line_style(rmitem)
    return unless rmitem.mapped?

    i = 3
    group = rmitem.group
    group.each do |g|
      break if g.active?
      i -= 1
    end
    if (@ctree.line_style == Gtk::CTree::LINES_TABBED &&
	i != Gtk::CTree::LINES_TABBED) ||
       (@ctree.line_style != Gtk::CTree::LINES_TABBED &&
	i == Gtk::CTree::LINES_TABBED) then
      @ctree.pre_recursive(nil) do |n|
        style = nil
	if @ctree.line_style != Gtk::CTree::LINES_TABBED then
	  if !n.leaf? then
	    style = @ctree.node_get_row_data(n)
          elsif n.parent != nil then
	    style = @ctree.node_get_row_data(n.parent)
          end
	end
	@ctree.node_set_row_style(n, style)
      end
    end
    @ctree.line_style = i;
    @line_style = i;
  end

  def toggle_expander_style(rmitem)
    return unless rmitem.mapped?

    i = 3
    group = rmitem.group
    group.each do |g|
      break if g.active?
      i -= 1
    end

    @ctree.set_expander_style(i)
  end

  def toggle_justify(rmitem)
    return unless rmitem.mapped?

    i = 1
    group = rmitem.group
    group.each do |g|
      break if g.active?
      i -= 1
    end

    @ctree.set_column_justification(@ctree.tree_column, i)
  end

  def toggle_sel_mode(rmitem)
    return unless rmitem.mapped?

    i = 3
    group = rmitem.group
    group.each do |g|
      break if g.active?
      i -= 1
    end

    @ctree.set_selection_mode(i)
    after_press
  end

end
