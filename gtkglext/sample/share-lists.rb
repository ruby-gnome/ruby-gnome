#
# share_lists.rb:
# Simple Ruby/GtkGLExt display list sharing example.
#
# C original version written by Naofumi Yasufuku <naofumi@users.sourceforge.net>
# Ruby port by Vincent Isambart
#

require 'gtkglext'

RED    = [1.0, 0.0, 0.0, 1.0]
YELLOW = [1.0, 1.0, 0.0, 1.0]
GREEN  = [0.0, 1.0, 0.0, 1.0]

LIGHT_POSITION = [1.0, 1.0, 1.0, 0.0]
def init_gl(l)
    GL.Light(GL::LIGHT0, GL::DIFFUSE, l)
    GL.Light(GL::LIGHT0, GL::POSITION, LIGHT_POSITION)
    GL.Enable(GL::LIGHTING)
    GL.Enable(GL::LIGHT0)
    GL.Enable(GL::DEPTH_TEST)

    GL.ClearColor(1.0, 1.0, 1.0, 1.0)
    GL.ClearDepth(1.0)

    GL.MatrixMode(GL::PROJECTION)
    GL.LoadIdentity
    GLU.Perspective(40.0, 1.0, 1.0, 10.0)

    GL.MatrixMode(GL::MODELVIEW)
    GL.LoadIdentity
    GLU.LookAt(0.0, 0.0, 3.0,
               0.0, 0.0, 0.0,
               0.0, 1.0, 0.0)
    GL.Translate(0.0, 0.0, -3.0)
end

def realize_main(w, l)
    glcontext = w.gl_context
    gldrawable = w.gl_drawable

    #*** OpenGL BEGIN ***
    return if !gldrawable.gl_begin(glcontext)

    qobj = GLU.NewQuadric
    GLU.QuadricDrawStyle(qobj, GLU::FILL)
    GL.NewList(1, GL::COMPILE)
    GLU.Sphere(qobj, 1.0, 20, 20)
    GL.EndList

    init_gl(l)

    #*** OpenGL END ***
    gldrawable.gl_end
end

def realize_sub(w, l)
    glcontext = w.gl_context
    gldrawable = w.gl_drawable

    #*** OpenGL BEGIN ***
    return if !gldrawable.gl_begin(glcontext)

    init_gl(l)

    #*** OpenGL END ***
    gldrawable.gl_end
end

def configure_event(w)
    glcontext = w.gl_context
    gldrawable = w.gl_drawable

    #*** OpenGL BEGIN ***
    return false if !gldrawable.gl_begin(glcontext)

    GL.Viewport(0, 0, w.allocation.width, w.allocation.height)

    #*** OpenGL END ***
    gldrawable.gl_end

    true
end

def expose_event(w)
    glcontext = w.gl_context
    gldrawable = w.gl_drawable

    #*** OpenGL BEGIN ***
    return false if !gldrawable.gl_begin(glcontext)

    GL.Clear(GL::COLOR_BUFFER_BIT | GL::DEPTH_BUFFER_BIT)
    GL.CallList(1)

    if gldrawable.double_buffered?
        gldrawable.swap_buffers
    else
        GL.Flush
    end

    #*** OpenGL END ***
    gldrawable.gl_end

    true
end

# Init GTK
Gtk.init
# Init GtkGlExt
Gtk::GL.init

# Configure OpenGL-capable visual
# Try double-buffered visual
glconfig = Gdk::GLConfig.new(Gdk::GLConfig::MODE_RGB   |
                             Gdk::GLConfig::MODE_DEPTH |
                             Gdk::GLConfig::MODE_DOUBLE)
if !glconfig
    puts "*** Cannot find the double-buffered visual.\n"
    puts "*** Trying single-buffered visual.\n"
    # Try single-buffered visual
    glconfig = Gdk::GLConfig.new(Gdk::GLConfig::MODE_RGB  |
                                 Gdk::GLConfig::MODE_DEPTH)
    if !glconfig
        puts "*** No appropriate OpenGL-capable visual found.\n"
        exit 1
    end
end

# Top-level window
window = Gtk::Window.new
window.title = "simple"
# Perform the resize immediately
window.resize_mode = Gtk::RESIZE_IMMEDIATE
# Get automatically redrawn if any of their children changed allocation.
window.reallocate_redraws = true
window.signal_connect("delete_event") { Gtk.main_quit }

# VBox
vbox = Gtk::VBox.new
window.add(vbox)
vbox.show

# OpenGL drawing area #1 (main: creates display lists).
drawing_area = Gtk::DrawingArea.new
drawing_area.set_size_request(120, 120)
drawing_area.set_gl_capability(glconfig)

drawing_area.signal_connect_after("realize") { |w| realize_main(w, RED) }
drawing_area.signal_connect("configure_event") { |w, e| configure_event(w) }
drawing_area.signal_connect("expose_event") { |w, e| expose_event(w) }

vbox.pack_start(drawing_area)
drawing_area.show

# Get OpenGL rendering context.
drawing_area.realize
glcontext = drawing_area.gl_context

# OpenGL drawing area #2 (sub: shares display lists).
drawing_area = Gtk::DrawingArea.new
drawing_area.set_size_request(120, 120)
drawing_area.set_gl_capability(glconfig,
                               glcontext) # share list

drawing_area.signal_connect_after("realize")   { |w| realize_sub(w, YELLOW) }
drawing_area.signal_connect("configure_event") { |w, e| configure_event(w) }
drawing_area.signal_connect("expose_event")    { |w, e| expose_event(w) }

vbox.pack_start(drawing_area)
drawing_area.show

# OpenGL drawing area #3 (sub: shares display lists).
drawing_area = Gtk::DrawingArea.new
drawing_area.set_size_request(120, 120)
drawing_area.set_gl_capability(glconfig,
                               glcontext) # share list

drawing_area.signal_connect_after("realize")   { |w| realize_sub(w, GREEN) }
drawing_area.signal_connect("configure_event") { |w, e| configure_event(w) }
drawing_area.signal_connect("expose_event")    { |w, e| expose_event(w) }

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
