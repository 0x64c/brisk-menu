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

#include "util.h"

#include <stdlib.h>

SOLUS_BEGIN_PEDANTIC
#include "menu-private.h"
#include <gtk/gtk.h>
#include <string.h>
SOLUS_END_PEDANTIC

/**
 * sol_menu_window_clear_search:
 *
 * Simply put, resets the active search term
 */
void sol_menu_window_clear_search(GtkEntry *entry, GtkEntryIconPosition pos,
                                  __solus_unused__ GdkEvent *event, __solus_unused__ gpointer v)
{
        if (pos != GTK_ENTRY_ICON_SECONDARY) {
                return;
        }
        gtk_entry_set_text(entry, "");
}

/**
 * sol_menu_window_search:
 *
 * Callback for the text entry changing. Set the search term and force
 * an invalidation of our filters.
 */
void sol_menu_window_search(SolMenuWindow *self, GtkEntry *entry)
{
        const gchar *search_term = NULL;

        /* Remove old search term */
        search_term = gtk_entry_get_text(entry);
        g_clear_pointer(&self->search_term, g_free);

        /* New search term, always lower case for simplicity */
        self->search_term = g_strstrip(g_ascii_strdown(search_term, -1));

        /* Reset our search term if it's not valid anymore, or whitespace */
        if (strlen(self->search_term) > 0) {
                gtk_widget_set_sensitive(self->sidebar, FALSE);
        } else {
                gtk_widget_set_sensitive(self->sidebar, TRUE);
                g_clear_pointer(&self->search_term, g_free);
        }

        /* Now filter again */
        gtk_list_box_invalidate_filter(GTK_LIST_BOX(self->apps));
}

/**
 * sol_menu_window_filter_group:
 *
 * This function will handle filtering the selection based on the active
 * group, when no search term is applied.
 *
 * Returning TRUE means the app should be displayed
 */
static gboolean sol_menu_window_filter_group(SolMenuWindow *self, MateMenuTreeEntry *entry)
{
        MateMenuTreeDirectory *parent = NULL;

        /* All visible */
        if (!self->active_group) {
                return TRUE;
        }

        parent = matemenu_tree_item_get_parent(MATEMENU_TREE_ITEM(entry));

        /* Check if it matches the current group */
        if (self->active_group != parent) {
                return FALSE;
        }
        return TRUE;
}

/**
 * sol_menu_window_filter_term:
 *
 * This function will handle filtering the selection based on the active search
 * term. It looks for the string within a number of the entry's fields, and will
 * hide them if they don't turn up.
 *
 * Note: This function uses g_str_match_string so that ASCII alternatives are
 * searched. This allows searching for text containing accents, etc, so that
 * the menu can be more useful in more locales.
 *
 * This could probably be improved in future to generate internal state to allow
 * the search itself to be sorted based on the results, with the "most similar"
 * appearing near the top.
 */
static gboolean sol_menu_window_filter_term(SolMenuWindow *self, MateMenuTreeEntry *entry)
{
        const gchar *fields[] = {
                matemenu_tree_entry_get_display_name(entry),
                matemenu_tree_entry_get_comment(entry),
                matemenu_tree_entry_get_name(entry),
                matemenu_tree_entry_get_exec(entry),
        };

        for (size_t i = 0; i < sizeof(fields) / sizeof(fields[0]); i++) {
                if (!fields[i]) {
                        continue;
                }
                autofree(gchar) *contents = g_strstrip(g_ascii_strdown(fields[i], -1));
                if (g_str_match_string(self->search_term, contents, TRUE)) {
                        return TRUE;
                }
        }
        return FALSE;
}

/**
 * sol_menu_window_filter_apps:
 *
 * Responsible for filtering the selection based on active group or search
 * term.
 */
gboolean sol_menu_window_filter_apps(GtkListBoxRow *row, gpointer v)
{
        SolMenuWindow *self = NULL;
        MateMenuTreeEntry *entry = NULL;
        GtkWidget *child = NULL;

        self = SOL_MENU_WINDOW(v);

        /* Grab our Entry widget */
        child = gtk_bin_get_child(GTK_BIN(row));

        g_object_get(child, "entry", &entry, NULL);
        if (!entry) {
                return FALSE;
        }

        /* Have search term? Filter on that. */
        if (self->search_term) {
                return sol_menu_window_filter_term(self, entry);
        }

        /* Filter based on group */
        return sol_menu_window_filter_group(self, entry);
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
