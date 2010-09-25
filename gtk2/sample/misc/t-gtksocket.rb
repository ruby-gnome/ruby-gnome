#!/usr/bin/env ruby
=begin
  Sample script using Gtk::Socket and Gtk::Plug.

 $ ruby t-gtksocket.rb

  Written by Alex Boussinet <mailto:dbug@wanadoo.fr> for testing purpose only.

  Copyright (c) 2003-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: t-gtksocket.rb,v 1.5 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

class MyGtkSocket < Gtk::Window
  def initialize
    super("Gtk::Socket Test")
    set_window_position(Gtk::Window::POS_CENTER)
    signal_connect("delete_event"){Gtk::main_quit}

    @buttons = []
    6.times {|n|
      @buttons << Gtk::Button.new("Plug #{n}")
      @buttons.last.signal_connect("clicked"){ plug(n) }
    }
    
    @table = Gtk::Table.new(1, 2)
    @table.set_size_request(320, 200)
    add(@table)
    @vbox = Gtk::VBox.new(true, 5)
    @buttons.each{|b| @vbox.add(b) }
    @vbox.set_size_request(150, 190)
    @table.attach(@vbox, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL, 5, 5)
    @socket = Gtk::Socket.new
    @table.attach(@socket, 1, 2, 0, 1, Gtk::FILL, Gtk::FILL, 5, 5)
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
	@table.remove(@socket) unless @socket.destroyed?
      rescue ArgumentError
        # socket has been destroyed because child process finished unexpectedly
      end
      @socket = Gtk::Socket.new
      @table.attach(@socket, 1, 2, 0, 1, Gtk::FILL, Gtk::FILL, 5, 5)
      @socket.set_size_request(150, 190)
      @socket.show
      @xid = @socket.id
    end
    @pid = fork { exec "ruby t-gtkplug.rb -x #{@xid} Plug#{arg}" }
  end
end

MyGtkSocket.new
Gtk.main
