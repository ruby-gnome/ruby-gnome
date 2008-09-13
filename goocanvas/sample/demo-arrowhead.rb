class CanvasSampleArrowhead < Gtk::VBox
	LEFT = 50.0
	RIGHT = 350.0
	MIDDLE = 150.0
	DEFAULT_WIDTH = 2
	DEFAULT_SHAPE_A = 4
	DEFAULT_SHAPE_B = 5
	DEFAULT_SHAPE_C = 4

	def initialize()
		super(false, 4)
	   	border_width = 4
	   	show()

	    w = Gtk::Label.new <<-END
			This demo allows you to edit arrowhead shapes.  Drag the little boxes
			to change the shape of the line and its arrowhead.  You can see the
			arrows at their normal scale on the right hand side of the window.
	    END
	    pack_start(w, false, false, 0)
	    w.show

	    w = Gtk::Alignment.new(0.5, 0.5, 0.0, 0.0)
	    pack_start(w, true, true, 0)
	    w.show
	
	    frame = Gtk::Frame.new
	    frame.shadow_type = Gtk::SHADOW_IN
	    w.add(frame)
	    frame.show
	
	    canvas = Goo::Canvas.new
	    root = canvas.root_item
	
	    canvas.set_size_request(500, 350)
	    canvas.set_bounds(0, 0, 500, 350)
	    frame.add(canvas)
	    canvas.show

	    canvas.instance_variable_set(:@width, DEFAULT_WIDTH)
	    canvas.instance_variable_set(:@shape_a, DEFAULT_SHAPE_A)
	    canvas.instance_variable_set(:@shape_b, DEFAULT_SHAPE_B)
	    canvas.instance_variable_set(:@shape_c, DEFAULT_SHAPE_C)

    	# Big arrow

	    item = Goo::CanvasPolyline.new_line(root,
	                 LEFT, MIDDLE, RIGHT, MIDDLE,
	                 :stroke_color => "mediumseagreen",
	                 :end_arrow => true)
	    canvas.instance_variable_set(:@big_arrow, item)

	    # Arrow outline
	
	    item = Goo::CanvasPolyline.new(root, true, [],
	            :stroke_color, "black",
	            :line_width => 2.0,
	            :line_cap => Cairo::LINE_CAP_ROUND,
	            :line_join => Cairo::LINE_JOIN_ROUND)
	    canvas.instance_variable_set(:@outline, item)
	
	    # Drag boxes
	
	    create_drag_box(canvas, root, "width_drag_box")
	    create_drag_box(canvas, root, "shape_a_drag_box")
	    create_drag_box(canvas, root, "shape_b_c_drag_box")
	
	    # Dimensions
	
	    create_dimension(canvas, root, "width_arrow", "width_text", Gtk::ANCHOR_E)
	    create_dimension(canvas, root, "shape_a_arrow", "shape_a_text", Gtk::ANCHOR_N)
	    create_dimension(canvas, root, "shape_b_arrow", "shape_b_text", Gtk::ANCHOR_N)
	    create_dimension(canvas, root, "shape_c_arrow", "shape_c_text", Gtk::ANCHOR_W)
	
	    # Info

	    create_info(canvas, root, "width_info", LEFT, 260)
	    create_info(canvas, root, "shape_a_info", LEFT, 280)
	    create_info(canvas, root, "shape_b_info", LEFT, 300)
	    create_info(canvas, root, "shape_c_info", LEFT, 320)
	
	    # Division line
	
	    Goo::CanvasPolyline.new_line(root, RIGHT + 50, 0, RIGHT + 50, 1000,
	                :fill_color => "black", :line_width => 2.0)
	
	    # Sample arrows
	
	    create_sample_arrow(canvas, root, "sample_1",
	             RIGHT + 100, 30, RIGHT + 100, MIDDLE - 30)
	    create_sample_arrow(canvas, root, "sample_2",
	             RIGHT + 70, MIDDLE, RIGHT + 130, MIDDLE)
	    create_sample_arrow(canvas, root, "sample_3",
	             RIGHT + 70, MIDDLE + 30, RIGHT + 130, MIDDLE + 120)
	
	    # Done!
   		set_arrow_shape(canvas)
	end
  
 	def set_dimension(canvas, arrow_name, text_name, x1, y1, x2, y2, tx, ty, dim)
   		points = [ x1, y1, x2, y2 ]

    	arrow = canvas.instance_variable_get("@#{arrow_name}")
		arrow.points = points

		text = canvas.instance_variable_get("@#{text_name}")
	    text.text = dim.to_s
	    text.x = tx
	    text.y = ty
  	end

	def move_drag_box(item, x, y)
    	item.x = x - 5.0
    	item.y = y - 5.0
  	end

  	def set_arrow_shape(canvas)
	    width = canvas.instance_variable_get(:@width)
	    shape_a = canvas.instance_variable_get(:@shape_a)
	    shape_b = canvas.instance_variable_get(:@shape_b)
	    shape_c = canvas.instance_variable_get(:@shape_c)
	
	    # Big arrow
	
	    big_arrow = canvas.instance_variable_get(:@big_arrow)
	    big_arrow.line_width = 10.0 * width
	    big_arrow.arrow_tip_length = shape_a
	    big_arrow.arrow_length = shape_b
	    big_arrow.arrow_width = shape_c

	    # Outline
	    
	    points = []
	    points[0] = RIGHT - 10 * shape_a * width
	    points[1] = MIDDLE - 10 * width / 2
	    points[2] = RIGHT - 10 * shape_b * width
	    points[3] = MIDDLE - 10 * (shape_c * width / 2.0)
	    points[4] = RIGHT
	    points[5] = MIDDLE
	    points[6] = points[2]
	    points[7] = MIDDLE + 10 * (shape_c * width / 2.0)
	    points[8] = points[0]
	    points[9] = MIDDLE + 10 * width / 2
    
	    outline = canvas.instance_variable_get(:@outline)
	    outline.points = points
	
	    # Drag boxes
	    move_drag_box(canvas.instance_variable_get(:@width_drag_box), LEFT, MIDDLE - 10 * width / 2.0)
	    move_drag_box(canvas.instance_variable_get(:@shape_a_drag_box), RIGHT - 10 * shape_a * width, MIDDLE)
	    move_drag_box(canvas.instance_variable_get(:@shape_b_c_drag_box), RIGHT - 10 * shape_b * width, MIDDLE - 10 * (shape_c * width / 2.0))

	    # Dimensions
	
	    set_dimension(canvas, "width_arrow", "width_text",
	           LEFT - 10,
	           MIDDLE - 10 * width / 2.0,
	           LEFT - 10,
	           MIDDLE + 10 * width / 2.0,
	           LEFT - 15,
	           MIDDLE,
	           width)
	
	    set_dimension(canvas, "shape_a_arrow", "shape_a_text",
	           RIGHT - 10 * shape_a * width,
	           MIDDLE + 10 * (shape_c * width / 2.0) + 10,
	           RIGHT,
	           MIDDLE + 10 * (shape_c * width / 2.0) + 10,
	           RIGHT - 10 * shape_a * width / 2.0,
	           MIDDLE + 10 * (shape_c * width / 2.0) + 15,
	           shape_a)
	
	    set_dimension(canvas, "shape_b_arrow", "shape_b_text",
	           RIGHT - 10 * shape_b * width,
	           MIDDLE + 10 * (shape_c * width / 2.0) + 35,
	           RIGHT,
	           MIDDLE + 10 * (shape_c * width / 2.0) + 35,
	           RIGHT - 10 * shape_b * width / 2.0,
	           MIDDLE + 10 * (shape_c * width / 2.0) + 40,
	           shape_b)
	
	    set_dimension(canvas, "shape_c_arrow", "shape_c_text",
	           RIGHT + 10,
	           MIDDLE - 10 * shape_c * width / 2.0,
	           RIGHT + 10,
	           MIDDLE + 10 * shape_c * width / 2.0,
	           RIGHT + 15,
	           MIDDLE,
	           shape_c)

	    # Info
	
	    width_info = canvas.instance_variable_get(:@width_info)
	    width_info.text = "line-width: #{width}"
	
	    shape_a_info = canvas.instance_variable_get(:@shape_a_info)
	    shape_a_info.text = "arrow-tip-length: #{shape_a} (* line-width)"
	
	    shape_b_info = canvas.instance_variable_get(:@shape_b_info)
	    shape_b_info.text = "arrow-length: #{shape_b} (* line-width)"
	
	    shape_c_info = canvas.instance_variable_get(:@shape_c_info)
	    shape_c_info.text = "arrow-length: #{shape_c} (* line-width)"
	
	    # Sample arrows
	
	    sample_1 = canvas.instance_variable_get(:@sample_1)
	    sample_1.line_width = width
	    sample_1.arrow_tip_length = shape_a
	    sample_1.arrow_length = shape_b
	    sample_1.arrow_width = shape_c
	    sample_2 = canvas.instance_variable_get(:@sample_2)
	    sample_2.line_width = width
	    sample_2.arrow_tip_length = shape_a
	    sample_2.arrow_length = shape_b
	    sample_2.arrow_width = shape_c
	    sample_3 = canvas.instance_variable_get(:@sample_3)
	    sample_3.line_width = width
	    sample_3.arrow_tip_length = shape_a
	    sample_3.arrow_length = shape_b
	    sample_3.arrow_width = shape_c
	end

	def create_dimension(canvas, root, arrow_name, text_name, anchor)
	 	item = Goo::CanvasPolyline.new(root, false, 0,
	              :fill_color => "black",
	              :start_arrow => true,
	              :end_arrow => true)
	    canvas.instance_variable_set("@#{arrow_name}", item)
	
	    item = Goo::CanvasText.new(root, nil, 0, 0, -1, anchor,
	            :fill_color => "black",
	            :font => "Sans 12")
	    canvas.instance_variable_set("@#{text_name}", item)
	end
  
 	def create_info(canvas, root, info_name, x, y)
	    item = Goo::CanvasText.new(root, nil, x, y, -1, Gtk::ANCHOR_NW,
	            :fill_color => "black",
	            :font => "Sans 14")
	    canvas.instance_variable_set("@#{info_name}", item)
	end
  
  	def create_sample_arrow(canvas, root, sample_name, x1, y1, x2, y2)
    	item = Goo::CanvasPolyline.new_line(root, x1, y1, x2, y2,
               :start_arrow => true,
               :end_arrow => true)
    	canvas.instance_variable_set("@#{sample_name}", item)
  	end

  	def create_drag_box(canvas, root, box_name)
	    item = Goo::CanvasRect.new(root, 0, 0, 10, 10,
	            :fill_color => 'black',
	            :stroke_color => 'black',
	            :line_width => 1.0)
	    canvas.instance_variable_set("@#{box_name}", item)
	  
	    item.signal_connect('enter_notify_event') do
	      item.fill_color = 'red'
	      true
	    end
	    item.signal_connect('leave_notify_event') do
	      item.fill_color = 'black'
	      true
	    end
	    item.signal_connect('button_press_event') do |item, target, event|
	      fleur = Gdk::Cursor.new(Gdk::Cursor::FLEUR)
	      canvas.pointer_grab(item, Gdk::Event::POINTER_MOTION_MASK | Gdk::Event::BUTTON_RELEASE_MASK, fleur, event.time)
	      true
	    end
	    item.signal_connect('button_release_event') do |item, target, event|
	      canvas.pointer_ungrab(item, event.time)
	      true
	    end
	    item.signal_connect('motion_notify_event') do |item, target, event|
	      catch :done do
	        throw :done, false unless event.state & Gdk::Window::BUTTON1_MASK == Gdk::Window::BUTTON1_MASK
	  
	        if item == canvas.instance_variable_get(:@width_drag_box)
	          y = event.y
	          width = (MIDDLE - y) / 5
	          throw :done, false if width < 0
	          canvas.instance_variable_set(:@width, width)
	          set_arrow_shape(canvas)
	        elsif item == canvas.instance_variable_get(:@shape_a_drag_box)
	          x = event.x
	          width = canvas.instance_variable_get(:@width)
	          shape_a = (RIGHT - x) / 10 / width
	          throw :done, false if (shape_a < 0) || (shape_a > 30)
	          width = canvas.instance_variable_set(:@shape_a, shape_a)
	          set_arrow_shape(canvas)
	        elsif item == canvas.instance_variable_get(:@shape_b_c_drag_box)
	          change = false
	          width = canvas.instance_variable_get(:@width)
	          x = event.x
	          shape_b = (RIGHT - x) / 10 / width
	          if (shape_b >= 0) && (shape_b <= 30)
	            canvas.instance_variable_set(:@shape_b, shape_b)
	            change = true
	          end
	  
	          y = event.y
	          shape_c = (MIDDLE - y) * 2 / 10 / width
	          if shape_c >= 0
	            canvas.instance_variable_set(:@shape_c, shape_c)
	            change = true
	          end
	  
	          set_arrow_shape(canvas) if change
	        end
	        true
	      end
	    end
  	end 
end