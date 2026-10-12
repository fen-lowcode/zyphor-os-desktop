# Zyphor Command Center – Developer Guide

## Add a new page (3 steps)

1. Copy `pages/_template.py` to a new file in the same folder, e.g. `pages/network_status.py`
   (the name must **not** start with `_`).
2. Keep one of the examples, edit the `@page(...)` line and the class.
3. Restart the app. The page shows up in the sidebar by itself.

You never edit `app.py` or the sidebar.

### The smallest possible page

```python
from core.registry import page
from ui.components.command_page import CommandPage


@page(id="uptime", title="Uptime", section="System", icon="⏱", order=13)
class UptimePage(CommandPage):
    description = "How long this computer has been running."
    button_text = "Show uptime"
    command = "uptime -p"
```

### `@page(...)` options

| option    | meaning                                                              |
|-----------|----------------------------------------------------------------------|
| `id`      | unique short name                                                    |
| `title`   | sidebar text and page heading                                        |
| `section` | sidebar group heading (`""` = no heading, a new name = a new group)  |
| `icon`    | one character shown before the title                                 |
| `order`   | position in the sidebar, lower = higher up                           |

Current order numbers: Home 0 · Account 5 · System 10-19 · Doctor 20-29 · Packages 30-39.

## Page types (all in `ui/components/`)

| class              | use it when…                                                        |
|--------------------|---------------------------------------------------------------------|
| `CommandPage`      | a button runs a command and its output is shown live                |
| `InputCommandPage` | same, plus a text box; `{value}` in `command` is the (safely quoted) text |
| `WebPage`          | you want to show a website or local HTML file (`url = ...`)         |
| `BasePage`         | you want to build your own layout: add widgets to `self.body`       |

Useful `CommandPage` settings: `command`, `button_text`, `loading_text`, `description`,
`auto_run = True` (run when first opened). Useful `InputCommandPage` settings:
`placeholder`, `empty_message`, `confirm = "Remove '{value}'?"` (yes/no popup).

Hooks you can override: `on_first_show()`, `on_show()`, and for command pages
`build_command()` and `on_finished(exit_code)` (see `pages/doctor_report.py`).

## Change the look

Everything visual is in `styles/theme.py`:

* edit the colours in `PALETTES` (one place, whole app follows);
* switch palette with `ZYPHOR_THEME=light zyphor-command-center` or change `DEFAULT_THEME`;
* button styles: `variant` property `primary`, `danger`, `ghost` (see `ui/components/command_page.py`).

## Layout of the project

```
main.py              starts the app and applies the theme
app.py               main window (sidebar + page area) - rarely needs edits
pages/               ONE FILE PER PAGE  <- this is where you work
ui/components/       reusable parts (CommandPage, WebPage, Console, ...)
ui/sidebar.py        the menu, built automatically from the pages
core/registry.py     the @page decorator + auto-discovery
core/process.py      runs shell commands and streams output
core/webserver.py    local PHP server used by the User Management page
styles/theme.py      colours and stylesheet
```

## Developing without installing

```
cd usr/share/zyphor-command-center
python3 main.py
```
(For the User Management page set `ZYPHOR_WEB_DIR` to your web folder.)
Installed files are read-only (root, mode 555), so edit the source tree, then rebuild:

```
dpkg-deb --build --root-owner-group zyphor-command-center
```

## Safety note

Many commands run through `pkexec` (as root). Never paste user text into a command
yourself – use `InputCommandPage` and `{value}`, which quotes it for you.
