#pragma once
#include <gtk/gtk.h>
#ifdef HAVE_AYATANA_APPINDICATOR
#include <libayatana-appindicator/app-indicator.h>
#else
#include <libappindicator/app-indicator.h>
#endif
#include <string>
#include <functional>
#include "../settings/Config.h"

class Tray {
public:
    Tray();
    ~Tray();

    bool init(const std::string& iconPath);
    void update();
    void setOnToggle(std::function<void()> cb);
    void setOnQuit(std::function<void()> cb);
    void setOnColorMode(std::function<void(const std::string&)> cb);
    void setOnVisualMode(std::function<void(const std::string&)> cb);
    void setOnSideToggle(std::function<void(const std::string&, bool)> cb);
    void setOnEdgeWidth(std::function<void(int)> cb);
    void setOnBrightness(std::function<void(float)> cb);
    void setOnContrast(std::function<void(float)> cb);
    void setVisible(bool visible);
    void syncConfig(const AppConfig& config);

private:
    AppIndicator* indicator_ = nullptr;
    GtkWidget* menu_ = nullptr;
    GtkWidget* toggleItem_ = nullptr;
    GtkWidget* colorStaticItem_ = nullptr;
    GtkWidget* colorReactiveItem_ = nullptr;
    GtkWidget* colorSpectrumItem_ = nullptr;
    GtkWidget* visualAutoItem_ = nullptr;
    GtkWidget* visualSoftAuraItem_ = nullptr;
    GtkWidget* visualSpectrumFlowItem_ = nullptr;
    GtkWidget* visualBeatBloomItem_ = nullptr;
    GtkWidget* visualCornerHitsItem_ = nullptr;
    GtkWidget* sideTopItem_ = nullptr;
    GtkWidget* sideRightItem_ = nullptr;
    GtkWidget* sideBottomItem_ = nullptr;
    GtkWidget* sideLeftItem_ = nullptr;
    GtkWidget* widthSlimItem_ = nullptr;
    GtkWidget* widthComfortItem_ = nullptr;
    GtkWidget* widthWideItem_ = nullptr;
    GtkWidget* widthCinematicItem_ = nullptr;
    GtkWidget* brightnessLowItem_ = nullptr;
    GtkWidget* brightnessComfortItem_ = nullptr;
    GtkWidget* brightnessBrightItem_ = nullptr;
    GtkWidget* brightnessMaxItem_ = nullptr;
    GtkWidget* contrastSoftItem_ = nullptr;
    GtkWidget* contrastBalancedItem_ = nullptr;
    GtkWidget* contrastPunchyItem_ = nullptr;
    std::function<void()> onToggle_;
    std::function<void()> onQuit_;
    std::function<void(const std::string&)> onColorMode_;
    std::function<void(const std::string&)> onVisualMode_;
    std::function<void(const std::string&, bool)> onSideToggle_;
    std::function<void(int)> onEdgeWidth_;
    std::function<void(float)> onBrightness_;
    std::function<void(float)> onContrast_;
    bool visible_ = true;
    bool syncing_ = false;

    static void onToggleClicked(GtkMenuItem* item, void* data);
    static void onQuitClicked(GtkMenuItem* item, void* data);
    static void onColorModeClicked(GtkCheckMenuItem* item, void* data);
    static void onVisualModeClicked(GtkCheckMenuItem* item, void* data);
    static void onSideToggled(GtkCheckMenuItem* item, void* data);
    static void onWidthClicked(GtkCheckMenuItem* item, void* data);
    static void onBrightnessClicked(GtkCheckMenuItem* item, void* data);
    static void onContrastClicked(GtkCheckMenuItem* item, void* data);
};
