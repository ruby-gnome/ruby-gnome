#!/usr/bin/env ruby

require 'gtk'

=begin
Sample script using Gtk::Socket and Gtk::Plug.
Written by Alex Boussinet <mailto:dbug@wanadoo.fr> for testing purpose only.
=end

class MyGtkSocket < Gtk::Window
	def initialize
		super Gtk::WINDOW_TOPLEVEL
		set_title "Gtk::Socket Test"
		set_policy false, false, false
		set_position Gtk::WIN_POS_CENTER
		signal_connect "delete_event" do Gtk::main_quit end
		@buttons = []
		6.times { |n|
			@buttons << Gtk::Button.new("Plug #{n}")
			@buttons.last.signal_connect "clicked" do plug(n) end }
		@table = Gtk::Table.new 1,2
		@table.set_usize 320, 160
		add @table
		@vbox = Gtk::VBox.new true, 5
		@buttons.each { |b| @vbox.add b }
		@vbox.set_usize 150, 150
		@table.attach @vbox, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL, 5, 5
		@socket = Gtk::Socket.new
		@table.attach @socket, 1, 2, 0, 1, Gtk::FILL, Gtk::FILL, 5, 5
		@socket.set_usize 150, 150
		show_all
		@xid = @socket.xwindow
		@pid = 0
	end
	def plug(arg)
		if @pid != 0
			Process.kill "SIGKILL", @pid
			Process.waitpid @pid
			begin @table.remove @socket; rescue ArgumentError
			# socket has been destroyed because child process finished unexpectedly
			end
			@socket = Gtk::Socket.new
			@table.attach @socket, 1, 2, 0, 1, Gtk::FILL, Gtk::FILL, 5, 5
			@socket.set_usize 150, 150
			@socket.show
			@xid = @socket.xwindow
		end
		@pid = fork { exec "./t-gtkplug.rb -x#{@xid} Plug#{arg}" }
	end
end

MyGtkSocket.new
Gtk::main
