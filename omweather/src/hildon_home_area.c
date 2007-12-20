/*
 * This file is part of hildon-desktop

 * Copyright (C) 2006, 2007 Nokia Corporation.

 * Contact: Karoliina Salminen <karoliina.t.salminen@nokia.com>
 * Author:  Johan Bilien <johan.bilien@nokia.com>

 * Copyright (C) 2007 Vlad Vasiliev - the adapting for OMWeather
 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.

 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA

 */

#include "hildon_home_area.h"

#if HILDON == 1



static gint
find_by_widget (ChildData *data, GtkWidget *widget)
{
  return !(data->widget == widget);
}

ChildData *
hildon_home_area_get_child_data (HildonHomeArea *area,
                                 GtkWidget      *child)
{
  HildonHomeAreaPriv   *priv = HILDON_HOME_AREA_GET_PRIVATE (area);
  GList                *list_item;

  list_item = g_list_find_custom (priv->children_data,
                                  child,
                                  (GCompareFunc)find_by_widget);

  if (list_item)
    return (ChildData *)list_item->data;
  else
    return NULL;
}
void get_x_y_hildon_home_area(ChildData *data, int *x, int *y)
{
    *x = data->x + 80 ;
    *y = data->y + 60 ; /* HILDON_HOME_TITLEBAR_HEIGHT = 60 */
}
static void
hildon_home_area_child_compose (ChildData *child_data,
                                HildonHomeArea *area)
{
  HildonHomeAreaPriv   *priv;
  GtkAllocation        *alloc;

  priv = HILDON_HOME_AREA_GET_PRIVATE (area);

  if (child_data->picture == None)
    return;

  alloc = &(child_data->widget->allocation);

  g_return_if_fail (priv->picture != None);

  XRenderComposite (GDK_DISPLAY (),
                    (child_data->state == 0)?PictOpOver:PictOpSrc,
                    child_data->picture,
                    (child_data->state == 0)?child_data->alpha_mask:None,
                    priv->picture,
                    0,
                    0,
                    0,
                    0,
                    alloc->x - priv->x_offset,
                    alloc->y - priv->y_offset,
                    alloc->width,
                    alloc->height);
}


static gint
find_by_window (ChildData *data, Window *w)
{
  return !(data->window == *w);
}


gboolean
moving_resizing (GtkWidget *widget)
{

    HildonHomeAreaPriv *priv;

    priv = HILDON_HOME_AREA_GET_PRIVATE (widget);

    if (GTK_WIDGET_DRAWABLE (widget))
    {
      Window                   *wchildren, root, parent;
      guint                     n_children, i;
     
      gdk_error_trap_push ();
      XQueryTree (GDK_DISPLAY (),
                  GDK_WINDOW_XID (widget->window),
                  &root,
                  &parent,
                  &wchildren,
                  &n_children);
      if (gdk_error_trap_pop ())
        {
          if (wchildren)
            XFree (wchildren);
          return FALSE;
        }
     ChildData *data;
      for (i = 0 ; i < n_children; i++)
        {
          GList        *l = NULL;

          l = g_list_find_custom (priv->children_data,
                                  &wchildren[i],
                                  (GCompareFunc)find_by_window);
            if (l){ 
	    	data=l->data;
		if (data->state != 0){
		    XFree (wchildren); 
		    return TRUE;
		}    
	   }						    
        }
      XFree (wchildren);
    }
  return FALSE;
}


gboolean
hildon_home_area_expose (GtkWidget *widget,
                         GdkEventExpose *event,
			 ChildData *current_child_data)
{

    HildonHomeAreaPriv *priv;
    GError *error;
    error = NULL;
    GdkPixbuf *pixbuf;
    pixbuf = NULL;

    priv = HILDON_HOME_AREA_GET_PRIVATE (widget);

    if (GTK_WIDGET_DRAWABLE (widget))
    {
      XRectangle                rectangle;
      XserverRegion             region;
      GdkDrawable              *drawable;
      Window                   *wchildren, root, parent;
      guint                     n_children, i;



      gdk_window_get_internal_paint_info (widget->window,
                                          &drawable,
                                          &priv->x_offset,
                                          &priv->y_offset);
      rectangle.x = event->area.x - priv->x_offset;
      rectangle.y = event->area.y - priv->y_offset;
      rectangle.width = event->area.width;
      rectangle.height = event->area.height;

     
      priv->picture = hildon_desktop_picture_from_drawable (drawable);

      region = XFixesCreateRegion (GDK_DISPLAY (),
                                   &rectangle,
                                   1);

      gdk_error_trap_push ();
      XQueryTree (GDK_DISPLAY (),
                  GDK_WINDOW_XID (widget->window),
                  &root,
                  &parent,
                  &wchildren,
                  &n_children);
      if (gdk_error_trap_pop ())
        {
          if (wchildren)
            XFree (wchildren);

          XRenderFreePicture (GDK_DISPLAY (),
                              priv->picture);
          XFixesDestroyRegion (GDK_DISPLAY (),
                               region);
          return FALSE;
        }


      for (i = 0 ; i < n_children; i++)
        {
          GList        *l = NULL;

	
          l = g_list_find_custom (priv->children_data,
                                  &wchildren[i],
                                  (GCompareFunc)find_by_window);
	    if (l && (current_child_data ==  l->data))
		break;
            if (l){
        	hildon_home_area_child_compose (l->data,
                                            HILDON_HOME_AREA (widget));
	 
	   }	
					    
        }

      XFree (wchildren);

      XFixesDestroyRegion (GDK_DISPLAY (), region);
      XRenderFreePicture (GDK_DISPLAY (), priv->picture);
    }

  return TRUE;
}
#endif
