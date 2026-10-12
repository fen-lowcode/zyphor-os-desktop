"""
Page registry
=============
Every file inside the ``pages/`` folder can declare a page with the
``@page(...)`` decorator. The app discovers those files automatically and
builds the sidebar from them, so there is NOTHING else to edit when you add
a page.

    from core.registry import page

    @page(id="hello", title="Hello", section="Extras", icon="★", order=100)
    class HelloPage(BasePage):
        ...

Arguments
---------
id       unique short name (used internally)
title    text shown in the sidebar and as the page heading
section  sidebar group heading ("" = no heading)
icon     one character shown before the title
order    position in the sidebar (lower = higher up)
"""
from __future__ import annotations

import importlib
import pkgutil
import sys
import traceback
from dataclasses import dataclass


@dataclass(frozen=True)
class PageInfo:
    id: str
    title: str
    section: str
    icon: str
    order: int
    cls: type


_PAGES: dict[str, PageInfo] = {}


def page(id: str, title: str, section: str = "General", icon: str = "•", order: int = 100):
    """Class decorator that registers a page."""

    def decorator(cls):
        if id in _PAGES and _PAGES[id].cls is not cls:
            raise ValueError(f"Duplicate page id '{id}'")
        cls.page_id = id
        cls.page_title = title
        _PAGES[id] = PageInfo(id, title, section, icon, order, cls)
        return cls

    return decorator


def discover(package: str = "pages") -> None:
    """Import every module in the pages package (files starting with '_' are skipped)."""
    pkg = importlib.import_module(package)
    for mod in pkgutil.iter_modules(pkg.__path__):
        if mod.name.startswith("_"):
            continue
        try:
            importlib.import_module(f"{package}.{mod.name}")
        except Exception:
            # One broken page must never stop the whole app from starting.
            print(f"[zyphor] could not load page module '{mod.name}':", file=sys.stderr)
            traceback.print_exc()


def all_pages() -> list[PageInfo]:
    """Pages sorted by `order`, then grouped so each section stays together."""
    ordered = sorted(_PAGES.values(), key=lambda p: (p.order, p.title))
    section_rank: dict[str, int] = {}
    for p in ordered:
        section_rank.setdefault(p.section, len(section_rank))
    return sorted(ordered, key=lambda p: section_rank[p.section])  # stable sort keeps `order`


def get(page_id: str) -> PageInfo:
    return _PAGES[page_id]
