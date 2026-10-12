#pragma once
#include <gio/gio.h>

namespace es {

// Publishes "stroke active" on the session bus for the GNOME Shell extension.
// Degrades to a no-op if no session bus is available.
class StrokeSignal {
public:
    StrokeSignal() {
        bus_ = g_bus_get_sync(G_BUS_TYPE_SESSION, nullptr, nullptr);
        if (!bus_)
            return;
        GVariant *r = g_dbus_connection_call_sync(
            bus_, "org.freedesktop.DBus", "/org/freedesktop/DBus",
            "org.freedesktop.DBus", "RequestName",
            g_variant_new("(su)", "org.weazystroke.Daemon", 0u),
            nullptr, G_DBUS_CALL_FLAGS_NONE, 1000, nullptr, nullptr);
        if (r)
            g_variant_unref(r);
    }
    ~StrokeSignal() {
        set_active(false);
        if (bus_) {
            g_dbus_connection_flush_sync(bus_, nullptr, nullptr);
            g_object_unref(bus_);
        }
    }
    StrokeSignal(const StrokeSignal &) = delete;
    StrokeSignal &operator=(const StrokeSignal &) = delete;

    void set_active(bool on) {
        if (!bus_ || on == active_)
            return;
        active_ = on;
        g_dbus_connection_emit_signal(bus_, nullptr, "/org/weazystroke/Daemon",
                                      "org.weazystroke.Daemon", "StrokeActive",
                                      g_variant_new("(b)", on), nullptr);
    }

private:
    GDBusConnection *bus_ = nullptr;
    bool active_ = false;
};

} // namespace es
