require 'mkmf'


module PKGConfig
  @@cmd = with_config('pkg-config', 'pkg-config')
  if /mswin32/ =~ RUBY_PLATFORM and /^cl\b/ =~ Config::CONFIG['CC']
    @@cmd += ' --msvc-syntax'
  end

  def exists?(pkg)
    system("#{@@cmd} --exists #{pkg}")
  end
  module_function :exists?

  def libs(pkg)
    `#{@@cmd} --libs #{pkg}`.chomp
  end
  module_function :libs

  def cflags(pkg)
    `#{@@cmd} --cflags #{pkg}`.chomp
  end
  module_function :cflags

  def variable(pkg, var)
    `#{@@cmd} --variable=#{var} #{pkg}`.chomp
  end
  module_function :variable

  def have_package(pkg)
    STDOUT.print("checking for #{pkg}... ")
    STDOUT.flush
    if exists? pkg
      STDOUT.print "yes\n"
      $libs   += ' ' + libs(pkg)
      $CFLAGS += ' ' + cflags(pkg)
      true
    else
      STDOUT.print "no\n"
      false
    end
  end
  module_function :have_package
end

$CFLAGS += ' -I$(sitearchdir)'

unless defined? macro_defined?
  def macro_defined?(macro, src, opt="")
    try_cpp(src + <<EOP, opt)
#ifndef #{macro}
# error
#endif
EOP
  end
end


STDOUT.print("checking for GCC... ")
STDOUT.flush
if macro_defined?("__GNUC__", "")
  STDOUT.print "yes\n"
  $CFLAGS += ' -Wall' 
  $cc_is_gcc = true
else
  STDOUT.print "no\n"
  $cc_is_gcc = false
end


def check_win32()
  $G_PLATFORM_WIN32 = false
  $G_OS_WIN32       = false
  $G_WITH_CYGWIN    = false

  STDOUT.print("checking for G_PLATFORM_WIN32... ")
  STDOUT.flush
  if macro_defined?('G_PLATFORM_WIN32', "#include <glibconfig.h>\n")
    STDOUT.print "yes\n"
    $G_PLATFORM_WIN32 = true
  else
    STDOUT.print "no\n"
  end

  if $G_PLATFORM_WIN32
    STDOUT.print("checking for G_OS_WIN32... ")
    STDOUT.flush
    if macro_defined?('G_OS_WIN32', "#include <glibconfig.h>\n")
      STDOUT.print "yes\n"
      $G_OS_WIN32 = true
      if $cc_is_gcc
	if /^2\./ =~ `#{Config::CONFIG['CC']} -dumpversion`.chomp
	  $CFLAGS += ' -fnative-struct'
	else
	  $CFLAGS += ' -mms-bitfields'
	end
      end
    else
      STDOUT.print "no\n"
    end

#    STDOUT.print("checking for G_WITH_CYGWIN... ")
#    STDOUT.flush
#    if macro_defined?('G_WITH_CYGWIN', "#include <glibconfig.h>\n")
#      STDOUT.print "yes\n"
#      $G_WITH_CYGWIN = true
#    else
#      STDOUT.print "no\n"
#    end
  end

  nil
end


def set_output_lib(filename)
  if /cygwin|mingw/ =~ RUBY_PLATFORM
    if RUBY_VERSION > "1.8"
      CONFIG["DLDFLAGS"].gsub!(/ -Wl,--out-implib=[^ ]+/, '')
      CONFIG["DLDFLAGS"] << " -Wl,--out-implib=#{filename}" if filename
    else
      CONFIG["DLDFLAGS"].gsub!(/ --output-lib\s+[^ ]+/, '')
      CONFIG["DLDFLAGS"] << " --output-lib #{filename}" if filename
    end
  end
end


def create_top_makefile(sub_dirs = ["src"])
  mfile = File.open("Makefile", "w")
=begin
  if /mswin32/ =~ PLATFORM
    mfile.print <<END

all:
		@cd src
		@nmake -nologo

install:
		@cd src
		@nmake -nologo install DESTDIR=$(DESTDIR)

site-install:
		@cd src
		@nmake -nologo site-install DESTDIR=$(DESTDIR)

clean:
		@cd src
		@nmake -nologo clean
		@cd ..
		@-rm -f Makefile extconf.h conftest.*
		@-rm -f *.lib *~
END
  else
=end
    mfile.print <<END
all:
#{sub_dirs.map{|d| "	@cd #{d}; make all\n"}.join('')}

install:
#{sub_dirs.map{|d| "	@cd #{d}; make install\n"}.join('')}
site-install:
#{sub_dirs.map{|d| "	@cd #{d}; make site-install\n"}.join('')}
clean:
#{sub_dirs.map{|d| "	@cd #{d}; make clean\n"}.join('')}
distclean:	clean
#{sub_dirs.map{|d| "	@cd #{d}; make distclean\n"}.join('')}
	@rm -f Makefile extconf.h conftest.*
	@rm -f core *~ mkmf.log
END
#  end
  mfile.close
end


