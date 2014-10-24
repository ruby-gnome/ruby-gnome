# Copyright (C) 2014  Ruby-GNOME2 Project Team
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

class ClutterShaderEffect < Test::Unit::TestCase
  include ClutterTestUtils

  def setup
    @shader_effect = Clutter::ShaderEffect.new(Clutter::ShaderType::FRAGMENT_SHADER)
    @fragment_shader_source = <<-EOS
#ifdef GL_ES
precision mediump float;
#endif

void main( void ) {
  gl_FragColor = vec4( 1.0, 0.0, 1.0, 1.0 );
}
EOS
    # TODO: use original Cogl::INVALID_HANDLE
    # original value: NULL
    # ref: https://developer.gnome.org/cogl/1.18/cogl-General-API.html#COGL-INVALID-HANDLE:CAPS
    # It should create cogl gi based binding?
    @cogl_invalid_handle = 0
  end

  def test_set_uniform_value
    gvalue = 1.0
    assert_nothing_raised do
      @shader_effect.set_uniform_value("components", gvalue)
    end
  end

  def test_set_shader_source
    result = @shader_effect.set_shader_source(@fragment_shader_source)
    assert_true(result)
  end

  def test_get_program
    @shader_effect.shader_source = @fragment_shader_source
    assert_not_equal(@cogl_invalid_handle, @shader_effect.program)
  end

  def test_get_shader
    @shader_effect.shader_source = @fragment_shader_source
    assert_not_equal(@cogl_invalid_handle, @shader_effect.shader)
  end

  class TestEnum < self
    def test_shader_type
      assert_const_defined(Clutter::ShaderType, :VERTEX_SHADER)
    end
  end
end
