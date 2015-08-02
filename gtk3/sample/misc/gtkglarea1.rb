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

def getlibdir
  # http://www.pilotlogic.com/sitejoom/index.php/wiki?id=398<F37>
  # 32              64
  # /usr/lib        /usr/lib64       redhat, mandriva
  # /usr/lib32      /usr/lib64       arch, gento
  # /usr/lib        /usr/lib64       slackware
  # /usr/lib/i386.. /usr/lib/x86_64..debian
  libs = Dir.glob("/usr/lib*/libGL.so") # libs in /usr/lib or /usr/lib64 for most distribs
  libs = Dir.glob("/usr/lib*/*/libGL.so") if libs.empty? # debian like
  if libs.empty?
    puts "no libGL.so"
    exit 1
  end
  # Get the same architecture that the runnning ruby
  if 1.size == 8 # 64 bits
    File.dirname(libs.grep(/64/).first)
  else # 32 bits
    File.dirname(libs.first)
  end
end

case OpenGL.get_platform
when :OPENGL_PLATFORM_WINDOWS
  OpenGL.load_lib("opengl32.dll", "C:/Windows/System32")
when :OPENGL_PLATFORM_MACOSX
  OpenGL.load_lib("libGL.dylib", "/System/Library/Frameworks/OpenGL.framework/Libraries")
when :OPENGL_PLATFORM_LINUX
  OpenGL.load_lib("libGL.so", getlibdir)
else
  fail "Unsupported platform."
end

include OpenGL

window = Gtk::Window.new("OpenGL widget test")

window.set_size_request(400, 400)
glarea = Gtk::GLArea.new

window.add(glarea)

gl_vao = nil

glarea.signal_connect("realize") do |widget|
  widget.make_current
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
