/*
 * This file is part of mate-solmenu.
 *
 * Copyright © 2016 Ikey Doherty <ikey@solus-project.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>
#include <matemenu-tree.h>

G_BEGIN_DECLS

typedef struct _SolMenuCategoryButton SolMenuCategoryButton;
typedef struct _SolMenuCategoryButtonClass SolMenuCategoryButtonClass;

#define SOL_TYPE_MENU_CATEGORY_BUTTON sol_menu_category_button_get_type()
#define SOL_MENU_CATEGORY_BUTTON(o)                                                                \
        (G_TYPE_CHECK_INSTANCE_CAST((o), SOL_TYPE_MENU_CATEGORY_BUTTON, SolMenuCategoryButton))
#define SOL_IS_MENU_CATEGORY_BUTTON(o)                                                             \
        (G_TYPE_CHECK_INSTANCE_TYPE((o), SOL_TYPE_MENU_CATEGORY_BUTTON))
#define SOL_MENU_CATEGORY_BUTTON_CLASS(o)                                                          \
        (G_TYPE_CHECK_CLASS_CAST((o), SOL_TYPE_MENU_CATEGORY_BUTTON, SolMenuCategoryButtonClass))
#define SOL_IS_MENU_CATEGORY_BUTTON_CLASS(o)                                                       \
        (G_TYPE_CHECK_CLASS_TYPE((o), SOL_TYPE_MENU_CATEGORY_BUTTON))
#define SOL_MENU_CATEGORY_BUTTON_GET_CLASS(o)                                                      \
        (G_TYPE_INSTANCE_GET_CLASS((o), SOL_TYPE_MENU_CATEGORY_BUTTON, SolMenuCategoryButtonClass))

/**
 * Construct a new SolMenuCategoryButton from the given group
 */
GtkWidget *sol_menu_category_button_new(MateMenuTreeDirectory *group);

GType sol_menu_category_button_get_type(void);

G_END_DECLS

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */
