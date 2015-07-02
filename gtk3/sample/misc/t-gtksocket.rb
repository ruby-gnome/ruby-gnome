#!/usr/bin/env ruby
=begin
  Sample script using Gtk::Socket and Gtk::Plug.

 $ ruby t-gtksocket.rb

  Written by Alex Boussinet <mailto:dbug@wanadoo.fr> for testing purpose only.

  Copyright (c) 2003-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

class MyGtkSocket < Gtk::Window
  def initialize
    super("Gtk::Socket Test")
    set_window_position(Gtk::WindowPosition::CENTER)
    signal_connect("delete_event") { Gtk.main_quit }

    @buttons = []
    6.times do |n|
      @buttons << Gtk::Button.new(:label => "Plug #{n}")
      @buttons.last.signal_connect("clicked") { plug(n) }
    end

    @grid = Gtk::Grid.new
    @grid.set_row_spacing(5)
    @grid.set_column_spacing(5)
    add(@grid)

    @vbox = Gtk::Box.new(:vertical, 5)
    @buttons.each { |b| @vbox.add(b) }
    @vbox.set_size_request(150, 190)
    @grid.attach(@vbox, 0, 0, 1, 1)
    @socket = Gtk::Socket.new
    @grid.attach(@socket, 1, 0, 1, 1)
    @socket.set_size_request(150, 150)

    show_all
    @xid = @socket.id
    @pid = 0
  end

  def plug(arg)
    if @pid != 0
      Process.kill("SIGKILL", @pid)
      Process.waitpid(@pid)
      begin
        @grid.remove(@socket) unless @socket.destroyed?
      rescue ArgumentError
        # socket has been destroyed because child process finished unexpectedly
      end

      @socket = Gtk::Socket.new
      @grid.attach(@socket, 1, 0, 1, 1)
      @socket.set_size_request(150, 190)
      @socket.show
      @xid = @socket.id
    end
    @pid = fork { exec "ruby t-gtkplug.rb -x #{@xid} Plug#{arg}" }
  end
end

MyGtkSocket.new
Gtk.main
