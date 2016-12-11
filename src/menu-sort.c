/*
 * This file is part of brisk-menu.
 *
 * Copyright © 2016 Ikey Doherty <ikey@solus-project.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "util.h"

#include <stdlib.h>

BRISK_BEGIN_PEDANTIC
#include "entry-button.h"
#include "menu-private.h"
#include <matemenu-tree.h>
BRISK_END_PEDANTIC

gint brisk_menu_window_sort(GtkListBoxRow *row1, GtkListBoxRow *row2, __brisk_unused__ gpointer v)
{
        GtkWidget *child1, *child2 = NULL;
        MateMenuTreeEntry *entryA, *entryB = NULL;
        const gchar *nameA, *nameB = NULL;

        child1 = gtk_bin_get_child(GTK_BIN(row1));
        child2 = gtk_bin_get_child(GTK_BIN(row2));

        g_object_get(child1, "entry", &entryA, NULL);
        g_object_get(child2, "entry", &entryB, NULL);

        nameA = matemenu_tree_entry_get_display_name(entryA);
        nameB = matemenu_tree_entry_get_display_name(entryB);

        /* TODO: Get score for the search term. */
        return g_strcmp0(nameA, nameB);
}

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
