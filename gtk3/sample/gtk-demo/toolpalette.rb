# Copyright (c) 2017 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Tool Palette

 A tool palette widget shows groups of toolbar items as a grid of icons
 or a list of names.
=end
class ToolpaletteDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Tool Palette"
    @window.set_default_size(200, 600)
    @canvas_items = []

    # Add widgets to control the ToolPalette appearance
    @box = Gtk::Box.new(:vertical, 6)
    @box.margin = 6
    @window.add(@box)

    create_orientation_combobox
    create_style_combobox

    # Add hbox.
    @hbox = Gtk::Box.new(:horizontal, 5)
    @box.pack_start(@hbox, :expand => true, :fill => true, :padding => 0)

    create_and_fill_the_toolpalette
    add_comboboxes_signals

    # Notebook
    create_and_fill_the_notebook
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  private

  def create_style_combobox
    style_model = Gtk::ListStore.new(String, Integer)
    iter = style_model.append
    iter[0] = "Text"
    iter[1] = Gtk::ToolbarStyle::TEXT.to_i
    iter = style_model.append
    iter[0] = "Both"
    iter[1] = Gtk::ToolbarStyle::BOTH.to_i
    iter = style_model.append
    iter[0] = "Both: Horizontal"
    iter[1] = Gtk::ToolbarStyle::BOTH_HORIZ.to_i
    iter = style_model.append
    iter[0] = "Icons"
    iter[1] = Gtk::ToolbarStyle::ICONS.to_i
    iter = style_model.append
    iter[0] = "Default"
    iter[1] = -1 # A custom meaning for this demo.
    @combo_style = Gtk::ComboBox.new(:model => style_model)
    cell_renderer = Gtk::CellRendererText.new
    @combo_style.pack_start(cell_renderer, true)
    @combo_style.set_attributes(cell_renderer, "text" => 0)
    @combo_style.active_iter = iter
    @box.pack_start(@combo_style, :expand => false,
                                  :fill => false,
                                  :padding => 0)
  end

  def create_orientation_combobox
    orientation_model = Gtk::ListStore.new(String, Integer)
    iter = orientation_model.append
    iter[0] = "Horizontal"
    iter[1] = Gtk::Orientation::HORIZONTAL
    iter = orientation_model.append
    iter[0] = "Vertical"
    iter[1] = Gtk::Orientation::VERTICAL

    @combo_orientation = Gtk::ComboBox.new(:model => orientation_model)
    cell_renderer = Gtk::CellRendererText.new
    @combo_orientation.pack_start(cell_renderer, true)
    @combo_orientation.set_attributes(cell_renderer, "text" => 0)
    @combo_orientation.active_iter = iter
    @box.pack_start(@combo_orientation, :expand => false,
                                        :fill => false,
                                        :padding => 0)
  end

  def create_and_fill_the_toolpalette
    @palette = Gtk::ToolPalette.new
    load_icon_items
    load_toggle_items
    load_special_items

    palette_scroller = Gtk::ScrolledWindow.new
    palette_scroller.set_policy(:never, :automatic)
    palette_scroller.margin = 6
    palette_scroller.hexpand = true
    palette_scroller.add(@palette)
    @hbox.add(palette_scroller)
    @box.show_all
    add_dnd_for_tool_items_to_palette
  end

  def add_dnd_for_tool_items_to_palette
    @palette.signal_connect "drag-data-received" do |widget, context, x, y|
      drag_item = @drag_palette.drag_item
      drop_group = widget.get_drop_group(x, y)

      if drag_item.class == Gtk::ToolItemGroup
        palette_drop_group(drag_palette, drag_item, drop_group)
      elsif drag_item.class == Gtk::ToolButton && drop_group
        allocation = drop_group.allocation
        palette_drop_item(drag_item, drop_group, x - allocation.x, y - allocation.y)
      end
    end

    @palette.add_drag_dest(@palette,
                           Gtk::DestDefaults::ALL,
                           [Gtk::ToolPaletteDragTargets::ITEMS,
                            Gtk::ToolPaletteDragTargets::GROUPS],
                           Gdk::DragAction::COPY)
  end

  def add_comboboxes_signals
    @combo_orientation.signal_connect "changed" do |widget|
      on_combo_orientation_changed(widget)
    end

    @combo_style.signal_connect "changed" do |widget|
      on_combo_style_changed(widget)
    end

    # Kepp the widgets in sync
    on_combo_orientation_changed(@combo_orientation)
  end

  def create_passive_contents
    @passive_contents = Gtk::DrawingArea.new
    @passive_contents.signal_connect "draw" do |widget, cr|
      canvas_draw(widget, cr)
    end
    @passive_contents.signal_connect "drag-data-received" do |widget, _context, x, y, selection, _info, _time|
      tool_item = @palette.get_drag_item(selection)

      # append a new canvas item when a tool button was found
      if tool_item.class == Gtk::ToolButton
        canvas_item = CanvasItem.new(widget, tool_item, x, y)
        @canvas_items << canvas_item
        widget.queue_draw
      end
    end
  end

  def create_interactive_contents
    @interactive_contents = Gtk::DrawingArea.new
    @interactive_contents.signal_connect "draw" do |widget, cr|
      canvas_draw(widget, cr)
    end

    @interactive_contents.signal_connect "drag-motion" do |widget, context, x, y, time|
      if @drop_item
        # already have a drop indicator - just update position
        @drop_item.x = x
        @drop_item.y = y
        widget.queue_draw
        Gdk.drag_status(context, Gdk::DragAction::COPY, time)
      else
        # request DnD data for creating a drop indicator
        target = widget.drag_dest_find_target(context, nil)
        return false unless target
        @drag_data_requested_for_drop = false
        widget.drag_get_data(context, target, time)
      end
      true
    end

    @interactive_contents.signal_connect "drag-data-received" do |widget, context, x, y, selection, _info, time|
      # find the tool button which is the source of this DnD operation
      tool_item = @palette.get_drag_item(selection)

      if tool_item.class == Gtk::ToolButton
        item = CanvasItem.new(widget, tool_item, x, y)

        # Either create a new item or just create a preview item, depending on
        # why the drag data was requested.
        if @drag_data_requested_for_drop
          @canvas_items << item
          context.finish(:success => true, :delete => false, :time => time)
          @drop_item = nil
        else
          @drop_item = item
          Gdk.drag_status(context, Gdk::DragAction::COPY, time)
        end
        widget.queue_draw
      end
    end

    @interactive_contents.signal_connect "drag-leave" do |widget|
      @drop_item = nil if @drop_item
      widget.queue_draw
    end

    @interactive_contents.signal_connect "drag-drop" do |widget, context, _x, _y, time|
      target = widget.drag_dest_find_target(context, nil)
      if target
        @drag_data_requested_for_drop = true
        widget.drag_get_data(context, target, time)
      end
      false
    end
  end

  def create_and_fill_the_notebook
    notebook = Gtk::Notebook.new
    notebook.margin = 6
    @hbox.pack_end(notebook, :expand => false, :fill => false, :padding => false)

    # Passive DnD dest
    create_passive_contents
    @palette.add_drag_dest(@passive_contents, Gtk::DestDefaults::ALL,
                                     [Gtk::ToolPaletteDragTargets::ITEMS],
                                     Gdk::DragAction::COPY)
    contents_scroller = Gtk::ScrolledWindow.new
    contents_scroller.set_policy(:automatic, :always)
    contents_scroller.add(@passive_contents)
    notebook.append_page(contents_scroller, Gtk::Label.new("Passive DnD Mode"))
    contents_scroller.margin = 6

    # Interactive DnD dest
    create_interactive_contents
    @palette.add_drag_dest(@interactive_contents, Gtk::DestDefaults::HIGHLIGHT,
                           [Gtk::ToolPaletteDragTargets::ITEMS],
                           Gdk::DragAction::COPY)
    contents_scroller = Gtk::ScrolledWindow.new
    contents_scroller.set_policy(:automatic, :always)
    contents_scroller.add(@interactive_contents)
    notebook.append_page(contents_scroller, Gtk::Label.new("Interactive DnD Mode"))
  end

  def canvas_draw(widget, cr)
    cr.set_source_rgb(1, 1, 1)
    cr.paint

    @canvas_items.each do |canvas_item|
      canvas_item.draw(cr, false)
    end

    @drop_item.draw(cr, true) if @drop_item
  end

  def palette_drop_group(drag_palette, drag_group, drop_group)
    drop_position = -1

    drop_position = drag_palette.get_group_position(drop_group) if drop_group
    drag_palette.set_group_position(drag_group, drop_position)
  end

  def palette_drop_item(drag_item, drop_group, x, y)
    drag_group = drag_item.parent
    drop_item = drop_group.get_drop_item(x, y)
    drop_position = -1

    drop_position = drop_gropu.get_item_position(drop_item) if drop_item

    if drag_group != drop_group
      homogeneous = drag_group.child_get_property(drag_item, "homogeneous")
      expand = drag_group.child_get_property(drag_item, "expand")
      fill = drag_group.child_get_property(drag_item, "fill")
      new_row = drag_group.child_get_property(drag_item, "new-row")
      drop_group.insert(drag_item, drop_position)
      drop_group.child_set(drag_item, "homogeneous" => homogeneous,
                                      "expand" => expand,
                                      "fill" => fill,
                                      "new-row" => new_row)
    else
      drop_group.set_item_position(drag_item, drop_position)
    end
  end

  def on_combo_orientation_changed(combo_box)
    sw = @palette.parent
    iter = combo_box.active_iter
    return unless iter

    @palette.orientation = iter[1]

    if @palette.orientation == Gtk::Orientation::HORIZONTAL
      sw.set_policy(:automatic, :never)
    else
      sw.set_policy(:never, :automatic)
    end
  end

  def on_combo_style_changed(combo_box)
    iter = combo_box.active_iter
    return unless iter

    if iter[1] == -1
      @palette.unset_style
    else
      @palette.style = iter[1]
    end
  end

  def load_icon_items
    max_icons = 10
    icons_count = 0
    icon_theme = Gtk::IconTheme.get_for_screen(@palette.screen)
    contexts = icon_theme.contexts
    contexts.each do |context|
      group = Gtk::ToolItemGroup.new(context)
      @palette.add(group)

      next if context == "Animations"
      puts "Got context '#{context}"

      icon_names = icon_theme.icons(context)
      icon_names.sort

      icon_names.each do |icon_name|
        next if icon_name == "emblem-desktop"
        next if icon_name =~ /^.*-symbolic$/
        puts "Got icon name '#{icon_name}'"

        item = Gtk::ToolButton.new
        item.icon_name = icon_name
        item.tooltip_text = icon_name
        group.insert(item, -1)

        # Prevent us having an insane number of icons
        icons_count += 1
        break if icons_count >= max_icons
      end
    end
  end

  def load_toggle_items
    group = Gtk::ToolItemGroup.new("Radio Item")
    @palette.add(group)

    toggle_group = nil

    (1..10).each do |i|
      label = "##{i}"
      item = Gtk::RadioToolButton.new(toggle_group)
      item.label = label
      group .insert(item, -1)
      toggle_group = item.group
    end
  end

  def create_entry_item(text)
    entry = Gtk::Entry.new
    entry.text = text
    entry.width_chars = 5

    item = Gtk::ToolItem.new
    item.add(entry)
    item
  end

  def load_special_items
    group = Gtk::ToolItemGroup.new("Advanced Features group")
    label_button = Gtk::Button.new(:label => "Advanced Features")
    label_button.show
    group.label_widget = label_button
    @palette.add(group)

    item = create_entry_item("homogeneous=FALSE")
    group.insert(item, -1)
    group.child_set_property(item, "homogeneous", false)

    item = create_entry_item("homogeneous=FALSE, expand=TRUE")
    group.insert(item, -1)
    group.child_set_property(item, "homogeneous", false)
    group.child_set_property(item, "expand", true)

    item = create_entry_item("homogeneous=FALSE, expand=TRUE, fill=FALSE")
    group.insert(item, -1)
    group.child_set_property(item, "homogeneous", false)
    group.child_set_property(item, "expand", true)
    group.child_set_property(item, "fill", false)

    item = create_entry_item("homogeneous=FALSE, expand=TRUE, new-row=TRUE")
    group.insert(item, -1)
    group.child_set_property(item, "homogeneous", false)
    group.child_set_property(item, "expand", true)
    group.child_set_property(item, "new-row", true)

    item = Gtk::ToolButton.new
    item.icon_name = "go-up"
    item.tooltip_text = "Show on vertical palettes only"
    group.insert(item, -1)
    item.visible_horizontal = false

    item = Gtk::ToolButton.new
    item.icon_name = "go-next"
    item.tooltip_text = "Show on horizontal palettes only"
    group.insert(item, -1)
    item.visible_vertical = false

    item = Gtk::ToolButton.new
    item.icon_name = "edit-delete"
    item.tooltip_text = "Do not show at all"
    group.insert(item, -1)
    item.no_show_all = true

    item = Gtk::ToolButton.new
    item.icon_name = "view-fullscreen"
    item.tooltip_text = "Expanded this item"
    group.insert(item, -1)
    group.child_set_property(item, "homogeneous", false)
    group.child_set_property(item, "expand", true)

    item = Gtk::ToolButton.new
    item.icon_name = "help-browser"
    item.tooltip_text = "A regular item"
    group.insert(item, -1)
  end
end

class CanvasItem
  attr_reader :pixbuf
  attr_accessor :x, :y

  def initialize(widget, button, x, y)
    icon_name = button.icon_name
    icon_theme = Gtk::IconTheme.get_for_screen(widget.screen)
    width, _height = Gtk::IconSize.lookup(:dialog)
    @pixbuf = icon_theme.load_icon(icon_name,
                                   width,
                                   Gtk::IconLookupFlags::GENERIC_FALLBACK)
    return nil unless @pixbuf
    @x = x
    @y = y
  end

  def draw(cr, preview)
    cx = @pixbuf.width
    cy = @pixbuf.height
    cr.set_source_pixbuf(@pixbuf, @x - cx * 0.5, @y - cy * 0.5)

    if preview
      cr.paint(0.6)
    else
      cr.paint
    end
  end
end
