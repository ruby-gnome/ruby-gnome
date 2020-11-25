# Heroku installation

## Install rsvg2

This document describes how to install rsvg2 on heroku (cedar-16)

### Steps

  1. Add buildpack apt to your app: `heroku buildpacks:add --index 1 https://github.com/heroku/heroku-buildpack-apt.git`
  2. Add Aptfile to your rails root directory.
  3. Include in the Aptfile the following packages:
      ```
      gir1.2-freedesktop
      gir1.2-rsvg-2.0
      gir1.2-gdkpixbuf-2.0
      gir1.2-glib-2.0
      libgirepository-1.0-1
      libgirepository1.0-dev
      libcairo-gobject2
      ```
  4. In your gemfile add
      ```
      gem 'rsvg2'
      ```
  5. Add the following ENV var: `GI_TYPELIB_PATH = /app/.apt/usr/lib/x86_64-linux-gnu/girepository-1.0`
