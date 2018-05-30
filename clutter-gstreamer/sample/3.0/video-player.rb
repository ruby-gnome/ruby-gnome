#!/usr/bin/env ruby
#
# This sample code is a port of clutter-gst/examples/video-player.c. The
# image files used in this sample code are copied from clutter-gst/examples.
# They are licensed under the terms of the GNU Lesser General Public
# License, version 2.1 or (at your option) later.
#
# The original header:
#   video-player.c - A simple video player with an OSD using ClutterGstVideoActor.
#
#   Copyright (C) 2007,2008 OpenedHand
#   Copyright (C) 2013 Collabora
#
# Copyright (C) 2018  Ruby-GNOME2 Project Team
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
require "ostruct"

require "clutter-gst"

ClutterGst.load_version = "3.0"

SEEK_H = 14
SEEK_W = 440

GST_PLAY_FLAG_VIS = (1 << 3)

class VideoApp
  def initialize(uri, options)
    @uri = uri
    @options = options

    initialize_stage
    initialize_player
    initialize_controls

    @stage.add_child(@video)
    @stage.add_child(@control)

    position_controls

    @stage.hide_cursor

    animate(@control, :ease_out_quint, 1000) do |actor|
      actor.opacity = 0
    end

    @player.signal_connect("notify::progress") do
      @control_seek_bar.set_size(@player.progress * @seek_width,
                                 @seek_height)
    end

    initialize_stage_signals
  end

  def run
    @player.playing = true
    @stage.show
    Clutter.main
  end

  private
  def initialize_stage
    @stage = Clutter::Stage.new
    @stage.background_color = Clutter::Color.new(0x00, 0x00, 0x00, 0x00)
    @stage.set_size(768, 578)
    @stage.set_minimum_size(640, 480)
    @stage.fullscreen = @options.fullscreen
  end

  def initialize_player
    @player = ClutterGst::Playback.new

    @aspect_ratio = ClutterGst::Aspectratio.new
    @aspect_ratio.player = @player

    @video = Clutter::Actor.new
    @video.width = @stage.width
    @video.height = @stage.height
    @video.content = @aspect_ratio

    # By default ClutterGst seeks to the nearest key frame (faster). However
    # it has the weird effect that when you click on the progress bar, the fill
    # goes to the key frame position that can be quite far from where you
    # clicked. Using the ACCURATE flag tells playbin2 to seek to the actual
    # frame
    @player.seek_flags = :accurate

    @player.signal_connect(:eos) do
      if @options.loop
        @player.progress = 0.0
        @player.playing = true
      end
    end

    if local_file?(@uri)
      puts("Local media detected")
    else
      puts("Remote media detected, setting up buffering")
      @player.buffering_mode = :download
      @player.signal_connect("notify::buffer-fill") do
        puts("Buffering - percentage=%d%%" % @player["buffer-fill"] * 100)
      end
    end

    if uri?(@uri)
      @player.uri = @uri
    else
      @player.filename = @uri
    end

    if @player.live_media?
      puts("Playing live media")
    else
      puts("Playing non-live media")
    end

    pipeline = @player.pipeline
    iterator = pipeline.iterate_sinks
    loop do
      result, value = iterator.next
      break unless result == Gst::IteratorResult::OK
      play_sink = value.value
      if play_sink.name == "playsink"
        play_sink.flags |= :vis
        play_sink.vis_plugin = Gst::ElementFactory.make("goom", "source")
        break
      end
    end
  end

  def local_file?(uri)
    scheme = URI.parse(uri).scheme
    scheme.nil? or scheme.downcase == "file"
  end

  def uri?(uri)
    not URI.parse(uri).scheme.nil?
  end

  def initialize_controls
    @controls_timeout = nil

    @control = Clutter::Actor.new
    @control_bg = create_image_actor("vid-panel.png")
    @control_play = create_image_actor("media-actions-start.png")
    @control_pause = create_image_actor("media-actions-pause.png")

    color1 = Clutter::Color.new(73, 74, 77, 0xee)
    color2 = Clutter::Color.new(0xcc, 0xcc, 0xcc, 0xff)

    @control_seek1 = Clutter::Actor.new
    @control_seek1.background_color = color1
    @control_seek2 = Clutter::Actor.new
    @control_seek2.background_color = color2
    @control_seek_bar = Clutter::Actor.new
    @control_seek_bar.background_color = color1
    @control_seek_bar.opacity = 0x99

    @control_label = Clutter::Text.new("Sans Bold 14", "XXX", color1)

    @control_play.hide

    @control.add_child(@control_bg)
    @control.add_child(@control_play)
    @control.add_child(@control_pause)
    @control.add_child(@control_seek1)
    @control.add_child(@control_seek2)
    @control.add_child(@control_seek_bar)
    @control.add_child(@control_label)

    @control.opacity = 0xee

    @control_play.set_position(22, 31)
    @control_pause.set_position(18, 31)

    @seek_height = 14
    @seek_width = 440
    @control_seek1.set_size(@seek_width + 4, @seek_height + 4)
    @control_seek1.set_position(80, 57)
    @control_seek2.set_size(@seek_width, @seek_height)
    @control_seek2.set_position(82, 59)
    @control_seek_bar.set_size(0, @seek_height)
    @control_seek_bar.set_position(82, 59)

    @control_label.set_position(82, 29)
  end

  def create_image_actor(path)
    actor = Clutter::Actor.new
    image = Clutter::Image.new
    pixbuf = GdkPixbuf::Pixbuf.new(file: File.join(__dir__, path))
    image.set_data(pixbuf.pixels,
                   pixbuf.has_alpha? ? :rgba_8888 : :rgb_888,
                   pixbuf.width,
                   pixbuf.height,
                   pixbuf.rowstride)
    actor.width = pixbuf.width
    actor.height = pixbuf.height
    actor.content = image
    actor
  end

  def position_controls
    stage_width, stage_height = @stage.size
    control_width, control_height = @control.size
    @control.set_position((stage_width - control_width) / 2,
                          (stage_height - control_height) - 28)
  end

  def show_controls
    if @control.opacity > 0
      if @controls_timeout.nil?
        @controls_timeout = GLib::Timeout.add_seconds(5) do
          @controls_timeout = nil
          @stage.hide_cursor
          animate(@control, :ease_out_quint, 250) do |actor|
            actor.opacity = 0
          end
          GLib::Source::REMOVE
        end
      end
    else
      @stage.show_cursor
      animate(@control, :ease_out_quint, 250) do |actor|
        actor.opacity = 224
      end
    end
  end

  def animate(actor, mode, duration, transition_name=nil)
    actor.save_easing_state
    actor.easing_mode = mode
    actor.easing_duration = duration
    yield(actor)
    actor.restore_easing_state
    if transition_name
      actor.get_transition(transition_name)
    else
      nil
    end
  end

  def toggle_pause_state
    if @player.playing?
      @player.playing = false
      @control_play.show
      @control_pause.hide
    else
      @player.playing = true
      @control_play.hide
      @control_pause.show
    end
  end

  def initialize_stage_signals
    @stage.signal_connect(:event) do |_, event|
      handled = false
      case event.type
      when Clutter::EventType::MOTION
        show_controls
        handled = true
      when Clutter::EventType::BUTTON_PRESS
        if @control.opacity > 0
          actor = @stage.get_actor_at_pos(:all, event.x, event.y)
          case actor
          when @control_pause, @control_play
            toggle_pause_state
          when @control_seek1, @control_seek2, @control_seek_bar
            x, y = @control_seek_bar.transformed_position
            dist = event.x - x
            dist = [[dist, 0].max, @seek_width].min
            progress = dist.to_f / @seek_width
            @player.progress = progress
          end
        end
        handled = true
      when Clutter::EventType::KEY_PRESS
        case event.key_symbol
        when Clutter::Keys::KEY_d
          @stage.remove_child(@video)
          @video = nil
          @stage.remove_child(@control)
          @control = nil
        when Clutter::Keys::KEY_q, Clutter::Keys::KEY_Escape
          @stage.destroy
        when Clutter::Keys::KEY_e
          @video.set_pivot_point(0.5, 0)
          animation = animate(@video, :linear, 500, "rotation-angle-y") do |actor|
            @video.rotation_angle_y = 360.0
          end
          animation.signal_connect_after(:completed) do
            @video.rotation_angle_y = 0.0
          end
        else
          toggle_pause_state
        end
      when Clutter::EventType::ENTER
        @stage.cursor_visible = (@control.opacity > 0)
      when Clutter::EventType::LEAVE
        @stage.show_cursor
      end
      handled
    end

    @stage.signal_connect(:allocation_changed) do
      position_controls
      show_controls
    end

    @stage.signal_connect(:destroy) do
      Clutter.main_quit
    end
  end
end

options = OpenStruct.new
options.fullscreen = false
options.loop = false

parser = OptionParser.new
parser.on("-f", "--[no-]fullscreen",
          "Start the player in fullscreen",
          "(#{options.fullscreen})") do |boolean|
  options.fullscreen = boolean
end
parser.on("-l", "--[no-]loop",
          "Start the video again once reached EOS",
          "(#{options.loop})") do |boolean|
  options.loop = boolean
end
parser.parse!

if ARGV.length < 1
  puts "Usage: #{$0} [OPTIONS] <video file> - A simple video player"
  exit(false)
end

uri = ARGV[0]

app = VideoApp.new(uri, options)
app.run
