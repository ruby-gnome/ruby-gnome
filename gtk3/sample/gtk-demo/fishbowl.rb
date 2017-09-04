# Copyright (c) 2017 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Benchmark/Fishbowl

 This demo models the fishbowl demos seen on the web in a GTK way.
 It's also a neat little tool to see how fast your computer (or
 your GTK version) is.
=end

STATS_UPDATE_TIME = 1_000_000 # G_USEC_PER_SEC
N_STATS = 5

class Stats
  attr_accessor :last_stats, :last_frame, :last_suggestion, :frame_counter_max,
                :stats_index, :frame_counter, :item_counter

  def initialize(widget)
    @widget = widget
    @stats_index = 0
    @frame_counter_max = 0
    @last_frame = @widget.frame_clock.frame_time
    @last_stats = @last_frame
    @frame_counter = Array.new(N_STATS, 0)
    @item_counter = Array.new(N_STATS, 0)
    @last_suggestion = 0
  end
end

class Fish < Gtk::Image
  attr_accessor :x, :y, :x_speed, :y_speed
  def initialize
    super(:icon_name => random_icon_name, :size => :dialog)
    @x = 10
    @y = 10
    @x_speed = rand(1..200)
    @y_speed = rand(1..200)
  end

  private

  def random_icon_name
    theme = Gtk::IconTheme.default
    icon_list = theme.icons
    icons = icon_list.reject { |e| e.include?("symbolic") }
    icons[rand(icons.size)]
  end
end

class FishbowlDemo
  def initialize(main_window)
    builder = Gtk::Builder.new(:resource => "/fishbowl/fishbowl.ui")
    @window = builder["window"]
    @bowl = builder["bowl"]
    @info_label = builder["info_label"]
    @changes_prevent = builder["changes_prevent"]
    @changes_prevent.image = Gtk::Image.new(:icon_name => "changes-prevent")
    @changes_allow = builder["changes_allow"]
    @changes_allow.image = Gtk::Image.new(:icon_name => "changes-allow")
    @window.screen = main_window.screen
    @window.realize
    @bowl.add_tick_callback do
      move_fish
    end
  end

  def run
    if !@window.visible?
      @window.show
    else
      @window.destroy
    end
    @window
  end

  private

  def move_fish
    elapsed, suggested_change = do_stats
    @bowl.each do |widget|
      move_one_fish(widget, elapsed)
    end

    if suggested_change > 0
      add_fish(suggested_change)
    elsif suggested_change < 0
      remove_fish(-suggested_change)
    end

    GLib::Source::CONTINUE
  end

  def do_stats
    @stats ||= Stats.new(@bowl)
    frame_time = @bowl.frame_clock.frame_time
    elapsed = frame_time - @stats.last_frame
    n_frames = 0

    if @stats.last_stats + STATS_UPDATE_TIME < frame_time
      @stats.frame_counter.each do |f|
        n_frames += f
      end

      index = @stats.stats_index
      fps = STATS_UPDATE_TIME * n_frames / (N_STATS * STATS_UPDATE_TIME)
      new_label = format("%u icons - %.1f fps", @stats.item_counter[index], fps)
      @info_label.label = new_label

      if @stats.frame_counter[index] >= 19 * @stats.frame_counter_max / 20
        if @stats.last_suggestion > 0
          @stats.last_suggestion *= 2
        else
          @stats.last_suggestion = 1
        end
      else
        if @stats.last_suggestion < 0
          @stats.last_suggestion -= 1
        else
          @stats.last_suggestion = -1
        end
        @stats.last_suggestion = [@stats.last_suggestion,
                                  1 - @stats.item_counter[index]].max
      end

      @stats.stats_index = (@stats.stats_index + 1) % N_STATS
      @stats.frame_counter[@stats.stats_index] = 0
      @stats.item_counter[@stats.stats_index] = @stats.item_counter[(@stats.stats_index + N_STATS - 1) % N_STATS]
      @stats.last_stats = frame_time

      if @changes_allow.active?
        @stats.last_suggestion = 0
      end
      suggested_change = @stats.last_suggestion
    else
      suggested_change = 0
    end

    @stats.last_frame = frame_time
    @stats.frame_counter[@stats.stats_index] += 1
    @stats.frame_counter_max = [@stats.frame_counter_max,
                                @stats.frame_counter[@stats.stats_index]].max
    [elapsed, suggested_change]
  end

  def add_fish(n_fish)
    (1..n_fish).each do
      new_fish = Fish.new
      new_fish.show
      @bowl.put(new_fish, 10, 10)
      stats_update(n_fish)
    end
  end

  def stats_update(n_items)
    index = @stats.stats_index
    @stats.item_counter[index] += n_items
  end

  def remove_fish(n_fish)
    (0..(n_fish - 1)).each do
      @bowl.children.shift
    end
    stats_update(- n_fish)
  end

  def move_one_fish(fish, elapsed)
    fish.x += (fish.x_speed * elapsed / STATS_UPDATE_TIME)
    fish.y += (fish.y_speed * elapsed / STATS_UPDATE_TIME)

    if fish.x <= 0
      fish.x = 0
      fish.x_speed = - rand(1..200) * (fish.x_speed > 0 ? 1 : - 1)
    elsif fish.x > fish.parent.allocated_width - fish.allocated_width
      fish.x = fish.parent.allocated_width - fish.allocated_width
      fish.x_speed = - rand(1..200) * (fish.x_speed > 0 ? 1 : - 1)
    end

    if fish.y <= 0
      fish.y = 0
      fish.y_speed = - rand(1..200) * (fish.y_speed > 0 ? 1 : - 1)
    elsif fish.y > fish.parent.allocated_height - fish.allocated_height
      fish.y = fish.parent.allocated_height - fish.allocated_height
      fish.y_speed = - rand(1..200) * (fish.y_speed > 0 ? 1 : - 1)
    end

    fish.parent.move(fish, fish.x, fish.y)
  end
end
