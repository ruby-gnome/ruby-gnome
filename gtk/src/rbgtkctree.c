/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkctree.c -

  $Author: sakai $
  $Date: 2002/08/13 18:01:38 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifndef GTK_DISABLE_DEPRECATED

#include "global.h"

/* FIXME
#define RVAL2CTREENODE(n) GTK_CTREE_NODE(RVAL2GVAL(n))
*/
#define RVAL2CTREENODE(n) GTK_CTREE_NODE(RVAL2GOBJ(n))

static void
ctree_node_mark(ctree, node, notused)
    GtkCTree *ctree;
    GtkCTreeNode *node;
    gpointer notused;
{
    if (node) {
        GtkCTreeRow *row = GTK_CTREE_ROW(node);
        if (row && row->row.data) {
            rb_gc_mark_maybe((VALUE)row->row.data);
        }
    }
}

static void
ctree_mark(ctree)
    GtkCTree *ctree;
{
    if (ctree)
        gtk_ctree_pre_recursive(ctree, NULL, ctree_node_mark, NULL);
}

/*
 * initialize(titles, tree_column)
 *
 * initialize a new Gtk::CTree widget.
 *
 * arguments:
 *   titles - titles for the columns
 *            or Number of columns.
 *   tree_column - Which column has the tree graphic.
 */
static VALUE
ctree_initialize(self, titles, tree_column)
	VALUE self, titles, tree_column;
{
    GtkWidget *widget;

    if (TYPE(titles) == T_ARRAY) {
        char **buf;
        int i, len;

        len = RARRAY(titles)->len;
        buf = ALLOCA_N(char*, len);
        for (i=0; i<len; i++) {
            buf[i] = STR2CSTR(RARRAY(titles)->ptr[i]);
        }
        widget = gtk_ctree_new_with_titles(len, NUM2INT(tree_column), buf);
    } else {
        widget = gtk_ctree_new(NUM2INT(titles), NUM2INT(tree_column));
    }
    RBGTK_INITIALIZE(self, widget);

    return Qnil;
}

/*
 * insert_node(parent, sibling, texts, spacing,
 *         pixmap_closed, mask_closed,
 *         pixmap_opened, mask_opened,
 *         is_leaf, expanded)
 *
 * Insert a new node to the tree. The position is specified through
 * the parent-sibling notation, as explained in the introduction above.
 *
 * arguments:
 *   parent - The parent node to be.
 *   sibling - The sibling node to be.
 *   texts - The texts to be shown in each column.
 *   spacing - The extra space between the pixmap and the text.
 *   pixmap_closed - The pixmap to be used when the node is collapsed.
 *   mask_closed - The mask for the above pixmap.
 *   pixmap_opened - The pixmap to be used when the children are visible.
 *   mask_opened - The mask for the above pixmap.
 *   is_leaf - Whether this node is going to be a leaf.
 *   expanded - Whether this node should start out expanded or not.
 */
static VALUE
ctree_insert_node(self, parent, sibling, texts, spacing,
				  pixmap_closed, mask_closed,
				  pixmap_opened, mask_opened,
				  is_leaf, expanded)
    VALUE self, parent, sibling, texts, spacing;
    VALUE pixmap_closed, mask_closed;
    VALUE pixmap_opened, mask_opened;
    VALUE is_leaf, expanded;
{
    GtkCTreeNode *c_parent, *c_sibling, *result;
    int i, len;
    char **c_texts;

    c_parent = RVAL2CTREENODE(parent);
    c_sibling = RVAL2CTREENODE(sibling);

    Check_Type(texts, T_ARRAY);
    len = RARRAY(texts)->len;
    c_texts = ALLOCA_N(char*, len);
    for (i=0; i<len; i++) {
        VALUE text = RARRAY(texts)->ptr[i];
        c_texts[i] = NIL_P(text)?0:STR2CSTR(text);
    }

    result = gtk_ctree_insert_node(GTK_CTREE(RVAL2GOBJ(self)),
                                   c_parent, c_sibling, c_texts,
                                   NUM2INT(spacing),
                                   GDK_PIXMAP(RVAL2GOBJ(pixmap_closed)),
                                   GDK_BITMAP(RVAL2GOBJ(mask_closed)),
                                   GDK_PIXMAP(RVAL2GOBJ(pixmap_opened)),
                                   GDK_BITMAP(RVAL2GOBJ(mask_opened)),
                                   RTEST(is_leaf),
                                   RTEST(expanded));
    return GVAL2RVAL(result);
}

/*
 * remove_node(node)
 *
 * Remove the node and all nodes underneath it from the tree.
 *
 * argument:
 *   node - The node to be removed. 
 */
static VALUE
ctree_remove_node(self, node)
	VALUE self, node;
{
    gtk_ctree_remove_node(GTK_CTREE(RVAL2GOBJ(self)), RVAL2CTREENODE(node));
    return self;
}

static VALUE
ctree_insert_gnode(self, parent, sibling, gnode, func, data)
	VALUE self, parent, sibling, gnode, func, data;
{
    /* TODO */
    rb_notimplement();
    return Qnil;
}

static VALUE
ctree_export_to_gnode(self, parent, sibling, gnode, func, data)
	VALUE self, parent, sibling, gnode, func, data;
{
    /* TODO */
    rb_notimplement();
    return Qnil;
}

/*
 * post_recursive(node) { |n| ... }
 *
 * Recursively apply a block to all nodes of the tree at or below a certain
 * node. The method is called for each node after it has been called
 * for that node's children.
 *
 * arguments:
 *   node - The node where to start. NULL means to start at the root.
 *   { |n| ... } - The block to apply to each node.
 *                 each node `n' is passed as block argument.
 */
static VALUE
ctree_post_recursive(self, node)
    VALUE self, node;
{
    GtkCTree *ctree;
    GtkCTreeNode *c_node, *tmp;

    ctree = GTK_CTREE(RVAL2GOBJ(self));
    if (NIL_P(node)) {
        c_node = GTK_CTREE_NODE(GTK_CLIST(ctree)->row_list);
    } else {
        c_node = RVAL2CTREENODE(node);
        c_node = GTK_CTREE_ROW(c_node)->children;
    }

    while (c_node) {
        tmp = GTK_CTREE_ROW(c_node)->sibling;
        ctree_post_recursive(self, GTK_CTREE_NODE(GVAL2RVAL(c_node)));
        c_node = tmp;
    }

    if (!NIL_P(node))
        rb_yield(node);

    return Qnil;
}

/*
 * post_recursive_to_depth(node, depth) do { |n| ... }
 *
 * Recursively apply a function to nodes up to a certain depth.
 * The function is called for each node after it has been called
 * for that node's children.
 *
 * arguments:
 *   node - The node where to start. NULL means to start at the root.
 *   depth - The maximum absolute depth for applying the function.
 *           If depth is negative, this function just calls
 *           post_recursive.
 *   { |n| ... } - The block to apply to each node.
 *                 each node `n' is passed as block argument.
 */
static VALUE
ctree_post_recursive_to_depth(self, node, depth)
    VALUE self, node, depth;
{
    GtkCTree *ctree;
    GtkCTreeNode *c_node, *tmp;
    gint c_depth;

    c_depth = NUM2INT(depth);
    if (c_depth < 0) {
        ctree_post_recursive(self, node, depth);
        return Qnil;
    }

    ctree = GTK_CTREE(RVAL2GOBJ(self));
    if (NIL_P(node)) {
        c_node = GTK_CTREE_NODE(GTK_CLIST(ctree)->row_list);
    } else {
        c_node = RVAL2CTREENODE(node);
        c_node = GTK_CTREE_ROW(c_node)->children;
    }

    if (c_node && GTK_CTREE_ROW(c_node)->level <= c_depth) {
        while (c_node) {
            tmp = GTK_CTREE_ROW(c_node)->sibling;
            ctree_post_recursive_to_depth(self, GTK_CTREE_NODE(GVAL2RVAL(c_node)), depth);
            c_node = tmp;
        }
    }

    if (!NIL_P(node) && GTK_CTREE_ROW(c_node)->level <= depth)
        rb_yield(node);

    return Qnil;
}

/*
 * pre_recursive(node) { |n| ... }
 *
 * Recursively apply a function to all nodes of the tree at or below
 * a certain node. The function is called for each node after it has
 * been called for its parent.
 *
 * arguments:
 *   node - The node where to start. NULL means to start at the root.
 *   { |n| ... } - The block to apply to each node.
 *                 each node `n' is passed as block argument.
 */
static VALUE
ctree_pre_recursive(self, node)
    VALUE self, node;
{
    GtkCTree *ctree;
    GtkCTreeNode *c_node, *tmp;

    ctree = GTK_CTREE(RVAL2GOBJ(self));
    if (NIL_P(node)) {
        c_node = GTK_CTREE_NODE(GTK_CLIST(ctree)->row_list);
    } else {
        rb_yield(node);
        c_node = RVAL2CTREENODE(node);
        c_node = GTK_CTREE_ROW(c_node)->children;
    }

    while (c_node) {
        tmp = GTK_CTREE_ROW(c_node)->sibling;
        ctree_pre_recursive(self, GTK_CTREE_NODE(GVAL2RVAL(c_node)));
        c_node = tmp;
    }

    return Qnil;
}

/*
 * pre_recursive_to_depth(node, depth) { |n| ... }
 *
 * Recursively apply a function to nodes up to a certain depth.
 * The function is called for each node after it has been called
 * for that node's children.
 *
 * arguments:
 *   node - The node where to start. NULL means to start at the root.
 *   depth - The maximum absolute depth for applying the function.
 *           If depth is negative, this function just calls
 *           gtk_ctree_post_recursive.
 *   { |n| ... } - The block to apply to each node.
 *                 each node `n' is passed as block argument.
 */
static VALUE
ctree_pre_recursive_to_depth(self, node, depth)
    VALUE self, node, depth;
{
    GtkCTree *ctree;
    GtkCTreeNode *c_node, *tmp;
    gint c_depth;

    c_depth = NUM2INT(depth);
    if (c_depth < 0) {
        ctree_pre_recursive(self, node);
        return Qnil;
    }

    ctree = GTK_CTREE(RVAL2GOBJ(self));
    if (NIL_P(node)) {
        c_node = GTK_CTREE_NODE(GTK_CLIST(ctree)->row_list);
    } else {
        rb_yield(node);
        c_node = RVAL2CTREENODE(node);
        c_node = GTK_CTREE_ROW(c_node)->children;
    }

    if (c_node && GTK_CTREE_ROW(c_node)->level <= c_depth) {
        while (c_node) {
            tmp = GTK_CTREE_ROW(c_node)->sibling;
            ctree_pre_recursive_to_depth(self, GTK_CTREE_NODE(GVAL2RVAL(c_node)), depth);
            c_node = tmp;
        }
    }

    return Qnil;
}

/*
 * is_viewable_p(node)
 *
 * This function checks whether the given node is viewable i.e. so that
 * all of its parent nodes are expanded. This is different from being
 * actually visible: the node can be viewable but outside the scrolling
 * area of the window.
 *
 * return:
 *   Whether the node is viewable. 
 */
static VALUE
ctree_is_viewable_p(self, node)
    VALUE self, node;
{
    gboolean result = gtk_ctree_is_viewable(GTK_CTREE(RVAL2GOBJ(self)),
                                            RVAL2CTREENODE(node));
    return result?Qtrue:Qfalse;
}

/*
 * last(node)
 *
 * Returns the last child of the last child of the last child... 
 * of the given node.
 */
static VALUE
ctree_last(self, node)
	VALUE self, node;
{
    return GVAL2RVAL(gtk_ctree_last(GTK_CTREE(RVAL2GOBJ(self)),
									RVAL2CTREENODE(node)));
}

/*
 * find(node, child)
 *
 * Return true if child is on some level a child (grandchild...) of the node.
 *
 * arguments:
 *   node - The node to start searching from. May be NULL.
 *   child - 
 */
static VALUE
ctree_find(self, node, child)
	VALUE self, node, child;
{
    gboolean result = gtk_ctree_find(GTK_CTREE(RVAL2GOBJ(self)),
                                     RVAL2CTREENODE(node),
                                     RVAL2CTREENODE(child));
    return result?Qtrue:Qfalse;
}

/*
 * is_ancestor_p(node, child)
 *
 * Return true if node is an ancestor of child.
 */
static VALUE
ctree_is_ancestor_p(self, node, child)
	VALUE self, node, child;
{
    gboolean result = gtk_ctree_is_ancestor(GTK_CTREE(RVAL2GOBJ(self)),
                                            RVAL2CTREENODE(node),
                                            RVAL2CTREENODE(child));
    return result?Qtrue:Qfalse;
}

#if 0
/*
 * find_by_row_data(node, data)
 *
 * Finds a node in the tree under node that has the given
 * user data pointer.
 *
 * arguments:
 *   node - The node where to start searching.
 *   data - User data.
 *
 * return:
 *   The node, or NULL if not found.
 */
static VALUE
ctree_find_by_row_data(self, node, data)
	VALUE self, node, data;
{
    GtkCTreeNode *result;

    result = gtk_ctree_find_by_row_data_custom(
		GTK_CTREE(RVAL2GOBJ(self)), RVAL2CTREENODE(node),
		data, ...);

    return GVAL2RVAL(resulte);
}

/*
 * find_all_by_row_data(node, data)
 *
 * Find the first node under node whose row data pointer fulfills
 * a custom criterion.
 *
 * arguments:
 *   node - The node where to start searching.
 *   data - User data.
 *
 * return:
 *   The array of nodes that have the given data. 
 */
static VALUE
ctree_find_all_by_row_data(self, node, data)
	VALUE self, node, data;
{
}
#endif /* 0 */

/*
 * is_hot_spot_p(x, y)
 *
 * Return true if the given coordinates lie on an expander button.
 */
static VALUE
ctree_is_hot_spot_p(self, x, y)
	VALUE self, x, y;
{
    gboolean result = gtk_ctree_is_hot_spot(GTK_CTREE(RVAL2GOBJ(self)),
                                            NUM2INT(x),
                                            NUM2INT(y));
    return result?Qtrue:Qfalse;
}

/*
 * move(node, new_parent, new_sibling)
 *
 * Move a node in the tree to another location.
 *
 * arguments:
 *   node - The node to be moved.
 *   new_parent - The new parent-to-be of the node.
 *   new_sibling - The new sibling-to-be of the node.
 */
static VALUE
ctree_move(self, node, new_parent, new_sibling)
	VALUE self, node, new_parent, new_sibling;
{
    gtk_ctree_move(GTK_CTREE(RVAL2GOBJ(self)),
				   RVAL2CTREENODE(node),
				   RVAL2CTREENODE(new_parent),
				   RVAL2CTREENODE(new_sibling));
    return self;
}

/*
 * expand(node)
 *
 * Expand one node.
 *
 * argument:
 *   node - Node.
 */
static VALUE
ctree_expand(self, node)
    VALUE self, node;
{
    gtk_ctree_expand(GTK_CTREE(RVAL2GOBJ(self)), RVAL2CTREENODE(node));
    return self;
}

/*
 * expand_recursive(node)
 *
 * Expand one node and all nodes underneath.
 *
 * argument:
 *   node - Node.
 */
static VALUE
ctree_expand_recursive(self, node)
    VALUE self, node;
{
    gtk_ctree_expand_recursive(GTK_CTREE(RVAL2GOBJ(self)),
                               RVAL2CTREENODE(node));
    return self;
}

/*
 * expand_to_depth(node, depth)
 *
 * Expand a node and its children up to the depth given.
 *
 * arguments:
 *   node - Node.
 *   depth - The (absolute) depth up to which to expand nodes.
 */
static VALUE
ctree_expand_to_depth(self, node, depth)
    VALUE self, node, depth;
{
    gtk_ctree_expand_to_depth(GTK_CTREE(RVAL2GOBJ(self)),
                              RVAL2CTREENODE(node),
                              NUM2INT(depth));
    return self;
}

/*
 * collapse(node)
 *
 * Collapse one node.
 *
 * argument:
 *   node - Node.
 */
static VALUE
ctree_collapse(self, node)
    VALUE self, node;
{
    gtk_ctree_collapse(GTK_CTREE(RVAL2GOBJ(self)),
                       RVAL2CTREENODE(node));
    return self;
}

/*
 * collapse_recursive(node)
 *
 * Collapse one node and all its subnodes.
 *
 * argument:
 *   node - Node.
 */
static VALUE
ctree_collapse_recursive(self, node)
    VALUE self, node;
{
    gtk_ctree_collapse_recursive(GTK_CTREE(RVAL2GOBJ(self)),
                                 RVAL2CTREENODE(node));
    return self;
}

/*
 * collapse_to_depth(node, depth)
 *
 * Collapse a node and its children up to the depth given.
 *
 * arguments:
 *   node - Node.
 *   depth - The (absolute) depth up to which to collapse nodes.
 */
static VALUE
ctree_collapse_to_depth(self, node, depth)
    VALUE self, node, depth;
{
    gtk_ctree_collapse_to_depth(GTK_CTREE(RVAL2GOBJ(self)),
                                RVAL2CTREENODE(node),
                                NUM2INT(depth));
    return self;
}

/*
 * toggle_expansion(node)
 *
 * Toggle a node, i.e. if it is collapsed, expand it and vice versa.
 */
static VALUE
ctree_toggle_expansion(self, node)
	VALUE self, node;
{
    gtk_ctree_toggle_expansion(GTK_CTREE(RVAL2GOBJ(self)),
                               RVAL2CTREENODE(node));
    return self;
}

/*
 * toggle_expansion_recursive(node)
 *
 * Toggle the expansion of a node and all its children.
 */
static VALUE
ctree_toggle_expansion_recursive(self, node)
	VALUE self, node;
{
    gtk_ctree_toggle_expansion_recursive(GTK_CTREE(RVAL2GOBJ(self)),
                                         RVAL2CTREENODE(node));
    return self;
}

/*
 * select(node)
 *
 * Cause the given node to be selected and emit the appropriate signal.
 *
 * argument:
 *   node - Node.
 */
static VALUE
ctree_select(self, node)
    VALUE self, node;
{
    gtk_ctree_select(GTK_CTREE(RVAL2GOBJ(self)), RVAL2CTREENODE(node));
    return self;
}

/*
 * select_recursive(node)
 *
 * Cause the given node and its subnodes to be selected
 * and emit the appropriate signal(s).
 *
 * argument:
 *   node - Node.
 */
static VALUE
ctree_select_recursive(self, node)
    VALUE self, node;
{
    gtk_ctree_select_recursive(GTK_CTREE(RVAL2GOBJ(self)),
                               RVAL2CTREENODE(node));
    return self;
}

/*
 * unselect(node)
 *
 * Unselect the given node and emit the appropriate signal.
 *
 * argument:
 *   node - Node.
 */
static VALUE
ctree_unselect(self, node)
    VALUE self, node;
{
    gtk_ctree_unselect(GTK_CTREE(RVAL2GOBJ(self)), RVAL2CTREENODE(node));
    return self;
}

/*
 * unselect_recursive(node)
 *
 * Unselect the given node and its subnodes
 * and emit the appropriate signal(s).
 *
 * argument:
 *   node - Node.
 */
static VALUE
ctree_unselect_recursive(self, node)
    VALUE self, node;
{
    gtk_ctree_unselect_recursive(GTK_CTREE(RVAL2GOBJ(self)),
                                 RVAL2CTREENODE(node));
    return self;
}

/*
 * node_set_text(node, column, text)
 *
 * Set the text in a node.
 *
 * arguments:
 *   node -
 *   column - The column whose text to change.
 *   text - The new text.
 */
static VALUE
ctree_node_set_text(self, node, column, text)
	VALUE self, node, column, text;
{
    gtk_ctree_node_set_text(GTK_CTREE(RVAL2GOBJ(self)),
                            RVAL2CTREENODE(node),
                            NUM2INT(column),
                            STR2CSTR(text));
    return self;
}

/*
 * node_set_pixmap(node, column, pixmap, mask)
 *
 * Set the pixmap in a node.
 *
 * arguments:
 *   node -
 *   column - The column whose pixmap to change.
 *   pixmap - 
 *   mask - 
 */
static VALUE
ctree_node_set_pixmap(self, node, column, pixmap, mask)
	VALUE self, node, column, pixmap, mask;
{
    gtk_ctree_node_set_pixmap(GTK_CTREE(RVAL2GOBJ(self)),
                              RVAL2CTREENODE(node),
                              NUM2INT(column),
							  GDK_PIXMAP(RVAL2GOBJ(pixmap)),
							  GDK_BITMAP(RVAL2GOBJ(mask)));
    return self;
}

/*
 * node_set_pixtext(node, column, text, spacing, pixmap, mask)
 *
 * Set both a pixmap and text in a node.
 *
 * arguments:
 *   node -
 *   column -
 *   text -
 *   spacing -
 *   pixmap -
 *   mask - 
 */
static VALUE
ctree_node_set_pixtext(self, node, column, text, spacing, pixmap, mask)
	VALUE self, node, column, text, spacing, pixmap, mask;
{
    gtk_ctree_node_set_pixtext(GTK_CTREE(RVAL2GOBJ(self)),
                               RVAL2CTREENODE(node),
                               NUM2INT(column),
                               STR2CSTR(text),
                               NUM2INT(spacing),
							   GDK_PIXMAP(RVAL2GOBJ(pixmap)),
							   GDK_BITMAP(RVAL2GOBJ(mask)));
    return self;
}

/*
 * set_node_info(node, texts, spacing,
 *       pixmap_closed, mask_closed,
 *       pixmap_opened, mask_opened,
 *       is_leaf, expanded)
 *
 * Change the information. Most parameters correspond to
 * the parameters of insert_node method.
 */
static VALUE
ctree_set_node_info(self, node, text, spacing,
					pixmap_closed, mask_closed,
					pixmap_opened, mask_opened,
					is_leaf, expanded)
	VALUE self, node, text, spacing;
	VALUE pixmap_closed, mask_closed;
	VALUE pixmap_opened, mask_opened;
	VALUE is_leaf, expanded;
{
    gtk_ctree_set_node_info(GTK_CTREE(RVAL2GOBJ(self)),
							RVAL2CTREENODE(node),
							STR2CSTR(text),
							NUM2INT(spacing),
							GDK_PIXMAP(RVAL2GOBJ(pixmap_closed)),
							GDK_BITMAP(RVAL2GOBJ(mask_closed)),
							GDK_PIXMAP(RVAL2GOBJ(pixmap_opened)),
							GDK_BITMAP(RVAL2GOBJ(mask_opened)),
							RTEST(is_leaf), RTEST(expanded));
    return self;
}

/*
 * node_set_shift(node, column, vertical, horizontal)
 *
 * Shift the given cell the given amounts in pixels.
 */
static VALUE
ctree_node_set_shift(self, node, column, vertical, horizontal)
	VALUE self, node, column, vertical, horizontal;
{
    gtk_ctree_node_set_shift(GTK_CTREE(RVAL2GOBJ(self)),
                             RVAL2CTREENODE(node),
                             NUM2INT(column),
                             NUM2INT(vertical),
                             NUM2INT(horizontal));
    return self;
}

/*
 * node_set_selectable(node, selectable)
 *
 * arguments:
 *   selectable - Whether this node can be selected by the user. 
 */
static VALUE
ctree_node_set_selectable(self, node, selectable)
	VALUE self, node, selectable;
{
    gtk_ctree_node_set_selectable(GTK_CTREE(RVAL2GOBJ(self)),
                                  RVAL2CTREENODE(node),
                                  RTEST(selectable));
    return self;
}

/*
 * node_get_selectable(node)
 *
 * return:
 *   Whether this node can be selected by the user. 
 */
static VALUE
ctree_node_get_selectable(self, node)
	VALUE self, node;
{
    gboolean result;

    result = gtk_ctree_node_get_selectable(GTK_CTREE(RVAL2GOBJ(self)),
										   RVAL2CTREENODE(node));

    return result?Qtrue:Qfalse;
}

/*
 * node_get_cell_type(node, column)
 *
 * Return the type of the given cell.
 */
static VALUE
ctree_node_get_cell_type(self, node, column)
	VALUE self, node, column;
{
    GtkCellType result;

    result = gtk_ctree_node_get_cell_type(GTK_CTREE(RVAL2GOBJ(self)),
                                          RVAL2CTREENODE(node),
                                          NUM2INT(column));

    return INT2FIX(result);
}

/*
 * node_get_text(node, column)
 *
 * return:
 *   The text that the given cell has. 
 */
static VALUE
ctree_node_get_text(self, node, column)
	VALUE self, node, column;
{
    gint result;
    gchar* text;

    result = gtk_ctree_node_get_text(GTK_CTREE(RVAL2GOBJ(self)),
									 RVAL2CTREENODE(node),
									 NUM2INT(column),
									 &text);
    if (!result) return Qnil;

    return rb_str_new2(text);
}

static VALUE
ctree_node_get_pixmap(self, node, column)
	VALUE self, node, column;
{
    gint result;
    GdkPixmap* pixmap;
    GdkBitmap* mask;

    result = gtk_ctree_node_get_pixmap(GTK_CTREE(RVAL2GOBJ(self)),
                                       RVAL2CTREENODE(node),
                                       NUM2INT(column),
                                       &pixmap, &mask);
    if (!result) return Qnil;

    return rb_ary_new3(2, GOBJ2RVAL(pixmap), GOBJ2RVAL(mask));
}

static VALUE
ctree_node_get_pixtext(self, node, column)
	VALUE self, node, column;
{
    gint result;
    gchar* text;
    guint8 spacing;
    GdkPixmap* pixmap;
    GdkBitmap* mask;

    result = gtk_ctree_node_get_pixtext(GTK_CTREE(RVAL2GOBJ(self)),
										RVAL2CTREENODE(node),
										NUM2INT(column),
										&text, &spacing,
										&pixmap, &mask);
    if (!result) return Qnil;

    return rb_ary_new3(4, CSTR2OBJ(text), INT2FIX(spacing),
					   GOBJ2RVAL(pixmap), GOBJ2RVAL(mask));
}

/*
 * get_node_info
 *
 * Get information corresponding to a node. Any of the return
 * parameters can be null.
 *
 * returns: [ text, spacing,
 *            pixmap_closed, mask_closed, pixmap_opened, mask_opened,
 *            is_leaf, expanded ]
 */
static VALUE
ctree_get_node_info(self, node)
    VALUE self, node;
{
    gint result;
    gchar* text;
    guint8 spacing;
    GdkPixmap* pixmap_closed, * pixmap_opened;
    GdkBitmap* mask_closed, * mask_opened;
    gboolean is_leaf, expanded;

    result = gtk_ctree_get_node_info(GTK_CTREE(RVAL2GOBJ(self)),
									 RVAL2CTREENODE(node),
									 &text, &spacing,
									 &pixmap_closed, &mask_closed,
									 &pixmap_opened, &mask_opened,
									 &is_leaf, &expanded);

    if (!result) return Qnil;
    
    return rb_ary_new3(8, CSTR2OBJ(text),
					   INT2FIX(spacing),
					   GOBJ2RVAL(pixmap_closed),
					   GOBJ2RVAL(mask_closed),
					   GOBJ2RVAL(pixmap_opened),
					   GOBJ2RVAL(mask_opened),
					   is_leaf?Qtrue:Qfalse,
					   expanded?Qtrue:Qfalse);
}

/*
 * node_set_row_style(node, style)
 *
 * Set the style of a row.
 *
 * arguments:
 *   node - tree node.
 *   style - style of row.
 */
static VALUE
ctree_node_set_row_style(self, node, style)
	VALUE self, node, style;
{
    gtk_ctree_node_set_row_style(GTK_CTREE(RVAL2GOBJ(self)),
                                 RVAL2CTREENODE(node),
                                 GTK_STYLE(GOBJ2RVAL(style)));
    return self;
}

/*
 * node_get_row_style(node)
 *
 * Get the style of a row.
 *
 * arguments:
 *   node - tree node.
 */
static VALUE
ctree_node_get_row_style(self, node)
	VALUE self, node;
{
    GtkStyle* result;
    result = gtk_ctree_node_get_row_style(GTK_CTREE(RVAL2GOBJ(self)),
                                          RVAL2CTREENODE(node));
    return GOBJ2RVAL(result);
}

/*
 * node_set_cell_style
 *
 * Set the style of an individual cell.
 */
static VALUE
ctree_node_set_cell_style(self, node, column, style)
	VALUE self, node, column, style;
{
    gtk_ctree_node_set_cell_style(GTK_CTREE(RVAL2GOBJ(self)),
                                  RVAL2CTREENODE(node),
                                  NUM2INT(column),
                                  GTK_STYLE(GOBJ2RVAL(style)));
    return self;
}

/*
 * node_get_cell_style
 *
 * Get the style of an individual cell.
 */
static VALUE
ctree_node_get_cell_style(self, node, column)
	VALUE self, node, column;
{
    GtkStyle* result;

    result = gtk_ctree_node_get_cell_style(GTK_CTREE(RVAL2GOBJ(self)),
                                           RVAL2CTREENODE(node),
                                           NUM2INT(column));
    return GOBJ2RVAL(result);
}

/*
 * node_set_foreground(node, color)
 */
static VALUE
ctree_node_set_foreground(self, node, color)
	VALUE self, node, color;
{
    gtk_ctree_node_set_foreground(GTK_CTREE(RVAL2GOBJ(self)),
                                  RVAL2CTREENODE(node),
                                  get_gdkcolor(color));
    return self;
}

/*
 * node_set_background(node, color)
 */
static VALUE
ctree_node_set_background(self, node, color)
	VALUE self, node, color;
{
    gtk_ctree_node_set_background(GTK_CTREE(RVAL2GOBJ(self)),
                                  RVAL2CTREENODE(node),
                                  get_gdkcolor(color));
    return self;
}

/*
 * node_set_row_data(node, data)
 *
 * Set the custom data associated with a node.
 *
 * arguments:
 *   node - tree node.
 *   data - custom data associated with a node.
 */
static VALUE
ctree_node_set_row_data(self, node, data)
	VALUE self, node, data;
{
    gtk_ctree_node_set_row_data(GTK_CTREE(RVAL2GOBJ(self)),
                                RVAL2CTREENODE(node),
                                (gpointer)data);
    return self;
}

/*
 * node_get_row_data(node)
 *
 * Get the custom data associated with a node.
 *
 * argument:
 *   node - tree node.
 */
static VALUE
ctree_node_get_row_data(self, node)
	VALUE self, node;
{
    return (VALUE)gtk_ctree_node_get_row_data(GTK_CTREE(RVAL2GOBJ(self)),
                                              RVAL2CTREENODE(node));
}

/*
 * node_moveto(node, column, row_align, col_align)
 *
 * Makes the given column of the given node visible by scrolling.
 *
 * arguments:
 *   node - The node to be made visible.
 *   column - The column to be made visible.
 *   row_align - Where in the window the row should appear.
 *   col_align - Where in the window the column should appear.
 */
static VALUE
ctree_node_moveto(self, node, column, row_align, col_align)
	VALUE self, node, column, row_align, col_align;
{
    gtk_ctree_node_moveto(GTK_CTREE(RVAL2GOBJ(self)),
                          RVAL2CTREENODE(node),
                          NUM2INT(column),
                          (gfloat)NUM2DBL(row_align),
                          (gfloat)NUM2DBL(col_align));
    return self;
}

/*
 * node_is_visible_p(node)
 *
 * Return true if the node is currently inside the bounds of the window.
 * Note that this method can return true even if the node is not viewable,
 * if the node's ancestor is visible. 
 */
static VALUE
ctree_node_is_visible_p(self, node)
	VALUE self, node;
{
    GtkVisibility result;
    result = gtk_ctree_node_is_visible(GTK_CTREE(RVAL2GOBJ(self)),
									   RVAL2CTREENODE(node));
    return result?Qtrue:Qnil; /* FIX2INT(result); */
}

/*
 * set_indent(indent)
 *
 * Set the number of pixels to shift the levels of the tree.
 */
static VALUE
ctree_set_indent(self, indent)
    VALUE self, indent;
{
    gtk_ctree_set_indent(GTK_CTREE(RVAL2GOBJ(self)), NUM2INT(indent));
    return self;   
}

/*
 * set_spacing(spacing)
 *
 * Set the spacing between the tree graphic and the actual node content.
 */
static VALUE
ctree_set_spacing(self, spacing)
    VALUE self, spacing;
{
    gtk_ctree_set_spacing(GTK_CTREE(RVAL2GOBJ(self)), NUM2INT(spacing));
    return self;   
}

/*
 * set_line_style(style)
 * line_style=style
 *
 * Set the line style.
 *
 * argument:
 *   style - The appearance of the lines in the tree graphics.
 *           Gtk::CTree::LINES_NONE   - No lines.
 *           Gtk::CTree::LINES_SOLID  - Solid lines.
 *           Gtk::CTree::LINES_DOTTED - Dotted lines.
 *           Gtk::CTree::LINES_TABBED - Tabbed lines.
 */
static VALUE
ctree_set_line_style(self, style)
	VALUE self, style;
{
    gtk_ctree_set_line_style(GTK_CTREE(RVAL2GOBJ(self)), NUM2INT(style));
    return self;
}

/*
 * line_style
 *
 * Return this tree's line style.
 */
static VALUE
ctree_get_line_style(self)
	VALUE self;
{
    return INT2FIX(GTK_CTREE(RVAL2GOBJ(self))->line_style);
}

static VALUE
ctree_set_expander_style(self, style)
	VALUE self, style;
{
    gtk_ctree_set_expander_style(GTK_CTREE(RVAL2GOBJ(self)), NUM2INT(style));
    return self;
}

#if 0
static VALUE
ctree_set_drag_compare_func(self, cmp_func)
{
}
#endif

/*
 * sort_node(node)
 *
 * Sort the children of a node.
 */
static VALUE
ctree_sort_node(self, node)
	VALUE self, node;
{
    gtk_ctree_sort_node(GTK_CTREE(RVAL2GOBJ(self)), RVAL2CTREENODE(node));
    return self;
}

/*
 * sort_recursive(node)
 *
 * Sort the descendants of a node.
 */
static VALUE
ctree_sort_recursive(self, node)
	VALUE self, node;
{
    gtk_ctree_sort_recursive(GTK_CTREE(RVAL2GOBJ(self)),
                             RVAL2CTREENODE(node));
    return self;
}

/*
 * node_nth(row)
 *
 * Return the node corresponding to the `row'th row.
 */
static VALUE
ctree_node_nth(self, row)
	VALUE self, row;
{
    GtkCTreeNode *result = gtk_ctree_node_nth(GTK_CTREE(RVAL2GOBJ(self)),
                                              NUM2INT(row));
    return GVAL2RVAL(result);
}

static VALUE
ctree_set_show_stub(self, show_stub)
    VALUE self, show_stub;
{
    gtk_ctree_set_show_stub(GTK_CTREE(RVAL2GOBJ(self)), RTEST(show_stub));
    return self;
}

/*
 * each_selection
 *
 * (Override)
 */
static VALUE
ctree_each_selection(self)
    VALUE self;
{
    GtkCList* clist;
    GList* sellist;

    clist = GTK_CLIST(RVAL2GOBJ(self));
    sellist = clist->selection;

    while (sellist) {
        GtkCTreeNode* node;
        node = GTK_CTREE_NODE(sellist->data);
        sellist = sellist->next;
        rb_yield(GVAL2RVAL(node));
    }

    return Qnil;
}

/*
 * tree_column
 *
 * The column in which the actual tree graphic appears.
 */
static VALUE
ctree_get_tree_column(self)
	VALUE self;
{
    return INT2NUM(GTK_CTREE(RVAL2GOBJ(self))->tree_column);
}

/* Gtk::CTreeNode */

static VALUE
ctreenode_get_next(self)
	VALUE self;
{
    return GVAL2RVAL(GTK_CTREE_NODE_NEXT(RVAL2CTREENODE(self)));
}

static VALUE
ctreenode_get_prev(self)
	VALUE self;
{
    return GVAL2RVAL(GTK_CTREE_NODE_PREV(RVAL2CTREENODE(self)));
}

static VALUE
ctreenode_get_parent(self)
	VALUE self;
{
    return GVAL2RVAL(GTK_CTREE_ROW(RVAL2CTREENODE(self))->parent);
}

static VALUE
ctreenode_get_sibling(self)
	VALUE self;
{
    GtkCTreeNode *result = GTK_CTREE_ROW(RVAL2CTREENODE(self))->sibling;
    return GVAL2RVAL(result);
}

static VALUE
ctreenode_get_children(self)
	VALUE self;
{
    GtkCTreeNode *result = GTK_CTREE_ROW(RVAL2CTREENODE(self))->children;
    return GVAL2RVAL(result);
}

static VALUE
ctreenode_is_leaf(self)
	VALUE self;
{
    GtkCTreeNode* node = RVAL2CTREENODE(self);
    return (GTK_CTREE_ROW(node)->is_leaf)?Qtrue:Qfalse;
}

static VALUE
ctreenode_expanded(self)
	VALUE self;
{
    GtkCTreeNode* node = RVAL2CTREENODE(self);
    return (GTK_CTREE_ROW(node)->expanded)?Qtrue:Qfalse;
}

#ifdef hoge
GValue*
rbobj_get(obj)
    VALUE obj;
{
    VALUE data;
    GValue* ret;

    if (NIL_P(obj)) { 
		rb_raise(rb_eTypeError, "wrong argument type nil");
    }

    Check_Type(obj, T_OBJECT);
    data = rb_ivar_get(obj, id_gobject_data);

    /* if (NIL_P(data) || data->dmark != gobj_mark) { */
    if (NIL_P(data)) {
		rb_raise(rb_eTypeError, "not a Glib::GObject");
    }

    Data_Get_Struct(data, GObject, result);
    if (!ret)
		rb_raise(rb_eArgError, "destroyed GLib::GObject");
    return G_OBJECT(ret);
}

VALUE
_ctree_node_to_ruby(node)
    GtkCTreeNode* node;
{
	VALUE ret;

    if (!node) return Qnil;
    ret = (VALUE)g_value_get_pointer(node);
    if (! ret){
		ret = rb_obj_alloc(rbgobj_lookup_rbclass(node));
		rbobj_initialize(ret, (GValue*)node);
	}
    return ret;
}

static void
_ctree_node_from_ruby(from, to)
	VALUE from;
	GValue* to;
{
    GtkCTreeNode* c_node;

    if (NIL_P(from)) return NULL;
    Data_Get_Struct(from, GtkCTreeNode, c_node);
	g_value_set_pointer(to, c_node);
}
#endif

void 
Init_gtk_ctree()
{
    VALUE gCTree = G_DEF_CLASS2(GTK_TYPE_CTREE, "CTree", mGtk, ctree_mark, 0);

    VALUE gCTreeNode = rb_define_class_under(mGtk, "CTreeNode", rb_cData);
/*
    rbgobj_register_r2g_func(GTK_TYPE_CTREE_NODE, _ctree_node_from_ruby);
    rbgobj_register_g2r_func(GTK_TYPE_CTREE_NODE, _ctree_node_to_ruby);
*/
 
    /* constants */

    /*
     * GtkCTreePos
     *   A value specifying the position of a new node relative to an old one.
     *
     *   Gtk::CTree::POS_BEFORE   - As a sibling, before the specified node.
     *   Gtk::CTree::POS_AS_CHILD - As a child of the specified node.
     *   Gtk::CTree::POS_AFTER    - As a sibling, after the specified node.
     */
    rb_define_const(gCTree, "POS_BEFORE", INT2FIX(GTK_CTREE_POS_BEFORE));
    rb_define_const(gCTree, "POS_AS_CHILD", INT2FIX(GTK_CTREE_POS_AS_CHILD));
    rb_define_const(gCTree, "POS_AFTER", INT2FIX(GTK_CTREE_POS_AFTER));

    /* GtkCTreeLineStyle */
    rb_define_const(gCTree, "LINES_NONE", INT2FIX(GTK_CTREE_LINES_NONE));
    rb_define_const(gCTree, "LINES_SOLID", INT2FIX(GTK_CTREE_LINES_SOLID));
    rb_define_const(gCTree, "LINES_DOTTED", INT2FIX(GTK_CTREE_LINES_DOTTED));
    rb_define_const(gCTree, "LINES_TABBED", INT2FIX(GTK_CTREE_LINES_TABBED));

    /*
     * GtkCTreeExpanderStyle
     *   The appearance of the expander buttons, i.e. the small buttons
     *   which expand or contract parts of the tree when pressed.
     *
     *   Gtk::CTree::EXPANDER_NONE     - No expanders.
     *   Gtk::CTree::EXPANDER_SQUARE   - Square expanders.
     *   Gtk::CTree::EXPANDER_TRIANGLE - Triangular expanders.
     *   Gtk::CTree::EXPANDER_CIRCULAR - Round expanders.
     */
    rb_define_const(gCTree, "EXPANDER_NONE", INT2FIX(GTK_CTREE_EXPANDER_NONE));
    rb_define_const(gCTree, "EXPANDER_SQUARE", INT2FIX(GTK_CTREE_EXPANDER_SQUARE));
    rb_define_const(gCTree, "EXPANDER_TRIANGLE", INT2FIX(GTK_CTREE_EXPANDER_TRIANGLE));
    rb_define_const(gCTree, "EXPANDER_CIRCULAR", INT2FIX(GTK_CTREE_EXPANDER_CIRCULAR));

    /*
     * GtkCTreeExpansionType
     *   How to expand or collapse a part of a tree.
     *
     *   Gtk::CTree::EXPANSION_EXPAND
     *     - Expand this node.
     *   Gtk::CTree::EXPANSION_EXPAND_RECURSIVE
     *     - Expand this node and everything below it in the hierarchy.
     *   Gtk::CTree::EXPANSION_COLLAPSE
     *     - Collapse this node.
     *   Gtk::CTree::COLLAPSE_RECURSIVE
     *     - Collapse this node and everything below it in the hierarchy.
     *   Gtk::CTree::EXPANSION_TOGGLE
     *     - Toggle this node (i.e. expand if collapsed and vice versa).
     *   Gtk::CTree::EXPANSION_TOGGLE_RECURSIVE
     *     - Toggle this node and everything below it in the hierarchy.
     */
    rb_define_const(gCTree, "EXPANSION_EXPAND", INT2FIX(GTK_CTREE_EXPANSION_EXPAND));
    rb_define_const(gCTree, "EXPANSION_EXPAND_RECURSIVE", INT2FIX(GTK_CTREE_EXPANSION_EXPAND_RECURSIVE));
    rb_define_const(gCTree, "EXPANSION_COLLAPSE", INT2FIX(GTK_CTREE_EXPANSION_COLLAPSE));
    rb_define_const(gCTree, "EXPANSION_COLLAPSE_RECURSIVE", INT2FIX(GTK_CTREE_EXPANSION_COLLAPSE_RECURSIVE));
    rb_define_const(gCTree, "EXPANSION_TOGGLE", INT2FIX(GTK_CTREE_EXPANSION_TOGGLE));
    rb_define_const(gCTree, "EXPANSION_TOGGLE_RECURSIVE", INT2FIX(GTK_CTREE_EXPANSION_TOGGLE_RECURSIVE));

    /* instance methods */
    rb_define_method(gCTree, "initialize", ctree_initialize, 2);
    rb_define_method(gCTree, "insert_node", ctree_insert_node, 10);
    rb_define_method(gCTree, "remove_node", ctree_remove_node, 1);
    rb_define_method(gCTree, "insert_gnode", ctree_insert_gnode, 5);
    rb_define_method(gCTree, "export_to_gnode", ctree_export_to_gnode, 5);
    rb_define_method(gCTree, "post_recursive", ctree_post_recursive, 1);
    rb_define_method(gCTree, "post_recursive_to_depth", ctree_post_recursive_to_depth, 2);
    rb_define_method(gCTree, "pre_recursive", ctree_pre_recursive, 1);
    rb_define_method(gCTree, "pre_recursive_to_depth", ctree_pre_recursive_to_depth, 2);
    rb_define_method(gCTree, "is_viewable?", ctree_is_viewable_p, 1);
    rb_define_method(gCTree, "last", ctree_last, 1);
    rb_define_method(gCTree, "find", ctree_find, 2);
    rb_define_method(gCTree, "is_ancestor?", ctree_is_ancestor_p, 2);
#if 0
    rb_define_method(gCTree, "find_by_row_data", ctree_find_by_row_dara, 2);
    rb_define_method(gCTree, "find_all_by_row_data", ctree_find_all_by_row_dara, 2);
#endif
    rb_define_method(gCTree, "is_hot_spot?", ctree_is_hot_spot_p, 2);
    rb_define_method(gCTree, "move", ctree_move, 3);
    rb_define_method(gCTree, "expand", ctree_expand, 1);
    rb_define_method(gCTree, "expand_recursive", ctree_expand_recursive, 1);
    rb_define_method(gCTree, "expand_to_depth", ctree_expand_to_depth, 2);
    rb_define_method(gCTree, "collapse", ctree_collapse, 1);
    rb_define_method(gCTree, "collapse_recursive", ctree_collapse_recursive, 1);
    rb_define_method(gCTree, "collapse_to_depth", ctree_collapse_to_depth, 2);
    rb_define_method(gCTree, "toggle_expansion", ctree_toggle_expansion, 1);
    rb_define_method(gCTree, "toggle_expansion_recursive", ctree_toggle_expansion_recursive, 1);
    rb_define_method(gCTree, "select", ctree_select, 1);
    rb_define_method(gCTree, "select_recursive", ctree_select_recursive, 1);
    rb_define_method(gCTree, "unselect", ctree_unselect, 1);
    rb_define_method(gCTree, "unselect_recursive", ctree_unselect_recursive, 1);
    rb_define_method(gCTree, "node_set_text", ctree_node_set_text, 3);
    rb_define_method(gCTree, "node_set_pixmap", ctree_node_set_pixmap, 4);
    rb_define_method(gCTree, "node_set_pixtext", ctree_node_set_pixtext, 6);
    rb_define_method(gCTree, "set_node_info", ctree_set_node_info, 9);
    rb_define_method(gCTree, "node_set_shift", ctree_node_set_shift, 4);
    rb_define_method(gCTree, "node_set_selectable", ctree_node_set_selectable, 2);
    rb_define_method(gCTree, "node_get_selectable", ctree_node_get_selectable, 1);
    rb_define_method(gCTree, "node_get_cell_type", ctree_node_get_cell_type, 2);
    rb_define_method(gCTree, "node_get_text", ctree_node_get_text, 2);
    rb_define_method(gCTree, "node_get_pixmap", ctree_node_get_pixmap, 2);
    rb_define_method(gCTree, "node_get_pixtext", ctree_node_get_pixtext, 2);
    rb_define_method(gCTree, "get_node_info", ctree_get_node_info, 1);
    rb_define_method(gCTree, "node_set_row_style", ctree_node_set_row_style, 2);
    rb_define_method(gCTree, "node_get_row_style", ctree_node_get_row_style, 1);
    rb_define_method(gCTree, "node_set_cell_style", ctree_node_set_cell_style, 3);
    rb_define_method(gCTree, "node_get_cell_style", ctree_node_get_cell_style, 2);
    rb_define_method(gCTree, "node_set_foreground", ctree_node_set_foreground, 2);
    rb_define_method(gCTree, "node_set_background", ctree_node_set_background, 2);
    rb_define_method(gCTree, "node_set_row_data", ctree_node_set_row_data, 2);
    rb_define_method(gCTree, "node_get_row_data", ctree_node_get_row_data, 1);
    rb_define_method(gCTree, "node_moveto", ctree_node_moveto, 4);
    rb_define_method(gCTree, "node_is_visible?", ctree_node_is_visible_p, 1);
    rb_define_method(gCTree, "set_indent", ctree_set_indent, 1);
    rb_define_method(gCTree, "set_spacing", ctree_set_spacing, 1);
    rb_define_method(gCTree, "set_line_style", ctree_set_line_style, 1);
    rb_define_method(gCTree, "line_style=", ctree_set_line_style, 1);
    rb_define_method(gCTree, "line_style", ctree_get_line_style, 0);
    rb_define_method(gCTree, "set_expander_style", ctree_set_expander_style, 1);
#if 0
    rb_define_method(gCTree, "set_drag_compare_func", ctree_drag_compare_func, 1);
#endif
    rb_define_method(gCTree, "sort_node", ctree_sort_node, 1);
    rb_define_method(gCTree, "sort_recursive", ctree_sort_recursive, 1);
    rb_define_method(gCTree, "node_nth", ctree_node_nth, 1);
    rb_define_method(gCTree, "[]", ctree_node_nth, 1);
    rb_define_method(gCTree, "set_show_stub", ctree_set_show_stub, 1);
    rb_define_method(gCTree, "each_selection", ctree_each_selection, 0);
    rb_define_method(gCTree, "tree_column", ctree_get_tree_column, 0);

    rb_define_method(gCTreeNode, "next", ctreenode_get_next, 0);
    rb_define_method(gCTreeNode, "prev", ctreenode_get_prev, 0);
    rb_define_method(gCTreeNode, "parent", ctreenode_get_parent, 0);
    rb_define_method(gCTreeNode, "sibling", ctreenode_get_sibling, 0);
    rb_define_method(gCTreeNode, "children", ctreenode_get_children, 0);
    rb_define_method(gCTreeNode, "leaf?", ctreenode_is_leaf, 0);
    rb_define_method(gCTreeNode, "expanded?", ctreenode_expanded, 0);
}

#endif
