#!/usr/bin/env ruby
# coding: utf-8

if ARGV.empty?
  puts "Usage: #{$0} <m3u file>|<media file> ..."
  exit(false)
elsif (file = ARGV.first).end_with?('.m3u')
  base = File.dirname(file)
  file_list = File.read(file).split("\n").map{|f| File.expand_path(f, base)}
else
  file_list = ARGV.map{|f| File.expand_path(f)}
end

unless %w(gtk3 gtk2).find{|l| begin; require l; rescue LoadError; false; end}
  puts "This sample requires gtk3 or gtk2."
  exit(false)
end
if Gtk.check_version?(3, 0, 0)
  def set_background(widget)
    widget.override_background_color(:normal, Gdk::RGBA.new(0, 0, 0, 0))
  end
elsif Gtk.check_version?(2, 16, 0)
  def set_background(widget)
    widget.modify_bg(:normal, Gdk::Color.new(0, 0, 0))
  end
else
  puts "This sample requires GTK+ 2.16.0 or later."
  exit(false)
end
require 'gvlc'

class Playlist
  COL_MEDIA     = 0
  COL_FILENAME  = 1

  def initialize(file_list)
    @builder = Gtk::Builder.new.add(__FILE__.sub(/\.rb$/, '.glade'))
    @builder.connect_signals{|name| method(name)}
    @window, @adjustment, @playlist, @listview = %w(win_main adj_position win_playlist tree_playlist).map{|n| @builder[n]}

    @listview.model = Gtk::ListStore.new(VLC::Media, String)
    column = Gtk::TreeViewColumn.new('file', Gtk::CellRendererText.new, text: COL_FILENAME)
    @listview.append_column(column)

    @listplayer = VLC::MediaListPlayer.new
    @player = @listplayer.media_player
    @list = @listplayer.media_list
    @player.attach_event(:mediaplayer_playing){|*a| on_mediaplayer_playing(*a)}
    @player.attach_event(:mediaplayer_timechanged){|*a| on_mediaplayer_timechanged(*a)}
    @list.lock do |list|
      file_list.each{|f| add_media(f)}
    end
  end

  def run
    @window.show_all
    @playlist.show_all
    @listplayer.play
    Gtk.main
  end

  def add_media(file)
    iter = @listview.model.append
    iter[COL_MEDIA]     = @list.add_media(path: file)
    iter[COL_FILENAME]  = File.basename(file)
  end

  def toggle_fullscreen
    if @window.window.state.fullscreen?
      @builder['box_control'].show_all
      @window.unfullscreen
    else
      @builder['box_control'].hide
      @window.fullscreen
    end
  end

  def time_str(time)
    return '' unless time

    time = (time / 1000).to_i
    time, sec = time.divmod(60)
    hour, min = time.divmod(60)
    '%d:%02d:%02d' % [hour, min, sec]
  end

  def on_mediaplayer_playing(player, event)
    @duration = nil
    size = player.get_size(0)
    @builder['draw_video'].set_size_request(*size) if size

    mrl = player.media.mrl
    iter = @listview.model.each{|m, p, i| break i if i[COL_MEDIA].mrl == mrl}
    @listview.selection.select_iter(iter)
    @window.title = iter[COL_FILENAME]
  end

  def on_mediaplayer_timechanged(player, event)
    return if @position_change

    @duration ||= player.media.duration
    @adjustment.upper = @duration
    @adjustment.page_increment = @duration / 10
    @adjustment.step_increment = @duration / 100
    @adjustment.value = player.time
  end

  def on_draw_video_realize(widget)
    @player.xwindow = widget.window.xid
    set_background(widget)
  end

  def on_scale_position_adjust_bounds(widget, value)
    @position_pending = value
  end

  def on_scale_position_button_press_event(widget, event)
    @position_change = true
    false
  end

  def on_scale_position_button_release_event(widget, event)
    @position_change = false
    @player.time, @position_pending = @position_pending, nil if @position_pending
    false
  end

  def on_win_main_scroll_event(widget, event)
    case event.direction
    when Gdk::EventScroll::Direction::UP
      @player.time += @adjustment.step_increment
    when Gdk::EventScroll::Direction::DOWN
      @player.time -= @adjustment.step_increment
    end
  end

  def on_scale_position_format_value(widget, value)
    "#{time_str(value)}/#{time_str(@duration)}"
  end

  def on_draw_video_button_press_event(widget, event)
    case event.event_type
    when Gdk::Event::Type::BUTTON_PRESS
      @listplayer.pause
    when Gdk::Event::Type::BUTTON2_PRESS
      toggle_fullscreen
    end
  end

  def on_action_activate(action)
    case action = action.name[/^act_(.*)/, 1].to_sym
    when :play, :pause, :stop, :previous, :next
      @listplayer.send(action)
    when :seek_backward
      @player.time -= @adjustment.page_increment
    when :seek_forward
      @player.time += @adjustment.page_increment
    when :playlist
      @playlist.show_all
    when :fullscreen
      toggle_fullscreen
    end
  end

  def on_tree_playlist_row_activated(widget, path, column)
    @listplayer.play(widget.model.get_iter(path)[COL_MEDIA])
  end

  def on_win_playlist_delete_event(widget, event)
    @playlist.hide
    true
  end

  def on_win_main_destroy(widget)
    Gtk.main_quit
  end
end

Playlist.new(file_list).run

