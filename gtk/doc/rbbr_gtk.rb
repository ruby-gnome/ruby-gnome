require 'rbbr_rb.rb'
require 'gtk2'

class ClassTree < ClassWalker
  def initialize(tree = nil)
    super(true)
    @p2c = Hash.new()
    @method = Hash.new()
    @constant = Hash.new()
  end

  def visitConstant(c)
    @constant[currentClass] << c
  end

  LAST_PRIVATE   = 1
  LAST_PROTECTED = 2
  LAST_PUBLIC    = 3
  LAST_SINGLETON = 4
  def visitPrivateMethod(m)
    ary = @method[currentClass]
    if @last != LAST_PRIVATE
      ary << "--private--"
    end
    @last = LAST_PRIVATE
    ary << m
  end
  def visitProtectedMethod(m)
    ary = @method[currentClass]
    if @last != LAST_PROTECTED
      ary << "--protected--"
    end
    @last = LAST_PROTECTED
    ary << m
  end
  def visitPublicMethod(m)
    ary = @method[currentClass]
    if @last != LAST_PUBLIC
      ary << "--public--"
    end
    @last = LAST_PUBLIC
    ary << m
  end
  def visitSingletonMethod(m)
    ary = @method[currentClass]
    if @last != LAST_SINGLETON
      ary << "--singleton--"
    end
    @last = LAST_SINGLETON
    ary << m
  end

  def visitModule(m)
    if not m.is_a? Module # raise
      super
      return
    end

    @constant[m] = [] if @constant[m].nil?
    @method[m] = [] if @method[m].nil?

    parents = m.included_modules
    if m.is_a? Class
      parents << m.superclass
    end
    if parents.size == 0
      @p2c[m] = []
    else
      parents.each do |p|
	ary = @p2c[p]
	if ary.nil?
	  ary = []
	  @p2c[p] = ary
	end
	ary << m
      end
    end
    super

    self
  end

  def createDescendantsTree
    descendants = Gtk::Tree::new()
    cc = []
    mm = []
    @p2c.each_key do |c|
      if c.type == Module
	next if c == Kernel
	mm << c
      elsif @p2c[c.superclass].nil?
	cc << c
      else
	@p2c[c.superclass] << c unless @p2c[c.superclass].index(c)
      end
    end

    cc.sort{|a,b| a.name<=>b.name}.each do |c|
      append(descendants, c, true)
    end
    mm.sort{|a,b| a.name<=>b.name}.each do |c|
      append(descendants, c, true)
    end
    descendants
  end

  def createConstantsList(cls)
    @cList = Gtk::CList::new(1)
    @cList.set_column_title(0,"constant")
    @cList.column_titles_show()
    @cList.set_column_auto_resize(0, true)
    changeConstants(cls)
  end
  def changeConstants(cls)
    @cList.clear
    if @constant[cls].is_a? Array
      @constant[cls].each do |c|
	@cList.append([c])
      end
    end
    @cList
  end

  def createMethodsList(cls)
    @mList = Gtk::CList::new(1)
    @mList.set_column_title(0,"method")
    @mList.column_titles_show()
    @mList.set_column_auto_resize(0, true)
    changeMethods(cls)
  end
  def changeMethods(cls)
    @mList.clear
    if @method[cls].is_a? Array
      @method[cls].each do |c|
	@mList.append([c])
      end
    end
    @mList
  end

  private
  def append(tree, cls, f=true)
    i = Gtk::TreeItem::new(cls.name)
    tree.append(i)
    i.show
    if f and @p2c[cls].is_a? Array and 0<@p2c[cls].size
      t = Gtk::Tree::new()
      i.set_subtree(t)
      @p2c[cls].sort { |a,b| a.name <=> b.name }.each { |c| append(t, c) }
    end
  end
end
