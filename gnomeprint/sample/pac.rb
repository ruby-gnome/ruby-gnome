=begin
  pac.rb - Ruby/GnomePrint sample script.

  Original: pac.rb in http://www.artima.com/rubycs/articles/pdf_writer3.html

  Copyright (c) 2005 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: pac.rb,v 1.2 2005/10/14 06:35:42 ktou Exp $
=end

require "gnomeprint2"

job = Gnome::PrintJob.new
context = job.context
config = job.config

paper_name = config[Gnome::PrintConfig::KEY_PAPER_SIZE]
paper = Gnome::PrintPaper.get(paper_name)

setup_config = Proc.new do
  pt = Gnome::PrintUnit.get_by_abbreviation("Pt")
  config[Gnome::PrintConfig::KEY_PAPER_SIZE] = "Custom"
  config.set(Gnome::PrintConfig::KEY_PAPER_WIDTH, paper.height, pt)
  config.set(Gnome::PrintConfig::KEY_PAPER_HEIGHT, paper.width, pt)
end

width = paper.height
height = paper.width

white = [65535, 65535, 65535]
black = [0, 0, 0]
magenta = [65535, 0, 65535]
cyan = [0, 65535, 65535]
yellow = [65535, 65535, 0]
blue = [0, 0, 65535]

context.begin_page("1") do
  context.set_rgb_color(*black)
  context.rect_filled(0, 0, width, height)

  # Wall
  context.set_rgb_color(*magenta)
  context.rounded_rect_filled(20, 500, 750, 20, 10)
  context.set_rgb_color(*cyan)
  context.rounded_rect_stroked(20, 500, 750, 20, 10)
  
  context.set_rgb_color(*magenta)
  context.rounded_rect_filled(20, 200, 750, 20, 10)
  context.set_rgb_color(*cyan)
  context.rounded_rect_stroked(20, 200, 750, 20, 10)
  
  # Body
  context.set_rgb_color(*yellow)
  context.fill do
    context.circle_to(150, 350, 100)
  end
  context.set_rgb_color(*black)
  context.stroke do
    context.circle_to(150, 350, 100)
  end

  # Mouth
  context.set_rgb_color(*black)
  context.fill do
    context.move_to(150, 350)
    context.arc_to(150, 350, 100, -30, 30, false)
  end

  # Dot
  context.set_rgb_color(*yellow)
  context.circle_to(250, 350, 20).fill
  context.circle_to(300, 350, 10).fill
  context.circle_to(350, 350, 10).fill
  context.circle_to(400, 350, 10).fill
  context.circle_to(450, 350, 10).fill

  # Ghost
  ghost = Proc.new do
    context.move_to(500, 250)
    context.line_to(500, 425)
    context.curve_to(550, 475, 600, 475, 650, 425)
    context.line_to(650, 250)
    context.line_to(625, 275)
    context.line_to(600, 250)
    context.line_to(575, 275)
    context.line_to(550, 250)
    context.line_to(525, 275)
    context.line_to(500, 250)
  end
  context.set_rgb_color(*blue)
  context.fill(&ghost)
  context.set_rgb_color(*cyan)
  context.stroke(&ghost)

  # Ghost Eyes
  context.set_rgb_color(*white)
  context.rect_filled(525, 375, 25, 25)
  context.rect_filled(575, 375, 25, 25)
  
  context.set_rgb_color(*black)
  context.rect_filled(525, 375, 10, 10)
  context.rect_filled(575, 375, 10, 10)
end

job.close

config["Printer"] = "GENERIC"
# or
#  printer = Gnome::GPARoot.printers.find{|pr| /Postscript/i =~ pr.value}.id
#  config["Printer"] = printer
setup_config.call
job.print_to_file("pac.ps")
job.print

config["Printer"] = "PDF"
# or
#  printer = Gnome::GPARoot.printers.find{|pr| /CONTEXT/i =~ pr.value}.id
#  config["Printer"] = printer
setup_config.call
job.print_to_file("pac.pdf")
job.print
