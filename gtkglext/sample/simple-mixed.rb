#
# simple-mixed.rb:
# Simple GtkGLExt example for mixing OpenGL and GDK rendering.
#
# C original version written by Naofumi Yasufuku <naofumi@users.sourceforge.net>
# Ruby port by Vincent Isambart
#

require 'gtkglext'

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

LIGHT_DIFFUSE  = [1.0, 0.0, 0.0, 1.0]
LIGHT_POSITION = [1.0, 1.0, 1.0, 0.0]
def realize(w)
    glcontext = w.gl_context
    gldrawable = w.gl_drawable

    #*** OpenGL BEGIN ***
    return if !gldrawable.gl_begin(glcontext)

    qobj = GLU.NewQuadric
    GLU.QuadricDrawStyle(qobj, GLU::FILL)
    GL.NewList(1, GL::COMPILE)
    GLU.Sphere(qobj, 1.0, 20, 20)
    GL.EndList

    GL.Light(GL::LIGHT0, GL::DIFFUSE, LIGHT_DIFFUSE)
    GL.Light(GL::LIGHT0, GL::POSITION, LIGHT_POSITION)
    GL.Enable(GL::LIGHTING)
    GL.Enable(GL::LIGHT0)
    GL.Enable(GL::DEPTH_TEST)

    GL.ClearColor(1.0, 1.0, 1.0, 1.0)
    GL.ClearDepth(1.0)

    GL.Viewport(0, 0, w.allocation.width, w.allocation.height)

    GL.MatrixMode(GL::PROJECTION)
    GL.LoadIdentity
    GLU.Perspective(40.0, 1.0, 1.0, 10.0)

    GL.MatrixMode(GL::MODELVIEW)
    GL.LoadIdentity
    GLU.LookAt(0.0, 0.0, 3.0,
               0.0, 0.0, 0.0,
               0.0, 1.0, 0.0)
    GL.Translate(0.0, 0.0, -3.0)

    #*** OpenGL END ***
    gldrawable.gl_end
end

# Init GTK
Gtk.init
# Init GtkGlExt
Gtk::GL.init
# Query OpenGL extension version
major, minor = Gdk::GL.query_version
puts "\nOpenGL extension version - #{major}.#{minor}\n"

# Configure OpenGL-capable visual
# Try single-buffered visual
glconfig = Gdk::GLConfig.new(Gdk::GLConfig::MODE_RGB |
                             Gdk::GLConfig::MODE_DEPTH)
if !glconfig
    puts "*** No appropriate OpenGL-capable visual found.\n"
    exit 1
end

examine_gl_config_attrib(glconfig)

# Top-level window
window = Gtk::Window.new
window.title = "simple"
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
drawing_area.set_size_request(200, 200)
# Set OpenGL-capability to the widget
drawing_area.set_gl_capability(glconfig)

drawing_area.signal_connect_after("realize") { |w| realize(w) }
drawing_area.signal_connect("configure_event") do |w, e|
    glcontext = w.gl_context
    gldrawable = w.gl_drawable

    if gldrawable.gl_begin(glcontext) #*** OpenGL BEGIN ***
        GL.Viewport(0, 0, w.allocation.width, w.allocation.height)
        gldrawable.gl_end #*** OpenGL END ***
        true
    else
        false
    end
end
drawing_area.signal_connect("expose_event") do |w,e|
    glcontext = w.gl_context
    gldrawable = w.gl_drawable

    if gldrawable.gl_begin(glcontext) #*** OpenGL BEGIN ***
        GL.Clear(GL::COLOR_BUFFER_BIT | GL::DEPTH_BUFFER_BIT)

        # Sync.
        gldrawable.wait_gl

        # GDK renderingfg_gc(w.state),
        gldrawable.draw_rectangle(w.style.fg_gc(w.state),
                                  true,
                                  w.allocation.width/10,
                                  w.allocation.height/10,
                                  w.allocation.width*8/10,
                                  w.allocation.height*8/10)

        # Sync.
        gldrawable.wait_gdk

        GL.CallList(1)

        GL.Flush

        gldrawable.gl_end #*** OpenGL END ***
        true
    else
        false
    end
end

vbox.pack_start(drawing_area)
drawing_area.show

# Simple quit button
button = Gtk::Button.new("Quit")
button.signal_connect("clicked") do
    window.destroy
    Gtk.main_quit
end
vbox.pack_start(button, false, false)
button.show

# Show window
window.show

# Main loop
Gtk.main
