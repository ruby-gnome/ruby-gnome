#
# font.rb:
# Simple Ruby/GtkGLExt bitmap font rendering example.
#
# C original version written by Naofumi Yasufuku <naofumi@users.sourceforge.net>
# Ruby port by Vincent Isambart
#

require 'gtkglext'
require 'pango'

FONT_STRING = "courier 12"

def examine_gl_config_attrib(glconfig)
    puts "\nOpenGL visual configurations :\n\n"

    [
      :rgba?, :double_buffered?, :stereo?, :has_alpha?, :has_depth_buffer?,
      :has_stencil_buffer?, :has_accum_buffer?
    ].each { |id| puts "glconfig.#{id} = #{glconfig.send(id)}\n" }

    puts "\n"

    [
      :USE_GL, :BUFFER_SIZE, :LEVEL, :RGBA, :DOUBLEBUFFER, :STEREO, :AUX_BUFFERS,
      :RED_SIZE, :GREEN_SIZE, :BLUE_SIZE, :ALPHA_SIZE, :DEPTH_SIZE, :STENCIL_SIZE,
      :ACCUM_RED_SIZE, :ACCUM_GREEN_SIZE, :ACCUM_BLUE_SIZE, :ACCUM_ALPHA_SIZE
    ].each do |id|
        val = glconfig.get_attrib(Gdk::GLConfig.const_get(id))
        if val == nil
            puts "*** Cannot get Gdk::GLConfig::#{id} attribute value\n"
        else
            puts "Gdk::GLConfig::#{id} = #{val}\n"
        end
    end

    puts "\n"
end

def realize(w)
    glcontext = w.gl_context
    gldrawable = w.gl_drawable

    light_diffuse  = [1.0, 0.0, 0.0, 1.0]
    light_position = [1.0, 1.0, 1.0, 0.0]

    #*** OpenGL BEGIN ***
    gldrawable.gl_begin(glcontext) do
        # Generate font display lists.
        $font_list_base = GL.GenLists(128)

        font_desc = Pango::FontDescription.new FONT_STRING

        font = Gdk::GL.use_pango_font(font_desc, 0, 128, $font_list_base)
        if !font
            puts "*** Can't load font '#{FONT_STRING}'\n"
            exit 1
        end

        font_metrics = font.metrics
        $font_height = font_metrics.ascent + font_metrics.descent
        $font_height = Pango.pixels($font_height)

        GL.ClearColor(1.0, 1.0, 1.0, 1.0)
        GL.ClearDepth(1.0)

        GL.Viewport(0, 0, w.allocation.width, w.allocation.height)

        GL.MatrixMode(GL::PROJECTION)
        GL.LoadIdentity
        GL.Ortho(0.0, w.allocation.width,
                 0.0, w.allocation.height,
                 -1.0, 1.0);

        GL.MatrixMode(GL::MODELVIEW)
        GL.LoadIdentity
    end
end

# Init GTK
Gtk.init
# Init GtkGlExt
Gtk::GL.init
# Query OpenGL extension version
major, minor = Gdk::GL.query_version
puts "\nOpenGL extension version - #{major}.#{minor}\n"

# Configure OpenGL-capable visual
# Try double-buffered visual
glconfig = Gdk::GLConfig.new(Gdk::GLConfig::MODE_RGB   |
                             Gdk::GLConfig::MODE_DEPTH |
                             Gdk::GLConfig::MODE_DOUBLE)
if !glconfig
    puts "*** Cannot find the double-buffered visual.\n"
    puts "*** Trying single-buffered visual.\n"
    # Try single-buffered visual
    glconfig = Gdk::GLConfig.new(Gdk::GLConfig::MODE_RGB | Gdk::GLConfig::MODE_DEPTH)
    if !glconfig
        puts "*** No appropriate OpenGL-capable visual found.\n"
        exit 1
    end
end

examine_gl_config_attrib(glconfig)

# Top-level window
window = Gtk::Window.new
window.title = "font"
# Perform the resize immediately
window.resize_mode = Gtk::RESIZE_IMMEDIATE
# Get automatically redrawn if any of their children changed allocation.
window.reallocate_redraws = true
window.signal_connect("delete_event") do
    Gtk.main_quit
    true
end

# VBox
vbox = Gtk::VBox.new
window.add(vbox)
vbox.show

# Drawing area for drawing OpenGL scene
drawing_area = Gtk::DrawingArea.new
drawing_area.set_size_request(640, 240)
# Set OpenGL-capability to the widget
drawing_area.set_gl_capability(glconfig)

drawing_area.signal_connect_after("realize") { |w| realize(w) }
drawing_area.signal_connect("configure_event") do |w, e|
    glcontext = w.gl_context
    gldrawable = w.gl_drawable

    gldrawable.gl_begin(glcontext) do
        GL.Viewport(0, 0, w.allocation.width, w.allocation.height)

        GL.MatrixMode(GL::PROJECTION)
        GL.LoadIdentity
        GL.Ortho(0.0, w.allocation.width,
                 0.0, w.allocation.height,
                 -1.0, 1.0)

        GL.MatrixMode(GL::MODELVIEW)
        GL.LoadIdentity
        true
    end
end
drawing_area.signal_connect("expose_event") do |w,e|
    glcontext = w.gl_context
    gldrawable = w.gl_drawable

    gldrawable.gl_begin(glcontext) do
        GL.Clear(GL::COLOR_BUFFER_BIT | GL::DEPTH_BUFFER_BIT)

        GL.Color(0.0, 0.0, 0.0)
        2.downto(-2) do |i|
            GL.RasterPos(10.0, 0.5*w.allocation.height + i*$font_height)
            (?\ ..?Z).each { |j| GL.CallList($font_list_base+j) }
        end

        # Show font description string.
        GL.Color(1.0, 0.0, 0.0)
        GL.RasterPos(10.0, 10.0)
        GL.ListBase($font_list_base)
        GL.CallLists(FONT_STRING)

        if gldrawable.double_buffered?
            gldrawable.swap_buffers
        else
            GL.Flush
        end
        true
    end
end

vbox.pack_start(drawing_area)
drawing_area.show

# Simple quit button
button = Gtk::Button.new("Quit")
button.signal_connect("clicked") do
    Gtk.main_quit
end
vbox.pack_start(button, false, false)
button.show

# Show window
window.show

# Main loop
Gtk.main
