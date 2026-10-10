## WeazyStroke 0.1.0

First packaged release of the WeazyStroke fork, software originally created by nine7nine: gesture recognition for Wayland, built on the easystroke recognition core.

### Highlights

- **Every pointing device:** mouse, pen/tablet, stylus and touch share one gesture engine.
- **Flexible triggers:** a mouse button with optional modifiers, the pen tip, the side button, or a tip + side button chord. Touch uses a configurable screen-edge hold plus a second finger.
- **Pen pressure and touch contact size** drive the trail width.
- **Typed actions:** run a command, send a key combo, type text, type text + Enter, click a button, scroll, or ignore.
- **Multi-example matching:** record a gesture several times and tune the match threshold.
- **Live stroke overlay:** a click-through trail with a start/end colour gradient, Plain / Glow / Sparkle styles and an animated finish.
- **GTK4 + libadwaita GUI:** Actions table with stroke thumbnails, paginated preferences, History tab and an in-window recorder.
- **System tray**, plus live reload: saving in the GUI applies to the running daemon without a restart.

### Desktop support

- **KDE and other layer-shell compositors:** everything works, including the overlay.
- **GNOME:** the engine, GUI and actions work. Mutter has no layer-shell, so the trail overlay is unavailable and its checkbox is greyed out.
- Start on login and the trail overlay are separate toggles on the third Preferences page.

### Packages

| Package | Architectures | Target |
| --- | --- | --- |
| `.rpm` | x86_64, aarch64 | Fedora |
| `.deb` | amd64, arm64 | Debian 13+, Ubuntu 25.04+ |

Install with `sudo dnf install ./weazystroke-*.rpm` or `sudo apt install ./weazystroke_*.deb`, then reload udev and join the `input` group (see the README).
