/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkconversions.h -

  Copyright (C) 2008 Ruby-GNOME2 Project Team
************************************************/

#ifndef __RB_GTK_MACROS_H__
#define __RB_GTK_MACROS_H__

#define RBGTK_MAJOR_VERSION RBGLIB_MAJOR_VERSION
#define RBGTK_MINOR_VERSION RBGLIB_MINOR_VERSION
#define RBGTK_MICRO_VERSION RBGLIB_MICRO_VERSION

#if defined(G_PLATFORM_WIN32) && !defined(RUBY_GTK2_STATIC_COMPILATION)
#  ifdef RUBY_GTK2_COMPILATION
#    define RUBY_GTK2_VAR __declspec(dllexport)
#  else
#    define RUBY_GTK2_VAR extern __declspec(dllimport)
#  endif
#else
#  define RUBY_GTK2_VAR extern
#endif

#endif /* __RB_GTK_MACROS_H__ */
