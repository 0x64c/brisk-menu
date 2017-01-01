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
BRISK_END_PEDANTIC

void brisk_menu_window_init_settings(BriskMenuWindow *self)
{
        GtkSettings *gtk_settings = NULL;

        self->settings = g_settings_new("com.solus-project.brisk-menu");

        gtk_settings = gtk_settings_get_default();

        /* Make dark-theme key work */
        g_settings_bind(self->settings,
                        "dark-theme",
                        gtk_settings,
                        "gtk-application-prefer-dark-theme",
                        G_SETTINGS_BIND_DEFAULT);
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
