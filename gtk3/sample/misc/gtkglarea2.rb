=begin
  gtkglarea2.rb draw a triangle using shaders on a grey background
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
  # /usr/lib/i386.. /usr/lib/x86_64..  debian
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

window = Gtk::Window.new("OpenGL widget shaders test")

window.set_size_request(400, 400)
glarea = Gtk::GLArea.new

window.add(glarea)

vertex_shader = <<EOF
#version 400
in vec3 vp;
void main () {
  gl_Position = vec4 (vp, 1.0);
};
EOF

fragment_shader = <<EOF
#version 400
out vec4 frag_colour;
void main () {
  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);
};
EOF

def display_compile_shader_log(shader_handle)
  info_log = " " * 1024
  glGetShaderInfoLog(shader_handle, 1023, nil, info_log)
  puts "Shader InfoLog:\n#{info_log}\n"
end

def compile_shader(shader_handle)
  # Compile the vertex shader
  glCompileShader(shader_handle)
  # Check the vertex shader compilation
  compiled_buf = "    "
  glGetShaderiv(shader_handle, GL_COMPILE_STATUS, compiled_buf)
  compiled = compiled_buf.unpack("L")[0]
  compiled
end

def setup_shaders(vertex, fragment)
  # Load the shaders sources
  vs_handle = glCreateShader(GL_VERTEX_SHADER)
  fs_handle = glCreateShader(GL_FRAGMENT_SHADER)

  glShaderSource(vs_handle,
                 1,
                 [vertex].pack("p"),
                 [vertex.bytesize].pack("I")
                )

  glShaderSource(fs_handle,
                 1,
                 [fragment].pack("p"),
                 [fragment.bytesize].pack("I")
                )

  vert_compiled = compile_shader(vs_handle)
  display_compile_shader_log(vs_handle) if vert_compiled == 0

  frag_compiled = compile_shader(fs_handle)
  display_compile_shader_log(fs_handle) if frag_compiled == 0

  return false if vert_compiled == 0 || frag_compiled == 0

  # Load those 2 shaders (vertex and fragment) into a GPU shader program
  prog_handle = glCreateProgram
  glAttachShader(prog_handle, vs_handle)
  glAttachShader(prog_handle, fs_handle)

  glLinkProgram(prog_handle)

  # Check if the program is valid
  linked_buf = "    "
  glGetProgramiv(prog_handle, GL_LINK_STATUS, linked_buf)
  linked = linked_buf.unpack("L")[0]
  if linked == 0
    info_log = " " * 1024
    glGetProgramInfoLog(prog_handle, 1023, nil, info_log)
    puts "Program InfoLog:\n#{info_log}\n"
  end
  return false if linked == 0

  return true, prog_handle
end

shader_program = nil
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
  vao_buf = "    "
  glGenVertexArrays(1, vao_buf)
  gl_vao = vao_buf.unpack("L")[0]
  glBindVertexArray(gl_vao)
  # We copy those points onto the graphics card in a unit called vertex buffer object (vbo).
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

  glBindVertexArray(0)
  # Load the shaders:
  _ok, shader_program = setup_shaders(vertex_shader, fragment_shader)
end

glarea.signal_connect("render") do |_widget, _context|
  puts "render"
  glClearColor(0.3, 0.3, 0.3, 1)
  glClear(GL_COLOR_BUFFER_BIT)
  # Set the created shader program as current
  glUseProgram(shader_program)
  glBindVertexArray(gl_vao)
  glDrawArrays(GL_TRIANGLES, 0, 3)
  true
end

glarea.signal_connect("unrealize") do |widget|
  puts "unrealize"
  widget.make_current
  glDeleteVertexArrays(1, [gl_vao].pack("L"))
  glDeleteProgram(shader_program)
end

window.signal_connect("destroy") { Gtk.main_quit }

window.show_all

Gtk.main
