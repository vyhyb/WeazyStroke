## WeazyStroke 0.2.1

Maintenance release of the WeazyStroke fork, software originally created by nine7nine: gesture recognition for Wayland, built on the easystroke recognition core.

- Grab mode (`--grab`) now keeps working across suspend/resume, docking and replugging: mice that appear after startup are grabbed, vanished ones are released (this also fixes the daemon spinning at 100% CPU), and the daemon starts even before any mouse is present.
- WeazyStroke now ships AppStream metadata, so it is listed in GNOME Software and KDE Discover.
