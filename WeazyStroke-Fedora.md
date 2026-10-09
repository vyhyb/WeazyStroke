# Build and Run WeazyStroke on Fedora

## Install build dependencies

```bash
sudo dnf install cmake ninja-build gcc gcc-c++ pkgconf-pkg-config \
  libinput-devel systemd-devel libevdev-devel libxkbcommon-devel \
  gtk4-devel gtk4-layer-shell-devel libadwaita-devel glib2-devel
```

## Get the source and build

```bash
git clone https://github.com/nine7nine/WeazyStroke.git
cd WeazyStroke
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DENABLE_ASAN=OFF
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Set input-device permissions

WeazyStroke needs the `input` group to access event devices and `/dev/uinput`. Check that the event rule uses mode `0660`:

```bash
grep 'KERNEL=="event\*"' packaging/99-easystroke-wayland.rules
```

Expected:

```text
KERNEL=="event*", SUBSYSTEM=="input", GROUP="input", MODE="0660"
```

Install the rule and reload udev:

```bash
sudo install -m 0644 packaging/99-easystroke-wayland.rules \
  /etc/udev/rules.d/99-easystroke-wayland.rules
sudo udevadm control --reload-rules
sudo udevadm trigger --subsystem-match=input
```

Add your account to the `input` group if needed, then log out and back in:

```bash
sudo usermod -aG input "$USER"
```

Verify group membership and device modes:

```bash
id -nG
stat -c '%a %U:%G %n' /dev/input/event* /dev/uinput
```

Event devices should show `660 root:input`. Restart the daemon after changing the rule.

## Install for your account

```bash
cmake --install build --prefix "$HOME/.local"
```

## Launch and configure

Open **WeazyStroke** from the applications menu, or run:

```bash
eswl-config
```

Configure a gesture and action, then start the daemon. The GUI's **Start on login** setting creates and enables a `systemd --user` service. To run the daemon manually instead:

```bash
eswl-daemon
```

In grab mode, keyboard modifiers are not tracked. Leave trigger modifiers disabled when running:

```bash
eswl-daemon --grab
```

## Fedora/GNOME notes

GNOME does not support the layer-shell protocol used by the live stroke overlay. The overlay may report a Session Lock/Layer Shell warning and the trail may not appear; gesture recognition and actions can still work.

If gestures are not captured, check that the daemon has input devices open:

```bash
pid=$(pgrep -n -x eswl-daemon)
ls -l "/proc/$pid/fd" | grep '/dev/input/event'
```
