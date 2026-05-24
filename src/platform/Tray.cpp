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
    GtkWidget* colorMenuItem = gtk_menu_item_new_with_label("Color Mode");
    GtkWidget* colorMenu = gtk_menu_new();
    colorAutoItem_ = gtk_radio_menu_item_new_with_label(nullptr, "Auto Colors");
    GSList* colorGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(colorAutoItem_));
    colorStaticItem_ = gtk_radio_menu_item_new_with_label(colorGroup, "Static");
    colorGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(colorStaticItem_));
    colorReactiveItem_ = gtk_radio_menu_item_new_with_label(colorGroup, "Reactive");
    colorGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(colorReactiveItem_));
    colorSpectrumItem_ = gtk_radio_menu_item_new_with_label(colorGroup, "Spectrum");

    GtkWidget* presetMenuItem = gtk_menu_item_new_with_label("Color Preset");
    GtkWidget* presetMenu = gtk_menu_new();
    colorVioletItem_ = gtk_menu_item_new_with_label("Deep Violet");
    colorBlueItem_ = gtk_menu_item_new_with_label("Ocean Blue");
    colorTealItem_ = gtk_menu_item_new_with_label("Forest Teal");
    colorAmberItem_ = gtk_menu_item_new_with_label("Warm Amber");
    colorRoseItem_ = gtk_menu_item_new_with_label("Soft Rose");
    colorIndigoItem_ = gtk_menu_item_new_with_label("Muted Indigo");

    GtkWidget* visualMenuItem = gtk_menu_item_new_with_label("VFX Mode");
    GtkWidget* visualMenu = gtk_menu_new();
    visualAutoItem_ = gtk_radio_menu_item_new_with_label(nullptr, "Auto");
    GSList* visualGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(visualAutoItem_));
    visualSoftAuraItem_ = gtk_radio_menu_item_new_with_label(visualGroup, "Soft Aura");
    visualGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(visualSoftAuraItem_));
    visualSpectrumFlowItem_ = gtk_radio_menu_item_new_with_label(visualGroup, "Spectrum Flow");
    visualGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(visualSpectrumFlowItem_));
    visualBeatBloomItem_ = gtk_radio_menu_item_new_with_label(visualGroup, "Beat Bloom");
    visualGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(visualBeatBloomItem_));
    visualCornerHitsItem_ = gtk_radio_menu_item_new_with_label(visualGroup, "Corner Hits");
    visualGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(visualCornerHitsItem_));
    visualNeonRailsItem_ = gtk_radio_menu_item_new_with_label(visualGroup, "Neon Rails");

    GtkWidget* sideMenuItem = gtk_menu_item_new_with_label("Sides");
    GtkWidget* sideMenu = gtk_menu_new();
    sideTopItem_ = gtk_check_menu_item_new_with_label("Top");
    sideRightItem_ = gtk_check_menu_item_new_with_label("Right");
    sideBottomItem_ = gtk_check_menu_item_new_with_label("Bottom");
    sideLeftItem_ = gtk_check_menu_item_new_with_label("Left");
    surroundSyncItem_ = gtk_check_menu_item_new_with_label("Surround Sync");
    partyModeItem_ = gtk_check_menu_item_new_with_label("Party Mode");
    autostartItem_ = gtk_check_menu_item_new_with_label("Start on Login");

    GtkWidget* tuningMenuItem = gtk_menu_item_new_with_label("Manual Tuning");
    GtkWidget* tuningMenu = gtk_menu_new();
    GtkWidget* widthMenuItem = gtk_menu_item_new_with_label("Width");
    GtkWidget* widthMenu = gtk_menu_new();
    widthSlimItem_ = gtk_radio_menu_item_new_with_label(nullptr, "Slim");
    GSList* widthGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(widthSlimItem_));
    widthComfortItem_ = gtk_radio_menu_item_new_with_label(widthGroup, "Comfort");
    widthGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(widthComfortItem_));
    widthWideItem_ = gtk_radio_menu_item_new_with_label(widthGroup, "Wide");
    widthGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(widthWideItem_));
    widthCinematicItem_ = gtk_radio_menu_item_new_with_label(widthGroup, "Cinematic");

    GtkWidget* brightnessMenuItem = gtk_menu_item_new_with_label("Brightness");
    GtkWidget* brightnessMenu = gtk_menu_new();
    brightnessLowItem_ = gtk_radio_menu_item_new_with_label(nullptr, "Low");
    GSList* brightnessGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(brightnessLowItem_));
    brightnessComfortItem_ = gtk_radio_menu_item_new_with_label(brightnessGroup, "Comfort");
    brightnessGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(brightnessComfortItem_));
    brightnessBrightItem_ = gtk_radio_menu_item_new_with_label(brightnessGroup, "Bright");
    brightnessGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(brightnessBrightItem_));
    brightnessMaxItem_ = gtk_radio_menu_item_new_with_label(brightnessGroup, "Max");

    GtkWidget* contrastMenuItem = gtk_menu_item_new_with_label("Contrast");
    GtkWidget* contrastMenu = gtk_menu_new();
    contrastSoftItem_ = gtk_radio_menu_item_new_with_label(nullptr, "Soft");
    GSList* contrastGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(contrastSoftItem_));
    contrastBalancedItem_ = gtk_radio_menu_item_new_with_label(contrastGroup, "Balanced");
    contrastGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(contrastBalancedItem_));
    contrastPunchyItem_ = gtk_radio_menu_item_new_with_label(contrastGroup, "Punchy");

    g_object_set_data(G_OBJECT(colorAutoItem_), "mode", (void*)"auto");
    g_object_set_data(G_OBJECT(colorStaticItem_), "mode", (void*)"static");
    g_object_set_data(G_OBJECT(colorReactiveItem_), "mode", (void*)"reactive");
    g_object_set_data(G_OBJECT(colorSpectrumItem_), "mode", (void*)"spectrum");
    g_object_set_data(G_OBJECT(colorVioletItem_), "color", (void*)"#7B2FFF");
    g_object_set_data(G_OBJECT(colorBlueItem_), "color", (void*)"#246BCE");
    g_object_set_data(G_OBJECT(colorTealItem_), "color", (void*)"#2C9690");
    g_object_set_data(G_OBJECT(colorAmberItem_), "color", (void*)"#BD7D28");
    g_object_set_data(G_OBJECT(colorRoseItem_), "color", (void*)"#C94D82");
    g_object_set_data(G_OBJECT(colorIndigoItem_), "color", (void*)"#5144A8");
    g_object_set_data(G_OBJECT(visualAutoItem_), "mode", (void*)"auto");
    g_object_set_data(G_OBJECT(visualSoftAuraItem_), "mode", (void*)"soft_aura");
    g_object_set_data(G_OBJECT(visualSpectrumFlowItem_), "mode", (void*)"spectrum_flow");
    g_object_set_data(G_OBJECT(visualBeatBloomItem_), "mode", (void*)"beat_bloom");
    g_object_set_data(G_OBJECT(visualCornerHitsItem_), "mode", (void*)"corner_hits");
    g_object_set_data(G_OBJECT(visualNeonRailsItem_), "mode", (void*)"neon_rails");
    g_object_set_data(G_OBJECT(sideTopItem_), "side", (void*)"top");
    g_object_set_data(G_OBJECT(sideRightItem_), "side", (void*)"right");
    g_object_set_data(G_OBJECT(sideBottomItem_), "side", (void*)"bottom");
    g_object_set_data(G_OBJECT(sideLeftItem_), "side", (void*)"left");
    g_object_set_data(G_OBJECT(widthSlimItem_), "value", GINT_TO_POINTER(10));
    g_object_set_data(G_OBJECT(widthComfortItem_), "value", GINT_TO_POINTER(18));
    g_object_set_data(G_OBJECT(widthWideItem_), "value", GINT_TO_POINTER(28));
    g_object_set_data(G_OBJECT(widthCinematicItem_), "value", GINT_TO_POINTER(42));
    g_object_set_data(G_OBJECT(brightnessLowItem_), "value", GINT_TO_POINTER(35));
    g_object_set_data(G_OBJECT(brightnessComfortItem_), "value", GINT_TO_POINTER(55));
    g_object_set_data(G_OBJECT(brightnessBrightItem_), "value", GINT_TO_POINTER(75));
    g_object_set_data(G_OBJECT(brightnessMaxItem_), "value", GINT_TO_POINTER(90));
    g_object_set_data(G_OBJECT(contrastSoftItem_), "value", GINT_TO_POINTER(75));
    g_object_set_data(G_OBJECT(contrastBalancedItem_), "value", GINT_TO_POINTER(100));
    g_object_set_data(G_OBJECT(contrastPunchyItem_), "value", GINT_TO_POINTER(135));

    GtkWidget* separator1 = gtk_separator_menu_item_new();
    GtkWidget* separator2 = gtk_separator_menu_item_new();
    GtkWidget* quitItem = gtk_menu_item_new_with_label("Quit");

    g_signal_connect(toggleItem_, "activate", G_CALLBACK(onToggleClicked), this);
    g_signal_connect(colorAutoItem_, "toggled", G_CALLBACK(onColorModeClicked), this);
    g_signal_connect(colorStaticItem_, "toggled", G_CALLBACK(onColorModeClicked), this);
    g_signal_connect(colorReactiveItem_, "toggled", G_CALLBACK(onColorModeClicked), this);
    g_signal_connect(colorSpectrumItem_, "toggled", G_CALLBACK(onColorModeClicked), this);
    g_signal_connect(colorVioletItem_, "activate", G_CALLBACK(onPrimaryColorClicked), this);
    g_signal_connect(colorBlueItem_, "activate", G_CALLBACK(onPrimaryColorClicked), this);
    g_signal_connect(colorTealItem_, "activate", G_CALLBACK(onPrimaryColorClicked), this);
    g_signal_connect(colorAmberItem_, "activate", G_CALLBACK(onPrimaryColorClicked), this);
    g_signal_connect(colorRoseItem_, "activate", G_CALLBACK(onPrimaryColorClicked), this);
    g_signal_connect(colorIndigoItem_, "activate", G_CALLBACK(onPrimaryColorClicked), this);
    g_signal_connect(visualAutoItem_, "toggled", G_CALLBACK(onVisualModeClicked), this);
    g_signal_connect(visualSoftAuraItem_, "toggled", G_CALLBACK(onVisualModeClicked), this);
    g_signal_connect(visualSpectrumFlowItem_, "toggled", G_CALLBACK(onVisualModeClicked), this);
    g_signal_connect(visualBeatBloomItem_, "toggled", G_CALLBACK(onVisualModeClicked), this);
    g_signal_connect(visualCornerHitsItem_, "toggled", G_CALLBACK(onVisualModeClicked), this);
    g_signal_connect(visualNeonRailsItem_, "toggled", G_CALLBACK(onVisualModeClicked), this);
    g_signal_connect(sideTopItem_, "toggled", G_CALLBACK(onSideToggled), this);
    g_signal_connect(sideRightItem_, "toggled", G_CALLBACK(onSideToggled), this);
    g_signal_connect(sideBottomItem_, "toggled", G_CALLBACK(onSideToggled), this);
    g_signal_connect(sideLeftItem_, "toggled", G_CALLBACK(onSideToggled), this);
    g_signal_connect(surroundSyncItem_, "toggled", G_CALLBACK(onSurroundSyncToggled), this);
    g_signal_connect(partyModeItem_, "toggled", G_CALLBACK(onPartyModeToggled), this);
    g_signal_connect(autostartItem_, "toggled", G_CALLBACK(onAutostartToggled), this);
    g_signal_connect(widthSlimItem_, "toggled", G_CALLBACK(onWidthClicked), this);
    g_signal_connect(widthComfortItem_, "toggled", G_CALLBACK(onWidthClicked), this);
    g_signal_connect(widthWideItem_, "toggled", G_CALLBACK(onWidthClicked), this);
    g_signal_connect(widthCinematicItem_, "toggled", G_CALLBACK(onWidthClicked), this);
    g_signal_connect(brightnessLowItem_, "toggled", G_CALLBACK(onBrightnessClicked), this);
    g_signal_connect(brightnessComfortItem_, "toggled", G_CALLBACK(onBrightnessClicked), this);
    g_signal_connect(brightnessBrightItem_, "toggled", G_CALLBACK(onBrightnessClicked), this);
    g_signal_connect(brightnessMaxItem_, "toggled", G_CALLBACK(onBrightnessClicked), this);
    g_signal_connect(contrastSoftItem_, "toggled", G_CALLBACK(onContrastClicked), this);
    g_signal_connect(contrastBalancedItem_, "toggled", G_CALLBACK(onContrastClicked), this);
    g_signal_connect(contrastPunchyItem_, "toggled", G_CALLBACK(onContrastClicked), this);
    g_signal_connect(quitItem, "activate", G_CALLBACK(onQuitClicked), this);

    gtk_menu_shell_append(GTK_MENU_SHELL(colorMenu), colorAutoItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(colorMenu), colorStaticItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(colorMenu), colorReactiveItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(colorMenu), colorSpectrumItem_);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(colorMenuItem), colorMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(presetMenu), colorVioletItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(presetMenu), colorBlueItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(presetMenu), colorTealItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(presetMenu), colorAmberItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(presetMenu), colorRoseItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(presetMenu), colorIndigoItem_);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(presetMenuItem), presetMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(visualMenu), visualAutoItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(visualMenu), visualSoftAuraItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(visualMenu), visualSpectrumFlowItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(visualMenu), visualBeatBloomItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(visualMenu), visualCornerHitsItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(visualMenu), visualNeonRailsItem_);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(visualMenuItem), visualMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(sideMenu), sideTopItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(sideMenu), sideRightItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(sideMenu), sideBottomItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(sideMenu), sideLeftItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(sideMenu), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(sideMenu), surroundSyncItem_);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(sideMenuItem), sideMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(widthMenu), widthSlimItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(widthMenu), widthComfortItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(widthMenu), widthWideItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(widthMenu), widthCinematicItem_);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(widthMenuItem), widthMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(brightnessMenu), brightnessLowItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(brightnessMenu), brightnessComfortItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(brightnessMenu), brightnessBrightItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(brightnessMenu), brightnessMaxItem_);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(brightnessMenuItem), brightnessMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(contrastMenu), contrastSoftItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(contrastMenu), contrastBalancedItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(contrastMenu), contrastPunchyItem_);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(contrastMenuItem), contrastMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(tuningMenu), widthMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(tuningMenu), brightnessMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(tuningMenu), contrastMenuItem);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(tuningMenuItem), tuningMenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), toggleItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), separator1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), colorMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), presetMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), visualMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), sideMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), tuningMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), partyModeItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), autostartItem_);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_), separator2);
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

void Tray::setOnColorMode(std::function<void(const std::string&)> cb) {
    onColorMode_ = cb;
}

void Tray::setOnPrimaryColor(std::function<void(const std::string&)> cb) {
    onPrimaryColor_ = cb;
}

void Tray::setOnVisualMode(std::function<void(const std::string&)> cb) {
    onVisualMode_ = cb;
}

void Tray::setOnSideToggle(std::function<void(const std::string&, bool)> cb) {
    onSideToggle_ = cb;
}

void Tray::setOnSurroundSync(std::function<void(bool)> cb) {
    onSurroundSync_ = cb;
}

void Tray::setOnPartyMode(std::function<void(bool)> cb) {
    onPartyMode_ = cb;
}

void Tray::setOnAutostart(std::function<void(bool)> cb) {
    onAutostart_ = cb;
}

void Tray::setOnEdgeWidth(std::function<void(int)> cb) {
    onEdgeWidth_ = cb;
}

void Tray::setOnBrightness(std::function<void(float)> cb) {
    onBrightness_ = cb;
}

void Tray::setOnContrast(std::function<void(float)> cb) {
    onContrast_ = cb;
}

void Tray::setVisible(bool visible) {
    visible_ = visible;
    if (toggleItem_) {
        gtk_menu_item_set_label(GTK_MENU_ITEM(toggleItem_), visible_ ? "Hide Overlay" : "Show Overlay");
    }
}

void Tray::syncConfig(const AppConfig& config) {
    syncing_ = true;
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(colorAutoItem_), config.colorMode == "auto");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(colorStaticItem_), config.colorMode == "static");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(colorReactiveItem_), config.colorMode == "reactive");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(colorSpectrumItem_), config.colorMode == "spectrum");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(visualAutoItem_), config.visualMode == "auto");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(visualSoftAuraItem_), config.visualMode == "soft_aura");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(visualSpectrumFlowItem_), config.visualMode == "spectrum_flow");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(visualBeatBloomItem_), config.visualMode == "beat_bloom");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(visualCornerHitsItem_), config.visualMode == "corner_hits");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(visualNeonRailsItem_), config.visualMode == "neon_rails");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(sideTopItem_), config.sideTop);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(sideRightItem_), config.sideRight);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(sideBottomItem_), config.sideBottom);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(sideLeftItem_), config.sideLeft);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(surroundSyncItem_), config.surroundSync);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(partyModeItem_), config.partyMode);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(autostartItem_), config.autostart);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widthSlimItem_), config.edgeWidth <= 13);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widthComfortItem_), config.edgeWidth > 13 && config.edgeWidth <= 23);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widthWideItem_), config.edgeWidth > 23 && config.edgeWidth <= 35);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widthCinematicItem_), config.edgeWidth > 35);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(brightnessLowItem_), config.intensity < 0.45f);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(brightnessComfortItem_), config.intensity >= 0.45f && config.intensity < 0.65f);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(brightnessBrightItem_), config.intensity >= 0.65f && config.intensity < 0.83f);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(brightnessMaxItem_), config.intensity >= 0.83f);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(contrastSoftItem_), config.contrast < 0.9f);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(contrastBalancedItem_), config.contrast >= 0.9f && config.contrast < 1.18f);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(contrastPunchyItem_), config.contrast >= 1.18f);
    syncing_ = false;
}

void Tray::onToggleClicked(GtkMenuItem*, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->onToggle_) {
        self->onToggle_();
    }
}

void Tray::onColorModeClicked(GtkCheckMenuItem* item, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->syncing_ || !gtk_check_menu_item_get_active(item)) return;
    const char* mode = static_cast<const char*>(g_object_get_data(G_OBJECT(item), "mode"));
    if (mode && self->onColorMode_) {
        self->onColorMode_(mode);
    }
}

void Tray::onPrimaryColorClicked(GtkMenuItem* item, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->syncing_) return;
    const char* color = static_cast<const char*>(g_object_get_data(G_OBJECT(item), "color"));
    if (color && self->onPrimaryColor_) {
        self->onPrimaryColor_(color);
    }
}

void Tray::onVisualModeClicked(GtkCheckMenuItem* item, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->syncing_ || !gtk_check_menu_item_get_active(item)) return;
    const char* mode = static_cast<const char*>(g_object_get_data(G_OBJECT(item), "mode"));
    if (mode && self->onVisualMode_) {
        self->onVisualMode_(mode);
    }
}

void Tray::onSideToggled(GtkCheckMenuItem* item, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->syncing_) return;
    const char* side = static_cast<const char*>(g_object_get_data(G_OBJECT(item), "side"));
    if (side && self->onSideToggle_) {
        self->onSideToggle_(side, gtk_check_menu_item_get_active(item));
    }
}

void Tray::onSurroundSyncToggled(GtkCheckMenuItem* item, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->syncing_) return;
    if (self->onSurroundSync_) {
        self->onSurroundSync_(gtk_check_menu_item_get_active(item));
    }
}

void Tray::onPartyModeToggled(GtkCheckMenuItem* item, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->syncing_) return;
    if (self->onPartyMode_) {
        self->onPartyMode_(gtk_check_menu_item_get_active(item));
    }
}

void Tray::onAutostartToggled(GtkCheckMenuItem* item, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->syncing_) return;
    if (self->onAutostart_) {
        self->onAutostart_(gtk_check_menu_item_get_active(item));
    }
}

void Tray::onWidthClicked(GtkCheckMenuItem* item, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->syncing_ || !gtk_check_menu_item_get_active(item)) return;
    int value = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(item), "value"));
    if (self->onEdgeWidth_) {
        self->onEdgeWidth_(value);
    }
}

void Tray::onBrightnessClicked(GtkCheckMenuItem* item, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->syncing_ || !gtk_check_menu_item_get_active(item)) return;
    int value = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(item), "value"));
    if (self->onBrightness_) {
        self->onBrightness_(static_cast<float>(value) / 100.0f);
    }
}

void Tray::onContrastClicked(GtkCheckMenuItem* item, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->syncing_ || !gtk_check_menu_item_get_active(item)) return;
    int value = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(item), "value"));
    if (self->onContrast_) {
        self->onContrast_(static_cast<float>(value) / 100.0f);
    }
}

void Tray::onQuitClicked(GtkMenuItem*, void* data) {
    Tray* self = static_cast<Tray*>(data);
    if (self->onQuit_) {
        self->onQuit_();
    }
}
