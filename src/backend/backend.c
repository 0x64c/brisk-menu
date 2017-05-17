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

#define _GNU_SOURCE

#include "util.h"

BRISK_BEGIN_PEDANTIC
#include "backend.h"
BRISK_END_PEDANTIC

/**
 * IDs for our signals
 */
enum { BACKEND_SIGNAL_ITEM_ADDED = 0,
       BACKEND_SIGNAL_ITEM_REMOVED,
       BACKEND_SIGNAL_RESET,
       N_SIGNALS };

static guint backend_signals[N_SIGNALS] = { 0 };

G_DEFINE_TYPE(BriskBackend, brisk_backend, G_TYPE_OBJECT)

/**
 * brisk_backend_dispose:
 *
 * Clean up a BriskBackend instance
 */
static void brisk_backend_dispose(GObject *obj)
{
        G_OBJECT_CLASS(brisk_backend_parent_class)->dispose(obj);
}

/**
 * brisk_backend_class_init:
 *
 * Handle class initialisation
 */
static void brisk_backend_class_init(BriskBackendClass *klazz)
{
        GObjectClass *obj_class = G_OBJECT_CLASS(klazz);

        /* gobject vtable hookup */
        obj_class->dispose = brisk_backend_dispose;

        /**
         * BriskBackend::item-added
         * @backend: The backend that created the item
         * @item: The newly available item
         *
         * Used to notify the frontend that a new item is available for consumption
         */
        backend_signals[BACKEND_SIGNAL_ITEM_ADDED] =
            g_signal_new("item-added",
                         BRISK_TYPE_BACKEND,
                         G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                         G_STRUCT_OFFSET(BriskBackendClass, item_added),
                         NULL,
                         NULL,
                         NULL,
                         G_TYPE_NONE,
                         1,
                         G_TYPE_POINTER);

        /**
         * BriskBackend::item-removed
         * @backend: The backend that removed the item
         * @id: The item's ID that is being removed
         *
         * Used to notify the frontend that an item is being removed
         */
        backend_signals[BACKEND_SIGNAL_ITEM_ADDED] =
            g_signal_new("item-removed",
                         BRISK_TYPE_BACKEND,
                         G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                         G_STRUCT_OFFSET(BriskBackendClass, item_removed),
                         NULL,
                         NULL,
                         NULL,
                         G_TYPE_NONE,
                         1,
                         G_TYPE_STRING);

        /**
         * BriskBackend::reset
         * @backend: The backend that was reset
         *
         * Used to notify the frontend that all Item's should be destroyed
         */
        backend_signals[BACKEND_SIGNAL_RESET] =
            g_signal_new("reset",
                         BRISK_TYPE_BACKEND,
                         G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                         G_STRUCT_OFFSET(BriskBackendClass, reset),
                         NULL,
                         NULL,
                         NULL,
                         G_TYPE_NONE,
                         0);
}

/**
 * brisk_backend_init:
 *
 * Handle construction of the BriskBackend
 */
static void brisk_backend_init(__brisk_unused__ BriskBackend *self)
{
}

/**
 * brisk_backend_get_flags:
 *
 * Return the supported flags for the backend
 */
unsigned int brisk_backend_get_flags(BriskBackend *backend)
{
        g_assert(backend != NULL);
        BriskBackendClass *klazz = BRISK_BACKEND_GET_CLASS(backend);
        g_assert(klazz->get_flags != NULL);
        return klazz->get_flags(backend);
}

/**
 * brisk_backend_get_id:
 *
 * Return the unique ID for the backend
 * @note This string is owned by the backend and must not be freed
 */
const gchar *brisk_backend_get_id(BriskBackend *backend)
{
        g_assert(backend != NULL);
        BriskBackendClass *klazz = BRISK_BACKEND_GET_CLASS(backend);
        g_assert(klazz->get_id != NULL);
        return klazz->get_id(backend);
}

/**
 * brisk_backend_get_display_name:
 *
 * Return the display name for the backend
 * @note This string is owned by the backend and must not be freed
 */
const gchar *brisk_backend_get_display_name(BriskBackend *backend)
{
        g_assert(backend != NULL);
        BriskBackendClass *klazz = BRISK_BACKEND_GET_CLASS(backend);
        g_assert(klazz->get_display_name != NULL);
        return klazz->get_display_name(backend);
}

/* Favourites specific functionality */

/**
 * brisk_backend_pin_item:
 *
 * Attempt to pin the selected item in the backend for prioritising
 * in access and display
 */
gboolean brisk_backend_pin_item(BriskBackend *backend, BriskItem *item)
{
        g_assert(backend != NULL);
        BriskBackendClass *klazz = BRISK_BACKEND_GET_CLASS(backend);

        g_return_val_if_fail(klazz->pin_item != NULL, FALSE);
        return klazz->pin_item(backend, item);
}

/**
 * brisk_backend_is_item_pinned:
 *
 * Determine if the given item was previously pinned or not
 */
gboolean brisk_backend_is_item_pinned(BriskBackend *backend, BriskItem *item)
{
        g_assert(backend != NULL);
        BriskBackendClass *klazz = BRISK_BACKEND_GET_CLASS(backend);

        g_return_val_if_fail(klazz->is_item_pinned != NULL, FALSE);
        return klazz->is_item_pinned(backend, item);
}

/**
 * brisk_backend_unpin_item:
 *
 * Unpin a previously pinned item.
 */
gboolean brisk_backend_unpin_item(BriskBackend *backend, BriskItem *item)
{
        g_assert(backend != NULL);
        BriskBackendClass *klazz = BRISK_BACKEND_GET_CLASS(backend);

        g_return_val_if_fail(klazz->unpin_item != NULL, FALSE);
        return klazz->unpin_item(backend, item);
}

/**
 * brisk_backend_load:
 *
 * Attempt to load the backend for the first time
 */
gboolean brisk_backend_load(BriskBackend *backend)
{
        g_assert(backend != NULL);
        BriskBackendClass *klazz = BRISK_BACKEND_GET_CLASS(backend);

        g_return_val_if_fail(klazz->load != NULL, FALSE);
        return klazz->load(backend);
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
