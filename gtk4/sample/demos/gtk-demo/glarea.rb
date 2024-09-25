# Copyright (C) 2024  Ruby-GNOME Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

# Original:
#   https://gitlab.gnome.org/GNOME/gtk/-/blob/main/demos/gtk-demo/glarea.c
#
# See the following for license information:
#   * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/AUTHORS
#   * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/COPYING

require "opengl"

class GtkGLAreaDemo < GtkDemo
  X_AXIS = 0
  Y_AXIS = 1
  Z_AXIS = 2
  N_AXISES = 3

  def initialize
    super(__FILE__,
          "OpenGL",
          "OpenGL Area",
          "Gtk::GLArea is a widget that allows custom drawing using OpenGL calls.",
          [])
  end

  def run(parent)
    window = Gtk::Window.new
    window.display = parent.display
    window.title = "OpenGL Area"
    window.set_default_size(400, 600)

    box = Gtk::Box.new(:vertical)
    box.margin_start = 12
    box.margin_end = 12
    box.margin_top = 12
    box.margin_bottom = 12
    box.spacing = 6
    window.child = box

    gl_area = Gtk::GLArea.new
    gl_area.hexpand = true
    gl_area.vexpand = true
    gl_area.set_size_request(100, 200)
    box.append(gl_area)

    # We need to initialize and free GL resources, so we use the
    # realize and unrealize signals on the widget
    gl_area.signal_connect("realize") do
      realize(gl_area)
    end
    gl_area.signal_connect("unrealize") do
      unrealize(gl_area)
    end

    # The main "draw" call for GtkGLArea
    gl_area.signal_connect("render") do
      render(gl_area)
    end

    controls = Gtk::Box.new(:vertical)
    controls.hexpand = true
    box.append(controls)

    N_AXISES.times do |i|
      controls.append(create_axis_slider(gl_area, i))
    end

    button = Gtk::Button.new(label: "Quit")
    button.hexpand = true
    box.append(button)

    button.signal_connect("clicked") do
      window.destroy
    end

    window.present

    window
  end

  private
  # We need to set up our state when we realize the GtkGLArea widget
  def realize(gl_area)
    @rotation_angles = [0.0] * N_AXISES

    GL.load_lib

    gl_area.make_current
    api_name = gl_area.api.nick
    vertex_path = "/glarea/glarea-#{api_name}.vs.glsl"
    fragment_path = "/glarea/glarea-#{api_name}.fs.glsl"
    init_buffers
    init_shaders(vertex_path, fragment_path)
  end

  # The object we are drawing
  VERTEX_DATA = [
     0.0,  0.5,   0.0, 1.0,
     0.5, -0.366, 0.0, 1.0,
    -0.5, -0.366, 0.0, 1.0,
  ]

  def init_buffers
    # We only use one VAO, so we always keep it bound
    vao_buffer = " " * Fiddle::SIZEOF_UINT32_T
    GL.GenVertexArrays(1, vao_buffer)
    @vao = vao_buffer.unpack1("L")
    GL.BindVertexArray(@vao)

    # This is the buffer that holds the vertices
    position_buffer = " " * Fiddle::SIZEOF_UINT32_T
    GL.GenBuffers(1, position_buffer)
    GL.GenBuffers(1, position_buffer)
    @position = position_buffer.unpack1("L")
    GL.BindBuffer(GL::ARRAY_BUFFER, @position)
    GL.BufferData(GL::ARRAY_BUFFER,
                  Fiddle::SIZEOF_FLOAT * VERTEX_DATA.size,
                  VERTEX_DATA.pack("f*"),
                  GL::STATIC_DRAW)
    GL.BindBuffer(GL::ARRAY_BUFFER, 0)
  end

  # Initialize the shaders and link them into a program
  def init_shaders(vertex_path, fragment_path)
    @program = nil
    vertex = 0
    fragment = 0

    begin
      source = Gio::Resources.lookup_data(vertex_path)
      vertex = create_shader(GL::VERTEX_SHADER, source.to_s)
      return if vertex.zero?

      source = Gio::Resources.lookup_data(fragment_path)
      fragment = create_shader(GL::FRAGMENT_SHADER, source.to_s)
      return if fragment.zero?

      @program = GL.CreateProgram
      return if @program.zero?

      GL.AttachShader(@program, vertex)
      GL.AttachShader(@program, fragment)

      GL.LinkProgram(@program)

      program_ok_buffer = " " * Fiddle::SIZEOF_UINT32_T
      GL.GetProgramiv(@program, GL::LINK_STATUS, program_ok_buffer)
      program_ok = (program_ok_buffer.unpack1("L") == GL::TRUE)
      unless program_ok
        log_length_buffer = " " * Fiddle::SIZEOF_UINT32_T
        GL.GetProgramiv(@program, GL::INFO_LOG_LENGTH, log_length_buffer)
        log_length = log_length_buffer.unpack1("L")
        log = " " * log_length
        GL.GetProgramInfoLog(@program, log_length, nil, log)
        $stderr.puts("Linking failure:")
        $stderr.puts(log)
        GL.DeleteProgram(@program)
        @program = nil
        return
      end

      # Get the location of the "mvp" uniform
      @mvp_location = GL.GetUniformLocation(@program, "mvp")

      GL.DetachShader(@program, vertex)
      GL.DetachShader(@program, fragment)
    ensure
      GL.DeleteShader(vertex) if vertex.zero?
      GL.DeleteShader(fragment) if fragment.zero?
    end
  end

  # Create and compile a shader
  def create_shader(type, source)
    shader = GL.CreateShader(type)
    GL.ShaderSource(shader, 1, [source].pack("p"), nil)
    GL.CompileShader(shader)

    shader_ok_buffer = " " * Fiddle::SIZEOF_UINT32_T
    GL.GetShaderiv(shader, GL::COMPILE_STATUS, shader_ok_buffer)
    shader_ok = (shader_ok_buffer.unpack1("L") == GL::TRUE)
    unless shader_ok
      log_length_buffer = " " * Fiddle::SIZEOF_UINT32_t
      GL.GetShaderiv(shader, GL::INFO_LOG_LENGTH, log_length_buffer)
      log_length = log_length_buffer.unpack1("L")
      log = " " * log_length
      GL.GetShaderInfoLog(shader, log_length, nil, buffer)
      type_name = (type == GL::VERTEX_SHADER ? "vertex" : "fragment")
      $stderr.puts("Compile failure in #{type_name} shader:")
      $stderr.puts(log)
      GL.DeleteShader(shader)
      return 0
    end

    shader
  end

  # We should tear down the state when unrealizing
  def unrealize(gl_area)
    gl_area.make_current
    GL.DeleteBuffers(1, [[@position].pack("L")].pack("p"))
    GL.DeleteProgram(@program)
  end

  def compute_mvp(phi, theta, psi)
    x = phi * (Math::PI / 180.0)
    y = theta * (Math::PI / 180.0)
    z = psi * (Math::PI / 180.0)
    c1 = Math.cos(x)
    s1 = Math.sin(x)
    c2 = Math.cos(y)
    s2 = Math.sin(y)
    c3 = Math.cos(z)
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

    # apply all three rotations using the three matrices:
    #
    # ⎡  c3 s3 0 ⎤ ⎡ c2  0 -s2 ⎤ ⎡ 1   0  0 ⎤
    # ⎢ -s3 c3 0 ⎥ ⎢  0  1   0 ⎥ ⎢ 0  c1 s1 ⎥
    # ⎣   0  0 1 ⎦ ⎣ s2  0  c2 ⎦ ⎣ 0 -s1 c1 ⎦
    mvp = [0.0] * 16
    mvp[0] = c3c2;  mvp[4] = s3c1 + c3s2s1; mvp[8] = s3s1 - c3s2c1; mvp[12] = 0.0;
    mvp[1] = -s3c2; mvp[5] = c3c1 - s3s2s1; mvp[9] = c3s1 + s3s2c1; mvp[13] = 0.0;
    mvp[2] = s2;    mvp[6] = -c2s1;         mvp[10] = c2c1;         mvp[14] = 0.0;
    mvp[3] = 0.0;   mvp[7] = 0.0;           mvp[11] = 0.0;          mvp[15] = 1.0;
    mvp
 end

  def draw_triangle
    # Compute the model view projection matrix using the
    # rotation angles specified through the GtkRange widgets
    mvp = compute_mvp(@rotation_angles[X_AXIS],
                      @rotation_angles[Y_AXIS],
                      @rotation_angles[Z_AXIS])

    # Use our shaders
    GL.UseProgram(@program)

    # Update the "mvp" matrix we use in the shader
    GL.UniformMatrix4fv(@mvp_location, 1, GL::FALSE, mvp.pack("f*"))

    # Use the vertices in our buffer
    GL.BindBuffer(GL::ARRAY_BUFFER, @position)
    GL.EnableVertexAttribArray(0)
    GL.VertexAttribPointer(0, 4, GL::FLOAT, GL::FALSE, 0, 0)

    # Draw the three vertices as a triangle
    GL.DrawArrays(GL::TRIANGLES, 0, 3)

    # We finished using the buffers and program
    GL.DisableVertexAttribArray(0)
    GL.BindBuffer(GL::ARRAY_BUFFER, 0)
    GL.UseProgram(0)
  end

  def render(gl_area)
    # Clear the viewport
    GL.ClearColor(0.5, 0.5, 0.5, 1.0)
    GL.Clear(GL::COLOR_BUFFER_BIT)

    # Draw our object
    draw_triangle

    # Flush the contents of the pipeline
    GL.Flush

    true
  end

  def create_axis_slider(gl_area, axis)
    box = Gtk::Box.new(:horizontal)
    case axis
    when X_AXIS
      text = "X axis"
    when Y_AXIS
      text = "Y axis"
    when Z_AXIS
      text = "Z axis"
    end
    label = Gtk::Label.new(text)
    box.append(label)

    adjustment = Gtk::Adjustment.new(0.0, 0.0, 360.0, 1.0, 12.0, 0.0)
    adjustment.signal_connect("value-changed") do
      @rotation_angles[axis] = adjustment.value
      # Update the contents of the GL drawing area
      gl_area.queue_draw
    end

    slider = Gtk::Scale.new(:horizontal, adjustment)
    box.append(slider)
    slider.hexpand = true

    box
  end

  GtkDemo.register(new)
end
