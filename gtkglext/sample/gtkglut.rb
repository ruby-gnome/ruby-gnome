#
# gtkglut.rb:
# Sort of GLUT-compatible library on top of Ruby/GtkGKExt.
# It does only work for simple cases, you should really use Ruby/GtkGKExt
# instead. If you really want to use GLUT, use yoshi's Ruby/GLUT.
#
# But it can be an interesting introduction for those who only know GLUT.
# It can show them that using GLUT is quite a bad idea ;o),
# that it must be using lots of global variables...
#
# I am sure there is a better way to do most things, but I do not know Gtk+
# and Ruby well enough. You are free to improve it :o)
#
# If you really want to use it, put "require 'gtkglut'" instead of
# "require 'glut'" at the beginning of your file.
# You can then (try to) use it as yoshi's Ruby/GLUT binding.
#
# written by Vincent Isambart <isambart@netcourrier.com>
#
require 'gtkglext'

module GLUT
    RGB     = Gdk::GL::MODE_RGB
    RGBA    = Gdk::GL::MODE_RGBA
    SINGLE  = Gdk::GL::MODE_SINGLE
    DOUBLE  = Gdk::GL::MODE_DOUBLE
    DEPTH   = Gdk::GL::MODE_DEPTH
    ACCUM   = Gdk::GL::MODE_ACCUM
    ALPHA   = Gdk::GL::MODE_ALPHA
    STENCIL = Gdk::GL::MODE_STENCIL

    @@window_size = [300, 300]
    @@window_pos  = [100, 100]

    def GLUT.Init(argc = 1, argv = [])
        Gtk.init
        Gtk::GL.init
    end

    def GLUT.InitWindowPosition(x, y)
        @@window_pos = [x, y]
    end

    def GLUT.InitWindowSize(w, h)
        @@window_size = [w, h]
    end

    def GLUT.InitDisplayMode(mode)
        @@glconfig = Gdk::GL::Config.new(mode)
        raise "can't initialize OpenGL" if !@@glconfig
    end

    def GLUT.SwapBuffers
        @@drawing_area.gl_drawable.swap_buffers
    end

    def GLUT.PostRedisplay
        @@drawing_area.queue_draw
    end

    def GLUT.method_missing(id, *args)
        name = id.id2name
        if name[0..3] == 'Wire'
            Gdk::GL.send("draw_#{name.downcase[4..-1]}", false, *args)
        elsif name[0..4] == 'Solid'
            Gdk::GL.send("draw_#{name.downcase[5..-1]}", true,  *args)
        else
            raise NoMethodError, "undefined method `#{name}' for GLUT:Module"
        end
    end
    
    def GLUT.CreateWindow(name = "gtkglext")
        # Top-level window
        @@window = Gtk::Window.new
        # Perform the resize immediately
        @@window.resize_mode = Gtk::RESIZE_IMMEDIATE
        # Get automatically redrawn if any of their children
        # changed allocation.
        @@window.reallocate_redraws = true
        @@window.signal_connect("delete_event") { Gtk.main_quit }
        @@window.title = name

        @@window.set_events(@@window.events      |
            Gdk::Event::BUTTON_MOTION_MASK       |
            Gdk::Event::POINTER_MOTION_HINT_MASK |
            Gdk::Event::BUTTON_PRESS_MASK)

        # VBox
        vbox = Gtk::VBox.new
        @@window.add(vbox)
        vbox.show

        # Drawing area for drawing OpenGL scene
        @@drawing_area = Gtk::DrawingArea.new
        # Set OpenGL-capability to the widget
        @@drawing_area.set_gl_capability(@@glconfig)
        vbox.pack_start(@@drawing_area)
        @@drawing_area.show

        # The following is a big hack!
        # Do not do it in a Ruby/GtkGKExt program!
        # As with GLUT the OpenGL initializations are done between a
        # CreateWindow and a MainLoop, we must show the window (or else,
        # we can't get the context) and do a gl_begin.
        # In a real Ruby/GtkGLExt program, the OpenGL initialization stuff
        # should be in a signal_connect_after("realize") event callback,
        # which is called for the first display of the window,
        # during a window.show (and not after the window.show as I do here)
        @@window.show
        glcontext  = @@drawing_area.gl_context
        gldrawable = @@drawing_area.gl_drawable

        raise "cannot activate OpenGL" if !gldrawable.gl_begin(glcontext)
    end

    def GLUT.MainLoop
        @@drawing_area.gl_drawable.gl_end
        @@drawing_area.set_size_request(*@@window_size)
        Gtk.main
    end

    UP   = false
    DOWN = true

    LEFT_BUTTON   = 1
    MIDDLE_BUTTON = 2
    RIGHT_BUTTON  = 3

    def GLUT.MouseFunc(mouse)
        if mouse
            @@window.signal_connect("button_press_event")   { |w, e|
                mouse.call(e.button, DOWN, e.x, e.y) }
            @@window.signal_connect("button_release_event") { |w, e|
                mouse.call(e.button, UP  , e.x, e.y) }
        else
            @@window.signal_connect("button_press_event")   { |w, e| }
            @@window.signal_connect("button_release_event") { |w, e| }
        end
    end

    @@idle_num = nil
    def GLUT.IdleFunc(idle)
        Gtk.idle_remove(@@idle_num) if @@idle_num
        @@idle_num = nil
        @@idle_num = Gtk.idle_add { idle.call() } if idle
    end

    def GLUT.KeyboardFunc(keyboard)
        if keyboard
            @@window.signal_connect_after("key_press_event") do |i, e|
                keyval = nil
                case e.keyval
                    when Gdk::Keyval::GDK_Escape then keyval = 27
                    else keyval = e.keyval
                end
                # TODO: get the cursor pos
                keyboard.call(keyval, 0, 0)
            end
        else
            @@window.signal_connect_after("key_press_event") { |i, e| }
        end
    end

    def GLUT.DisplayFunc(display)
        if display
            @@drawing_area.signal_connect("expose_event") do |w, e|
                glcontext  = w.gl_context
                gldrawable = w.gl_drawable

                if gldrawable.gl_begin(glcontext)
                    display.call
                    gldrawable.gl_end
                    true
                else
                    false
                end
            end
        else
            @@drawing_area.signal_connect("expose_event") { |w, e| }
        end
    end

    def GLUT.ReshapeFunc(reshape)
        if reshape
            @@drawing_area.signal_connect("configure_event") do |w, e|
                glcontext  = w.gl_context
                gldrawable = w.gl_drawable

                if gldrawable.gl_begin(glcontext)
                    reshape.call(w.allocation.width, w.allocation.height)
                    gldrawable.gl_end
                    true
                else
                    false
                end
            end
        else
            @@drawing_area.signal_connect("configure_event") { |w, e| }
        end
    end
end
