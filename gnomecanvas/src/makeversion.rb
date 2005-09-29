=begin
  makeversion.rb -

  $Author: mutoh $
  $Date: 2005/09/29 17:17:39 $

  Copyright (C) 2005 Masao Mutoh
=end

version = ARGV[0].split(/\./)

puts %Q[/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgnomecanvasversion.h -

  $Author: mutoh $
  $Date: 2005/09/29 17:17:39 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#ifndef __RBGNOMECANVAS_VERSION_H__
#define __RBGNOMECANVAS_VERSION_H__

#define GNOMECANVAS_MAJOR_VERSION (#{version[0]})
#define GNOMECANVAS_MINOR_VERSION (#{version[1]})
#define GNOMECANVAS_MICRO_VERSION (#{version[2]})

#define GNOMECANVAS_CHECK_VERSION(major,minor,micro)    \\
    (GNOMECANVAS_MAJOR_VERSION > (major) || \\
     (GNOMECANVAS_MAJOR_VERSION == (major) && GNOME_MINOR_VERSION > (minor)) || \\
     (GNOMECANVAS_MAJOR_VERSION == (major) && GNOME_MINOR_VERSION == (minor) && \\
      GNOMECANVAS_MICRO_VERSION >= (micro)))


#endif /* __RBGNOMECANVAS_VERSION_H__ */
]
