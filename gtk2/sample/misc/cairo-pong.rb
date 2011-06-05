#!/usr/bin/env ruby
=begin
  cairo-pong.rb - Ruby/GTK with cairo sample script.

  Original: gtkcairo sample by Evan Martins.

  Copyright (c) 2005,2006  Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: cairo-pong.rb,v 1.3 2006/06/17 13:18:12 mutoh Exp $
=end

require "gtk2"

unless Gdk.cairo_available?
  raise "GTK+ 2.8.0 or later and cairo support are required."
end

module Pong
  class CenteredItem
    attr_accessor :x, :y
    attr_reader :width, :height
    
    def initialize(x, y, width, height)
      @x = x
      @y = y
      @width = width
      @height = height
    end

    def min_x
      @x - @width / 2
    end
  
    def max_x
      @x + @width / 2
    end
  
    def min_y
      @y - @height / 2
    end
  
    def max_y
      @y + @height / 2
    end
  end

  class CenteredCircle < CenteredItem
    def draw(cr)
      cr.translate(min_x, min_y)
      cr.scale(@width, @height)
      cr.arc(0.5, 0.5, 0.5, 0, 2 * Math::PI)
      cr.fill
    end
  end
  
  class CenteredRect < CenteredItem
    def draw(cr)
      cr.translate(min_x, min_y)
      cr.scale(@width, @height)
      cr.rectangle(0, 0, 1, 1)
      cr.fill
    end
  end
  
  class Ball < CenteredCircle
    attr_accessor :dx, :dy
    def initialize(dx=0.02, dy=0.02)
      super(0.8, 0.5, 0.04, 0.04)
      @dx = dx
      @dy = dy
    end
    
    def update
      @x += @dx
      @y += @dy

      # ball bouncing
      if max_y > 1
        @y = 1 - (max_y - 1)
        @dy *= -1
      elsif min_y < 0
        @y -= min_y
        @dy *= -1
      end
      
      if max_x > 1
        @x = 1 - (max_x - 1)
        @dx *= -1
      elsif min_x < 0
        @x -= min_x
        @dx *= -1
      end
    end
  end
  
  class Paddle < CenteredRect
    def initialize(field, x, y)
      super(x, y, 0.05, 0.3)
      @field = field
    end
    
    def update(ball)
      # is the ball coming towards us?
      if (ball.x < @x and ball.dx > 0) or
          (ball.x > @x and ball.dx < 0)
        # move to intercept it
        @y = ball.y
      end
    end

    def ball_hit?(ball)
      ball.y > min_y and ball.y < max_y
    end
    
    def update_ball(ball)
      if ball_hit?(ball)
        if ball.min_x < @x and ball.max_x > min_x # hit our left side
          ball.x -= (ball.max_x - min_x)
          ball.dx = -ball.dx
        elsif ball.max_x > @x and ball.min_x < max_x # hit our right side
          ball.x += (max_x - ball.min_x)
          ball.dx = -ball.dx
        end
      end
    end
  end

  class Field
    attr_accessor :width, :height
    
    def initialize(margin=0.05)
      @margin = margin
      
      @left_paddle = Paddle.new(self, @margin, 0.5)
      @right_paddle = Paddle.new(self, 1 - @margin, 0.7)
      @paddles = [@left_paddle, @right_paddle]
      @ball = Ball.new
    end
    
    def update
      @paddles.each do |paddle|
        paddle.update(@ball)
      end
      @ball.update
      @paddles.each do |paddle|
        paddle.update_ball(@ball)
      end
    end
    
    def draw(cr)
      cr.set_source_rgba(1, 1, 1)
      cr.rectangle(0, 0, 1, 1)
      cr.fill

      cr.save do
        cr.set_source_rgba(0.8, 0.8, 0.8, 0.8)
        cr.set_line_join(Cairo::LINE_JOIN_ROUND)
        @paddles.each do |paddle|
          cr.save {paddle.draw(cr)}
        end
      end
      
      cr.set_source_rgba(0, 0, 0)
      cr.save {@ball.draw(cr)}
    end
  end

  class Window < Gtk::Window
    def initialize(speed=30)
      super()
      @speed = speed
      
      self.title = 'Pong Demonstration'
      signal_connect('destroy') { Gtk.main_quit }
      signal_connect("key_press_event") do |widget, event|
        if event.state.control_mask? and event.keyval == Gdk::Keyval::GDK_q
          destroy
          true
        else
          false
        end
      end

      set_default_size(260, 200)

      @field = Field.new

      @drawing_area = Gtk::DrawingArea.new
      set_expose_event

      vb = Gtk::VBox.new(false, 5)
      vb.border_width = 10
      vb.pack_start(@drawing_area, true, true, 0)
      vb.show_all
      add(vb)

      Gtk.timeout_add(@speed) do
        @field.update
        @drawing_area.queue_draw unless @drawing_area.destroyed?
      end
    end

    def set_expose_event
      @drawing_area.signal_connect('expose_event') do |widget, event|
        cr = widget.window.create_cairo_context
        cr.scale(*widget.window.size)
        @field.draw(cr)
      end
    end
  end
end

Pong::Window.new.show_all
Gtk.main
