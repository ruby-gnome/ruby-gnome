=begin header

  tree.rb - a part of testgtk.c rewritten in ruby-gtk

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2002/05/19 12:29:45 $
  $Id: tree.rb,v 1.1 2002/05/19 12:29:45 mutoh Exp $

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

class TreeSample < SampleWindow

  # macro, structure and variables used by tree window demos 
  DEFAULT_NUMBER_OF_ITEM = 3
  DEFAULT_RECURSION_LEVEL = 3

# struct {
#   GSList* selection_mode_group
#   GtkWidget* single_button
#   GtkWidget* browse_button
#   GtkWidget* multiple_button
#   GtkWidget* draw_line_button
#   GtkWidget* view_line_button
#   GtkWidget* no_root_item_button
#   GtkWidget* nb_item_spinner
#   GtkWidget* recursion_spinner
# } sTreeSampleSelection
  TreeSampleSelection = Struct.new("TreeSampleSelection",
                                   :selection_mode_group,
                                   :single_button,
                                   :browse_button,
                                   :multiple_button,
                                   :draw_line_button,
                                   :view_line_button,
                                   :no_root_item_button,
                                   :nb_item_spinner,
                                   :recursion_spinner)

# typedef struct sTreeButtons {
#   guint nb_item_add
#   GtkWidget* add_button
#   GtkWidget* remove_button
#   GtkWidget* subtree_button
# } sTreeButtons
  TreeButtons = Struct.new("TreeButtons",
                           :nb_item_add,
                           :add_button,
                           :remove_button,
                           :subtree_button)

# def
# cb_tree_destroy_event(GtkWidget* w)
#   sTreeButtons* tree_buttons

#   # free buttons structure associate at this tree 
#   tree_buttons = w.get_user_data
#   free(tree_buttons)
# end


  class TreeSampleWindow < Gtk::Window
    def cb_add_new_item(tree)
# cb_add_new_item(GtkWidget* w, GtkTree* tree)
#   sTreeButtons* tree_buttons
#   GList* selected_list
#   GtkWidget* selected_item
#   GtkWidget* subtree
#   GtkWidget* item_new
#   char buffer[255]

      #tree_buttons = tree.get_user_data
      selected_list = tree.selection
      #p(selected_list)

      if selected_list.empty?
        # there is no item in tree 
        subtree = tree
      else
        # list can have only one element 
        selected_item = selected_list[0]
        subtree = selected_item.subtree
        if subtree.nil?
          # current selected item have not subtree ... create it 
          subtree = Gtk::Tree::new()
          selected_item.set_subtree(subtree)
        end
      end

      # at this point, we know which subtree will be used to add new item 
      # create a new item 
      buffer = sprintf("item add %d", @tree_buttons.nb_item_add)
      item_new = Gtk::TreeItem::new(buffer)
      subtree.append(item_new)
      item_new.show

      @tree_buttons.nb_item_add += 1
    end

    def cb_remove_item(tree)
# cb_remove_item(GtkWidget*w, GtkTree* tree)
#   GList* selected_list
#   GList* clear_list
  
      selected_list = tree.selection
#      clear_list = nil
    
#      until selected_list.nil?
#        clear_list = g_list_prepend (clear_list, selected_list.data)
#        selected_list = selected_list.next
#      end  

#      clear_list = g_list_reverse (clear_list)
#      tree.remove_items(clear_list)
      tree.remove_items(selected_list)
      p(selected_list)

#      g_list_free (clear_list)
    end

    def cb_remove_subtree(tree)
# cb_remove_subtree(GtkWidget*w, GtkTree* tree)
#   GList* selected_list
#   GtkTreeItem *item
  
      selected_list = tree.selection

      if not selected_list.empty?
        item = selected_list[0]
        item.remove_subtree unless item.subtree.nil?
      end
    end

    def cb_tree_changed(tree)
# cb_tree_changed(GtkTree* tree)
#   sTreeButtons* tree_buttons
#   GList* selected_list
#   guint nb_selected

      selected_list = tree.selection
      nb_selected = selected_list.length

      if nb_selected == 0
        if tree.children == nil
          @tree_buttons.add_button.set_sensitive(true)
        else
          @tree_buttons.add_button.set_sensitive(false)
        end
        @tree_buttons.remove_button.set_sensitive(false)
        @tree_buttons.subtree_button.set_sensitive(false)
      else 
        @tree_buttons.remove_button.set_sensitive(true)
        @tree_buttons.add_button.set_sensitive(nb_selected == 1)
        @tree_buttons.subtree_button.set_sensitive(nb_selected == 1)
      end
    end

    def create_subtree(item, level, nb_item_max, recursion_level_max)
      #create_subtree(GtkWidget* item, guint level, guint nb_item_max, guint recursion_level_max)
      #   GtkWidget* item_subtree
      #   GtkWidget* item_new
      #   guint nb_item
      #   char buffer[255]
      #   int no_root_item
      #p([item, level, nb_item_max, recursion_level_max])

      return if level == recursion_level_max

      if level == -1
        # query with no root item 
        level = 0
        item_subtree = item
        no_root_item = true
      else
        # query with no root item 
        # create subtree and associate it with current item 
        item_subtree = Gtk::Tree::new()
        no_root_item = false
      end
  
      for nb_item in 0..(nb_item_max-1)
        buffer = sprintf("item %d-%d", level, nb_item)
        item_new = Gtk::TreeItem::new(buffer)
        item_subtree.append(item_new)
        create_subtree(item_new, level+1, nb_item_max, recursion_level_max)
        item_new.show
      end

      if not no_root_item
        item.set_subtree(item_subtree)
      end
    end

    def initialize(selection_mode,
                   draw_line, view_line, no_root_item,
                   nb_item_max, recursion_level_max)
      super(Gtk::WINDOW_TOPLEVEL)
      set_title("Tree Sample")
      #signal_connect("destroy") do cb_tree_destroy_event end

      # create tree buttons struct 
      @tree_buttons = TreeButtons.new
      @tree_buttons.nb_item_add = 0
      box1 = Gtk::VBox::new(false, 0)
      add(box1)
      box1.show

      # create tree box 
      box2 = Gtk::VBox::new(false, 0)
      box1.pack_start(box2, true, true, 0)
      box2.border_width(5)
      box2.show

      # create scrolled window 
      scrolled_win = Gtk::ScrolledWindow::new(nil, nil)
      scrolled_win.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
      box2.pack_start(scrolled_win, true, true, 0)
      scrolled_win.set_usize(200, 200)
      scrolled_win.show
  
      # create root tree widget 
      root_tree = Gtk::Tree::new()
      root_tree.signal_connect("selection_changed") do
        cb_tree_changed(root_tree)
      end
      #root_tree.set_user_data(@tree_buttons)
      scrolled_win.add_with_viewport(root_tree)
      root_tree.set_selection_mode(selection_mode)
      root_tree.set_view_lines(draw_line)
      root_tree.set_view_mode(view_line)
      root_tree.show

      if no_root_item
        # set root tree to subtree function with root item variable 
        root_item = root_tree
      else
        # create root tree item widget 
        root_item = Gtk::TreeItem::new("root item")
        root_tree.append(root_item)
        root_item.show
      end
      root_item_num = if no_root_item then 1 else 0 end
      create_subtree(root_item, root_item_num, nb_item_max, 
                     recursion_level_max)

      box2 = Gtk::VBox::new(false, 0)
      box1.pack_start(box2, false, false, 0)
      box2.border_width(5)
      box2.show

      button = Gtk::Button::new("Add Item")
      button.set_sensitive(false)
      button.signal_connect("clicked") do cb_add_new_item(root_tree) end
      box2.pack_start(button, true, true, 0)
      button.show
      @tree_buttons.add_button = button

      button = Gtk::Button::new("Remove Item(s)")
      button.set_sensitive(false)
      button.signal_connect("clicked") do cb_remove_item(root_tree) end
      box2.pack_start(button, true, true, 0)
      button.show
      @tree_buttons.remove_button = button

      button = Gtk::Button::new("Remove Subtree")
      button.set_sensitive(false)
      button.signal_connect("clicked") do cb_remove_subtree(root_tree) end
      box2.pack_start(button, true, true, 0)
      button.show
      @tree_buttons.subtree_button = button
    
      # create separator 
      separator = Gtk::HSeparator::new()
      box1.pack_start(separator, false, false, 0)
      separator.show
    
      # create button box 
      box2 = Gtk::VBox::new(false, 0)
      box1.pack_start(box2, false, false, 0)
      box2.border_width(5)
      box2.show
      
      button = Gtk::Button::new("Close")
      box2.pack_start(button, true, true, 0)
      button.signal_connect("clicked") do destroy end
      button.show
    end
  end

  def create_tree_sample(selection_mode,
                         draw_line, view_line, no_root_item,
                         nb_item_max, recursion_level_max)
# create_tree_sample(guint selection_mode, 
# 		   guint draw_line, guint view_line, guint no_root_item,
# 		   guint nb_item_max, guint recursion_level_max) 
#   GtkWidget* window
#   GtkWidget* box1
#   GtkWidget* box2
#   GtkWidget* separator
#   GtkWidget* button
#   GtkWidget* scrolled_win
#   GtkWidget* root_tree
#   GtkWidget* root_item
#   sTreeButtons* tree_buttons

    # create top level window 
    #sample_window = Gtk::Window::new(Gtk::WINDOW_TOPLEVEL)
    sample_window = TreeSampleWindow::new(selection_mode,
                                          draw_line, view_line, no_root_item,
                                          nb_item_max, recursion_level_max)
    sample_window.show
  end

  #def cb_create_tree(GtkWidget* w)
  def cb_create_tree
#   guint selection_mode = Gtk::SELECTION_SINGLE
#   guint view_line
#   guint draw_line
#   guint no_root_item
#   guint nb_item
#   guint recursion_level

    # get selection mode choice 
    if @sTreeSampleSelection.single_button.active?
      selection_mode = Gtk::SELECTION_SINGLE
    else
      if @sTreeSampleSelection.browse_button.active?
        selection_mode = Gtk::SELECTION_BROWSE
     else
        selection_mode = Gtk::SELECTION_MULTIPLE
      end
    end

    # get options choice 
    draw_line = @sTreeSampleSelection.draw_line_button.active?
    view_line = if @sTreeSampleSelection.view_line_button.active?
		  Gtk::Tree::VIEW_LINE
		else
		  Gtk::Tree::VIEW_ITEM
		end
    no_root_item = @sTreeSampleSelection.no_root_item_button.active?
    
    # get levels 
    nb_item = @sTreeSampleSelection.nb_item_spinner.get_value_as_int
    recursion_level = @sTreeSampleSelection.recursion_spinner.get_value_as_int

    items_num = nb_item ** recursion_level
    if (items_num) > 10000
       printf("%d total items? That will take a very long time. Try less\n",
              items_num)
      return
    end

    create_tree_sample(selection_mode,
                       draw_line, view_line, no_root_item, nb_item,
                       recursion_level)
  end

  def initialize
    super("tree")
# void 
# create_tree_mode_window(void)
#   static GtkWidget* window
#   GtkWidget* box1
#   GtkWidget* box2
#   GtkWidget* box3
#   GtkWidget* box4
#   GtkWidget* box5
#   GtkWidget* button
#   GtkWidget* frame
#   GtkWidget* separator
#   GtkWidget* label
#   GtkWidget* spinner
#   GtkAdjustment *adj

    super("Tree Mode Selection Window")
#       window.signal_connect("destroy",
# 			  gtk_widget_destroyed,
# 			  window)

    @sTreeSampleSelection = TreeSampleSelection.new

    box1 = Gtk::VBox::new(false, 0)
    add(box1)
    box1.show

    # create upper box - selection box 
    box2 = Gtk::VBox::new(false, 5)
    box1.pack_start(box2, true, true, 0)
    box2.border_width(5)
    box2.show

    box3 = Gtk::HBox::new(false, 5)
    box2.pack_start(box3, true, true, 0)
    box3.show

    # create selection mode frame 
    frame = Gtk::Frame::new("Selection Mode")
    box3.pack_start(frame, true, true, 0)
    frame.show

    box4 = Gtk::VBox::new(false, 0)
    frame.add(box4)
    box4.border_width(5)
    box4.show
    
    # create radio button
    button = Gtk::RadioButton::new(nil, "SINGLE")
    box4.pack_start(button, true, true, 0)
    button.show
    @sTreeSampleSelection.single_button = button

    button = Gtk::RadioButton::new(button.group, "BROWSE")
    box4.pack_start(button, true, true, 0)
    button.show
    @sTreeSampleSelection.browse_button = button

    button = Gtk::RadioButton::new(button.group, "MULTIPLE")
    box4.pack_start(button, true, true, 0)
    button.show
    @sTreeSampleSelection.multiple_button = button

    @sTreeSampleSelection.selection_mode_group = button.group

    # create option mode frame 
    frame = Gtk::Frame::new("Options")
    box3.pack_start(frame, true, true, 0)
    frame.show

    box4 = Gtk::VBox::new(false, 0)
    frame.add(box4)
    box4.border_width(5)
    box4.show

    # create check button 
    button = Gtk::CheckButton::new("Draw line")
    box4.pack_start(button, true, true, 0)
    button.set_state(true)
    button.show
    @sTreeSampleSelection.draw_line_button = button
  
    button = Gtk::CheckButton::new("View Line mode")
    box4.pack_start(button, true, true, 0)
    button.set_state(true)
    button.show
    @sTreeSampleSelection.view_line_button = button
 
    button = Gtk::CheckButton::new("Without Root item")
    box4.pack_start(button, true, true, 0)
    button.show
    @sTreeSampleSelection.no_root_item_button = button

    # create recursion parameter 
    frame = Gtk::Frame::new("Size Parameters")
    box2.pack_start(frame, true, true, 0)
    frame.show

    box4 = Gtk::HBox::new(false, 5)
    frame.add(box4)
    box4.border_width(5)
    box4.show

    # create number of item spin button 
    box5 = Gtk::HBox::new(false, 5)
    box4.pack_start(box5, false, false, 0)
    box5.show

    label = Gtk::Label::new("Number of Item")
    label.set_alignment(0, 0.5)
    box5.pack_start(label, false, true, 0)
    label.show

    adj = Gtk::Adjustment::new(DEFAULT_NUMBER_OF_ITEM, 1.0, 255.0, 1.0,
                               5.0, 0.0)
    spinner = Gtk::SpinButton::new(adj, 0, 0)
    box5.pack_start(spinner, false, true, 0)
    spinner.show
    @sTreeSampleSelection.nb_item_spinner = spinner
  
    # create recursion level spin button 
    box5 = Gtk::HBox::new(false, 5)
    box4.pack_start(box5, false, false, 0)
    box5.show

    label = Gtk::Label::new("Depth Level")
    label.set_alignment(0, 0.5)
    box5.pack_start(label, false, true, 0)
    label.show

    adj = Gtk::Adjustment::new(DEFAULT_RECURSION_LEVEL, 0.0, 255.0, 1.0,
                               5.0, 0.0)
    spinner = Gtk::SpinButton::new(adj, 0, 0)
    box5.pack_start(spinner, false, true, 0)
    spinner.show
    @sTreeSampleSelection.recursion_spinner = spinner
  
    # create horizontal separator 
    separator = Gtk::HSeparator::new()
    box1.pack_start(separator, false, false, 0)
    separator.show

    # create bottom button box 
    box2 = Gtk::HBox::new(false, 0)
    box1.pack_start(box2, false, false, 0)
    box2.border_width(5)
    box2.show

    button = Gtk::Button::new("Create Tree Sample")
    box2.pack_start(button, true, true, 0)
    button.signal_connect("clicked") do cb_create_tree end
    button.show

    button = Gtk::Button::new("Close")
    box2.pack_start(button, true, true, 0)
    button.signal_connect("clicked") do destroy end
    button.show
  
  end
end
