=begin
  makeversion.rb -

  $Author: mutoh $
  $Date: 2005/09/15 17:30:46 $

  Copyright (C) 2005 Masao Mutoh
=end

version = ARGV[0].split(/\./)

puts %Q[/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkversion.h -

  $Author: mutoh $
  $Date: 2005/09/15 17:30:46 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#ifndef __RBATK_VERSION_H__
#define __RBATK_VERSION_H__

#define ATK_MAJOR_VERSION (#{version[0]})
#define ATK_MINOR_VERSION (#{version[1]})
#define ATK_MICRO_VERSION (#{version[2]})

#define ATK_CHECK_VERSION(major,minor,micro)    \\
  (ATK_MAJOR_VERSION > (major) || \\
   (ATK_MAJOR_VERSION == (major) && ATK_MINOR_VERSION > (minor)) || \\
   (ATK_MAJOR_VERSION == (major) && ATK_MINOR_VERSION == (minor) && \\
    ATK_MICRO_VERSION >= (micro)))

#endif /* __RBATK_VERSION_H__ */
]
