#include "Tray.h"
#include <iostream>

Tray::Tray() {}

Tray::~Tray() {
    // GTK cleanup is usually handled by the main loop,
    // but we should at least null our pointers.
}

bool Tray::init(const std::string& iconPath) {
    if (!gtk_init_check(nullptr, nullptr)) {
        std::cerr << "[AMBIENCE] Failed to initialize GTK" << std::endl;
        return false;
    }

    indicator_ = app_indicator_new(
        "loopin-ambience-mode",
        iconPath.c_str(),
        APP_INDICATOR_CATEGORY_APPLICATION_STATUS
    );

    app_indicator_set_status(indicator_, APP_INDICATOR_STATUS_ACTIVE);

    menu_ = gtk_menu_new();
    toggleItem_ = gtk_menu_item_new_with_label("Hide Overlay");
    GtkWidget* separator = gtk_separator_menu_item_new();
    GtkWidget* quitItem = gtk_menu_item_new_with_label("Quit");

    g_signal_connect(toggleItem_, "activate", G_CALLBACK(onToggleClicked), this);
    g_signal_connect(quitItem, "activate", G_CALLBACK(onQuitClicked), this);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), toggleItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), separator);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), quitItem);

    gtk_widget_show_all(menu_);
    app_indicator_set_menu(indicator_, GTK_MENU(menu_));

    return true;
}

void Tray::update() {
    while (gtk_events_pending()) {
        gtk_main_iteration();
    }
}

void Tray::setOnToggle(std::function<void()> cb) {
    onToggle_ = cb;
}

void Tray::setOnQuit(std::function<void()> cb) {
    onQuit_ = cb;
}

void Tray::setVisible(bool visible) {
    visible_ = visible;
    if (toggleItem_) {
        gtk_menu_item_set_label(GTK_MENU_ITEM(toggleItem_), visible_ ? "Hide Overlay" : "Show Overlay");
    }
}

void Tray::onToggleClicked(GtkMenuItem*, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->onToggle_) {
        self->onToggle_();
    }
}

void Tray::onQuitClicked(GtkMenuItem*, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->onQuit_) {
        self->onQuit_();
    }
}
