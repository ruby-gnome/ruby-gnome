/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcemain.h -

  $Author: mutoh $
  $Date: 2005/10/07 19:26:15 $

  Copyright (C) 2004,2005 Ruby-GNOME2 Project Team
  Copyright (C) 2003 Geoff Youngs
************************************************/

#ifndef __RBGTKSOURCEVIEWMAIN_H_
#define __RBGTKSOURCEVIEWMAIN_H_

#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagesmanager.h>
#include <gtksourceview/gtksourcemarker.h>
#include <gtksourceview/gtksourcestylescheme.h>
#include <gtksourceview/gtksourcetag.h>
#include <gtksourceview/gtksourcetagstyle.h>
#include <gtksourceview/gtksourcetagtable.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourceiter.h>
#include <gtksourceview/gtksourceview-typebuiltins.h>

#include "ruby.h"
#include "rbgtk.h"
#include "rbgtksourceviewversion.h"

#if defined(G_PLATFORM_WIN32) && !defined(RUBY_GTK2_STATIC_COMPILATION)
#  ifdef RUBY_GTK2_COMPILATION
#    define RUBY_GTK2_VAR __declspec(dllexport)
#  else
#    define RUBY_GTK2_VAR extern __declspec(dllimport)
#  endif
#else
#  define RUBY_GTK2_VAR extern
#endif

RUBY_GTK2_VAR VALUE mGtk;
RUBY_GTK2_VAR VALUE mGdk;

#endif /* __RBGTKSOURCEVIEWMAIN_H_ */
