=begin
  makeversion.rb -

  $Author: mutoh $
  $Date: 2005/01/28 09:24:45 $

  Copyright (C) 2005 Masao Mutoh
=end

version = ARGV[0].split(/\./)

puts %Q[
#ifndef __RBPANGO_VERSION_H__
#define __RBPANGO_VERSION_H__

#define PANGO_MAJOR_VERSION (#{version[0]})
#define PANGO_MINOR_VERSION (#{version[1]})
#define PANGO_MICRO_VERSION (#{version[2]})

#define PANGO_CHECK_VERSION(major,minor,micro)    \\
    (PANGO_MAJOR_VERSION > (major) || \\
     (PANGO_MAJOR_VERSION == (major) && PANGO_MINOR_VERSION > (minor)) || \\
     (PANGO_MAJOR_VERSION == (major) && PANGO_MINOR_VERSION == (minor) && \\
      PANGO_MICRO_VERSION >= (micro)))


#endif /* __RBPANGO_VERSION_H__ */
]
