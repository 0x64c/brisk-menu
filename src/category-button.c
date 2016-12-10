/*
 * This file is part of mate-solmenu.
 *
 * Copyright © 2016 Ikey Doherty <ikey@solus-proejct.com>
 *
 * mate-solmenu is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */

#define _GNU_SOURCE

#include "util.h"

SOLUS_BEGIN_PEDANTIC
#include "category-button.h"
#include <gtk/gtk.h>
#include <matemenu-tree.h>
SOLUS_END_PEDANTIC

struct _SolMenuCategoryButtonClass {
        GtkRadioButtonClass parent_class;
};

/**
 * SolMenuCategoryButton is the toplevel window type used within the applet.
 */
struct _SolMenuCategoryButton {
        GtkRadioButton parent;
        MateMenuTreeDirectory *group;
};

G_DEFINE_TYPE(SolMenuCategoryButton, sol_menu_category_button, GTK_TYPE_RADIO_BUTTON)

enum { PROP_GROUP = 1, N_PROPS };

static GParamSpec *obj_properties[N_PROPS] = {
        NULL,
};

static void sol_menu_category_button_set_property(GObject *object, guint id, const GValue *value,
                                                  GParamSpec *spec)
{
        SolMenuCategoryButton *self = SOL_MENU_CATEGORY_BUTTON(object);

        switch (id) {
        case PROP_GROUP:
                self->group = g_value_get_pointer(value);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID(object, id, spec);
                break;
        }
}

static void sol_menu_category_button_get_property(GObject *object, guint id, GValue *value,
                                                  GParamSpec *spec)
{
        SolMenuCategoryButton *self = SOL_MENU_CATEGORY_BUTTON(object);

        switch (id) {
        case PROP_GROUP:
                g_value_set_pointer(value, self->group);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID(object, id, spec);
                break;
        }
}

/**
 * sol_menu_category_button_new:
 *
 * Construct a new SolMenuCategoryButton object
 */
GtkWidget *sol_menu_category_button_new()
{
        return g_object_new(SOL_TYPE_MENU_CATEGORY_BUTTON, NULL);
}

/**
 * sol_menu_category_button_dispose:
 *
 * Clean up a SolMenuCategoryButton instance
 */
static void sol_menu_category_button_dispose(GObject *obj)
{
        G_OBJECT_CLASS(sol_menu_category_button_parent_class)->dispose(obj);
}

/**
 * sol_menu_category_button_class_init:
 *
 * Handle class initialisation
 */
static void sol_menu_category_button_class_init(SolMenuCategoryButtonClass *klazz)
{
        GObjectClass *obj_class = G_OBJECT_CLASS(klazz);

        /* gobject vtable hookup */
        obj_class->dispose = sol_menu_category_button_dispose;
        obj_class->set_property = sol_menu_category_button_set_property;
        obj_class->get_property = sol_menu_category_button_get_property;

        obj_properties[PROP_GROUP] = g_param_spec_pointer("group",
                                                          "The MateMenuTreeDirectory",
                                                          "Directory that this category represents",
                                                          G_PARAM_READWRITE);
        g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

/**
 * sol_menu_category_button_init:
 *
 * Handle construction of the SolMenuCategoryButton
 */
static void sol_menu_category_button_init(__solus_unused__ SolMenuCategoryButton *self)
{
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
