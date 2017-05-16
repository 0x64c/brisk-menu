/*
 * This file is part of brisk-menu.
 *
 * Copyright © 2017 Ikey Doherty <ikey@solus-project.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _BriskBackend BriskBackend;
typedef struct _BriskBackendClass BriskBackendClass;

struct _BriskBackendClass {
        GObjectClass parent_class;
};

/**
 * BriskBackend is an abstract top-level class which is used as the base
 * of all other backend implementations within Brisk.
 */
struct _BriskBackend {
        GObject parent;

        /* TODO: Add vfuncs */
        gpointer padding[12];
};

#define BRISK_TYPE_BACKEND brisk_backend_get_type()
#define BRISK_BACKEND(o) (G_TYPE_CHECK_INSTANCE_CAST((o), BRISK_TYPE_BACKEND, BriskBackend))
#define BRISK_IS_BACKEND(o) (G_TYPE_CHECK_INSTANCE_TYPE((o), BRISK_TYPE_BACKEND))
#define BRISK_BACKEND_CLASS(o) (G_TYPE_CHECK_CLASS_CAST((o), BRISK_TYPE_BACKEND, BriskBackendClass))
#define BRISK_IS_BACKEND_CLASS(o) (G_TYPE_CHECK_CLASS_TYPE((o), BRISK_TYPE_BACKEND))
#define BRISK_BACKEND_GET_CLASS(o)                                                                 \
        (G_TYPE_INSTANCE_GET_CLASS((o), BRISK_TYPE_BACKEND, BriskBackendClass))

GType brisk_backend_get_type(void);

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
