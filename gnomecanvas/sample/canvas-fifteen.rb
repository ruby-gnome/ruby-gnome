class CanvasSampleFifteen < Gtk::VBox
  PIECE_SIZE = 50

  def test_win
    0.upto(14) do |i|
      if @board[i].nil? || @board[i].num != i
	return
      end
    end
#    dialog = Gnome::MessageBox.new("You stud, you win!", Gnome::MessageBox::INFO, "OK")
#    dialog.set_modal(true)
#    dialog.run_and_close()
  end

  def piece_event(item, event)
    case event.event_type
    when Gdk::ENTER_NOTIFY
      item.text.set("fill_color", "white")
    when Gdk::LEAVE_NOTIFY
      item.text.set("fill_color", "black")
    when Gdk::BUTTON_PRESS
      y = item.pos / 4
      x = item.pos % 4

      move = true

      if (y > 0) && @board[(y - 1) * 4 + x].nil?
	dx = 0.0
	dy = -1.0
	y -= 1
      elsif (y < 3) && @board[(y + 1) * 4 + x].nil?
	dx = 0.0
	dy = 1.0
	y += 1
      elsif (x > 0) && @board[y * 4 + x - 1].nil?
	dx = -1.0
	dy = 0.0
	x -= 1
      elsif (x < 3) && @board[y * 4 + x + 1].nil?
	dx = 1.0
	dy = 0.0
	x += 1
      else
	move = false
      end

      if move
	newpos = y * 4 + x
	@board[item.pos] = nil
	@board[newpos] = item
	item.pos = newpos
	item.move(dx * PIECE_SIZE, dy * PIECE_SIZE)
	test_win()
      end
    end
  end

  SCRAMBLE_MOVES = 32
  def scramble
    srand()

    # First, find the blank spot
    pos = 0
    0.upto(15) do |i|
      if @board[i].nil?
	pos = i
	break
      end
    end

    # "Move the blank spot" around in order to scramble the pieces

    0.upto(SCRAMBLE_MOVES) do
      dir = rand(4).to_i

      x = y = 0

      if (dir == 0) && (pos > 3) # up
	y = -1
      elsif (dir == 1) && (pos < 12) # down
	y = 1
      elsif (dir == 2) && ((pos % 4) != 0) # left
	x = -1
      elsif (dir == 3) && ((pos % 4) != 3) # right
	x = 1
      else
	retry
      end

      oldpos = pos + y * 4 + x;
      @board[pos] = @board[oldpos];
      @board[oldpos] = nil
      @board[pos].pos = pos
      @board[pos].move(-x * PIECE_SIZE, -y * PIECE_SIZE)
      @canvas.update_now()
      pos = oldpos
    end
  end

  def initialize
    super(false, 4)
    border_width = 4
    show()

    alignment = Gtk::Alignment.new(0.5, 0.5, 0.0, 0.0)
    pack_start(alignment, true, true, 0)
    alignment.show()

    frame = Gtk::Frame.new
    frame.set_shadow_type(Gtk::SHADOW_IN);
    alignment.add(frame)
    frame.show()

    # Create the canvas and board

    @canvas = Gnome::Canvas.new()
    @canvas.set_usize(PIECE_SIZE * 4 + 1, PIECE_SIZE * 4 + 1)
    @canvas.set_scroll_region(0, 0, PIECE_SIZE * 4 + 1, PIECE_SIZE * 4 + 1)
    frame.add(@canvas)
    @canvas.show()

    @board = Array.new(16)

    0.upto(14) do |i|
      @board[i] = Piece.new(@canvas.root)
      @board[i].setup(self, i)
    end
    @board[15] = nil;

    # Scramble button
    button = Gtk::Button.new("Scramble")
    pack_start(button, false, false, 0)
    button.signal_connect("clicked") do |button|
      scramble()
    end
    button.show()
  end

  class Piece < Gnome::CanvasGroup
    attr_reader :text, :num, :pos
    attr_writer :pos

    def initialize(*arg)
      super(*arg)
    end

    def setup(app, i)
      y = i / 4
      x = i % 4

      set("x", x * PIECE_SIZE,
	  "y", y * PIECE_SIZE)
      Gnome::CanvasRect.new(self,
			    "x1", 0.0,
			    "y1", 0.0,
			    "x2", PIECE_SIZE,
			    "y2", PIECE_SIZE,
			    "fill_color", get_piece_color(x, y),
			    "outline_color", "black",
			    "width_pixels", 0)
 
      @text = Gnome::CanvasText.new(self,
				    "text", i.to_s,
				    "x", PIECE_SIZE / 2.0,
				    "y", PIECE_SIZE / 2.0,
				    "font", "Sans bold 24",
				    "anchor", Gtk::ANCHOR_CENTER,
				    "fill_color", "black")
      @num = i
      @pos = i
      signal_connect("event") do |item, event|
	app.piece_event(item, event)
      end
    end

    private
    def get_piece_color(x, y)
      r = ((4 - x) * 255) / 4;
      g = ((4 - y) * 255) / 4;
      b = 128;
      return sprintf("#%02x%02x%02x", r, g, b)
    end

  end
end
