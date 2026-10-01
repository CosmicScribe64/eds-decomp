#!/usr/bin/env python3
"""Health-check the wiki: broken links, orphans, frontmatter, index coverage."""
import re, sys
from pathlib import Path

WIKI = Path(__file__).resolve().parent.parent / "wiki"
LINK = re.compile(r"\[\[([^\]|#]+)(?:[#|][^\]]*)?\]\]")
REQUIRED = ("title", "type", "status", "confidence", "updated")

pages = {p.stem: p for p in WIKI.rglob("*.md")}
inbound = {name: set() for name in pages}
broken, bad_fm = [], []

for name, path in pages.items():
    text = path.read_text()
    if not text.startswith("---"):
        bad_fm.append((name, "no frontmatter"))
    else:
        fm = text.split("---", 2)[1]
        missing = [k for k in REQUIRED if not re.search(rf"^{k}:", fm, re.M)]
        if missing:
            bad_fm.append((name, "missing " + ", ".join(missing)))
    for target in LINK.findall(text):
        target = target.strip()
        if target in pages:
            if target != name:
                inbound[target].add(name)
        else:
            broken.append((name, target))

index_links = set(LINK.findall(pages["index"].read_text())) if "index" in pages else set()
special = {"index", "log", "overview"}
orphans = [n for n, src in inbound.items() if not src and n not in special]
unindexed = [n for n in pages if n not in index_links and n not in special]

def section(title, items):
    print(f"\n## {title} ({len(items)})")
    for i in items:
        print("  -", i if isinstance(i, str) else f"{i[0]} -> {i[1]}")

print(f"# wiki lint: {len(pages)} pages")
section("Broken links / wanted pages", sorted(broken))
section("Orphans (no inbound links)", sorted(orphans))
section("Not in index.md", sorted(unindexed))
section("Frontmatter problems", sorted(bad_fm))
sys.exit(1 if bad_fm else 0)
