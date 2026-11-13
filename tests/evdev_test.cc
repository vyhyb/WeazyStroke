// Verifies EvdevSource device lifecycle with a virtual uinput mouse: a grabbed
// mouse that disappears is dropped (instead of leaving a dead fd that keeps the
// event loop spinning). Needs write access to /dev/uinput; skips otherwise.
// Note: EvdevSource grabs every real mouse too, so they freeze while this runs.

#include "evdev_source.h"
#include "input_injector.h"
#include "input_sink.h"

#include <libevdev/libevdev-uinput.h>
#include <libevdev/libevdev.h>

#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#include <cassert>
#include <cstdio>

using namespace es;

struct NullInjector final : InputInjector {
    void move_relative(double, double) override {}
    void button(Button, bool) override {}
    void scroll(int, int) override {}
    void key(uint16_t, bool) override {}
    void flush() override {}
};

struct CountingSink final : InputSink {
    void on_button(Button, bool, Sample) override {}
    void on_motion(Sample, double, double) override { ++motions; }
    void on_scroll(double, double, Sample) override {}
    int motions = 0;
};

// Creates a minimal relative mouse with a right button; nullptr if uinput is unusable.
libevdev_uinput *make_mouse() {
    int fd = ::open("/dev/uinput", O_RDWR | O_CLOEXEC);
    if (fd < 0)
        return nullptr;
    libevdev *dev = libevdev_new();
    libevdev_set_name(dev, "eswl evdev_test mouse");
    libevdev_enable_event_code(dev, EV_REL, REL_X, nullptr);
    libevdev_enable_event_code(dev, EV_REL, REL_Y, nullptr);
    libevdev_enable_event_code(dev, EV_KEY, BTN_LEFT, nullptr);
    libevdev_enable_event_code(dev, EV_KEY, BTN_RIGHT, nullptr);
    libevdev_uinput *ui = nullptr;
    int rc = libevdev_uinput_create_from_device(dev, fd, &ui);
    libevdev_free(dev);
    if (rc < 0) {
        ::close(fd);
        return nullptr;
    }
    return ui;
}

void destroy_mouse(libevdev_uinput *ui) {
    int fd = libevdev_uinput_get_fd(ui);
    libevdev_uinput_destroy(ui);
    ::close(fd);
}

// Runs the source's event loop until `done` holds or ~3 s pass.
template <typename Pred> bool pump_until(EvdevSource &src, Pred done) {
    for (int i = 0; i < 150 && !done(); ++i) {
        pollfd p{src.fd(), POLLIN, 0};
        if (::poll(&p, 1, 20) > 0)
            src.dispatch();
    }
    return done();
}

int main() {
    libevdev_uinput *mouse = make_mouse();
    if (!mouse) {
        std::printf("evdev_test: SKIP (no write access to /dev/uinput)\n");
        return 0;
    }
    ::usleep(300 * 1000); // let udev apply the input-group permissions

    NullInjector inj;
    CountingSink sink;
    EvdevSource src(sink, inj, 3, 1920, 1080);
    size_t with_mouse = src.device_count();
    assert(with_mouse >= 1);

    // Events from the grabbed virtual mouse reach the sink.
    libevdev_uinput_write_event(mouse, EV_REL, REL_X, 5);
    libevdev_uinput_write_event(mouse, EV_SYN, SYN_REPORT, 0);
    assert(pump_until(src, [&] { return sink.motions > 0; }));

    // Unplugging it drops the device rather than leaving a dead fd behind.
    destroy_mouse(mouse);
    assert(pump_until(src, [&] { return src.device_count() == with_mouse - 1; }));

    std::printf("evdev_test: OK\n");
    return 0;
}
