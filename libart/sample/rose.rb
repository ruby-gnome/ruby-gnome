require 'libart2'

module Art

  class Vpath

    def Vpath.line(x0, y0, x1, y1)
      Vpath.new([[Art::MOVETO, x0, y0], [Art::LINETO, x1, y1], [Art::END]])
    end

    def Vpath.slice(x, y, r, from, to)
      delta = 2.0 * Math::PI / 128
      from = Math::PI * from / 180.0
      to = Math::PI * to / 180.0
      from += 2.0 * Math::PI while from < 0
      to += 2.0 * Math::PI while to <= from
      a = [[Art::MOVETO, x, y], [Art::LINETO, x + r * Math.cos(from), y + r * Math.sin(from)]]
      theta = ((from / delta).to_i + 1) * delta
      while theta < to
        a.push([Art::LINETO, x + r * Math.cos(theta), y + r * Math.sin(theta)])
        theta += delta
      end
      a.push([Art::LINETO, x + r * Math.cos(to), y + r * Math.sin(to)])
      a.push([Art::LINETO, x, y])
      a.push([Art::END])
      Vpath.new(a)
    end

  end

  class Canvas

    class Context < Hash

      def initialize(overrides = {})
        self[:affine_transform] = nil
        self[:dash] = nil
        self[:stroke] = false
        self[:join] = Art::PATH_STROKE_JOIN_ROUND
        self[:cap] = Art::PATH_STROKE_CAP_ROUND
        self[:line_width] = 1.0
        self[:miter_limit] = 1.0
        self[:flatness] = 0.5
        self[:color] = Art::Canvas::Color::BLACK
        update(overrides)
      end

      def with(overrides)
        clone.update(overrides)
      end
      
    end

    def render_vpath_with_context(vpath, context)
      vpath = vpath.affine_transform(context[:affine_transform]) unless context[:affine_transform].nil?
      vpath = vpath.dash(context[:dash]) unless context[:dash].nil?
      if context[:stroke]
        svp = vpath.stroke(context[:join], context[:cap], context[:line_width], context[:miter_limit], context[:flatness])
      else
        svp = vpath.to_svp
      end
      render_svp(svp, context[:color])
    end

  end

end

class Array

  def to_h(default = nil)
    result = Hash.new(default)
    each_with_index do |element, index|
      result[index] = element
    end
    result
  end

end

directions = ['e', 'se', 's', 'sw', 'w', 'nw', 'n', 'ne']
direction_index = directions.to_h.invert

width = 145
size = 128
border = 4
height = size + 2 * border

context = Art::Canvas::Context.new({:affine_transform => Art::Affine.translate(width / 2 + 0.5, height / 2 + 0.5) * Art::Affine.scale(size / 2.0, size / 2.0)})
line_context = context
line_context = context.with({:stroke => true, :line_width => 1, :color => Art::Canvas::Color.new(0x00, 0x66, 0x00)})
crosshair_fill_context = context.with({:color => Art::Canvas::Color.new(0xff, 0xff, 0xff)})
pie_fill_context = context.with({:color => Art::Canvas::Color.new(0xee, 0xff, 0xee)})
hline = Art::Vpath.line(-1, 0, 1, 0)
vline = Art::Vpath.line(0, -1, 0, 1)
circle = Art::Vpath.circle(0, 0, 0.8)
overlap = 0.25 * 360.0 / directions.length

directions.each do |from|
  directions.each do |to|
    next if from == to
    canvas = Art::Canvas.new(width, height, Art::Canvas::Color::WHITE, Art::Canvas::ALPHA_MASK)
    canvas.render_vpath_with_context(circle, crosshair_fill_context)
    canvas.render_vpath_with_context(circle, line_context)
    canvas.render_vpath_with_context(hline, line_context)
    canvas.render_vpath_with_context(vline, line_context)
    slice = Art::Vpath.slice(0.0, 0.0, 0.9, 360.0 * direction_index[from] / directions.length - overlap, 360.0 * direction_index[to] / directions.length + overlap)
    canvas.render_vpath_with_context(slice, pie_fill_context)
    canvas.render_vpath_with_context(slice, line_context)
    File.open("rose-#{width}x#{height}-#{from}-#{to}.png", 'w') do |file|
      file.write(canvas.to_png)
    end
  end
end
