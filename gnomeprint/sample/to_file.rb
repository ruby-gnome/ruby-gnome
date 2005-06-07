require "gnomeprint2"
require "gdk_pixbuf2" # for loading image.

class Renderer
  def initialize(filename)
    @job = Gnome::PrintJob.new
    @context = @job.context
    @config = @job.config
    @filename = filename
    init_setting
    draw
    @job.close
  end
  
  def print
    @job.print
  end
  
  private
  def init_setting
    init_printer
    @job.print_to_file(@filename)
    key = Gnome::PrintConfig::KEY_DOCUMENT_NAME
    @config[key] = "Sample gnome-print document"
  end

  def init_printer
    printers = Gnome::GPARoot.printers
    target_printer = find_printer(printers)
    
    if target_printer.nil?
      printer_names = printers.collect {|x| x.value}
      raise "Could not find available printer in #{printer_names.join(', ')}"
    end
    
    @config["Printer"] = target_printer.id
    if @config["Printer", :string] != target_printer.id
      raise "Could not set printer to #{target_printer.value}"
    end
  end
  
  def find_printer(printers)
    case File.extname(@filename)
    when /\.ps/i
      printers.find do |printer|
        /Postscript/i =~ printer.value
      end
    when /\.pdf/i
      printers.find do |printer|
        /PDF/i =~ printer.value
      end
    else
      nil
    end
  end
  
  def draw
    @context.begin_page("1")
    draw_line
    draw_rectangle
    draw_arc
    draw_path
    draw_curve
    draw_text
    draw_pango
    draw_image
    @context.show_page
  end

  def draw_line
    @context.save do
      @context.set_rgb_color(0, 0, 1)

      label("line", 150, 150)
      
      @context.move_to(100, 100)
      @context.line_to(200, 200)
      @context.stroke
      @context.line_stroked(100, 500, 200, 200)
      # @context.line_stroked(200, 200, 100, 500)
    end
  end

  def draw_rectangle
    @context.save do
      @context.set_rgb_color(0, 0.5, 0.5)

      label("rectangle", 310, 300)
      
      @context.rect_stroked(200, 200, 100, 100)
      @context.rect_filled(200, 300, 100, 100)
    end
  end

  def draw_arc
    @context.save do
      @context.set_rgb_color(0.5, 0.5, 0)

      label("arc", 70, 450)
      
      @context.arc_to(50, 450, 10, 0, 90, true)
      @context.stroke
      @context.save do
        @context.set_rgb_color(1, 0.4, 0.9)
        @context.set_opacity(0.7)
        @context.arc_to(40, 450, 10, 0, 359, false)
        @context.fill
      end
      @context.arc_to(40, 450, 10, 0, 359, false)
      @context.stroke
    end
  end

  def draw_path
    draw_bpath
    draw_vpath
  end

  def draw_bpath
    @context.save do
      @context.set_rgb_color(1, 0, 0)
      
      label("bpath", 500, 180)
      
      path = [
        [Art::MOVETO_OPEN, 580, 180],
        [Art::CURVETO, 630, 180, 350, 120, 420, 150],
        [Art::LINETO, 300, 50],
        [Art::END],
      ]
      bpath = Art::Bpath.new(path)
      @context.path(bpath)

      # append
      path = [
        [Art::LINETO, 500, 180],
        [Art::END],
      ]
      bpath = Art::Bpath.new(path)
      @context.path(bpath)
      
      @context.stroke
    end
  end

  def draw_vpath
    @context.save do
      @context.set_rgb_color(0, 1, 0)

      label("vpath", 400, 600)
      
      path = [
        [Art::MOVETO_OPEN, 400, 600],
        [Art::LINETO, 440, 680],
        [Art::END],
      ]
      vpath = Art::Vpath.new(path)
      @context.path(vpath)

      # append
      path = [
        [Art::LINETO, 500, 700],
        [Art::END],
      ]
      vpath = Art::Vpath.new(path)
      @context.path(vpath)
      
      @context.stroke
    end
  end
  
  def draw_curve
    @context.save do
      @context.set_rgb_color(0.5, 0, 0.5)
      
      label("curve", 500, 450)
      
      @context.move_to(500, 300)
      @context.curve_to(470, 400, 470, 500, 520, 600)
      @context.stroke
    end
  end
  
  def draw_text
    @context.move_to(100, 50)
    @context.show("abcde")
  end
  
  def draw_pango
    @context.move_to(350, 350)
    markupped_text = "<span color='red'>"
    markupped_text << "<span size='larger'>Pa</span>"
    markupped_text << "<span underline='double'>n</span>"
    markupped_text << "<span weight='bold' style='italic'>go</span>"
    markupped_text << "</span>"
    attrs, text = Pango.parse_markup(markupped_text)
    layout = @context.create_layout
    layout.text = text
    layout.set_attributes(attrs)
    @context.layout(layout)
  end

  def draw_image
    filename = Dir["**/*.png"].first
    pixbuf = Gdk::Pixbuf.new(filename)
    @context.save do
      @context.translate(350, 500)
      @context.scale(pixbuf.width, pixbuf.height)
      args = [pixbuf.pixels, pixbuf.width, pixbuf.height, pixbuf.rowstride]
      if pixbuf.has_alpha?
        @context.rgba_image(*args)
      else
        @context.rgb_image(*args)
      end
    end
  end

  def label(text, x, y)
    @context.save do
      @context.move_to(x, y)
      layout = @context.create_layout
      layout.text = text
      layout.context_changed
      @context.layout(layout)
    end
  end
  
end

Renderer.new("sample.ps").print
Renderer.new("sample.pdf").print
