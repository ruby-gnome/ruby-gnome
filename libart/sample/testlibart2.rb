=begin
   testlibart2.rb

   $Author: mutoh $
   $Date: 2003/02/01 16:55:26 $

   Copyright (C) 2003 Tom Payne <ruby-gnome-users-en@tompayne.org>
=end

require 'libart2'

module Art

  class Vpath

    def Vpath.line(x0, y0, x1, y1)
      Vpath.new([[Art::MOVETO_OPEN, x0, y0], [Art::LINETO, x1, y1], [Art::END]])
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

    def Vpath.rectangle(x0, y0, x1, y1)
      Vpath.new([[Art::MOVETO, x0, y0], [Art::LINETO, x0, y1], [Art::LINETO, x1, y1], [Art::LINETO, x1, y0], [Art::LINETO, x0, y0], [Art::END]])
    end

    def close
      result = []
      start = nil
      to_a.each do |point|
        case point[0]
        when Art::MOVETO
          result.push([Art::LINETO, start[1], start[2]]) unless start.nil?
          start = point
        when Art::END
          result.push([Art::LINETO, start[1], start[2]]) unless start.nil?
          start = nil
        end
        result.push(point)
      end
      Vpath.new(result)
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

def main

  border = 8
  width = height = 256
  canvas = Art::Canvas.new(width + 2 * border, height + 2 * border, Art::Canvas::Color::WHITE, Art::Canvas::ALPHA_MASK)
  context = Art::Canvas::Context.new({:affine_transform => Art::Affine.translate(border, border) * Art::Affine.scale(width, height)})

  box = Art::Vpath.rectangle(0.0, 0.0, 1.0, 1.0)
  box_fill_context = context.with({:color => Art::Canvas::Color::WHITE})
  canvas.render_vpath_with_context(box, box_fill_context)
  box_outline_context = context.with({:stroke => true, :line_width => 4, :color => Art::Canvas::Color::BLACK})
  canvas.render_vpath_with_context(box, box_outline_context)

  circle_context = context.with({:affine_transform => context[:affine_transform] * Art::Affine.translate(0.0, 0.0) * Art::Affine.scale(0.45, 0.45)})
  canvas.render_vpath_with_context(Art::Vpath.circle(0.5, 0.4, 0.25), circle_context.with({:color => Art::Canvas::Color.new(1.0, 0.0, 0.0, 0.5)}))
  canvas.render_vpath_with_context(Art::Vpath.circle(0.4, 0.55, 0.25), circle_context.with({:color => Art::Canvas::Color.new(0.0, 1.0, 0.0, 0.5)}))
  canvas.render_vpath_with_context(Art::Vpath.circle(0.6, 0.55, 0.25), circle_context.with({:color => Art::Canvas::Color.new(0.0, 0.0, 1.0, 0.5)}))

  lines_context = context.with({:affine_transform => context[:affine_transform] * Art::Affine.translate(0.5, 0.0) * Art::Affine.scale(0.45, 0.45), :stroke => true, :line_width => 8, :miter_limit => 0.5})
  line = Art::Vpath.new([[Art::MOVETO, 0.3, 0.2], [Art::LINETO, 0.0, 0.3], [Art::LINETO, 0.3, 0.6], [Art::LINETO, 0.0, 0.8], [Art::END]])
  canvas.render_vpath_with_context(line.affine_transform(Art::Affine.translate(0.0, 0.0)), lines_context.with({:cap => Art::PATH_STROKE_CAP_ROUND, :join => Art::PATH_STROKE_JOIN_ROUND, :color => Art::Canvas::Color.new(0.5, 0.0, 0.0)}))
  canvas.render_vpath_with_context(line.affine_transform(Art::Affine.translate(0.3, 0.0)), lines_context.with({:cap => Art::PATH_STROKE_CAP_BUTT, :join => Art::PATH_STROKE_JOIN_MITER, :color => Art::Canvas::Color.new(0.0, 0.5, 0.0)}))
  canvas.render_vpath_with_context(line.affine_transform(Art::Affine.translate(0.6, 0.0)), lines_context.with({:cap => Art::PATH_STROKE_CAP_SQUARE, :join => Art::PATH_STROKE_JOIN_BEVEL, :color => Art::Canvas::Color.new(0.0, 0.0, 0.5)}))

  overlap_context = context.with({:affine_transform => context[:affine_transform] * Art::Affine.translate(0.0, 0.5) * Art::Affine.scale(0.45, 0.45)})
  shape1 = Art::Vpath.rectangle(0.25, 0.25, 0.75, 0.75)
  shape2 = Art::Vpath.circle(0.7, 0.7, 0.15)
  shape3 = Art::Vpath.circle(0.3, 0.3, 0.15)
  shape4 = Art::Vpath.rectangle(0.0, 0.0, 1.0, 1.0)
  shape5 = Art::Vpath.circle(0.7, 0.3, 0.15)
  shape6 = Art::Vpath.circle(0.3, 0.7, 0.15)
  overlap = ((shape1.to_svp ^ shape2.to_svp | shape3.to_svp) & (shape4.to_svp ^ shape5.to_svp ^ shape6.to_svp)).to_vpath
  canvas.render_vpath_with_context(overlap, overlap_context.with({:color => Art::Canvas::Color::MAGENTA}))

  pie_context = context.with({:affine_transform => context[:affine_transform] * Art::Affine.translate(0.5, 0.5) * Art::Affine.scale(0.45, 0.45)})
  line_context = pie_context.with({:stroke => true, :line_width => 1, :color => Art::Canvas::Color.new(0.0, 0.0, 0.25)})
  canvas.render_vpath_with_context(Art::Vpath.line(0.5, 0.0, 0.5, 1.0), line_context)
  canvas.render_vpath_with_context(Art::Vpath.line(0.0, 0.5, 1.0, 0.5), line_context)
  canvas.render_vpath_with_context(Art::Vpath.circle(0.5, 0.5, 0.4), line_context)
  slice = Art::Vpath.slice(0.5, 0.5, 0.45, -30.0, 135.0)
  canvas.render_vpath_with_context(slice, pie_context.with({:color => Art::Canvas::Color.new(0.5, 0.5, 1.0)}))
  canvas.render_vpath_with_context(slice, line_context)

  File.open('testlibart2.png', 'w') do |file|
    file.write(canvas.to_png(Art::Canvas::PNG_INTERLACE_ADAM7))
  end

  File.open('testlibart2.jpeg', 'w') do |file|
    file.write(canvas.to_jpeg)
  end

end

main if $0 == __FILE__
