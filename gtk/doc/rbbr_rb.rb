#!/usr/local/bin/ruby

class ClassWalker
  protected
  @class9 = [] # class queue
  @down = true; attr("down", true)

  def initialize(down = true)
    @class9 = []
    @down = down
  end

  def currentClass
    @class9[@class9.size-1]
  end
  def nest
    @class9.size
  end

  def visitIncludeModule(m); end
  def visitConstant(c); end
  def visitPrivateMethod(m); end
  def visitProtectedMethod(m); end
  def visitPublicMethod(m); end
  def visitSingletonMethod(m); end


  def visitModule(cls)
    if not cls.is_a? Module
      raise TypeError, format("argument must be Module: %s", cls.inspect)
    end
    @class9.push(cls)

    cls.included_modules.each do |m|
      next if m == Kernel
      visitIncludeModule(m)
    end

    underModule = []
    underClass = Hash.new
    const = []
    const_ = []
    cls.constants.each do |c|
      begin
	val = cls.const_get(c)
      rescue
	printf "cannot get constant %s from %s.\n", c, cls if $DEBUG
	next
      end
      if val.is_a? Class
	key = val.superclass
	ary = underClass[key]
	if ary.nil?
	  ary = []
	  underClass[key] = ary
	end
	ary << val

      elsif val.is_a? Module
	underModule << val

      elsif c.index('_').nil?
	const << c
      else
	const_ << c
      end
    end

    const.sort.each do |c|
      visitConstant(c)
    end
    const_.sort.each do |c|
      visitConstant(c)
    end

    s_methods = cls.singleton_methods
    [ "private", "protected", "public" ].each do |s|
      methods = cls.module_eval(s + "_instance_methods")
      next if methods.size == 0
      s_methods -= methods
      methods.sort.each do |m|
	eval("visit" + s.capitalize + "Method(m)")
      end
    end

    cls.ancestors.each do |c|
      next if c == cls
      # it influece to ignore overriding class method...
      s_methods -= c.singleton_methods
    end
    s_methods.sort.each do |m|
      visitSingletonMethod(m)
    end

    ([underModule] + underClass.values).each do |a|
      a.uniq.each do |c|
		visitModule(c)
      end
    end if @down
    @class9.pop
    self
  end

  public
  def visit(o)
    if o.is_a? Module
      visitModule(o)
    elsif defined? o.each
      o.each { |c| visitModule(c) }
    else
      raise TypeError, format("argument must be Module: %s", o.inspect)
    end
    self
  end
end

class ClassDumper < ClassWalker
  @last = nil
  @out = nil
  @cname9 = []

  def initialize(out = $stdout)
    super(true)
    @last = nil
    @cname9 = []

    if out.is_a? IO
      @out = out
    elsif defined? "out.print" and defined? "out.printf"
      @out = out
    elsif not out.nil?
      raise TypeError,
	format("argument must be IO: %s", out.inspect)
    end
  end

  LAST_INCLUDE   = 1
  LAST_CONSTANT  = 2
  LAST_PRIVATE   = 3
  LAST_PROTECTED = 4
  LAST_PUBLIC    = 5
  LAST_SINGLETON = 6
  LAST_MODULE    = 7

  def currentClassName
    @cname9[@cname9.size-1]
  end

  def indent(diff=0)
    return if @out.nil?
    @out.print( "  " * (nest()+diff) )
  end

  def println(*args)
    return if @out.nil?

    if 0 < args.size
      if args.size == 1
	@out.print args[0]
      else
	@out.printf *args
      end
    end
    @out.print "\n"
  end

  def visitIncludeModule(m)
    if @last != LAST_PRIVATE and not @last.nil?
      println
    end
    @last = LAST_INCLUDE
    indent
    println("include %s", m.name)
  end
  def visitConstant(c)
    if @last != LAST_CONSTANT and not @last.nil?
      println
    end
    @last = LAST_CONSTANT
    indent
    println("%s \t= %s", c, currentClass.const_get(c))
  end
  def visitPrivateMethod(m)
    if @last != LAST_PRIVATE
      println unless @last.nil?
      indent
      println("private")
    end
    @last = LAST_PRIVATE
    indent
    println("def %s(); end", m)
  end
  def visitProtectedMethod(m)
    if @last != LAST_PROTECTED
      println unless @last.nil?
      indent
      println("protected")
    end
    @last = LAST_PROTECTED
    indent
    println("def %s(); end", m)
  end
  def visitPublicMethod(m)
    if @last != LAST_PUBLIC
      println unless @last.nil?
      indent
      println("public")
    end
    @last = LAST_PUBLIC
    indent
    println("def %s(); end", m)
  end
  def visitSingletonMethod(m)
    if @last != LAST_SINGLETON
      println unless @last.nil?
      indent
      println("#class methods")
    end
    @last = LAST_SINGLETON
    indent
    println("def %s.%s(); end", currentClassName(), m)
  end

  def visitModule(cls)
    println if not @last.nil?
    @last = nil

    cname = cls.name
    i = cname.rindex("::")
    if not i.nil?
      cname = cname[i+2..cname.size]
    end
    @cname9.push(cname)

    defkey = ""
    superclass = ""
    if cls.is_a? Class
      defkey = "class"
      superclass = format(" < %s", cls.superclass)
    else
      defkey = "module"
    end
    indent
    println("%s %s%s # %s", defkey, cname, superclass, cls.name)

    super

    indent
    println("end # of %s %s", defkey, cls.name)
    @cname9.pop
    @last = LAST_MODULE
  end
end

# defaults is a Hash. it's key and value is: {require => [classes]}
def initialize_args(defaults = [])
  tmp = []
  ARGV.each do |arg|
    begin
      c = eval(arg)
      tmp << c
    rescue
      print "cannot found Module: " + arg + "\n"
    end
  end
  if tmp.size == 0
    defaults.each do |req, cls|
      begin
	require req
      rescue
	print "cannot load module file: " << req << "\n"
	next
      end if req.is_a? String and 0 < req.size

      cls = [cls] unless cls.is_a? Array
      cls.each do |cname|
	if cname.is_a? Module
	  tmp << c
	else
	  begin
	    c = eval(cname.to_s)
	    tmp << c
	  rescue
	    print "cannot found Module: " + cname.to_s + "\n"
	  end
	end
      end
    end
  end
  if tmp.size == 0
    print "you must specify dump class!\n"
    exit
  end
  classes = []
p tmp
  tmp.each do |ances|
    classes.delete_if { |descen| descen.is_a? ances }
    classes << ances
  end
  classes
end

