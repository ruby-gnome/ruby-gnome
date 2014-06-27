#!/usr/bin/env ruby
#
# This sample code is a port of clutter-gst/examples/video-player.c. The
# image files used in this sample code are copied from clutter-gst/examples.
# They are licensed under the terms of the GNU Lesser General Public
# License, version 2.1 or (at your option) later.
#
# The original header:
#   video-player.c - A simple video player with an OSD.
#
#   Copyright (C) 2007,2008 OpenedHand
#   Copyright (C) 2013 Collabora
#
# Copyright (C) 2013  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

require "optparse"

require "clutter-gst"

SEEK_H = 14
SEEK_W = 440

GST_PLAY_FLAG_VIS = (1 << 3)

class VideoApp
  attr_accessor :stage
  attr_accessor :vtexture
  attr_accessor :control, :control_bg, :control_label
  attr_accessor :control_play, :control_pause
  attr_accessor :control_seek1, :control_seek2, :control_seekbar
  attr_accessor :controls_showing, :paused, :mouse_in_window
  attr_accessor :controls_timeout
  def initialize
    @controls_showing = false
    @paused = false
    @mouse_in_window = false
    @controls_timeout = 0
  end
end

opt_fullscreen = false
opt_loop = false

parser = OptionParser.new
parser.on("-f", "--[no-]fullscreen",
          "Start the player in fullscreen",
          "(#{opt_fullscreen})") do |boolean|
  opt_fullscreen = boolean
end
parser.on("-l", "--[no-]loop",
          "Start the video again once reached EOS",
          "(#{opt_loop})") do |boolean|
  opt_loop = boolean
end
parser.parse!

def controls_timeout_cb(app)
  app.controls_timeout = 0
  show_controls(app, false)
  false
end

def actor_animate(actor, mode, duration, first_property, *args)
  actor.save_easing_state
  actor.easing_mode = mode
  actor.easing_duration = duration

  actor.set_property(first_property, args.first)
end

def show_controls(app, vis)
  return if app.control.nil?

  if vis == true && app.controls_showing == true
    if app.controls_timeout == 0
      app.controls_timeout = GLib::Timeout.add_seconds(5) do
                               controls_timeout_cb(app)
                             end
    end
    return
  end

  if vis == true && app.controls_showing == false
    app.controls_showing = true
    app.stage.show_cursor
    actor_animate(app.control, :ease_out_quint, 250, "opacity", 224)
    return
  end

  if vis == false && app.controls_showing == true
    app.controls_showing = false
    if app.mouse_in_window
      app.stage.hide_cursor
    end
    actor_animate(app.control, :ease_out_quint, 250, "opacity", 0)
    return
  end
end

def toggle_pause_state(app)
  return if app.vtexture.nil?

  if app.paused
    app.vtexture.playing = true
    app.paused = false
    app.control_play.hide
    app.control_pause.show
  else
    app.vtexture.playing = false
    app.paused = true
    app.control_pause.hide
    app.control_play.show
  end
end

def position_controls(app, controls)
  stage_width, stage_height = app.stage.size
  bg_width, bg_height = app.control.size

  x = ((stage_width - bg_width) / 2).floor
  y =  stage_height - bg_height - 28

  controls.set_position(x, y)
end

def new_rectangle_with_color(color)
  actor = Clutter::Actor.new
  actor.background_color = color
  actor
end

stage_color = Clutter::Color.new(0, 0, 0, 0)
control_color1 = Clutter::Color.new(73, 74, 77, 0xee)
control_color2 = Clutter::Color.new(0xcc, 0xcc, 0xcc, 0xff)

if ARGV.length < 1
  puts "Usage: #{$0} [OPTIONS] <video file> - A simple video player"
  exit(false)
end

stage = Clutter::Stage.new
stage.background_color = Clutter::Color.new(0, 0, 0, 0)
stage.set_size(768, 576)
stage.set_minimum_size(640, 480)
stage.fullscreen = true if opt_fullscreen

app = VideoApp.new
app.stage = stage
app.vtexture = ClutterGst::VideoTexture.new

raise "failed to create vtexture" if app.vtexture.nil?

# By default ClutterGst seeks to the nearest key frame (faster). However
# it has the weird effect that when you click on the progress bar, the fill
# goes to the key frame position that can be quite far from where you
# clicked. Using the ACCURATE flag tells playbin2 to seek to the actual
# frame
app.vtexture.seek_flags = :accurate

app.vtexture.signal_connect("eos") do |media, app|
  if opt_loop
    media.progress = 0.0
    media.playing = true
  end
end

stage.signal_connect("allocation-changed") do |stage, box, flags|
  position_controls(app, app.control)
  show_controls(app, true)
end

stage.signal_connect("destroy") do
  Clutter.main_quit
end

# Handle it ourselves so can scale up for fullscreen better
app.vtexture.signal_connect_after("size-change") do |texture, base_width, base_height|
  stage_width, stage_height = stage.size

  # base_width and base_height are the actual dimensions of the buffers before
  # taking the pixel aspect ratio into account. We need to get the actual
  # size of the texture to display
  frame_width, frame_height = texture.size

  new_height = (frame_height * stage_width) / frame_width
  if new_height <= stage_height
    new_width = stage_width
    new_x = 0
    new_y = (stage_height - new_height) / 2
  else
    new_width = (frame_width * stage_height) / frame_height
    new_height = stage_height
    new_x = (stage_width - new_width) / 2
    new_y = 0
  end
  texture.set_position(new_x, new_y)
  texture.set_size(new_width, new_height)
end

# Load up out video texture
app.vtexture.filename = ARGV[0]

# Set up things so that a visualisation is played if there's no video
pipe = app.vtexture.pipeline
raise "Unable to get gstreamer pipeline!" unless pipe

# TODO: want to not require Gst.init
# prepare Gst's methods (e.g. iterate_sinks)
Gst.init

iter = pipe.iterate_sinks
raise "Unable to iterate over sinks!" unless iter

playsink = nil
while (value = iter.next)[0] == :ok
  playsink = value[1].value
  sink_name = playsink.name  # unused
end

goomsource = Gst::ElementFactory.make("goom", "source")
raise "Unable to create goom visualiser!" unless goomsource

# TODO: warnings occurred
#playsink_flags = playsink.flags.value
#playsink_flags |= GST_PLAY_FLAG_VIS
playsink.vis_plugin = goomsource
#playsink.flags = playsink_flags

# Create the control UI
app.control = Clutter::Actor.new

app.control_bg = Clutter::Texture.new
app.control_bg.from_file = File.expand_path("vid-panel.png", File.dirname(__FILE__))
app.control_play = Clutter::Texture.new
app.control_play.from_file = File.expand_path("media-actions-start.png", File.dirname(__FILE__))
app.control_pause = Clutter::Texture.new
app.control_pause.from_file = File.expand_path("media-actions-pause.png", File.dirname(__FILE__))

app.control_seek1 = new_rectangle_with_color(control_color1)
app.control_seek2 = new_rectangle_with_color(control_color2)
app.control_seekbar = new_rectangle_with_color(control_color1)
app.control_seekbar.opacity = 0x99

app.control_label = Clutter::Text.new
app.control_label.font_name = "Sans Bold 14"
# TODO: segfault
#app.control_label.text = File.basename(ARGV[0])
stage.title = File.basename(ARGV[0])  # substitutes for label text
app.control_label.color = control_color1

app.control_play.hide

app.control.add_child(app.control_bg)
app.control.add_child(app.control_play)
app.control.add_child(app.control_pause)
app.control.add_child(app.control_seek1)
app.control.add_child(app.control_seek2)
app.control.add_child(app.control_seekbar)
app.control.add_child(app.control_label)

app.control.opacity = 0xee

app.control_play.set_position(22, 31)
app.control_pause.set_position(18, 31)

app.control_seek1.set_size(SEEK_W + 4, SEEK_H + 4)
app.control_seek1.set_position(80, 57)
app.control_seek2.set_size(SEEK_W, SEEK_H)
app.control_seek2.set_position(82, 59)
app.control_seekbar.set_size(0, SEEK_H)
app.control_seekbar.set_position(82, 59)

app.control_label.set_position(82, 29)

# Add control UI to stage
stage.add_child(app.vtexture)
stage.add_child(app.control)

position_controls(app, app.control)

stage.hide_cursor
actor_animate(app.control, :ease_out_quint, 1000, "opacity", 0)

# Hook up other events
stage.signal_connect("event") do |stage, event|
  handled = false

  case event.type
  when Clutter::EventType::MOTION
    show_controls(app, true)
    handled = true
  when Clutter::EventType::BUTTON_PRESS
    if app.controls_showing
      actor = stage.get_actor_at_pos(:all, event.x, event.y)
      if actor == app.control_pause || actor == app.control_play
        toggle_pause_state(app)
      elsif actor == app.control_seek1 ||
            actor == app.control_seek2 ||
            actor == app.control_seekbar
        x, y = app.control_seekbar.transformed_position
        dist = event.x - x
        def clamp(x, low, high)
          if x > high
            high
          elsif x < low
            low
          else
            x
          end
        end
        dist = clamp(dist, 0, SEEK_W)
        progress = 1.0 * dist / SEEK_W
        app.vtexture.progress == progress
      end
    end
    handled = true
  when Clutter::EventType::KEY_PRESS
    animation = nil
    case event.key_symbol
    when Clutter::Keys::KEY_d
      if app.vtexture
        app.stage.remove_child(app.vtexture)
        app.vtexture = nil
      end
      if app.control
        app.stage.remove_child(app.control)
        app.control = nil
      end
    when Clutter::Keys::KEY_q, Clutter::Keys::KEY_Escape
      app.stage.destroy
    when Clutter::Keys::KEY_e
      if app.vtexture
        app.vtexture .set_pivot_point(0.5, 0)
        animation = actor_animate(app.vtexture,
                                  :linear, 500,
                                  "rotation-angle-y", 360.0)
        animation.signal_connect_after("transitions-completed") do
          if app.vtexture
            app.vtexture.set_rotation_angle(:y_axis, 0.0)
          end
        end
        handled = true
      end
    else
      toggle_pause_state(app)
      handled = true
    end
  when Clutter::EventType::ENTER
    app.mouse_in_window = true
    app.stage.cursor_visible = app.controls_showing
  when Clutter::EventType::LEAVE
    app.mouse_in_window = false
    app.stage.show_cursor
  end
  handled
end

app.vtexture.signal_connect("notify::progress") do |video_texture, pspec|
  progress = video_texture.progress
  app.control_seekbar.set_size(progress * SEEK_W, SEEK_H)
end

app.vtexture.playing = true

stage.show

Clutter.main
