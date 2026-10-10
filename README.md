# WeazyStroke

Gesture recognition for **Wayland**. Draw a stroke with your **mouse, pen/tablet,
stylus, or fingers** and WeazyStroke runs an action — launch a command, send a
key combo, type text, click a button, scroll, and more.

A desktop-environment-agnostic GTK4 + libadwaita app, built on the recognition
core of the classic [easystroke](https://github.com/thjaeger/easystroke) and
rebuilt for Wayland. The gesture engine, GUI, and actions run on any Wayland
compositor. The only compositor-dependent piece is the **live stroke-trail
overlay** — it uses the `wlr-layer-shell` protocol (via `gtk4-layer-shell`),
which KDE, wlroots-based compositors, etc. implement but GNOME's Mutter does
not, so on GNOME everything works *except* the on-screen trail.

![WeazyStroke — the Actions table](docs/screenshot.png)

## Features

- **Every pointing device** — one gesture engine across **mouse**, **pen/tablet**,
  **stylus**, and **touch**.
- **Pen pressure → line width** — the live trail thickens as you press harder,
  driven by real tablet pressure.
- **Two-finger touch gestures** — hold a finger inside a screen-edge band to *arm*,
  draw with a second finger; an expanding ring marks the armed point and the trail
  responds to finger **contact size**.
- **Flexible triggers** — a mouse button (with optional modifiers), the pen tip,
  the side button, or a "tip + side button" chord that keeps the side button free
  for other uses; plus a configurable touch edge.
- **Live stroke overlay** — a click-through trail that follows your stroke: a
  direction **gradient** with customizable start/end colors, **Plain / Glow /
  Sparkle** styles, adjustable width, a hairline onset taper, and an animated
  completion retract.
- **Typed actions** — per gesture: run a **command**, send a **key** combo, type
  **text**, click a **button**, **scroll**, or **ignore**.
- **Sturdy recognition** — record a gesture several times for multi-example
  matching; tune the match threshold.
- **GTK4 + libadwaita GUI** — an Actions table with per-gesture stroke thumbnails,
  paginated preferences, a glass aesthetic, a History tab, and an in-window stroke
  recorder.
- **System tray** — enable/disable, open preferences, quit (StatusNotifierItem).
- **Autostart & live reload** — one toggle installs a systemd user service (the overlay
  has its own toggle, unavailable on GNOME); saving
  in the GUI applies instantly to the running daemon, no restart.

## Install

### Prebuilt packages

Download the latest `.rpm` or `.deb` (x86_64 and arm64) from the
[Releases page](https://github.com/vyhyb/WeazyStroke/releases/latest):

```sh
sudo dnf install ./weazystroke-*.rpm     # Fedora
sudo apt install ./weazystroke_*.deb     # Debian 13+ / Ubuntu 25.04+
```

The packages install the four binaries, the launcher, the icon and the udev rule.
Then apply the rule and join the `input` group (see [Permissions](#permissions)).

**Arch:**

```sh
cd packaging && makepkg -si
```

### Build from source

Build dependencies (`gtk4-layer-shell` is needed even on GNOME, the binaries link it):

```sh
# Fedora
sudo dnf install cmake ninja-build gcc-c++ pkgconf-pkg-config libinput-devel \
    systemd-devel libevdev-devel libxkbcommon-devel gtk4-devel libadwaita-devel \
    gtk4-layer-shell-devel

# Debian 13+ / Ubuntu 25.04+
sudo apt install build-essential cmake ninja-build pkg-config libinput-dev \
    libudev-dev libevdev-dev libxkbcommon-dev libgtk-4-dev libadwaita-1-dev \
    libgtk4-layer-shell-dev
```

Build, test and install:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DENABLE_ASAN=OFF \
    -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build
ctest --test-dir build --output-on-failure
sudo cmake --install build
```

For a development build, drop the `-DCMAKE_BUILD_TYPE`/`-DENABLE_ASAN` flags
(Debug with sanitizers). To build a package instead of installing, run
`cpack -G RPM` or `cpack -G DEB` inside `build/`.

The four binaries are `eswl-daemon` (engine), `eswl-overlay` (trail renderer),
`eswl-config` (GUI), `eswl-tray` (tray).

## Usage

1. Open **WeazyStroke** (`eswl-config`); enable *Start on login* to autostart it.
2. **Record Stroke** → draw it → name it → choose a Type + Argument (command, key,
   text, button, scroll) → **Save**.
3. Draw the gesture with your trigger:
   - **Mouse** — hold the configured button (+ optional modifiers) and draw.
   - **Pen** — tip, side button, or the "tip + side button" chord.
   - **Touch** — hold a finger in the chosen edge band, draw with a second finger.

Triggers, the trail, colors, pressure, and touch are all tunable in
**Preferences**; changes apply live.

## Permissions

The engine needs read-write access to `/dev/input/event*` (libinput) and
`/dev/uinput` (action injection). The packages and `cmake --install` install the
udev rule; apply it and join the `input` group:

```sh
sudo udevadm control --reload-rules && sudo udevadm trigger
sudo usermod -aG input "$USER"        # then re-login
```

## GNOME

The engine, GUI, and actions work on GNOME, but Mutter does not implement
`wlr-layer-shell`, so the daemon's live overlay cannot draw its trail there.
The companion [WeazyStroke GNOME Tail extension](weazystroke-gnome-tail/README.md)
draws the trail on the GNOME Shell stage instead. It reads the trigger from the
WeazyStroke config and reports config detection in its preferences; install and
enable it separately from the daemon.

## License

ISC, inheriting easystroke's license. Recognition core from
[easystroke](https://github.com/thjaeger/easystroke) by Thomas Jaeger.
