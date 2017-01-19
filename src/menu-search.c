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
#include "menu-private.h"
#include <gio/gdesktopappinfo.h>
#include <gtk/gtk.h>
#include <string.h>
BRISK_END_PEDANTIC

/**
 * Update sensitivity - ignoring non checkboxes
 */
static void brisk_menu_set_categories_sensitive(BriskMenuWindow *self, gboolean sensitive)
{
        autofree(GList) *kids = NULL;
        GList *elem = NULL;

        kids = gtk_container_get_children(GTK_CONTAINER(self->sidebar));
        for (elem = kids; elem; elem = elem->next) {
                GtkWidget *kid = elem->data;
                if (!GTK_IS_CHECK_BUTTON(kid)) {
                        continue;
                }
                gtk_widget_set_sensitive(kid, sensitive);
        }
}

/**
 * brisk_menu_window_clear_search:
 *
 * Simply put, resets the active search term
 */
void brisk_menu_window_clear_search(GtkEntry *entry, GtkEntryIconPosition pos,
                                    __brisk_unused__ GdkEvent *event, __brisk_unused__ gpointer v)
{
        if (pos != GTK_ENTRY_ICON_SECONDARY) {
                return;
        }
        gtk_entry_set_text(entry, "");
}

/**
 * brisk_menu_window_search:
 *
 * Callback for the text entry changing. Set the search term and force
 * an invalidation of our filters.
 */
void brisk_menu_window_search(BriskMenuWindow *self, GtkEntry *entry)
{
        const gchar *search_term = NULL;

        if (!self->filtering) {
                return;
        }

        /* Remove old search term */
        search_term = gtk_entry_get_text(entry);
        g_clear_pointer(&self->search_term, g_free);

        /* New search term, always lower case for simplicity */
        self->search_term = g_strstrip(g_ascii_strdown(search_term, -1));

        /* Reset our search term if it's not valid anymore, or whitespace */
        if (strlen(self->search_term) > 0) {
                brisk_menu_set_categories_sensitive(self, FALSE);
        } else {
                brisk_menu_set_categories_sensitive(self, TRUE);
                g_clear_pointer(&self->search_term, g_free);
        }

        /* Now filter again */
        gtk_list_box_invalidate_filter(GTK_LIST_BOX(self->apps));
        gtk_list_box_invalidate_sort(GTK_LIST_BOX(self->apps));
}

/**
 * brisk_menu_window_filter_group:
 *
 * This function will handle filtering the selection based on the active
 * group, when no search term is applied.
 *
 * Returning TRUE means the app should be displayed
 */
__brisk_pure__ static gboolean brisk_menu_window_filter_group(BriskMenuWindow *self,
                                                              MateMenuTreeEntry *entry)
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

__brisk_pure__ static gboolean brisk_menu_array_contains(const gchar **fields, size_t n_fields,
                                                         const gchar *term)
{
        for (size_t i = 0; i < n_fields; i++) {
                if (!fields[i]) {
                        continue;
                }
                autofree(gchar) *contents = g_strstrip(g_ascii_strdown(fields[i], -1));
                if (g_str_match_string(term, contents, TRUE)) {
                        return TRUE;
                }
                if (strstr(contents, term)) {
                        return TRUE;
                }
        }
        return FALSE;
}

/**
 * brisk_menu_window_filter_term:
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
__brisk_pure__ static gboolean brisk_menu_window_filter_term(BriskMenuWindow *self, GAppInfo *info)
{
        const gchar *fields[] = {
                g_app_info_get_display_name(info),
                g_app_info_get_description(info),
                g_app_info_get_name(info),
                g_app_info_get_executable(info),
        };
        const gchar *const *keywords = g_desktop_app_info_get_keywords(G_DESKTOP_APP_INFO(info));

        if (brisk_menu_array_contains(fields, G_N_ELEMENTS(fields), self->search_term)) {
                return TRUE;
        }

        if (!keywords) {
                return FALSE;
        }

        return brisk_menu_array_contains((const gchar **)keywords,
                                         g_strv_length((gchar **)keywords),
                                         self->search_term);
}

/**
 * brisk_menu_window_filter_apps:
 *
 * Responsible for filtering the selection based on active group or search
 * term.
 */
__brisk_pure__ gboolean brisk_menu_window_filter_apps(GtkListBoxRow *row, gpointer v)
{
        BriskMenuWindow *self = NULL;
        MateMenuTreeEntry *entry = NULL;
        GAppInfo *info = NULL;
        GtkWidget *child = NULL;
        MateMenuTree *childTree = NULL;
        const gchar *desktop_id = NULL;
        GtkWidget *compare_child = NULL;

        self = BRISK_MENU_WINDOW(v);

        if (!self->filtering) {
                return FALSE;
        }

        /* Grab our Entry widget */
        child = gtk_bin_get_child(GTK_BIN(row));

        g_object_get(child, "entry", &entry, "tree", &childTree, "info", &info, NULL);
        if (!entry) {
                return FALSE;
        }

        /* Desktop ID's are unique, so the last entry added for an ID is the
         * button we want to display. Basically, a button can be duplicated and
         * appear in multiple categories. By keeping a unique ID -> button mapping,
         * we ensure we only ever show it once in the search function.
         */
        desktop_id = g_app_info_get_id(info);
        if (desktop_id) {
                compare_child = g_hash_table_lookup(self->desktop_store, desktop_id);
                if (compare_child && compare_child != child) {
                        return FALSE;
                }
        }

        /* Do they belong to the same tree ? */
        if (self->active_tree && self->active_tree != childTree) {
                return FALSE;
        }

        /* Have search term? Filter on that. */
        if (self->search_term) {
                return brisk_menu_window_filter_term(self, info);
        }

        /* Filter based on group */
        return brisk_menu_window_filter_group(self, entry);
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
