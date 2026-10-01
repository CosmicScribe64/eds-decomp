#!/usr/bin/env python3
"""Extract card-related data tables from the EDS (AY5E) baserom.

Stdlib only. Reads the ROM at runtime and prints to stdout; it never writes
game text or graphics into the repo.

Usage:
    python3 tools/extract_cards.py [--rom baserom.gba] <table> [--format json|csv] [--desc]
    python3 tools/extract_cards.py --verify

Tables: cards, duelists, decks, packs, starter, lists, all

See wiki/data/card-table.md and the pages it links for the layouts.
"""
import argparse
import csv
import json
import os
import struct
import sys

ROM_BASE = 0x08000000

# ---- per-card tables, all indexed by card ID (1..820, slot 0 = empty) ----
NUM_CARD_SLOTS = 821                 # IDs 0..820
CARD_NAMES      = 0x0822C720         # char[821][0x40]   (slot 0 empty; ID 1 at 0x0822C760)
CARD_NAMES_2    = 0x08239460         # char[821][0x40]   all zero (unused second name bank)
CARD_DESCS      = 0x082461A0         # char[821][0x1E0]  (ID 1 at 0x08246380)
CARD_ART        = 0x082A6500         # u8[821][0x10E0]   6bpp packed, 72x80 px, tiled 9x10
CARD_ART_PALS   = 0x08608360         # u16[821][64]      BGR555
CARD_STATS      = 0x08621DE0         # u32[821]          packed bitfield
CARD_ID_TO_NO   = 0x08622AB4         # u16[821]          ID -> card number (0xFFFF for slot 0)
CARD_PASSWORDS  = 0x08623120         # u8[821][4]        BCD, big-endian digit order; FF.. = none
CARD_NO_TO_ID   = 0x08623DF4         # u16[2048]         card number -> ID (0 = not in game)
CARD_JP_SORT    = 0x08624DF4         # u16[821]          JP (kana) sort key, hypothesis

# ---- other tables ----
TYPE_NAMES      = 0x081988D0         # const char *[25]
SPELL_SUFFIXES  = 0x08198934         # const char *[8]  (index 0 = NULL)
ATTR_NAMES      = 0x0819D264         # const char *[6]  (attr 1..6 -> index 0..5)
DUELISTS        = 0x08139F64         # struct[28], 0x84 bytes each
NUM_DUELISTS    = 28
DECKS_MAIN      = 0x0819DC6C         # {u16 *cards; u32 n}[25], index = duelist id
DECKS_ALT       = 0x0819DD34         # {u16 *cards; u32 n}[6],  index = duelist id 1..5
PACK_INFO       = 0x080865DC         # {u32 id; u8 *image; char name[0x40]}[23]
NUM_PACK_INFO   = 23
PACK_CONTENTS   = 0x081A562C         # {void *slots; u16 id; u16 pad}[28]
NUM_PACK_CONTENTS = 28
RARITY_THRESH   = 0x081A570C         # s32[8] cumulative, out of 180
PACK_ORDER      = 0x080819BE         # u16[28] pack ids, display order
STARTER_POOLS   = 0x08198744         # {u16 *pool; u32 packed}[11]
NUM_STARTER_POOLS = 11
AI_LIST_1       = 0x0819D2FC         # u16[13] card numbers
AI_LIST_2       = 0x0819D316         # u16[26] card numbers
WANTED_LIST     = 0x08081A6C         # u16[60] card numbers

KIND_NAMES = {0: "Normal", 1: "Effect", 2: "Fusion", 3: "Ritual"}


class Rom:
    def __init__(self, path):
        with open(path, "rb") as f:
            self.data = f.read()

    def off(self, addr):
        return addr - ROM_BASE

    def u16(self, addr):
        return struct.unpack_from("<H", self.data, self.off(addr))[0]

    def u32(self, addr):
        return struct.unpack_from("<I", self.data, self.off(addr))[0]

    def u16s(self, addr, n):
        return list(struct.unpack_from("<%dH" % n, self.data, self.off(addr)))

    def cstr(self, addr, maxlen=0x400):
        o = self.off(addr)
        return self.data[o:o + maxlen].split(b"\0")[0].decode("latin-1")


def decode_stats(v):
    """Split the packed u32 from CARD_STATS into fields."""
    typ = (v >> 20) & 0x1F
    d = {
        "raw": "0x%08X" % v,
        "type": typ,
        "attribute": v >> 29,
        "level": (v >> 25) & 0xF,
    }
    if typ in (21, 22):          # Trap, Magic: bits 17-19 = subtype
        d["subtype"] = (v >> 17) & 7
        d["atk"] = d["def"] = None
        d["kind"] = None
    else:
        d["atk"] = ((v >> 9) & 0x1FF) * 10
        d["def"] = (v & 0x1FF) * 10
        d["kind"] = (v >> 18) & 3
        d["subtype"] = None
    return d


class Eds:
    def __init__(self, rom):
        self.rom = rom
        self.id_to_no = rom.u16s(CARD_ID_TO_NO, NUM_CARD_SLOTS)
        self.no_to_id = rom.u16s(CARD_NO_TO_ID, 2048)
        self.types = [rom.cstr(rom.u32(TYPE_NAMES + 4 * i)) for i in range(25)]
        self.suffixes = [rom.cstr(p) if p else "" for p in
                         (rom.u32(SPELL_SUFFIXES + 4 * i) for i in range(7))]
        self.attrs = [rom.cstr(rom.u32(ATTR_NAMES + 4 * i)) for i in range(6)]

    def name(self, cid):
        return self.rom.cstr(CARD_NAMES + cid * 0x40, 0x40)

    def desc(self, cid):
        return self.rom.cstr(CARD_DESCS + cid * 0x1E0, 0x1E0)

    def id_from_no(self, no):
        """Mirror of the lookup at 0x08000FC0: numbers >= 2000 are alternate-art copies."""
        if no == 0xFFFF:
            return 0
        if no <= 1999:
            return self.no_to_id[no & 0x7FF]
        return self.no_to_id[(no - 2000) & 0x7FF] + 1

    def password(self, cid):
        o = self.rom.off(CARD_PASSWORDS + cid * 4)
        b = self.rom.data[o:o + 4]
        if b == b"\xff\xff\xff\xff" or b == b"\0\0\0\0":
            return None
        return b.hex()

    def card(self, cid, with_desc=False):
        st = decode_stats(self.rom.u32(CARD_STATS + cid * 4))
        d = {
            "id": cid,
            "card_no": self.id_to_no[cid],
            "name": self.name(cid),
            "type_name": self.types[st["type"]] if st["type"] < 25 else None,
            "attr_name": (self.attrs[st["attribute"] - 1]
                          if 1 <= st["type"] <= 20 and 1 <= st["attribute"] <= 6 else None),
            "password": self.password(cid),
            "jp_sort_key": self.rom.u16(CARD_JP_SORT + cid * 2),
        }
        d.update(st)
        if st["kind"] is not None:
            d["kind_name"] = KIND_NAMES[st["kind"]]
        if st["subtype"] is not None:
            d["subtype_name"] = self.suffixes[st["subtype"]].lstrip("/") or "Normal"
        if with_desc:
            d["desc"] = self.desc(cid)
        return d

    def cards(self, with_desc=False):
        return [self.card(i, with_desc) for i in range(1, NUM_CARD_SLOTS)]

    def cardlist(self, addr, n):
        out = []
        for no in self.rom.u16s(addr, n):
            cid = self.id_from_no(no)
            out.append({"card_no": no, "id": cid, "name": self.name(cid) if cid else None})
        return out

    def duelists(self):
        out = []
        for i in range(NUM_DUELISTS):
            a = DUELISTS + i * 0x84
            out.append({"index": i, "id": self.rom.u32(a),
                        "name": self.rom.cstr(a + 4, 0x40),
                        "short_name": self.rom.cstr(a + 0x44, 0x40)})
        return out

    def decks(self):
        out = []
        for label, base, n in (("main", DECKS_MAIN, 25), ("alt", DECKS_ALT, 6)):
            for i in range(n):
                p, cnt = struct.unpack_from("<II", self.rom.data, self.rom.off(base + 8 * i))
                if not p:
                    continue
                out.append({"table": label, "duelist_id": i, "addr": "0x%08X" % p,
                            "count": cnt, "cards": self.cardlist(p, cnt)})
        return out

    def packs(self):
        names = {}
        for i in range(NUM_PACK_INFO):
            a = PACK_INFO + i * 0x48
            names[self.rom.u32(a)] = (self.rom.cstr(a + 8, 0x40), "0x%08X" % self.rom.u32(a + 4))
        out = []
        for i in range(NUM_PACK_CONTENTS):
            hdr = PACK_CONTENTS + i * 8
            p = self.rom.u32(hdr)
            pid = self.rom.u16(hdr + 4)
            slots = []
            for s in range(8):
                lp, cnt = struct.unpack_from("<Ii", self.rom.data, self.rom.off(p + 8 * s))
                slots.append({"rarity_slot": s, "count": cnt,
                              "cards": self.cardlist(lp, cnt) if cnt else []})
            nm, img = names.get(pid, (None, None))
            out.append({"pack_id": pid, "name": nm, "image": img, "addr": "0x%08X" % p,
                        "slots": slots})
        return {"rarity_thresholds_of_180": list(struct.unpack_from(
                    "<8i", self.rom.data, self.rom.off(RARITY_THRESH))),
                "display_order": self.rom.u16s(PACK_ORDER, 28),
                "packs": out}

    def starter(self):
        out = []
        for i in range(NUM_STARTER_POOLS):
            p, w = struct.unpack_from("<II", self.rom.data, self.rom.off(STARTER_POOLS + 8 * i))
            n = w & 0x3FF
            out.append({"group": i, "pool_size": n,
                        "picks": [(w >> 10) & 31, (w >> 15) & 31, (w >> 20) & 31],
                        "flags_hi": w >> 25,
                        "pool": self.cardlist(p, n)})
        return out

    def lists(self):
        return {"ai_list_1@0x0819D2FC": self.cardlist(AI_LIST_1, 13),
                "ai_list_2@0x0819D316": self.cardlist(AI_LIST_2, 26),
                "wanted_list@0x08081A6C": self.cardlist(WANTED_LIST, 60)}


def verify(eds):
    """Spot checks against publicly known card facts. Returns number of failures."""
    known = [
        # name, atk, def, level, type, attribute, kind, password
        ("Blue-Eyes White Dragon", 3000, 2500, 8, "Dragon", "LIGHT", 0, "89631139"),
        ("Dark Magician", 2500, 2100, 7, "Spellcaster", "DARK", 0, "46986414"),
        ("Red-Eyes B. Dragon", 2400, 2000, 7, "Dragon", "DARK", 0, "74677422"),
        ("Summoned Skull", 2500, 1200, 6, "Fiend", "DARK", 0, "70781052"),
        ("Celtic Guardian", 1400, 1200, 4, "Warrior", "EARTH", 0, None),
        ("Mystical Elf", 800, 2000, 4, "Spellcaster", "LIGHT", 0, None),
        ("Kuriboh", 300, 200, 1, "Fiend", "DARK", 1, None),
        ("7 Colored Fish", 1800, 800, 4, "Fish", "WATER", 0, "23771716"),
        ("Flame Swordsman", 1800, 1600, 5, "Warrior", "FIRE", 2, None),
        ("Harpie Lady", 1300, 1400, 4, "Winged Beast", "WIND", 0, None),
        ("Jinzo", 2400, 1500, 6, "Machine", "DARK", 1, None),
        ("Man-Eater Bug", 450, 600, 2, "Insect", "EARTH", 1, None),
        ("Black Luster Soldier", 3000, 2500, 8, "Warrior", "EARTH", 3, None),
    ]
    by_name = {}
    for c in eds.cards():
        by_name.setdefault(c["name"], c)
    fails = 0
    for nm, atk, df, lv, ty, at, kind, pw in known:
        c = by_name.get(nm)
        got = None if c is None else (c["atk"], c["def"], c["level"], c["type_name"],
                                      c["attr_name"], c["kind"])
        exp = (atk, df, lv, ty, at, kind)
        ok = got == exp and (pw is None or c["password"] == pw)
        fails += not ok
        print("%-4s %-24s expected %s got %s" % ("ok" if ok else "FAIL", nm, exp, got))
    # structural checks
    for cid in range(1, NUM_CARD_SLOTS):
        no = eds.id_to_no[cid]
        if eds.id_from_no(no) != cid:
            print("FAIL id/no round trip for id", cid)
            fails += 1
    subs = {c["name"]: c.get("subtype_name") for c in eds.cards() if c["subtype"] is not None}
    for nm, st in (("Forest", "Field"), ("Toon World", "Continuous"), ("Mystical Space Typhoon", "Quick"),
                   ("Magic Jammer", "Counter"), ("Call Of The Haunted", "Continuous"),
                   ("Black Luster Ritual", "Ritual"), ("Axe of Despair", "Equip"), ("Dark Hole", "Normal")):
        ok = subs.get(nm) == st
        fails += not ok
        print("%-4s %-24s subtype %s got %s" % ("ok" if ok else "FAIL", nm, st, subs.get(nm)))
    print("failures:", fails)
    return fails


def main():
    here = os.path.dirname(os.path.abspath(__file__))
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("table", nargs="?", default="cards",
                    choices=["cards", "duelists", "decks", "packs", "starter", "lists", "all"])
    ap.add_argument("--rom", default=os.path.join(here, "..", "baserom.gba"))
    ap.add_argument("--format", choices=["json", "csv"], default="json")
    ap.add_argument("--desc", action="store_true", help="include card descriptions")
    ap.add_argument("--verify", action="store_true", help="run spot checks and exit")
    a = ap.parse_args()

    eds = Eds(Rom(a.rom))
    if a.verify:
        sys.exit(1 if verify(eds) else 0)

    if a.table == "cards" and a.format == "csv":
        rows = eds.cards(a.desc)
        w = csv.DictWriter(sys.stdout, fieldnames=list(rows[0].keys()) +
                           [k for k in ("kind_name", "subtype_name") if k not in rows[0]],
                           extrasaction="ignore")
        w.writeheader()
        for r in rows:
            w.writerow(r)
        return
    if a.format == "csv":
        sys.exit("csv output is only supported for 'cards'")

    fn = {"cards": lambda: eds.cards(a.desc), "duelists": eds.duelists, "decks": eds.decks,
          "packs": eds.packs, "starter": eds.starter, "lists": eds.lists}
    out = {k: f() for k, f in fn.items()} if a.table == "all" else fn[a.table]()
    json.dump(out, sys.stdout, indent=1)
    print()


if __name__ == "__main__":
    try:
        main()
    except BrokenPipeError:          # e.g. piped into `head`
        sys.stderr.close()
