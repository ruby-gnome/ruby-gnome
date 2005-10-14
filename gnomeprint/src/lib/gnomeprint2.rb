require "glib2"
require "libart2"
require "pango"
require "gnomeprint2.so"

module Gnome
  module Print
    LOG_DOMAIN = "GnomePrint"
  end
  
  module PrintCupsPlugin
    LOG_DOMAIN = "GnomePrintCupsPlugin"
  end
  
  module PrintPAPIPlugin
    LOG_DOMAIN = "GnomePrintPAPIPlugin"
  end
  
  module PrintLpdPlugin
    LOG_DOMAIN = "GnomePrintLpdPlugin"
  end

  class PrintContext
    def image(pixbuf)
      save do
        scale(pixbuf.width, pixbuf.height)
        yield(self, pixbuf) if block_given?
        args = [pixbuf.pixels, pixbuf.width, pixbuf.height, pixbuf.rowstride]
        if pixbuf.has_alpha?
          rgba_image(*args)
        else
          rgb_image(*args)
        end
      end
    end

    def rounded_rect(x, y, width, height, x_radius, y_radius=nil)
      x1 = x
      x2 = x1 + width
      y1 = y
      y2 = y1 - height

      y_radius ||= x_radius

      x_radius = [x_radius, width / 2].min
      y_radius = [y_radius, height / 2].min
      
      xr1 = x_radius
      xr2 = x_radius / 2.0
      yr1 = y_radius
      yr2 = y_radius / 2.0

      move_to(x1 + xr1, y1)
      line_to(x2 - xr1, y1)
      curve_to(x2 - xr2, y1, x2, y1 - yr2, x2, y1 - yr1)
      line_to(x2, y2 + yr1)
      curve_to(x2, y2 + yr2, x2 - xr2, y2, x2 - xr1, y2)
      line_to(x1 + xr1, y2)
      curve_to(x1 + xr2, y2, x1, y2 + yr2, x1, y2 + yr1)
      line_to(x1, y1 - yr1)
      curve_to(x1, y1 - yr2, x1 + xr2, y1, x1 + xr1, y1)
    end
    
    def rounded_rect_filled(x, y, width, height, x_radius, y_radius=nil)
      rounded_rect(x, y, width, height, x_radius, y_radius)
      fill
    end

    def rounded_rect_stroked(x, y, width, height, x_radius, y_radius=nil)
      rounded_rect(x, y, width, height, x_radius, y_radius)
      stroke
    end

    def circle_to(x, y, radius)
      arc_to(x, y, radius, -0.001, 360, true)
    end
  end
end

GLib::Log.set_log_domain(Gnome::Print::LOG_DOMAIN)
GLib::Log.set_log_domain(Gnome::PrintCupsPlugin::LOG_DOMAIN)
GLib::Log.set_log_domain(Gnome::PrintPAPIPlugin::LOG_DOMAIN)
GLib::Log.set_log_domain(Gnome::PrintLpdPlugin::LOG_DOMAIN)
