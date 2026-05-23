#pragma once
#include <gtk/gtk.h>
#ifdef HAVE_AYATANA_APPINDICATOR
#include <libayatana-appindicator/app-indicator.h>
#else
#include <libappindicator/app-indicator.h>
#endif
#include <string>
#include <functional>

class Tray {
public:
    Tray();
    ~Tray();

    bool init(const std::string& iconPath);
    void update();
    void setOnToggle(std::function<void()> cb);
    void setOnQuit(std::function<void()> cb);
    void setVisible(bool visible);

private:
    AppIndicator* indicator_ = nullptr;
    GtkWidget* menu_ = nullptr;
    GtkWidget* toggleItem_ = nullptr;
    std::function<void()> onToggle_;
    std::function<void()> onQuit_;
    bool visible_ = true;

    static void onToggleClicked(GtkMenuItem* item, void* data);
    static void onQuitClicked(GtkMenuItem* item, void* data);
};
