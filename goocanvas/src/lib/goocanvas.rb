require 'gtk2'
require 'cairo'
require 'goocanvas.so'

module Goo
  LOG_DOMAIN="Goo"
  
  def self.args_to_hash(args)
    hash = args.pop if args.last.respond_to?(:to_hash)
    hash ||= Hash.new
    (args.length/2).times do |i|
      key_index, value_index = i*2, i*2+1
      hash[args[key_index]] = args[value_index]
    end
    hash
  end
  
  module PropsInit
    def init_props(*args)
      hash = Goo.args_to_hash(args)
      hash.each_pair { |key, value| set_property(key.to_s.gsub(/-/, '_').to_sym, value) }
    end
    
    def self.append_features(klass)
      super
      arity = klass.instance_method(:initialize).arity
      raise 'the initialize method of a class including PropsInit must have a fixed arity' if arity < 0
      args_list = (1..arity).collect { |i| "param#{i}" }.join(", ")
      klass.module_eval <<-END
        alias :_initialize :initialize
        def initialize(#{args_list}, *args)
          _initialize(#{args_list})
          init_props(*args)
        end
        
        alias :_set_property :set_property
        def set_property(prop_name, value)
          pspec = self.class.property(prop_name)
          value = value.to_goo if pspec.value_type.name =~ /^GooCairo/ and value.respond_to?(:to_goo)
          _set_property(prop_name, value)
        end        
      END
    end
  end

  module CanvasItem
    def set_child_properties(child, *args)
      hash = Goo.args_to_hash(args)
      hash.each_pair { |key, value| set_child_property(child, key, value) }
    end
    
    def bounds
      [x1, x2, y1, y2]
    end
    
    def width
      x2 - x1
    end
    
    def height
      y2 - y1
    end
    
    def x
      x1
    end
    
    def y
      y1
    end
  end

  class CanvasText
    include PropsInit
  end

  class CanvasRect
    include PropsInit
  end

  class CanvasEllipse
    include PropsInit
  end

  class CanvasPolyline
    include PropsInit
    def initialize(parent, close_path, points, *args)
      _initialize(parent, close_path)
      set_points(points)
      init_props(*args)
    end
    
    def set_points(points)
      points = CanvasPoints.new(points) unless points.instance_of?(CanvasPoints)
      set_property(:points, points)
    end
    alias :points= :set_points
    
    def self.new_line(parent, x1, y1, x2, y2, *args)
      self.new(parent, false, [ x1, y1, x2, y2 ], *args)
    end
  end
  
  class CanvasPoints
    alias :_initialize :initialize
    def initialize(arg)
      if arg.respond_to?(:to_ary)
        points = arg.flatten
        num_points = points.length / 2
        _initialize(num_points)
        num_points.times { |i| self[i] = [ points[i*2], points[i*2+1] ] }
      else
        _initialize(arg)
      end
    end
    
    def each
      num_points.times { |i| yield self[i] }
    end

    def to_a
      a = []
      each { |e| a.push(e) }
      a
    end
  end

  class CanvasTable
    include PropsInit
  end

  class CanvasGroup
    include PropsInit
  end

  class CanvasWidget
    include PropsInit
  end

  class CanvasImage
    include PropsInit
  end
end

GLib::Log.set_log_domain(Goo::LOG_DOMAIN)
