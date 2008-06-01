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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "weather-common.h"

#ifdef OS2008
 typedef struct HildonHomeAreaPriv_
{
  gboolean      layout_changed;

  gboolean      snap_to_grid;

  gint          applet_padding;

  GHashTable   *layout;

  GList        *children_data;

  gdouble       default_alpha;

  Picture       picture;
  gint          x_offset;
  gint          y_offset;

} HildonHomeAreaPriv;

typedef struct
{
  GtkWidget    *widget;
  Window        window;
  gint          x;
  gint          y;
  gint          stack_index;
  gulong        realize_handler;
  gint          mask_width, mask_height;
  Damage        damage;
  Picture       picture;
  Picture       alpha_mask;
  Picture       alpha_mask_unscaled;
  gint          background_width, background_height;
  gint          state;
  guint         style_set_handler,
                size_allocate_handler,
                state_change_handler;
} ChildData;


#define HILDON_HOME_AREA_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), HILDON_TYPE_HOME_AREA, HildonHomeAreaPriv));

gboolean hildon_home_area_expose (GtkWidget *widget,GdkEventExpose *event, ChildData *current_child_data);
gboolean moving_resizing (GtkWidget *widget);
ChildData * hildon_home_area_get_child_data (HildonHomeArea *area, GtkWidget      *child);
void get_x_y_hildon_home_area(ChildData *data, int *x, int *y);
#endif
