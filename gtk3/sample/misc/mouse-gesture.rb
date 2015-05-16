=begin
  mouse-gesture.rb - mouse gesture sample script.

  Copyright (C) 2005,2006 Kouhei Sutou
  Copyright (c) 2005-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

class GestureProcessor
  DEFAULT_THRESHOLD = 16
  DEFAULT_SKEW_THRESHOLD_ANGLE = 30

  attr_accessor :threshold, :skew_threshold_angle
  attr_reader :motions

  def initialize(threshold = nil, skew_threshold_angle = nil)
    @threshold = threshold || DEFAULT_THRESHOLD
    @skew_threshold_angle = skew_threshold_angle
    @skew_threshold_angle ||= DEFAULT_SKEW_THRESHOLD_ANGLE
    reset
  end

  def started?
    @started
  end

  MOTIONS = %w(L R U D UL UR LL LR)

  def available_motion?(motion)
    MOTIONS.include?(motion)
  end

  def start(x, y)
    @prev_x = @x = x
    @prev_y = @y = y
    @started = true
    @motions = []
  end

  def update_position(x, y)
    mx = x - @prev_x
    my = y - @prev_y

    motion = judge_motion(mx, my)
    if motion
      @prev_x = @x = x
      @prev_y = @y = y
      if @motions.last == motion
        false
      else
        @motions << motion
        true
      end
    else
      false
    end
  end

  def reset
    @started = false
    @x = @y = -1
    @motions = []
  end

  def to_a
    @motions
  end

  def position
    [@x, @y]
  end

  private
  def judge_motion(mx, my)
    mxa = mx.abs
    mya = my.abs
    distance = Math.sqrt(mxa ** 2 + mya ** 2)
    upper_theta = (45 + @skew_threshold_angle) * (Math::PI / 180.0)
    lower_theta = (45 - @skew_threshold_angle) * (Math::PI / 180.0)
    if distance > @threshold and
        mya < Math.tan(upper_theta) * mxa and
        mya > Math.tan(lower_theta) * mxa
      judge_corner_motion(mx, my)
    elsif mxa > @threshold or mya > @threshold
      judge_cross_motion(mx, my)
    else
      nil
    end
  end

  def judge_corner_motion(mx, my)
    if mx < 0
      if my < 0
        "UL"
      else
        "LL"
      end
    else
      if my < 0
        "UR"
      else
        "LR"
      end
    end
  end

  def judge_cross_motion(mx, my)
    if mx.abs > my.abs
      if mx < 0
        "L"
      else
        "R"
      end
    else
      if my < 0
        "U"
      else
        "D"
      end
    end
  end
end

class Gesture < Gtk::EventBox
  DEFAULT_BACK_RGBA = [0.2, 0.2, 0.2, 0.5]
  DEFAULT_LINE_RGBA = [1, 0, 0, 1]
  DEFAULT_NEXT_RGBA = [0, 1, 0, 0.8]
  DEFAULT_CURRENT_RGBA = [1, 0, 1, 0.8]

  def initialize(conf={})
    super()
    set_visible_window(false)
    conf ||= {}
    @back_rgba = conf[:back_rgba] || DEFAULT_BACK_RGBA
    @line_rgba = conf[:line_rgba] || DEFAULT_LINE_RGBA
    @next_rgba = conf[:next_rgba] || DEFAULT_NEXT_RGBA
    @current_rgba = conf[:current_rgba] || DEFAULT_CURRENT_RGBA
    @processor = GestureProcessor.new(conf[:threshold],
                                      conf[:skew_threshold_angle])
    @actions = []
    set_expose_event
    set_motion_notify_event
    set_button_release_event
  end

  def add_action(sequence, action = proc)
    invalid_motion = sequence.find do |motion|
      not @processor.available_motion?(motion)
    end
    raise "invalid motion: #{invalid_motion}" if invalid_motion
    @actions << [sequence, action]
  end

  def start(widget, button, x, y, base_x, base_y)
    self.grab_add
    @widget = widget
    @button = button
    @processor.start(x, y)
    @base_x = base_x
    @base_y = base_y
    @cr = window.create_cairo_context
    @cr.set_source_rgba(@line_rgba)
    @cr.move_to(x, y)
  end

  private
  def perform_action
    act = action
    act.call(@widget) if act
    @processor.reset
  end

  def action
    motions = @processor.motions
    @actions.each do |sequence, act|
      return act if sequence == motions
    end
    nil
  end

  def available_motions
    motions = @processor.motions
    @actions.collect do |sequence, act|
      if sequence == motions
        sequence.last
      else
        nil
      end
    end.compact.uniq
  end

  def next_available_motions
    motions = @processor.motions
    @actions.collect do |sequence, act|
      if sequence[0..-2] == motions
        sequence.last
      else
        nil
      end
    end.compact.uniq
  end

  def match?
    not action.nil?
  end

  def set_expose_event
    signal_connect("draw") do |widget, event|
      if @processor.started?
        cr = widget.window.create_cairo_context

        cr.rectangle(*widget.allocation.to_a)
        cr.set_source_rgba(@back_rgba)
        cr.fill

        cr.set_source_rgba(@next_rgba)
        draw_available_marks(cr, next_available_motions)

        if action
          cr.set_source_rgba(@current_rgba)
          draw_mark(cr, *@processor.position)
        end

        @cr.stroke_preserve

        true
      else
        false
      end
    end
  end

  def draw_mark(cr, x = nil, y = nil, radius = nil)
    x ||= @processor.position[0]
    y ||= @processor.position[1]
    radius ||= @processor.threshold
    cr.save do
      cr.translate(x, y)
      cr.scale(radius, radius)
      cr.arc(0, 0, 0.5, 0, 2 * Math::PI)
      cr.fill
    end
  end

  def draw_available_marks(cr, motions)
    motions.each do |motion|
      adjust_x = calc_position_ratio(motion, %w(R), %w(L), %w(UR LR), %w(UL LL))
      adjust_y = calc_position_ratio(motion, %w(D), %w(U), %w(LR LL), %w(UR UL))

      threshold = @processor.threshold
      x, y = @processor.position
      x += threshold * adjust_x
      y += threshold * adjust_y
      draw_mark(cr, x, y, threshold)
    end
  end

  def calc_position_ratio(motion, inc, dec, inc_skew, dec_skew)
    case motion
    when *inc
      1
    when *inc_skew
      1 / Math.sqrt(2)
    when *dec
      -1
    when *dec_skew
      -1 / Math.sqrt(2)
    else
      0
    end
  end

  def set_motion_notify_event
    signal_connect("motion_notify_event") do |widget, event|
      if @processor.started?
        x = @base_x + event.x
        y = @base_y + event.y
        @cr.line_to(x, y)
        @cr.save do
          if @processor.update_position(x, y)
            queue_draw
          end
        end
        @cr.stroke_preserve
        true
      else
        false
      end
    end
  end

  def set_button_release_event
    signal_connect("button_release_event") do |widget, event|
      if event.button == @button and @processor.started?
        self.grab_remove
        perform_action
        hide
        true
      else
        false
      end
    end
  end
end

class GesturedWidget < Gtk::EventBox
  DEFAULT_GESTURE_BUTTON = 3

  def initialize(gesture_button = nil)
    super()
    set_visible_window(false)
    @gesture_button = gesture_button || DEFAULT_GESTURE_BUTTON
    set_button_press_event
  end

  def layout
    parent
  end

  def gesture(x, y, base_x, base_y)
    if layout
      layout.gesture(self, @gesture_button, x, y, base_x, base_y)
    end
  end

  private
  def set_button_press_event
    signal_connect("button_press_event") do |widget, event|
      if event.button == @gesture_button
        x, y, w, h = widget.allocation.to_a
        gesture(x + event.x, y + event.y, x, y)
      else
        false
      end
    end
  end
end

class Layout < Gtk::Layout
  def initialize
    super()
    @gesture = Gesture.new
    put(@gesture, 0, 0)
  end

  def gesture(widget, button, x, y, base_x, base_y)
    remove(@gesture)
    put(@gesture, 0, 0)
    _, _, w, h = allocation.to_a
    @gesture.set_size_request(w, h)
    @gesture.show
    @gesture.start(widget, button, x, y, base_x, base_y)
  end

  def add_gesture_action(sequence, action = proc)
    @gesture.add_action(sequence, action)
  end
end

window = Gtk::Window.new("Mouse Gesture sample")

layout = Layout.new

gestured_widget = GesturedWidget.new
gestured_widget.set_size_request(50, 50)
gestured_widget.signal_connect("draw") do |widget, event|
  x, y, w, h = widget.allocation.to_a
  cr = widget.window.create_cairo_context
  cr.set_source_rgba([0.8, 0.8, 0.3, 1])
  cr.translate(x, y)
  cr.scale(w, h)
  cr.arc(0.5, 0.5, 0.5, -0.1, 360)
  cr.fill
  false
end

layout.put(gestured_widget, 75, 75)


gestured_widget2 = GesturedWidget.new
gestured_widget2.set_size_request(100, 50)
gestured_widget2.signal_connect("draw") do |widget, event|
  x, y, w, h = widget.allocation.to_a
  cr = widget.window.create_cairo_context
  cr.set_source_rgba([0.3, 0.3, 0.8, 1])
  cr.translate(x, y)
  cr.scale(w, h)
  cr.arc(0.5, 0.5, 0.5, -0.1, 360)
  cr.fill
  false
end

layout.put(gestured_widget2, 0, 25)


# gesture handlers
expand_size = 20

expand_left = proc do |widget|
  x = layout.child_get_property(widget, :x)
  y = layout.child_get_property(widget, :y)
  w, h = widget.size_request
  layout.move(widget, x - expand_size, y)
  widget.set_size_request(w + expand_size, h)
end

expand_right = proc do |widget|
  x = layout.child_get_property(widget, :x)
  y = layout.child_get_property(widget, :y)
  w, h = widget.size_request
  layout.move(widget, x, y)
  widget.set_size_request(w + expand_size, h)
end

expand_top = proc do |widget|
  x = layout.child_get_property(widget, :x)
  y = layout.child_get_property(widget, :y)
  w, h = widget.size_request
  layout.move(widget, x, y - expand_size)
  widget.set_size_request(w, h + expand_size)
end

expand_bottom = proc do |widget|
  x = layout.child_get_property(widget, :x)
  y = layout.child_get_property(widget, :y)
  w, h = widget.size_request
  layout.move(widget, x, y)
  widget.set_size_request(w, h + expand_size)
end

layout.add_gesture_action(["L"]) do |widget|
  expand_left.call(widget)
end

layout.add_gesture_action(["U"]) do |widget|
  expand_top.call(widget)
end

layout.add_gesture_action(["LL"]) do |widget|
  expand_left.call(widget)
  expand_bottom.call(widget)
end

layout.add_gesture_action(["R", "LR"]) do |widget|
  expand_right.call(widget)
  expand_bottom.call(widget)
  expand_right.call(widget)
end


window.add(layout)
window.signal_connect("destroy") { Gtk.main_quit }

window.show_all

Gtk.main
