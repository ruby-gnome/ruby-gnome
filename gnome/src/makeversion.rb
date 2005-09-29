=begin
  makeversion.rb -

  $Author: mutoh $
  $Date: 2005/09/29 17:04:33 $

  Copyright (C) 2005 Masao Mutoh
=end

version = ARGV[0].split(/\./)

puts %Q[/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgnomeversion.h -

  $Author: mutoh $
  $Date: 2005/09/29 17:04:33 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#ifndef __RBGNOME_VERSION_H__
#define __RBGNOME_VERSION_H__

#define GNOME_MAJOR_VERSION (#{version[0]})
#define GNOME_MINOR_VERSION (#{version[1]})
#define GNOME_MICRO_VERSION (#{version[2]})

#define GNOME_CHECK_VERSION(major,minor,micro)    \\
    (GNOME_MAJOR_VERSION > (major) || \\
     (GNOME_MAJOR_VERSION == (major) && GNOME_MINOR_VERSION > (minor)) || \\
     (GNOME_MAJOR_VERSION == (major) && GNOME_MINOR_VERSION == (minor) && \\
      GNOME_MICRO_VERSION >= (micro)))


#endif /* __RBGNOME_VERSION_H__ */
]
