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

#define _GNU_SOURCE

#include "util.h"

BRISK_BEGIN_PEDANTIC
#include "menu-private.h"
#include <gtk/gtk.h>
BRISK_END_PEDANTIC

static gboolean brisk_menu_window_logout_real(BriskMenuWindow *self)
{
        if (!self->session) {
                return FALSE;
        }

        gnome_session_manager_call_logout_sync(self->session, 0, NULL, NULL);
        return FALSE;
}

/**
 * Handle log out
 */
static void brisk_menu_window_logout(BriskMenuWindow *self, __brisk_unused__ gpointer v)
{
        gtk_widget_hide(GTK_WIDGET(self));

        g_idle_add((GSourceFunc)brisk_menu_window_logout_real, self);
}

static inline gboolean brisk_menu_window_shutdown_real(BriskMenuWindow *self)
{
        if (!self->session) {
                return FALSE;
        }

        gnome_session_manager_call_shutdown_sync(self->session, NULL, NULL);
        return FALSE;
}

/**
 * Handle shut down
 */
static void brisk_menu_window_shutdown(BriskMenuWindow *self, __brisk_unused__ gpointer v)
{
        gtk_widget_hide(GTK_WIDGET(self));
        g_idle_add((GSourceFunc)brisk_menu_window_shutdown_real, self);
}

/**
 * Create the graphical buttons for session control
 */
static void brisk_menu_window_setup_session_controls(BriskMenuWindow *self)
{
        GtkWidget *widget = NULL;
        GtkWidget *box = NULL;
        GtkStyleContext *style = NULL;

        box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_widget_set_margin_bottom(box, 4);
        style = gtk_widget_get_style_context(box);
        gtk_style_context_add_class(style, GTK_STYLE_CLASS_LINKED);

        gtk_box_pack_end(GTK_BOX(self->sidebar_wrap), box, FALSE, FALSE, 0);
        gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

        /* Logout */
        widget = gtk_button_new_from_icon_name("brisk_system-log-out-symbolic", GTK_ICON_SIZE_MENU);
        g_signal_connect_swapped(widget, "clicked", G_CALLBACK(brisk_menu_window_logout), self);
        gtk_widget_set_tooltip_text(widget, "End the current session");
        gtk_widget_set_can_focus(widget, FALSE);
        gtk_container_add(GTK_CONTAINER(box), widget);

        /* Lock */
        widget = gtk_button_new_from_icon_name("system-lock-screen-symbolic",
                                               GTK_ICON_SIZE_SMALL_TOOLBAR);
        gtk_widget_set_tooltip_text(widget, "Lock the screen");
        gtk_widget_set_can_focus(widget, FALSE);
        gtk_container_add(GTK_CONTAINER(box), widget);

        /* Shutdown */
        widget =
            gtk_button_new_from_icon_name("system-shutdown-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
        g_signal_connect_swapped(widget, "clicked", G_CALLBACK(brisk_menu_window_shutdown), self);
        gtk_widget_set_tooltip_text(widget, "Turn off the device");
        gtk_widget_set_can_focus(widget, FALSE);
        gtk_container_add(GTK_CONTAINER(box), widget);
}

void brisk_menu_window_setup_session(BriskMenuWindow *self)
{
        autofree(GError) *error = NULL;
        brisk_menu_window_setup_session_controls(self);

        /* Sort out gnome-session dbus */
        self->session = gnome_session_manager_proxy_new_for_bus_sync(
            G_BUS_TYPE_SESSION,
            G_DBUS_PROXY_FLAGS_DO_NOT_AUTO_START | G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES,
            "org.gnome.SessionManager",
            "/org/gnome/SessionManager",
            NULL,
            &error);
        if (error) {
                g_warning("Failed to contact org.gnome.SessionManager: %s\n", error->message);
                return;
        }
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
