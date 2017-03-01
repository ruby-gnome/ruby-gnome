# Copyright (c) 2017 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Touch and Drawing Tablets

 Demonstrates advanced handling of event information from exotic
 input devices.

 On one hand, this snippet demonstrates management of drawing tablets,
 those contain additional information for the pointer other than
 X/Y coordinates. Tablet pads events are mapped to actions, which
 are both defined and interpreted by the application.

 Input axes are dependent on hardware devices, on linux/unix you
 can see the device axes through xinput list <device>. Each time
 a different hardware device is used to move the pointer, the
 master device will be updated to match the axes it provides,
 these changes can be tracked through GdkDevice::changed, or
 checking gdk_event_get_source_device().

 On the other hand, this demo handles basic multitouch events,
 each event coming from an specific touchpoint will contain a
 GdkEventSequence that's unique for its lifetime, so multiple
 touchpoints can be tracked.
=end
class EventAxesDemo
  AXES_INFO = Struct.new(:last_source,
                         :last_tool,
                         :axes,
                         :color,
                         :x,
                         :y)

  EVENT_DATA = Struct.new(:pointer_info, :touch_info)

  COLORS = %w(black orchid fuchsia indigo thistle sienna azure plum
              lime navy maroon burlywood).freeze
  PAD_ACTION_RESULTS = %w(☢ ♨ ☼ ☄ ⚡ 💫 ◑ ⚛).freeze
  # type index mode label action_name
  PAD_ACTIONS =
    [
      [:button, 1, -1, "Nuclear strike", "pad.nuke"],
      [:button, 2, -1, "Release siberian methane reserves", "pad.heat"],
      [:button, 3, -1, "Release solar flare", "pad.fry"],
      [:button, 4, -1, "De-stabilize Oort cloud", "pad.fall"],
      [:button, 5, -1, "Ignite WR-104", "pad.burst"],
      [:button, 6, -1, "Lart whoever asks about this button", "pad.lart"],
      [:ring,  -1, -1, "Earth axial tilt", "pad.tilt"],
      [:strip, -1, -1, "Extent of weak nuclear force", "pad.dissolve"]
    ].freeze

  def initialize(main_window)
    @cur_color = 0
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Event Axes"
    @window.set_default_size(400, 400)

    box = Gtk::EventBox.new
    @window.add(box)
    box.support_multidevice = true
    box.add_events([:pointer_motion_mask, :button_press_mask,
                    :button_release_mask, :smooth_scroll_mask,
                    :enter_notify_mask, :leave_notify_mask,
                    :touch_mask])

    @event_data = EVENT_DATA.new({}, {})

    box.signal_connect "event" do |widget, event|
      update_axes_from_event(event)
      widget.queue_draw
      false
    end

    box.signal_connect "draw" do |widget, cr|
      y = 0
      allocation = widget.allocation

      # Draw Abs info
      @event_data.pointer_info.each do |_key, value|
        draw_axes_info(cr, value, allocation)
      end

      @event_data.touch_info.each do |_key, value|
        draw_axes_info(cr, value, allocation)
      end

      # Draw name, color legend and misc data
      @event_data.pointer_info.each do |_key, value|
        y = draw_device_info(widget, cr, nil, y, value)
      end

      @event_data.touch_info.each do |key, value|
        y = draw_device_info(widget, cr, key, y, value)
      end

      false
    end

    @label = Gtk::Label.new("")
    @label.use_markup = true
    box.add(@label)
    init_pad_controller
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

  def init_pad_controller
    action_group = Gio::SimpleActionGroup.new
    @pad_controller = Gtk::PadController.new(@window, action_group, nil)

    PAD_ACTIONS.each_with_index do |pad_action, i|
      action = if pad_action[0] == :button
                 Gio::SimpleAction.new(pad_action[4])
               else
                 Gio::SimpleAction.new(pad_action[4], "d")
               end
      action.signal_connect "activate" do |_action, param|
        if param
          update_label_and_timeout("#{PAD_ACTION_RESULTS[i]} #{param}")
        else
          update_label_and_timeout(PAD_ACTION_RESULTS[i])
        end
      end
      action_group.add_action(action)
      @pad_controller.set_action(*pad_action)
    end
  end

  def draw_axes_info(cr, info, allocation)
    axes = info.last_source.axes
    cr.save
    cr.set_line_width(1)
    cr.set_source_rgba(info.color)
    cr.move_to(0, info.y)
    cr.line_to(allocation.width, info.y)
    cr.move_to(info.x, 0)
    cr.line_to(info.x, allocation.height)
    cr.stroke

    cr.translate(info.x, info.y)

    if info.axes.nil?
      cr.restore
      return
    end

    if axes.include?(Gdk::AxisFlags::XTILT) && axes.include?(Gdk::AxisFlags::YTILT)
      tiltx = info.last_source.get_axis(info.axes, :axis_xtilt)
      tilty = info.last_source.get_axis(info.axes, :axis_ytilt)

      render_arrow(cr, tiltx * 100, tilty * 100, "Tilt")
    end

    if axes.include?(Gdk::AxisFlags::DISTANCE)
      distance = info.last_source.get_axis(info.axes, :axis_distance)

      cr.save
      cr.move_to(distance * 100, 0)
      cr.set_source_rgb(0.0, 0.0, 0.0)
      cr.set_dash([5.0, 5.0], 2, 0.0)
      cr.arc(0, 0, distance * 100, 0, 2 * Math::PI)
      cr.stroke

      cr.move_to(0, -distance * 100)
      extents = cr.text_extents("Distance")
      cr.rel_move_to(-extents.width / 2, 0)
      cr.show_text("Distance")

      cr.move_to(cr, 0, 0)

      cr.restore
    end

    if axes.include?(Gdk::AxisFlags::WHEEL)
      wheel = info.last_source.get_axis(info.axes, :axis_wheel)
      cr.save
      cr.set_line_width(10)
      cr.set_source_rgba(0, 0, 0, 0.5)

      cr.new_sub_path
      cr.arc(0, 0, 100, 0, wheel * 2 * Math::PI)
      cr.stroke
      cr.restore
    end

    if axes.include?(Gdk::AxisFlags::ROTATION)
      rotation = info.last_source.get_axis(info.axes, :axis_rotation)
      rotation *= (2 * Math::PI)

      cr.save
      cr.rotate(-1 * (Math::PI / 2))
      cr.set_line_cap(Cairo::LINE_CAP_ROUND)
      cr.set_line_width(5)

      cr.new_sub_path
      cr.arc(0, 0, 100, 0, rotation)
      cr.stroke
      cr.restore
    end

    if axes.include?(Gdk::AxisFlags::SLIDER)
      slider = info.last_source.get_axis(info.axes, :axis_slider)

      cr.save
      cr.movet_to(0, -10)
      cr.rel_line_to(0, -50)
      cr.rel_line_to(10, 0)
      cr.rel_line_to(-5, 50)
      cr.close_path

      cr.clip_preserve

      pattern = Cairo::LinearPattern.new(0, -10, 0, -60)
      pattern.add_color_stop_rgb(0, 0, 1, 0)
      pattern.add_color_stop_rgb(1, 1, 0, 0)
      cr.set_source(pattern)

      mask = Cairo::LinearPattern.new(0, -10, 0, -60)
      mask.add_color_stop_rgba(0, 0, 0, 0, 1)
      mask.add_color_stop_rgba(slider, 0, 0, 0, 1)
      mask.add_color_stop_rgba(slider, 0, 0, 0, 0)
      mask.add_color_stop_rgba(1, 0, 0, 0, 0)
      cr.mask = mask

      cr.set_source_rgb(0, 0, 0)
      cr.stroke

      cr.restore
    end

    cr.restore
  end

  def draw_device_info(widget, cr, sequence, y, info)
    cr.save
    str = "Source: #{info.last_source.name}"
    if sequence && sequence.class.name
      str += "\nSequence: #{sequence.class.name}"
    end

    if info.last_tool
      tool_type = tool_type_to_string(info.last_tool.tool_type)
      serial = info.last_tool.serial
      str += "\Tool: #{tool_type}"
      str += ", Serial: #{serial.to_s(16)}" if serial != 0
    end

    cr.move_to(10, y)
    layout = widget.create_pango_layout(str)
    cr.show_pango_layout(layout)
    cr.stroke

    _, h = layout.pixel_size
    cr.set_source_rgba(info.color)
    cr.set_line_width(10)
    cr.move_to(0, y)
    y += h
    cr.line_to(0, y)
    cr.stroke
    cr.restore

    y
  end

  def update_axes_from_event(event)
    device = event.device
    source_device = event.source_device
    sequence = event.event_sequence
    tool = event.device_tool

    if event.type == :touch_end || event.type == :touch_cancel
      @event_data.touch_info.delete(sequence)
      return
    elsif event.type == :leave_notify
      @event_data.pointer_info.delete(device)
      return
    end

    info = nil

    if sequence.nil?
      info = @event_data.pointer_info[device]

      unless info
        info = AXES_INFO.new
        info.color = Gdk::RGBA.parse(COLORS[@cur_color])
        @cur_color = (@cur_color + 1) % COLORS.size

        @event_data.pointer_info[device] = info
      end
    else
      info = @event_data.touch_info[sequence]

      unless info
        info = AXES_INFO.new
        info.color = Gdk::RGBA.parse(COLORS[@cur_color])
        @cur_color = (@cur_color + 1) % COLORS.size
        @event_data.touch_info[sequence] = info
      end
    end

    info.last_source = source_device if info.last_source != source_device
    info.last_tool = tool if info.last_tool != tool

    if event.type == :touch_begin || event.type == :touch_update
      @event_data.pointer_info.delete(device) if (sequence && event.touch.emulating_pointer)
    end

    if event.type == :motion_notify
      info.axes = source_device.axes
    elsif event.type == :button_press || event.type == :button_release
      info.axes = source_device.axes
    end
    x, y = event.coords
    info.x = x
    info.y = y
  end

  def tool_type_to_string(tool_type)
    case tool_type
    when Gdk::DeviceToolType::PEN
      "Pen"
    when Gdk::DeviceToolType::ERASER
      "Eraser"
    when Gdk::DeviceToolType::BRUSH
      "Brush"
    when Gdk::DeviceToolType::PENCIL
      "Pencil"
    when Gdk::DeviceToolType::AIRBRUSH
      "Airbrush"
    when Gdk::DeviceToolType::MOUSE
      "Mouse"
    when Gdk::DeviceToolType::LENS
      "Lens cursor"
    else
      "Unknown"
    end
  end

  def render_arrow(cr, x_diff, y_diff, label)
    cr.save
    cr.set_source_rgb(0, 0, 0)
    cr.new_path
    cr.move_to(0, 0)
    cr.line_to(x_diff, y_diff)
    cr.stroke
    cr.move_to(x_diff, y_diff)
    cr.show_text(label)

    cr.restore
  end

  def update_label_and_timeout(text)
    GLib::Source.remove(@pad_action_timeout_id) if @pad_action_timeout_id

    @label.markup = "<span font='48.0'>#{text}</span>"

    @pad_action_timeout_id = GLib::Timeout.add(200) do
      @label.markup = "<span font='48.0'>#{text}</span>"
      @pad_action_timeout_id = 0
      GLib::Source::REMOVE
    end
  end
end
