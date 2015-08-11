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

OpenGL.load_lib

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
