/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  init.c -

  $Author: mutoh $
  $Date: 2002/11/05 10:39:11 $

  Copyright (C) 1998-2001 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

void
Init_gtk2()
{
    gtk_set_locale();

    {
        /* Gdk modifies sighandlers, sigh */
#ifdef NT
        RETSIGTYPE (*sigfunc[3])();
#else
        RETSIGTYPE (*sigfunc[7])();
#endif

#ifdef NT
        sigfunc[0] = signal(SIGINT, SIG_IGN);
        sigfunc[1] = signal(SIGSEGV, SIG_IGN);
        sigfunc[2] = signal(SIGTERM, SIG_IGN);
#else
        sigfunc[0] = signal(SIGHUP, SIG_IGN);
        sigfunc[1] = signal(SIGINT, SIG_IGN);
        sigfunc[2] = signal(SIGQUIT, SIG_IGN);
        sigfunc[3] = signal(SIGBUS, SIG_IGN);
        sigfunc[4] = signal(SIGSEGV, SIG_IGN);
        sigfunc[5] = signal(SIGPIPE, SIG_IGN);
        sigfunc[6] = signal(SIGTERM, SIG_IGN);
#endif
#ifdef NT
        signal(SIGINT,  sigfunc[0]);
        signal(SIGSEGV, sigfunc[1]);
        signal(SIGTERM, sigfunc[2]);
#else
        signal(SIGHUP,  sigfunc[0]);
        signal(SIGINT,  sigfunc[1]);
        signal(SIGQUIT, sigfunc[2]);
        signal(SIGBUS,  sigfunc[3]);
        signal(SIGSEGV, sigfunc[4]);
        signal(SIGPIPE, sigfunc[5]);
        signal(SIGTERM, sigfunc[6]);
#endif
    }

    Init_gtk_gdk();
    Init_gtk_gtk();

    Init_gtk_inits();
}
