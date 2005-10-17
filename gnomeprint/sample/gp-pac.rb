=begin
  gp-pac.rb - Ruby/GnomePrint sample script. (GnomePrint-ly version)

  Original: pac.rb in http://www.artima.com/rubycs/articles/pdf_writer3.html

  Copyright (c) 2005 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: gp-pac.rb,v 1.2 2005/10/17 12:39:57 ktou Exp $
=end

require "gnomeprint2"

job = Gnome::PrintJob.new
context = job.context
config = job.config

paper_name = config[Gnome::PrintConfig::KEY_PAPER_SIZE]
paper = Gnome::PrintPaper.get(paper_name)

width = paper.height / 2
height = paper.width / 2

setup_config = Proc.new do
  pt = Gnome::PrintUnit.get_by_abbreviation("Pt")
  config[Gnome::PrintConfig::KEY_PAPER_SIZE] = "Custom"
  config.set(Gnome::PrintConfig::KEY_PAPER_WIDTH, width, pt)
  config.set(Gnome::PrintConfig::KEY_PAPER_HEIGHT, height, pt)
end

white = [65535, 65535, 65535]
black = [0, 0, 0]
magenta = [65535, 0, 65535]
cyan = [0, 65535, 65535]
yellow = [65535, 65535, 0]
blue = [0, 0, 65535]

context.begin_page("1") do
  context.scale(width, height)

  context.save do
    context.set_rgb_color(*black)
    context.rect_filled(0, 0, 1, 1)
  end

  # Wall
  wall_width = 0.89
  wall_height = 0.03
  wall_space = 0.5
  wall_x = 0.02
  wall1_y = 0.83
  wall2_y = wall1_y - wall_space
  wall_radius = 0.01
  
  context.set_rgb_color(*magenta)
  context.rounded_rect_filled(wall_x, wall1_y, wall_width, wall_height,
                              wall_radius)
  context.set_rgb_color(*cyan)
  context.rounded_rect_stroked(wall_x, wall1_y, wall_width, wall_height,
                               wall_radius)

  context.set_rgb_color(*magenta)
  context.rounded_rect_filled(wall_x, wall2_y, wall_width, wall_height,
                              wall_radius)
  context.set_rgb_color(*cyan)
  context.rounded_rect_stroked(wall_x, wall2_y, wall_width, wall_height,
                               wall_radius)

  # Body
  body_x = 0.17
  body_y = 0.58
  body_width = 0.23
  body_height = 0.33
  
  context.save do
    context.translate(body_x, body_y)
    context.set_rgb_color(*yellow)
    context.scale(body_width, body_height)
    context.arc_to(0, 0, 0.5, 30, 330, false)
    context.line_to(0, 0)
    context.fill
  end

  # Dot
  dot_width = 0.02
  dot_height = 0.03
  small_dot_width = 0.01
  small_dot_height = 0.015
  dot_x = 0.29
  dot_y = 0.58
  dot_step = 0.05
  
  context.save do
    context.set_rgb_color(*yellow)
    context.save do
      context.translate(dot_x, dot_y)
      context.scale(dot_width, dot_height)
      context.circle_to(0, 0, 1).fill
    end

    4.times do |i|
      context.save do
        context.translate(dot_x + dot_step * (i + 1), dot_y)
        context.scale(small_dot_width, small_dot_height)
        context.circle_to(0, 0, 1).fill
      end
    end
   end

  # Ghost
  ghost_x = 0.59
  ghost_x_step = 0.03
  ghost_y = 0.42
  ghost_y_step = 0.04
  ghost_width = 0.18
  ghost_height = 0.29
  ghost_radius= 0.08
  ghost = Proc.new do
    context.move_to(ghost_x, ghost_y)
    context.line_to(ghost_x, ghost_y + ghost_height)
    context.curve_to(ghost_x + ghost_width / 3.0,
                     ghost_y + ghost_height + ghost_radius,
                     ghost_x + ghost_width * (2.0 / 3.0),
                     ghost_y + ghost_height + ghost_radius,
                     ghost_x + ghost_width,
                     ghost_y + ghost_height)
    context.line_to(ghost_x + ghost_width, ghost_y)
    i = 0
    (ghost_x + ghost_width).step(ghost_x, -ghost_x_step) do |x|
      context.line_to(x, ghost_y + ghost_y_step * (i % 2))
      i += 1
    end
  end
  context.set_rgb_color(*blue)
  context.fill(&ghost)
  context.set_rgb_color(*cyan)
  context.stroke(&ghost)

  # Ghost Eyes
  eye_x = 0.62
  eye_y = 0.63
  eye_space = 0.06
  white_eye_width = 0.03
  white_eye_height = 0.04
  black_eye_width = 0.01
  black_eye_height = 0.02

  context.set_rgb_color(*white)
  context.rect_filled(eye_x, eye_y,
                      white_eye_width, white_eye_height)
  context.rect_filled(eye_x + eye_space, eye_y,
                      white_eye_width, white_eye_height)
  
  context.set_rgb_color(*black)
  context.rect_filled(eye_x, eye_y, black_eye_width, black_eye_height)
  context.rect_filled(eye_x + eye_space, eye_y,
                      black_eye_width, black_eye_height)
end

job.close

config["Printer"] = "GENERIC"
# or
#  printer = Gnome::GPARoot.printers.find{|pr| /Postscript/i =~ pr.value}.id
#  config["Printer"] = printer
setup_config.call
job.print_to_file("gp-pac.ps")
job.print

config["Printer"] = "PDF"
# or
#  printer = Gnome::GPARoot.printers.find{|pr| /PDF/i =~ pr.value}.id
#  config["Printer"] = printer
setup_config.call
job.print_to_file("gp-pac.pdf")
job.print
