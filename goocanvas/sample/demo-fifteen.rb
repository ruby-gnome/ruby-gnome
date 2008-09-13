#  -*- indent-tabs-mode: nil -*-
=begin header

  demo-fifteen.rb - Canvas test rewritten in Ruby/GNOME	
	
  Rewritten by  Emmanuel Pinault <seatmanu@yahoo.com>

Original Copyright:
 
  Author :  Richard Hestilow <hestgray@ionet.net>

  Copyright (C) 1998 Free Software Foundation

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
  02111-1307, USA.

=end

class CanvasSampleFifteen < Gtk::VBox
	PIECE_SIZE = 50
	SCRAMBLE_MOVES = 32

	def initialize
		super(false, 4)
		border_width = 4
		show()

		alignment = Gtk::Alignment.new(0.5, 0.5, 0.0, 0.0)
		pack_start(alignment, true, true, 0)
		alignment.show()

		frame = Gtk::Frame.new
		frame.set_shadow_type(Gtk::SHADOW_IN);
		alignment.add(frame)
		frame.show()

		# Create the canvas and board

		@canvas = Goo::Canvas.new()
		@canvas.set_size_request(PIECE_SIZE * 4 + 1, PIECE_SIZE * 4 + 1)
		#@canvas.set_scroll_region(0, 0, PIECE_SIZE * 4 + 1, PIECE_SIZE * 4 + 1)
		frame.add(@canvas)
		@canvas.show()
		
		@board = Array.new(16)
		
		0.upto(14) do |i|
			@board[i] = Piece.new(@canvas.root_item)
			@board[i].setup(self, i)
		end
		@board[15] = nil;

		# Scramble button
		button = Gtk::Button.new("Scramble")
		pack_start(button, false, false, 0)
		button.signal_connect("clicked") do |button|
	  	scramble()
		end
		button.show()
	end
	
 	def test_win
		0.upto(14) do |i|
			if @board[i].nil? || @board[i].num != i
	  		return
			end
		end
	  dialog = Gtk::MessageDialog.new(parent_window,
                         Gtk::Dialog::DESTROY_WITH_PARENT,
                         Gtk::MessageDialog::INFO,
                         Gtk::MessageDialog::BUTTONS_OK,
                         "You stud, you win!")
		dialog.set_modal(true)
		dialog.run_and_close()
	end

	def piece_enter_notify(item)
		item.text.set_property(:fill_color, "white")
	end

	def piece_leave_notify(item)
		item.text.set_property(:fill_color, "black")
	end

	def piece_button_press(item)
		y = item.pos / 4
		x = item.pos % 4
	
		move = true
	
		if (y > 0) && @board[(y - 1) * 4 + x].nil?
	  	dx = 0.0
	  	dy = -1.0
	  	y -= 1
		elsif (y < 3) && @board[(y + 1) * 4 + x].nil?
	  	dx = 0.0
	  	dy = 1.0
	  	y += 1
		elsif (x > 0) && @board[y * 4 + x - 1].nil?
	  	dx = -1.0
	  	dy = 0.0
	  	x -= 1
		elsif (x < 3) && @board[y * 4 + x + 1].nil?
	  	dx = 1.0
	  	dy = 0.0
	  	x += 1
		else
	 		move = false
		end
	
		if move
	  	newpos = y * 4 + x
	  	@board[item.pos] = nil
	  	@board[newpos] = item
	  	item.pos = newpos
	  	item.translate(dx * PIECE_SIZE, dy * PIECE_SIZE)
	  	test_win()
		end
	end

  
	def scramble
		srand()		
		# First, find the blank spot
		pos = 0
		0.upto(15) do |i|
  		if @board[i].nil?
  			pos = i
				break
			end
		end
		# "Move the blank spot" around in order to scramble the pieces

		0.upto(SCRAMBLE_MOVES) do
			dir = rand(4).to_i
			x = y = 0

			if (dir == 0) && (pos > 3) # up
				y = -1
			elsif (dir == 1) && (pos < 12) # down
				y = 1
			elsif (dir == 2) && ((pos % 4) != 0) # left
				x = -1
			elsif (dir == 3) && ((pos % 4) != 3) # right
				x = 1
			else
				retry
			end
	
			oldpos = pos + y * 4 + x;
			@board[pos] = @board[oldpos];
			@board[oldpos] = nil
			@board[pos].pos = pos
			@board[pos].translate(-x * PIECE_SIZE, -y * PIECE_SIZE)
			pos = oldpos
		end
	end

 	class Piece < Goo::CanvasGroup
		attr_reader :text, :num, :pos
		attr_writer :pos

		def initialize(*arg)
			super(*arg)
		end

		def setup(app, i)
			y = i / 4
			x = i % 4
			translate( x * PIECE_SIZE,y * PIECE_SIZE)

			Goo::CanvasRect.new(self, 0, 0, PIECE_SIZE, PIECE_SIZE,
													:line_width => 1.0,
													:fill_color => get_piece_color(x, y),
													:stroke_color => "black")

  
			@text = Goo::CanvasText.new(self,i.to_s, PIECE_SIZE / 2.0, 
																	PIECE_SIZE / 2.0, -1, Gtk::ANCHOR_CENTER,
																	:font=>"Sans bold 24",
																	:fill_color => "black")
			@num = i
			@pos = i
 
			self.signal_connect("enter_notify_event") do |item, event|
				app.piece_enter_notify(item)
			end
 
			self.signal_connect("leave_notify_event") do |item, event|
				app.piece_leave_notify(item)
			end
 
			self.signal_connect("button_press_event") do |item, event|
				app.piece_button_press(item)
			end
		end

		private
		def get_piece_color(x, y)
  		r = ((4 - x) * 255) / 4;
  		g = ((4 - y) * 255) / 4;
  		b = 128;
  		return sprintf("#%02x%02x%02x", r, g, b)
		end
	end
end
