=begin
  gtkglarea1.rb draw a triangle on a grey background
  this script need the opengl-bindings gem from :
  https://github.com/vaiorabbit/ruby-opengl

  gem install opengl-bindings

  sources:
  http://antongerdelan.net/opengl/hellotriangle.html
  opengl-bindings/sample/RedBook/varray
  opengl-bindings/sample/GLES/gles.rb
  http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
  https://developer.gnome.org/gtk3/stable/GtkGLArea.html
  https://www.bassi.io/articles/2015/02/17/using-opengl-with-gtk/
  http://stackoverflow.com/questions/30337845/gldrawarrays-not-working-using-gtkglarea-in-gtk3
  http://gamedev.stackexchange.com/questions/8042/vertex-array-object-opengl
=end

require "gtk3"
require "opengl"

unless Gtk::Version.or_later?(3, 16, 0)
  puts "This sample requires GTK+ 3.16.0 or later: #{Gtk::Version::STRING}"
  exit
end

OpenGL.load_lib

include OpenGL

window = Gtk::Window.new("OpenGL widget test")

window.set_size_request(400, 400)
glarea = Gtk::GLArea.new

window.add(glarea)

gl_vao = nil

glarea.signal_connect("realize") do |widget|

  widget.make_current

  # Check can only be done when we have an OpenGL context
  # https://www.opengl.org/wiki/FAQ#What_is_an_OpenGL_context_and_why_do_you_need_a_window_to_do_GL_rendering.3F
  # https://www.opengl.org/wiki/FAQ#How_do_I_tell_what_version_of_OpenGL_I.27m_using.3F

  version = glGetString(GL_VERSION).to_s.split(" ").first.split(".").collect(&:to_i)
  unless version[0] >= 3 && version[1] >= 2
    puts "Gtk::GLArea widget require an OpenGL version >= 3.2, your version is #{version[0]}.#{version[1]}"
    exit 1
  end

  puts "realize"
  # Define a triangle in a vertex buffer ( Vertex Buffer Object)

  points = [
    0.0, 0.5, 0.0,  # x1, y1, z1
    0.5, -0.5, 0.0, # x2, y2, z2
    -0.5, -0.5, 0.0 # x3, y3, z3
  ]
  # Create a vertex array object (VAO) and "bind" it (set it as the current VAO)
  vao_buf = "    "
  glGenVertexArrays(1, vao_buf)
  gl_vao = vao_buf.unpack("L")[0]
  glBindVertexArray(gl_vao)

  # We copy points onto the graphics card in a unit called vertex buffer object (vbo).
  # Create an empty buffer
  vbo_buf = "    "
  glGenBuffers(1, vbo_buf)
  g_vbo = vbo_buf.unpack("L")[0]
  # Set the empty buffer as the current OpenGL's state machine by "binding"
  glBindBuffer(GL_ARRAY_BUFFER, g_vbo)
  # Copy the points in the currently bound buffer
  glBufferData(GL_ARRAY_BUFFER,
               3 * 4 * Fiddle::SIZEOF_FLOAT,
               points.pack("F*"),
               GL_STATIC_DRAW
              )

  # First attribute buffer : vertices
  glEnableVertexAttribArray(0)
  glVertexAttribPointer(0,        # No particular reason for 0
                        3,        # size
                        GL_FLOAT, # type
                        GL_FALSE, # normalized?
                        0,        # stride
                        0         # array buffer offset
                       )
  # Clear the current VAO
  glBindVertexArray(0)
end

glarea.signal_connect("render") do |_widget, _context|
  puts "render"
  glClearColor(0.3, 0.3, 0.3, 1)
  glClear(GL_COLOR_BUFFER_BIT)
  # Reload the VAO we created previously
  glBindVertexArray(gl_vao)
  glDrawArrays(GL_TRIANGLES, 0, 3)

  # we completed our drawing; the draw commands will be
  # flushed at the end of the signal emission chain, and
  # the buffers will be drawn on the window
  true # to stop other handlers from being invoked for the event.
  # false to propagate the event further.
end

window.signal_connect("destroy") { Gtk.main_quit }

window.show_all

Gtk.main
