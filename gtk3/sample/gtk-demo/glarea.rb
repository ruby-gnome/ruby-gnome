# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  OpenGL Area

 GtkGLArea is a widget that allows custom drawing using OpenGL calls.
=end

begin
  require "opengl"
rescue LoadError
  puts("opengl-bindings gem is required")
  raise
end

OpenGL.load_lib

class GlareaDemo
  include OpenGL

  X_AXIS, Y_AXIS, Z_AXIS = (0..2).to_a
  # The object we are drawing
  VERTEX_DATA = [0.0, 0.5, 0.0, 1.0,
                 0.5, -0.366, 0.0, 1.0,
                 -0.5, -0.366, 0.0, 1.0].freeze

  def initialize(main_window)
    # Rotation angles on each axis
    @rotation_angles = [0.0, 0.0, 0.0]
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "OpenGL Area"
    @window.set_default_size(400, 600)

    box = Gtk::Box.new(:vertical, false)
    box.margin = 12
    box.spacing = 6
    @window.add(box)

    @gl_area = Gtk::GLArea.new
    @gl_area.hexpand = true
    @gl_area.vexpand = true
    box.add(@gl_area)

    # We need to initialize and free GL resources, so we use
    # the realize and unrealize signals on the widget
    @gl_area.signal_connect "realize" do |widget|
      # We need to set up our state when we realize the GtkGLArea widget
      widget.make_current
      unless widget.error
        init_buffers
        @program, @mvp_location = init_shaders(widget)
      end
    end

    @gl_area.signal_connect "unrealize" do |widget|
      widget.make_current
      unless widget.error
        # We should tear down the state when unrealizing
        glDeleteProgram(@program)
      end
    end

    # The main "draw" call for GtkGLArea
    @gl_area.signal_connect "render" do |area, _context|
      return false if area.error

      # Clear the viewport
      glClearColor(0.5, 0.5, 0.5, 1.0)
      glClear(GL_COLOR_BUFFER_BIT)
      draw_triangle
      glFlush
      true
    end

    controls = Gtk::Box.new(:vertical, false)
    box.add(controls)
    controls.hexpand = true
    (0..2).each do |i|
      controls.add(create_axis_slider(i))
    end

    button = Gtk::Button.new(:label => "Quit")
    button.hexpand = true
    box.add(button)
    button.signal_connect "clicked" do
      @window.destroy
    end
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  private

  def init_buffers
    vao = "    "
    glGenVertexArrays(1, vao)
    gl_vao = vao.unpack("L")[0]
    @position_buffer = gl_vao
    glBindVertexArray(gl_vao)

    # This is the buffer that holds the vertices
    buffer = "    "
    glGenBuffers(1, buffer)
    gl_vbo = buffer.unpack("L")[0]
    glBindBuffer(GL_ARRAY_BUFFER, gl_vbo)
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * Fiddle::SIZEOF_FLOAT,
                 VERTEX_DATA.pack("F*"), GL_STATIC_DRAW)
    glBindBuffer(GL_ARRAY_BUFFER, 0)
  end

  # Initialize shaders and link them into a programm
  def init_shaders(widget)
    context = widget.context

    if context.use_es?
      vertex_path = "/glarea/glarea-gles.vs.glsl"
      fragment_path = "/glarea/glarea-gles.fs.glsl"
    else
      vertex_path = "/glarea/glarea-gl.vs.glsl"
      fragment_path = "/glarea/glarea-gl.fs.glsl"
    end
    src_vertex = Gio::Resources.lookup_data(vertex_path, 0)
    src_fragment = Gio::Resources.lookup_data(fragment_path, 0)
    vertex = create_shader(GL_VERTEX_SHADER, src_vertex)
    return [0, nil] if vertex.zero?
    fragment = create_shader(GL_FRAGMENT_SHADER, src_fragment)
    return [0, nil] if fragment.zero?

    program = glCreateProgram
    glAttachShader(program, vertex)
    glAttachShader(program, fragment)
    glLinkProgram(program)
    # Check if the program is valid
    linked_buf = "    "
    glGetProgramiv(program, GL_LINK_STATUS, linked_buf)
    status = linked_buf.unpack("L")[0]
    if status.zero?
      log_len = "    "
      glGetShaderiv(program, GL_INFO_LOG_LENGTH, log_len)
      len = log_len.unpack("L")[0]
      info_log = " " * (len + 1)
      glGetProgramInfoLog(program, len, nil, info_log)
      STDERR.puts "Linking failure:\n#{info_log}\n"
      glDeleteProgram(program)
      glDeleteShader(vertex)
      glDeleteShader(fragment)
      program = 0
    end
    mvp = glGetUniformLocation(program, "mvp")
    [program, mvp]
  end

  def create_shader(type, source)
    # Load the shaders sources
    shader = glCreateShader(type)

    glShaderSource(shader, 1, [source.pointer].pack("j"), [source.size].pack("I"))
    # Compile the vertex shader
    glCompileShader(shader)
    # Check the vertex shader compilation
    compiled_buf = "    "
    glGetShaderiv(shader, GL_COMPILE_STATUS, compiled_buf)
    status = compiled_buf.unpack("L")[0]
    if status.zero?
      log_len = "    "
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, log_len)
      len = log_len.unpack("L")[0]
      info_log = " " * (len + 1)
      glGetShaderInfoLog(shader, len, nil, info_log)
      type_name = type == GL_VERTEX_SHADER ? "vertex" : "fragment"
      STDERR.puts "Compile failure in #{type_name}:\n#{info_log}"
      return 0
    end
    shader
  end

  def draw_triangle
    # Compute the model view projection matrix using the rotation angles
    # specified through the GtkRange widgets
    mvp = compute_mvp
    # Use our program
    glUseProgram(@program)
    # Update the "mvp" matrix we use in the shader
    glUniformMatrix4fv(@mvp_location, 1, GL_FALSE, mvp.pack("F16"))
    # Use the vertices in our buffer
    glBindBuffer(GL_ARRAY_BUFFER, @position_buffer)
    glEnableVertexAttribArray(0)
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0)

    # Draw the three vertices as a triangle
    glDrawArrays(GL_TRIANGLES, 0, 3)

    # We finished using the buffers and program
    glDisableVertexAttribArray(0)
    glBindBuffer(GL_ARRAY_BUFFER, 0)
    glUseProgram(0)
  end

  def compute_mvp
    phi = @rotation_angles[X_AXIS]
    theta = @rotation_angles[Y_AXIS]
    psi = @rotation_angles[Z_AXIS]
    res = Array.new(16, 0.0)
    x = phi * (Math::PI / 180.0)
    y = theta * (Math::PI / 180.0)
    z = psi * (Math::PI / 180.0)
    c1 = Math.cos(x)
    c2 = Math.cos(y)
    c3 = Math.cos(z)
    s1 = Math.sin(x)
    s2 = Math.sin(y)
    s3 = Math.sin(z)
    c3c2 = c3 * c2
    s3c1 = s3 * c1
    c3s2s1 = c3 * s2 * s1
    s3s1 = s3 * s1
    c3s2c1 = c3 * s2 * c1
    s3c2 = s3 * c2
    c3c1 = c3 * c1
    s3s2s1 = s3 * s2 * s1
    c3s1 = c3 * s1
    s3s2c1 = s3 * s2 * c1
    c2s1 = c2 * s1
    c2c1 = c2 * c1

    # initialize to the identity matrix
    res[0] = 1.0
    res[5] = 1.0
    res[10] = 1.0
    res[15] = 1.0

    # apply all three rotations using the three matrices:
    #
    # ⎡  c3 s3 0 ⎤ ⎡ c2  0 -s2 ⎤ ⎡ 1   0  0 ⎤
    # ⎢ -s3 c3 0 ⎥ ⎢  0  1   0 ⎥ ⎢ 0  c1 s1 ⎥
    # ⎣   0  0 1 ⎦ ⎣ s2  0  c2 ⎦ ⎣ 0 -s1 c1 ⎦

    res[0] = c3c2
    res[1] = -s3c2
    res[2] = s2
    res[3] = 0.0
    res[4] = s3c1 + c3s2s1
    res[5] = c3c1 - s3s2s1
    res[6] = -c2s1
    res[7] = 0.0
    res[8] = s3s1 - c3s2c1
    res[9] = c3s1 + s3s2c1
    res[10] = c2c1
    res[11] = 0.0
    res[12] = 0.0
    res[13] = 0.0
    res[14] = 0.0
    res[15] = 1.0
    res
  end

  def create_axis_slider(axis)
    box = Gtk::Box.new(:horizontal, 0)
    text = case axis
           when X_AXIS
             "X axis"
           when Y_AXIS
             "Y axis"
           when Z_AXIS
             "Z axis"
           end
    label = Gtk::Label.new(text)
    box.add(label)
    label.show

    adj = Gtk::Adjustment.new(0.0, 0.0, 360.0, 1.0, 12.0, 0.0)
    adj.signal_connect "value-changed" do |widget|
      # Updte the rotation angle
      @rotation_angles[axis] = widget.value
      # Update the contents of the GL drawing area
      @gl_area.queue_draw
    end
    slider = Gtk::Scale.new(:horizontal, adj)
    box.add(slider)
    slider.hexpand = true
    slider.show
    box.show
    box
  end
end
