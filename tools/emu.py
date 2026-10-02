#!/usr/bin/env python3
"""Scripted headless GBA emulation (mGBA 0.10.5) for dynamic analysis of EDS.

Runs inside the eds-emu Docker image (docker build -t eds-emu docker/emu/); when started on the host it
re-executes itself in a container with the repo mounted at /work, like tools/dr. All output goes to
build/emu/<run>/. Full documentation: build/emu/README.md.

  tools/emu.py run     [opts] --shot 60,120 --dump end        screenshots / RAM dumps / savestates
  tools/emu.py trace   [opts] [--calls]                        which functions run (hits, first frame)
  tools/emu.py watch   [opts] ADDR[:LEN[:r|w|rw]] ...          memory watchpoints (PC, frame, old/new)
  tools/emu.py break   [opts] FUNC|ADDR ... [--deref r0:32]    log registers each time an address executes
  tools/emu.py states  list | build [NAME...|--all] | show NAME | diff DIR_A DIR_B
  tools/emu.py lua     SCRIPT.lua [opts]                       run an mGBA Lua script headless
  tools/emu.py gdb     [opts] [-x FILE] [--ex CMD]             mGBA GDB stub + gdb-multiarch (batch)
  tools/emu.py compare [opts] --rom-b eds                      same run on two ROMs, compare RAM/video
  tools/emu.py tracediff RUN_A RUN_B                           functions that run in one trace but not the other
  tools/emu.py peek    [opts] ADDR[:LEN] ...                   print memory after loading a state / running
  tools/emu.py sym     NAME|ADDR ...                           resolve symbols the way the other commands do
  tools/emu.py selftest [--rom eds]                            analysis modes leave emulation unchanged; gdb works

Common options: --rom base|eds|PATH, --state NAME|PATH, --sram PATH, --frames N, --input FILE,
--keys "SPEC" (repeatable), --out NAME, --idle ignore|detect|remove, --shot FRAMES, --dump FRAMES,
--regions LIST, --savestate FRAME[:NAME], --sheet.
"""
import argparse
import bisect
import hashlib
import json
import os
import re
import secrets
import shutil
import subprocess
import sys
import time
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
OUT_ROOT = REPO / "build" / "emu"
STATES_DIR = OUT_ROOT / "states"
RECIPES_DIR = REPO / "tools" / "emu" / "states"
HARNESS_SRC = REPO / "tools" / "emu" / "emuh.c"
BIN_DIR = OUT_ROOT / ".bin"
IMAGE = os.environ.get("EDS_EMU_IMAGE", "eds-emu:latest")
MGBA_PREFIX = os.environ.get("MGBA_PREFIX", "/opt/mgba")

ROMS = {"base": REPO / "baserom.gba", "eds": REPO / "eds.gba"}

KEYS = {"A": 1, "B": 2, "SELECT": 4, "START": 8, "RIGHT": 16, "LEFT": 32, "UP": 64, "DOWN": 128, "R": 256, "L": 512}
KEY_ALIASES = {"SEL": "SELECT", "STA": "START", "RT": "RIGHT", "LT": "LEFT", "DN": "DOWN"}

REGIONS = {
    "ewram": (0x02000000, 0x40000),
    "iwram": (0x03000000, 0x8000),
    "io": (0x04000000, 0x400),
    "pal": (0x05000000, 0x400),
    "vram": (0x06000000, 0x18000),
    "oam": (0x07000000, 0x400),
}
ALL_REGIONS = list(REGIONS) + ["sram"]

# Code the game copies to RAM and runs there (see wiki/functions/crt0.md, wiki/rom/ram-map.md):
# (RAM address, length, ROM source). PCs in these ranges are reported as the ROM address.
RAM_ALIASES = [
    (0x0300004C, 0x400, 0x080000FC),  # IntrMain copy (GameInit DMA3 0x080000FC -> 0x0300004C)
    (0x03005A54, 0xE0, 0x0807EC1C),   # sound mixer inner loop (gSoundMixCodeRam)
]

# Used when build/eds.map is missing (crt0 region is not in config/functions.tsv).
BUILTIN_SYMBOLS = [(0x08000000, 0xC0, "RomHeader"), (0x080000C0, 0x3C, "crt0"), (0x080000FC, 0x12C, "IntrMain")]


def die(msg):
    print(f"emu.py: error: {msg}", file=sys.stderr)
    sys.exit(2)


# --------------------------------------------------------------------------- container dispatch

def in_container():
    return os.environ.get("EDS_EMU_INNER") == "1" or os.environ.get("EDS_NATIVE") == "1"


def reexec_in_docker():
    if shutil.which("docker") is None:
        die("docker not found; build the image with `docker build -t eds-emu docker/emu/` "
            "or set EDS_NATIVE=1 with mGBA installed under $MGBA_PREFIX")
    cwd = Path.cwd().resolve()
    try:
        rel = cwd.relative_to(REPO)
        workdir = "/work/" + str(rel) if str(rel) != "." else "/work"
    except ValueError:
        workdir = "/work"
        print("emu.py: note: running from outside the repo; relative paths resolve against the repo root",
              file=sys.stderr)
    tty = ["-t"] if sys.stdin.isatty() and sys.stdout.isatty() else []
    cmd = ["docker", "run", "--rm", "-i", *tty, "-u", f"{os.getuid()}:{os.getgid()}",
           "-v", f"{REPO}:/work", "-w", workdir, "-e", "EDS_EMU_INNER=1", "-e", "HOME=/tmp",
           IMAGE, "python3", "/work/tools/emu.py", *sys.argv[1:]]
    try:
        os.execvp("docker", cmd)
    except OSError as e:
        die(f"cannot run docker: {e}")


# --------------------------------------------------------------------------- harness

def harness_path():
    """Compile tools/emu/emuh.c against libmgba once per source version (cached in build/emu/.bin)."""
    src = HARNESS_SRC.read_bytes()
    flags = ["-O2", "-g", "-std=gnu11", "-Wall", "-Wno-unused-function", f"-I{MGBA_PREFIX}/include"]
    libs = [f"-L{MGBA_PREFIX}/lib", f"-Wl,-rpath,{MGBA_PREFIX}/lib", "-lmgba"]
    ver = ""
    for cand in (Path(MGBA_PREFIX) / "lib").glob("libmgba.so.*.*.*"):
        ver = f"{cand.name}:{cand.stat().st_size}"
    fl = Path(MGBA_PREFIX) / "include" / "mgba" / "flags.h"
    build_flags = fl.read_bytes() if fl.exists() else b""  # struct layouts depend on the library's build flags
    key = hashlib.sha1(src + build_flags + " ".join(flags + libs + [ver]).encode()).hexdigest()[:12]
    exe = BIN_DIR / f"emuh-{key}"
    if exe.exists():
        return exe
    BIN_DIR.mkdir(parents=True, exist_ok=True)
    tmp = BIN_DIR / f".emuh-{key}-{os.getpid()}-{secrets.token_hex(3)}"
    r = subprocess.run(["gcc", *flags, str(HARNESS_SRC), *libs, "-o", str(tmp)], capture_output=True, text=True)
    if r.returncode:
        sys.stderr.write(r.stdout + r.stderr)
        die("harness compile failed")
    os.replace(tmp, exe)
    return exe


def run_harness(job_path, run_dir, quiet=False):
    exe = harness_path()
    t0 = time.time()
    r = subprocess.run([str(exe), str(job_path)], capture_output=True, text=True)
    (run_dir / "harness.log").write_text(r.stdout + r.stderr)
    if r.returncode:
        sys.stderr.write(r.stdout + r.stderr)
        die(f"harness failed (exit {r.returncode}); job: {rel(job_path)}")
    if not quiet:
        for line in r.stdout.splitlines():
            if line.startswith("[lua]") or line.startswith("emuh:"):
                print(line)
        for line in r.stderr.splitlines():
            print(line, file=sys.stderr)
    return time.time() - t0


# --------------------------------------------------------------------------- symbols

class Symbols:
    """Function symbols from config/functions.tsv (+ libgcc/crt0 from build/eds.map), names from config/names.txt."""

    def __init__(self):
        self.funcs = {}  # addr -> dict(name, size, unit, mode)
        tsv = REPO / "config" / "functions.tsv"
        for line in tsv.read_text().splitlines():
            if not line or line.startswith("#"):
                continue
            p = line.split("\t")
            try:
                addr = int(p[0], 16)
            except ValueError:
                continue
            self.funcs[addr] = {"name": p[3], "size": int(p[2], 16), "unit": p[4] if len(p) > 4 else "",
                                "mode": p[1]}
        self._add_map_symbols()
        self.proposed = {}  # addr -> proposed name (config/names.txt)
        self.byname = {}
        names = REPO / "config" / "names.txt"
        if names.exists():
            for line in names.read_text().splitlines():
                m = re.match(r"\s*(0x[0-9A-Fa-f]+)\s+([A-Za-z_][\w.]*)", line)
                if m:
                    a = int(m.group(1), 16)
                    self.proposed[a] = m.group(2)
                    self.byname.setdefault(m.group(2), a)
        for a, f in self.funcs.items():
            self.byname[f["name"]] = a
        self.starts = sorted(self.funcs)

    def _add_map_symbols(self):
        mp = REPO / "build" / "eds.map"
        extra = []
        if mp.exists():
            syms = []
            for line in mp.read_text(errors="replace").splitlines():
                m = re.match(r"^\s+0x(0*8[0-9a-f]{6})\s+([A-Za-z_][\w.]*)\s*$", line)
                if m:
                    syms.append((int(m.group(1), 16), m.group(2)))
            syms = sorted(set(syms))
            for i, (a, name) in enumerate(syms):
                if a in self.funcs or a >= 0x08080A20:
                    continue
                nxt = next((b for b, _ in syms[i + 1:] if b > a), a + 4)
                extra.append((a, nxt - a, name))
        else:
            extra = BUILTIN_SYMBOLS
        covered = sorted((a, a + f["size"]) for a, f in self.funcs.items())
        for a, size, name in extra:
            i = bisect.bisect_right(covered, (a, 1 << 40)) - 1
            if i >= 0 and covered[i][0] <= a < covered[i][1]:
                continue  # inside a known function (local label)
            self.funcs[a] = {"name": name, "size": size, "unit": "(asm/lib)", "mode": "?"}

    def func_at(self, addr):
        """Function containing addr (after RAM-alias mapping), or None."""
        addr = map_alias(addr) & ~1
        i = bisect.bisect_right(self.starts, addr) - 1
        if i >= 0:
            a = self.starts[i]
            f = self.funcs[a]
            if addr < a + max(f["size"], 2):
                return a, f
        return None

    def label(self, addr, with_proposed=True):
        """'name+0xoff' for code, a region tag otherwise."""
        if addr is None:
            return ""
        hit = self.func_at(addr)
        if hit:
            a, f = hit
            name = f["name"]
            if with_proposed and a in self.proposed and self.proposed[a] != name:
                name += f"[{self.proposed[a]}]"
            off = (map_alias(addr) & ~1) - a
            return name if off == 0 else f"{name}+0x{off:X}"
        if addr in self.proposed:
            return self.proposed[addr]
        if addr < 0x4000:
            return "BIOS"
        return region_name(addr)

    def func_name(self, a):
        f = self.funcs.get(a)
        return f["name"] if f else f"0x{a:08X}"

    def resolve(self, text):
        """Hex/decimal number, a function name, a names.txt name, optionally +offset."""
        t = text.strip()
        m = re.match(r"^(.*?)([+-](?:0x[0-9a-fA-F]+|\d+))?$", t)
        base, off = m.group(1), m.group(2)
        try:
            val = int(base, 0)
        except ValueError:
            if base not in self.byname:
                die(f"unknown symbol '{base}' (not in config/functions.tsv, build/eds.map or config/names.txt)")
            val = self.byname[base]
        if off:
            val += int(off, 0)
        return val


def map_alias(addr):
    for ram, ln, rom in RAM_ALIASES:
        if ram <= addr < ram + ln:
            return rom + (addr - ram)
    return addr


def region_name(addr):
    names = {0: "bios", 2: "ewram", 3: "iwram", 4: "io", 5: "pal", 6: "vram", 7: "oam", 8: "rom", 9: "rom",
             0xE: "sram"}
    return names.get(addr >> 24, "?")


_SYMS = None


def syms():
    global _SYMS
    if _SYMS is None:
        _SYMS = Symbols()
    return _SYMS


# --------------------------------------------------------------------------- inputs and frame specs

def parse_keys(text, where):
    t = text.upper()
    if t in ("-", "NONE", "WAIT"):
        return 0
    mask = 0
    for k in t.split("+"):
        k = KEY_ALIASES.get(k, k)
        if k not in KEYS:
            die(f"{where}: unknown key '{k}' (keys: {' '.join(KEYS)})")
        mask |= KEYS[k]
    return mask


def parse_input(lines, where):
    """Input script -> (ranges [(first,last,mask)], meta). See README 'Input scripts'."""
    ranges, meta = [], {}
    prev = 0
    for n, raw in enumerate(lines, 1):
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        parts = line.split()
        loc = f"{where}:{n}"
        head = parts[0].lower()
        if head in ("frames", "from", "stopat", "until", "settle", "title", "note"):
            meta[head] = parts[1:] if head == "stopat" else " ".join(parts[1:])
            continue
        if len(parts) != 2:
            die(f"{loc}: expected '<when> <keys>', got '{line}'")
        when, keys = parts
        m = re.match(r"^(\+?)(\d+)(?:-(\d+)|:(\d+))?(?:/(\d+))?$", when)
        if not m:
            die(f"{loc}: bad frame spec '{when}' (N, N-M, N:LEN, +D, +D:LEN, with optional /EVERY)")
        start = int(m.group(2)) + (prev if m.group(1) else 0)
        if m.group(3):
            end = int(m.group(3)) + (prev if m.group(1) else 0)
        elif m.group(4):
            end = start + int(m.group(4)) - 1
        else:
            end = start
        if end < start:
            die(f"{loc}: range ends before it starts")
        mask = parse_keys(keys, loc)
        if mask and m.group(5):
            every = int(m.group(5))
            if every < 2:
                die(f"{loc}: /EVERY must be at least 2 (a key must be released between taps)")
            ranges.extend((f, f, mask) for f in range(start, end + 1, every))
        elif mask:
            ranges.append((start, end, mask))
        prev = start
    meta["last_input_frame"] = max([e for _, e, _ in ranges], default=prev)
    return ranges, meta


def parse_frames(spec):
    """'60,120,200-400:50,end' -> sorted list of ints plus possibly 'end'."""
    out = []
    if not spec:
        return out
    for part in spec.split(","):
        part = part.strip()
        if not part:
            continue
        if part == "end":
            out.append("end")
            continue
        m = re.match(r"^(\d+)(?:-(\d+)(?::(\d+))?)?$", part)
        if not m:
            die(f"bad frame list item '{part}' (N, A-B, A-B:STEP, end)")
        a = int(m.group(1))
        if m.group(2):
            b, step = int(m.group(2)), int(m.group(3) or 1)
            out.extend(range(a, b + 1, step))
        else:
            out.append(a)
    ints = sorted(set(x for x in out if x != "end"))
    return ints + (["end"] if "end" in out else [])


def ftag(f):
    return "end" if f == "end" else f"f{f:05d}"


# --------------------------------------------------------------------------- run setup

def rel(p):
    try:
        return str(Path(os.path.abspath(p)).relative_to(REPO))
    except ValueError:
        return str(p)


def rom_path(spec):
    p = ROMS.get(spec, Path(spec))
    p = p if p.is_absolute() else (Path.cwd() / p)
    if not p.exists():
        die(f"ROM not found: {spec}" + (" (run `tools/dr make compare` to build eds.gba)" if spec == "eds" else ""))
    return Path(os.path.abspath(p))  # keep the baserom.gba symlink name in reports


def state_path(spec):
    if spec is None:
        return None
    p = Path(spec)
    if p.suffix or "/" in spec:
        p = p if p.is_absolute() else Path.cwd() / p
    else:
        p = STATES_DIR / f"{spec}.ss"
        recipes = load_recipes()
        if spec in recipes:
            meta = p.with_suffix(".json")
            fresh = p.exists() and meta.exists() and \
                json.loads(meta.read_text()).get("recipe_sha1") == recipes[spec]["sha1"]
            if not fresh:
                print(f"emu.py: building library state '{spec}' first (tools/emu.py states build {spec})",
                      file=sys.stderr)
                build_chain([spec], recipes, rom_path("base"), STATES_DIR, force=False, out=sys.stderr)
    if not p.exists():
        names = sorted(load_recipes())
        die(f"state not found: {spec}. Library states (tools/emu/states/*.txt): {', '.join(names) or '(none)'}; "
            "or pass a .ss path")
    meta = p.with_suffix(".json")
    return p.resolve(), (json.loads(meta.read_text()) if meta.exists() else None)


def sha1_file(p):
    h = hashlib.sha1()
    with open(p, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def make_run_dir(args, cmd):
    name = args.out or f"{cmd}-{time.strftime('%Y%m%d-%H%M%S')}-{secrets.token_hex(2)}"
    if "/" in name or name.startswith("."):
        die("--out takes a plain name; output goes to build/emu/<name>/")
    d = OUT_ROOT / name
    if d.exists() and not args.keep:
        shutil.rmtree(d)
    d.mkdir(parents=True, exist_ok=True)
    return d


class Job:
    def __init__(self, args, cmd, run_dir=None, default_frames=60):
        self.args = args
        self.cmd = cmd
        self.lines = []
        self.rom = rom_path(args.rom)
        sp = None
        if args.state:
            sp, self.state_meta = state_path(args.state)  # validate before creating the run directory
        else:
            self.state_meta = None
        self.dir = run_dir or make_run_dir(args, cmd)
        self.lines.append(f"rom {self.rom}")
        self.lines.append(f"idle {args.idle}")
        if args.sram:
            self.lines.append(f"sram {Path(args.sram).resolve()}")
        if sp:
            self.lines.append(f"state {sp}")
            if self.state_meta and self.state_meta.get("rom_sha1") not in (None, sha1_file(self.rom)):
                print(f"emu.py: warning: state {args.state} was made with a different ROM "
                      f"({self.state_meta.get('rom')})", file=sys.stderr)
        ranges, meta = [], {}
        if args.input:
            ranges, meta = parse_input(Path(args.input).read_text().splitlines(), args.input)
        for i, spec in enumerate(args.keys or []):
            r, m = parse_input(spec.replace(";", "\n").splitlines(), f"--keys[{i}]")
            ranges += r
            meta["last_input_frame"] = max(meta.get("last_input_frame", 0), m["last_input_frame"])
        self.ranges = ranges
        self.meta = meta
        if args.frames is not None:
            self.frames = args.frames
        elif meta.get("frames"):
            self.frames = int(meta["frames"])
        else:
            self.frames = max(default_frames, meta.get("last_input_frame", 0) + 1)
        self.lines.append(f"frames {self.frames}")
        for a, b, m in ranges:
            self.lines.append(f"keys {a} {b} {m}")
        for ram, ln, rom in RAM_ALIASES:
            self.lines.append(f"alias 0x{ram:08X} 0x{ln:X} 0x{rom:08X}")
        self.shots = []
        self._add_common_actions()

    def path(self, name):
        return self.dir / name

    def _add_common_actions(self):
        a = self.args
        shots = parse_frames(a.shot)
        if a.sheet and not shots:
            die("--sheet needs --shot FRAMES")
        if shots:
            (self.dir / "shots").mkdir(exist_ok=True)
        for f in shots:
            if f != "end" and f > self.frames:
                continue
            p = self.dir / "shots" / f"{ftag(f)}.png"
            self.lines.append(f"shot {f} {p}")
            self.shots.append((f, p))
        dumps = parse_frames(a.dump)
        regions = ALL_REGIONS if (a.regions or "all") == "all" else [r.strip() for r in a.regions.split(",")]
        for r in regions:
            if r not in ALL_REGIONS:
                die(f"unknown region '{r}' (choose from {', '.join(ALL_REGIONS)}, all)")
        extra = []
        for spec in a.dump_range or []:
            parts = spec.split(":")
            if len(parts) < 2:
                die("--dump-range ADDR:LEN[:NAME]")
            addr, ln = syms().resolve(parts[0]), int(parts[1], 0)
            extra.append((addr, ln, parts[2] if len(parts) > 2 else f"{addr:08X}"))
        if dumps:
            (self.dir / "dumps").mkdir(exist_ok=True)
        for f in dumps:
            if f != "end" and f > self.frames:
                continue
            for r in regions:
                p = self.dir / "dumps" / f"{ftag(f)}_{r}.bin"
                if r == "sram":
                    self.lines.append(f"dumpsram {f} {p}")
                else:
                    base, ln = REGIONS[r]
                    self.lines.append(f"dump {f} 0x{base:08X} 0x{ln:X} {p}")
            for addr, ln, nm in extra:
                self.lines.append(f"dump {f} 0x{addr:08X} 0x{ln:X} {self.dir / 'dumps' / f'{ftag(f)}_{nm}.bin'}")
        for spec in a.savestate or []:
            fr, _, nm = spec.partition(":")
            fr = "end" if fr == "end" else int(fr)
            nm = nm or ftag(fr)
            self.lines.append(f"savestate {fr} {self.dir / (nm + '.ss')}")

    def run(self, quiet=False):
        self.lines.append(f"info {self.dir / 'info.json'}")
        job = self.dir / "job.txt"
        job.write_text("\n".join(self.lines) + "\n")
        secs = run_harness(job, self.dir, quiet=quiet)
        info = json.loads((self.dir / "info.json").read_text())
        info.update({"rom": rel(self.rom), "state": self.args.state, "command": self.cmd,
                     "wall_seconds": round(secs, 2)})
        (self.dir / "info.json").write_text(json.dumps(info, indent=2) + "\n")
        if self.args.sheet and self.shots:
            make_sheet([(f, p) for f, p in self.shots if p.exists()], self.dir / "shots" / "sheet.png")
        return info


def make_sheet(shots, out, cols=4):
    try:
        from PIL import Image, ImageDraw, ImageFont
    except ImportError:
        print("emu.py: Pillow not available; skipping contact sheet", file=sys.stderr)
        return
    if not shots:
        return
    w, h, pad = 240, 160, 14
    rows = (len(shots) + cols - 1) // cols
    sheet = Image.new("RGB", (cols * w, rows * (h + pad)), (40, 40, 40))
    draw = ImageDraw.Draw(sheet)
    try:
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 11)
    except OSError:
        font = ImageFont.load_default()
    for i, (f, p) in enumerate(shots):
        x, y = (i % cols) * w, (i // cols) * (h + pad)
        sheet.paste(Image.open(p).convert("RGB"), (x, y + pad))
        draw.text((x + 2, y), f if isinstance(f, str) else f"frame {f}", fill=(255, 255, 0), font=font)
    sheet.save(out)


def write_tsv(path, header, rows):
    with open(path, "w") as f:
        f.write("\t".join(header) + "\n")
        for r in rows:
            f.write("\t".join(str(x) for x in r) + "\n")


def read_tsv(path):
    lines = Path(path).read_text().splitlines()
    if not lines:
        return []
    head = lines[0].split("\t")
    return [dict(zip(head, ln.split("\t"))) for ln in lines[1:] if ln]


def print_run_footer(job, info):
    shots = [rel(p) for _, p in job.shots]
    print(f"frames run: {info['frames_run']} (stop: {info['stop_reason']}), "
          f"{info['wall_seconds']}s; output: {rel(job.dir)}/")
    if shots:
        print(f"screenshots: {len(shots)} in {rel(job.dir / 'shots')}/"
              + (" (sheet.png)" if (job.dir / "shots" / "sheet.png").exists() else ""))
    if (job.dir / "dumps").exists():
        print(f"dumps: {rel(job.dir / 'dumps')}/")


# --------------------------------------------------------------------------- commands

def cmd_run(args):
    job = Job(args, "run")
    add_stop_conditions(job, args)
    info = job.run()
    print_run_footer(job, info)


def add_stopat(job, specs):
    for spec in specs:
        name, _, count = spec.partition(":")
        job.lines.append(f"stopat 0x{syms().resolve(name):08X} {int(count or 1)}")


def add_until(job, spec):
    """ADDR[.b|.h|.w](=|!=)VALUE[:COUNT] -> until directive (checked at the end of each frame)."""
    m = re.match(r"^([^=!.]+?)(?:\.([bhw]))?\s*(=|==|!=)\s*([^:]+?)(?::(\d+))?$", spec.strip())
    if not m:
        die(f"bad --until '{spec}' (ADDR[.b|.h|.w]=VALUE[:COUNT], or != )")
    addr = syms().resolve(m.group(1))
    size = {"b": 1, "h": 2, "w": 4, None: 1}[m.group(2)]
    op = "ne" if m.group(3) == "!=" else "eq"
    job.lines.append(f"until 0x{addr:08X} {size} {op} {int(m.group(4), 0)} {int(m.group(5) or 1)}")


def add_stop_conditions(job, args):
    """--stopat/--until/--settle, or the same lines from the --input script (command line wins)."""
    meta = job.meta
    stopat = getattr(args, "stopat", None) or ([":".join(meta["stopat"])] if meta.get("stopat") else None)
    until = getattr(args, "until", None) or meta.get("until")
    settle = getattr(args, "settle", None) or meta.get("settle")
    if stopat:
        add_stopat(job, stopat)
    if until:
        add_until(job, until)
    if settle:
        job.lines.append(f"settle {int(settle)}")


def all_function_starts():
    return sorted(syms().funcs)


def add_funcs(job, stack=False):
    funcs = job.path("funcs.txt")
    funcs.write_text("".join(f"0x{a:08X}\n" for a in all_function_starts()))
    job.lines.append(f"funcs {funcs}")
    if stack:
        job.lines.append("stack")


def stack_label(text):
    """'0x0800A<0x0800B' -> 'FuncA < FuncB'."""
    if not text:
        return ""
    S = syms()
    return " < ".join(S.label(int(x, 16), with_proposed=False) for x in text.split("<"))


def cmd_trace(args):
    job = Job(args, "trace")
    add_funcs(job)
    job.lines.append(f"trace {job.path('trace_raw.tsv')}")
    if args.trace_from:
        job.lines.append(f"tracefrom {args.trace_from}")
    if args.calls:
        job.lines.append(f"calls {job.path('calls_raw.tsv')}")
    add_stop_conditions(job, args)
    info = job.run()
    S = syms()
    rows = []
    for r in read_tsv(job.path("trace_raw.tsv")):
        a = int(r["addr"], 16)
        f = S.funcs.get(a, {})
        prop = S.proposed.get(a, "")
        rows.append((int(r["first_frame"]), a, f.get("name", "?"), "" if prop == f.get("name") else prop,
                     f.get("unit", ""),
                     int(r["hits"]), int(r["last_frame"]), int(r["frames_active"])))
    rows.sort()
    write_tsv(job.path("functions.tsv"),
              ["addr", "name", "proposed", "unit", "hits", "first_frame", "last_frame", "frames_active"],
              [(f"0x{a:08X}", n, p, u, h, ff, lf, fa) for ff, a, n, p, u, h, lf, fa in rows])
    units = {}
    for ff, a, n, p, u, h, lf, fa in rows:
        units.setdefault(u, [0, 0])
        units[u][0] += 1
        units[u][1] += h
    write_tsv(job.path("units.tsv"), ["unit", "functions_hit", "total_hits"],
              sorted(((u, c, h) for u, (c, h) in units.items()), key=lambda x: -x[1]))
    nframes = max(1, info["frames_run"] - (args.trace_from or 0))
    print(f"functions executed: {len(rows)} of {len(S.funcs)} known, over {nframes} traced frames "
          f"(exact: every instruction checked)")
    ins = info.get("instructions", {})
    if ins:
        tot = sum(ins.values())
        print("instructions by region: " + ", ".join(f"{k} {v / tot:.1%}" for k, v in
                                                      sorted(ins.items(), key=lambda x: -x[1])))
    top = sorted(rows, key=lambda r: -r[5])[: args.top]
    print(f"\ntop {len(top)} by calls:   calls  calls/frame  frames  first  function (unit)")
    for ff, a, n, p, u, h, lf, fa in top:
        pn = f" [{p}]" if p and p != n else ""
        print(f"  {h:>10} {h / nframes:>11.1f} {fa:>7} {ff:>6}  {n}{pn} ({u})")
    if args.calls:
        agg = {}
        for r in read_tsv(job.path("calls_raw.tsv")):
            callee, ret, cnt = int(r["callee"], 16), int(r["return_addr"], 16), int(r["count"])
            caller = S.func_at(ret - 2)
            cname = S.func_name(caller[0]) if caller else S.label(ret)
            k = (cname, S.func_name(callee), f"0x{ret - 4:08X}" if ret >= 4 else "-")
            agg[k] = agg.get(k, 0) + cnt
        write_tsv(job.path("calls.tsv"), ["caller", "callee", "callsite", "count"],
                  sorted(((a, b, c, n) for (a, b, c), n in agg.items()), key=lambda x: (x[0], -x[3])))
        print(f"\ncall edges: {len(agg)} (caller -> callee from the return address at entry) in calls.tsv")
    print()
    print(f"wrote {rel(job.path('functions.tsv'))} (sorted by first frame), units.tsv"
          + (", calls.tsv" if args.calls else ""))
    print_run_footer(job, info)


def parse_watch(spec):
    parts = spec.split(":")
    addr = syms().resolve(parts[0])
    ln = int(parts[1], 0) if len(parts) > 1 and parts[1] else 1
    mode = parts[2] if len(parts) > 2 else "w"
    if not set(mode) <= {"r", "w"} or not mode:
        die(f"watch mode must be r, w or rw: {spec}")
    return addr, ln, mode


def cmd_watch(args):
    job = Job(args, "watch")
    for spec in args.targets:
        addr, ln, mode = parse_watch(spec)
        if args.reads and "r" not in mode:
            mode += "r"
        job.lines.append(f"watch 0x{addr:08X} 0x{ln:X} {mode}")
    if not args.no_stack:
        add_funcs(job, stack=True)
    job.lines.append(f"watchout {job.path('watch_raw.tsv')}")
    job.lines.append(f"watchsum {job.path('watch_summary_raw.tsv')}")
    job.lines.append(f"watchmax {args.max}")
    if args.changes:
        job.lines.append("watchchanges")
    add_stop_conditions(job, args)
    info = job.run()
    S = syms()
    out = []
    for e in read_tsv(job.path("watch_raw.tsv")):
        pc, lr, ctx = int(e["pc"], 16), int(e["lr"], 16), int(e["swi_caller"], 16)
        where = S.label(pc) if e["via"] == "cpu" else f"{e['via']} started by {S.label(pc)}"
        if e["via"] == "cpu" and pc < 0x4000 and ctx:
            where = f"BIOS SWI called from {S.label(ctx)}"
        out.append((e["frame"], e["pc"], where, e["access"], e["addr"], e["old"], e["new"],
                    S.label(lr) if lr else "", stack_label(e.get("stack", ""))))
    write_tsv(job.path("watch.tsv"), ["frame", "pc", "function", "access", "addr", "old", "new", "lr_function",
                                      "backtrace"], out)
    summ = []
    for r in read_tsv(job.path("watch_summary_raw.tsv")):
        pc = int(r["pc"], 16)
        summ.append((int(r["count"]), r["pc"], S.label(pc), r["via"], r["access"], r["first_frame"],
                     r["last_frame"], r["last_value"], stack_label(r.get("first_stack", ""))))
    summ.sort(key=lambda x: (-x[0], x[1]))
    write_tsv(job.path("watch_summary.tsv"), ["count", "pc", "function", "via", "access", "first_frame",
                                              "last_frame", "last_value", "first_backtrace"], summ)
    print(f"watch events: {info['watch_events']} (logged {info['watch_logged']}, --max {args.max})")
    print("\nby instruction:     count  access  via   frames        pc          function")
    for c, pc, fn, via, acc, ff, lf, lv, bt in summ[: args.top]:
        print(f"  {c:>15}  {acc:<6}  {via:<4}  {ff:>5}-{lf:<6}  {pc}  {fn}")
    changes = [o for o in out if o[3].startswith("w") and o[5] != o[6]]
    print(f"\nvalue changes logged: {len(changes)}" + ("" if changes or not out else " (only same-value writes)"))
    show = (changes if not args.reads else out)[: args.show]
    if show:
        what = "events" if args.reads else "value changes"
        print(f"first {len(show)} {what}:  frame access addr        old -> new   where")
        for fr, pc, fn, acc, addr, old, new, lrf, bt in show:
            print(f"  {fr:>6}  {acc:<5} {addr}  {old} -> {new}  {fn}")
            if bt:
                print(f"          backtrace: {bt}")
    print(f"\nwrote {rel(job.path('watch.tsv'))}, watch_summary.tsv")
    print_run_footer(job, info)


REGNAMES = {"sp": 13, "lr": 14, "pc": 15, "ip": 12, "fp": 11, "sl": 10, "sb": 9}


def cmd_break(args):
    job = Job(args, "break")
    S = syms()
    for t in args.targets:
        job.lines.append(f"break 0x{S.resolve(t) & ~1:08X}")
    for spec in args.deref or []:
        reg, _, ln = spec.partition(":")
        reg = reg.lower()
        r = REGNAMES.get(reg)
        if r is None:
            m = re.match(r"^r(\d+)$", reg)
            if not m or int(m.group(1)) > 15:
                die(f"bad --deref register '{reg}'")
            r = int(m.group(1))
        job.lines.append(f"deref {r} {int(ln or '16', 0)}")
    if not args.no_stack:
        add_funcs(job, stack=True)
    job.lines.append(f"breakout {job.path('break_raw.tsv')}")
    job.lines.append(f"breakmax {args.max}")
    add_stop_conditions(job, args)
    info = job.run()
    rows = read_tsv(job.path("break_raw.tsv"))
    regcols = ["pc", "lr", "sp"] + [f"r{i}" for i in range(13)] + ["cpsr"]
    dcols = [k for k in (rows[0] if rows else {}) if k.startswith("[")]
    out = []
    for r in rows:
        lr = int(r["lr"], 16)
        caller = S.func_at((lr & ~1) - 2)
        cname = (S.func_name(caller[0]) + f"+0x{(lr & ~1) - 4 - caller[0]:X}") if caller else S.label(lr)
        out.append([r["frame"], S.label(int(r["pc"], 16)), cname] + [r[k] for k in regcols] +
                   [r[k] for k in dcols] + [stack_label(r.get("stack", ""))])
    hdr = ["frame", "function", "callsite"] + regcols + dcols + ["backtrace"]
    write_tsv(job.path("break.tsv"), hdr, out)
    print(f"hits: {info.get('break_hits', 0)} (logged {info.get('break_logged', 0)}, --max {args.max})")
    for row in out[: args.show]:
        d = dict(zip(hdr, row))
        regs = " ".join(f"r{i}={d[f'r{i}'][2:]}" for i in range(4))
        extra = " ".join(f"{k}={d[k]}" for k in dcols)
        print(f"  frame {d['frame']:>5}  {d['function']:<24} from {d['callsite']:<30} {regs} {extra}")
        if d["backtrace"] and args.backtrace:
            print(f"          backtrace: {d['backtrace']}")
    print(f"\nwrote {rel(job.path('break.tsv'))} (r0-r12, sp, lr, cpsr, derefs, backtrace per hit)")
    print_run_footer(job, info)


def cmd_lua(args):
    job = Job(args, "lua", default_frames=600)
    script = Path(args.script).resolve()
    if not script.exists():
        die(f"no such script {args.script}")
    job.lines.append(f"lua {script}")
    job.lines.append(f"luaout {job.dir}")
    add_stop_conditions(job, args)
    info = job.run()
    print_run_footer(job, info)


def cmd_gdb(args):
    port = args.port
    job = Job(args, "gdb")
    job.lines = [ln for ln in job.lines if not ln.startswith(("shot ", "dump", "savestate "))]
    job.lines.append(f"gdb {port}")
    jobfile = job.path("job.txt")
    jobfile.write_text("\n".join(job.lines) + "\n")
    exe = harness_path()
    stub_log = open(job.path("stub.log"), "w")
    stub = subprocess.Popen([str(exe), str(jobfile)], stdout=subprocess.PIPE, stderr=stub_log, text=True)
    line = stub.stdout.readline()
    if "listening" not in line:
        stub.kill()
        die(f"GDB stub did not start: {line.strip()} (see {rel(job.path('stub.log'))})")
    gdb = ["gdb-multiarch", "-q", "-nx", "-ex", "set pagination off", "-ex", "set confirm off"]
    elf = REPO / "build" / "eds.elf"
    if not args.no_elf and elf.exists():
        gdb += ["-ex", f"file {elf}"]
    gdb += ["-ex", "set architecture armv4t", "-ex", f"target remote 127.0.0.1:{port}"]
    for x in args.x or []:
        gdb += ["-x", str(Path(x).resolve())]
    for e in args.ex or []:
        gdb += ["-ex", e]
    if not args.interactive:
        gdb = gdb[:1] + ["-batch"] + gdb[1:] + ["-ex", "kill"]
        r = subprocess.run(gdb, capture_output=True, text=True, timeout=args.timeout)
        log = r.stdout + r.stderr
        job.path("gdb.log").write_text(log)
        print(log, end="")
    else:
        subprocess.run(gdb)
    try:
        stub.wait(timeout=3)
    except subprocess.TimeoutExpired:
        stub.kill()
    print(f"output: {rel(job.dir)}/")


def compare_files(a, b):
    da, db = Path(a).read_bytes(), Path(b).read_bytes()
    if da == db:
        return None
    n = min(len(da), len(db))
    i = next((i for i in range(n) if da[i] != db[i]), n)
    return i


def cmd_compare(args):
    if not args.dump:
        args.dump = "end"
    if not args.shot:
        args.shot = args.dump
    base_out = args.out or f"compare-{time.strftime('%Y%m%d-%H%M%S')}-{secrets.token_hex(2)}"
    results = []
    for tag, rom in (("a", args.rom), ("b", args.rom_b)):
        args.rom, args.out = rom, f"{base_out}-{tag}"
        job = Job(args, "compare")
        if args.trace:
            add_funcs(job)
            job.lines.append(f"trace {job.path('trace_raw.tsv')}")
        job.run(quiet=True)
        results.append(job)
    a, b = results
    print(f"A: {rel(a.rom)}  sha1 {sha1_file(a.rom)[:12]}\nB: {rel(b.rom)}  sha1 {sha1_file(b.rom)[:12]}")
    diffs = 0
    files = sorted(p.relative_to(a.dir) for p in a.dir.rglob("*") if p.is_file() and
                   (p.suffix in (".bin", ".png") or p.name == "trace_raw.tsv") and p.name != "sheet.png")
    for f in files:
        d = compare_files(a.dir / f, b.dir / f)
        if d is not None:
            diffs += 1
            print(f"  DIFF {f} (first difference at byte 0x{d:X})")
    print(f"compared {len(files)} files: {'IDENTICAL' if not diffs else f'{diffs} differ'}")
    print(f"output: {rel(a.dir)}/, {rel(b.dir)}/")
    sys.exit(1 if diffs else 0)


def cmd_peek(args):
    if args.frames is None:
        args.frames = 0
    job = Job(args, "peek", default_frames=0)
    add_stop_conditions(job, args)
    specs = []
    for i, spec in enumerate(args.ranges):
        name, _, ln = spec.partition(":")
        addr, ln = syms().resolve(name), int(ln or "16", 0)
        out = job.path(f"peek{i}.bin")
        job.lines.append(f"dump end 0x{addr:08X} 0x{ln:X} {out}")
        specs.append((spec, addr, ln, out))
    info = job.run(quiet=True)
    width = {"u8": 1, "u16": 2, "u32": 4}[args.fmt]
    print(f"after {info['frames_run']} frames (stop: {info['stop_reason']})"
          + (f" from state {args.state}" if args.state else " from boot"))
    for spec, addr, ln, out in specs:
        data = out.read_bytes()
        print(f"{spec}: 0x{addr:08X} ({syms().label(addr)}), {ln} bytes")
        for off in range(0, len(data), 16):
            chunk = data[off:off + 16]
            n = len(chunk) // width * width
            vals = [int.from_bytes(chunk[i:i + width], "little") for i in range(0, n, width)]
            vals += list(chunk[n:])  # trailing bytes that do not fill a word
            txt = " ".join(f"{v:0{width * 2}X}" for v in vals[: n // width]) + \
                "".join(f" {b:02X}" for b in chunk[n:])
            dec = "  " + " ".join(str(v) for v in vals) if args.decimal else ""
            print(f"  {addr + off:08X}: {txt}{dec}")
    if not args.out:
        shutil.rmtree(job.dir, ignore_errors=True)  # nothing worth keeping unless --out was given


def cmd_tracediff(args):
    def load(run):
        p = OUT_ROOT / run / "functions.tsv"
        if not p.exists():
            p = Path(run) / "functions.tsv"
        if not p.exists():
            die(f"no functions.tsv for run {run}")
        return {r["addr"]: r for r in read_tsv(p)}
    a, b = load(args.run_a), load(args.run_b)
    only_a = sorted(set(a) - set(b), key=lambda k: int(a[k]["first_frame"]))
    only_b = sorted(set(b) - set(a), key=lambda k: int(b[k]["first_frame"]))
    for title, keys, src in ((f"only in {args.run_a}", only_a, a), (f"only in {args.run_b}", only_b, b)):
        print(f"{title}: {len(keys)}")
        for k in keys[: args.top]:
            r = src[k]
            name = r["name"] + (f" [{r['proposed']}]" if r.get("proposed") else "")
            print(f"  {k}  {name:<40} hits {r['hits']:>8}  first {r['first_frame']:>5}  {r['unit']}")
    print(f"in both: {len(set(a) & set(b))}")


def cmd_sym(args):
    S = syms()
    for t in args.names:
        a = S.resolve(t)
        f = S.func_at(a)
        extra = ""
        if f:
            extra = f"  size 0x{f[1]['size']:X}  unit {f[1]['unit']}  mode {f[1]['mode']}"
        print(f"{t}: 0x{a:08X}  {S.label(a)}{extra}")


def cmd_selftest(args):
    """Checks that the analysis modes do not perturb emulation, that runs are deterministic, and that the
    Lua and GDB front-ends work. Uses the duel_main1 state and the duel_turn2 recipe's inputs."""
    me = [sys.executable, str(Path(__file__).resolve())]
    tag = args.out or f"selftest-{secrets.token_hex(2)}"
    common_args = ["--state", "duel_main1", "--input", str(RECIPES_DIR / "duel_turn2.txt"), "--dump", "end",
                   "--shot", "end", "--rom", args.rom]
    lua = OUT_ROOT / f".{tag}-noop.lua"
    OUT_ROOT.mkdir(parents=True, exist_ok=True)
    lua.write_text('local n = 0\ncallbacks:add("frame", function() n = n + 1 end)\n')
    modes = {
        "run": ["run"], "run-again": ["run"], "trace": ["trace", "--calls"],
        "watch": ["watch", "0x020192E4:2", "0x03000040:0x488C:rw", "--max", "1000"],
        "break": ["break", "DuelMainStep", "sub_08007418", "--deref", "r0:4"],
        "lua": ["lua", str(lua)],
    }
    ok = True
    dirs = {}
    for name, cmd in modes.items():
        out = f"{tag}-{name}"
        r = subprocess.run(me + cmd + common_args + ["--out", out], capture_output=True, text=True)
        dirs[name] = OUT_ROOT / out
        if r.returncode:
            print(f"FAIL {name}: exit {r.returncode}\n{r.stdout}{r.stderr}")
            ok = False
    ref = dirs["run"]
    files = sorted(x.relative_to(ref) for x in ref.rglob("*") if x.suffix in (".bin", ".png"))
    for name, d in dirs.items():
        if name == "run" or not d.exists():
            continue
        bad = [str(f) for f in files if not (d / f).exists() or compare_files(ref / f, d / f) is not None]
        print(f"{'PASS' if not bad else 'FAIL'} {name:<9} final RAM/VRAM/OAM/palette/IO/SRAM + screen "
              f"{'identical to plain run' if not bad else 'differ: ' + ', '.join(bad)}")
        ok &= not bad
    tr = read_tsv(dirs["trace"] / "functions.tsv") if (dirs["trace"] / "functions.tsv").exists() else []
    hit = {r["name"] for r in tr}
    need = {"sub_08021A48", "sub_08007418"}
    print(f"{'PASS' if need <= hit else 'FAIL'} trace     saw {len(tr)} functions incl. DuelMainStep and the LP "
          "setter sub_08007418")
    ok &= need <= hit
    sp = dirs["watch"] / "watch_summary.tsv"
    w = read_tsv(sp) if sp.exists() else []
    lp = [e for e in w if e["function"].startswith("sub_08007418") and e["access"] == "w16"]
    good = len(lp) == 1 and lp[0]["last_value"] == "0x1EDC" and "sub_08013888" in lp[0]["first_backtrace"]
    print(f"{'PASS' if good else 'FAIL'} watch     LP 8000 -> 7900 written by sub_08007418, backtrace through "
          "sub_08013888")
    ok &= good
    r = subprocess.run(me + ["gdb", "--state", "duel_start", "--rom", args.rom, "--out", f"{tag}-gdb",
                             "--ex", "break *0x0804E420", "--ex", "continue", "--ex", "info registers pc"],
                       capture_output=True, text=True)
    good = "0x804e420" in r.stdout
    print(f"{'PASS' if good else 'FAIL'} gdb       GDB stub breakpoint at 0x0804E420 hit")
    ok &= good
    lua.unlink()
    if ok and not args.keep:
        for d in list(dirs.values()) + [OUT_ROOT / f"{tag}-gdb"]:
            shutil.rmtree(d, ignore_errors=True)
    print("selftest:", "PASS" if ok else f"FAIL (outputs kept in build/emu/{tag}-*)")
    sys.exit(0 if ok else 1)


# --------------------------------------------------------------------------- state library

def load_recipes():
    recipes = {}
    if not RECIPES_DIR.exists():
        return recipes
    for p in sorted(RECIPES_DIR.glob("*.txt")):
        text = p.read_text()
        ranges, meta = parse_input(text.splitlines(), rel(p))
        desc = []
        for line in text.splitlines():
            if not line.startswith("#"):
                break
            desc.append(line.lstrip("# ").strip())
        desc = " ".join(desc)
        recipes[p.stem] = {"path": p, "text": text, "ranges": ranges, "meta": meta, "desc": desc,
                           "from": meta.get("from", "boot"), "sha1": hashlib.sha1(text.encode()).hexdigest()}
    return recipes


def recipe_chain(recipes, name):
    chain, cur = [], name
    while cur != "boot":
        if cur not in recipes:
            die(f"no recipe tools/emu/states/{cur}.txt")
        if cur in chain:
            die(f"recipe cycle at {cur}")
        chain.append(cur)
        cur = recipes[cur]["from"]
    return list(reversed(chain))


def install(src, dst):
    tmp = dst.with_name(f".{dst.name}.{secrets.token_hex(3)}")
    shutil.copyfile(src, tmp)
    os.replace(tmp, dst)


def build_chain(names, recipes, rom, outdir, force=False, out=sys.stdout):
    order = []
    for n in names:
        for c in recipe_chain(recipes, n):
            if c not in order:
                order.append(c)
    print(f"building {len(order)} state(s) with {rel(rom)} into {rel(outdir)}/", file=out)
    built = {}
    for n in order:
        build_state(n, recipes, rom, outdir, force, built, out)
    metas = []
    for m in outdir.glob("*.json"):
        meta = json.loads(m.read_text())
        if m.with_suffix(".png").exists():
            metas.append((meta.get("frame_from_boot", 0), m.stem, m.with_suffix(".png")))
    make_sheet([(f"{n} (frame {fr})", png) for fr, n, png in sorted(metas)], outdir / "sheet.png", cols=3)


def build_state(name, recipes, rom, outdir, force, built, out_stream=sys.stdout):
    r = recipes[name]
    out = outdir / f"{name}.ss"
    meta_p = out.with_suffix(".json")
    parent = r["from"]
    parent_meta = None
    if parent != "boot":
        parent_meta = built.get(parent) or json.loads((outdir / f"{parent}.json").read_text())
    rom_sha = sha1_file(rom)
    if out.exists() and meta_p.exists() and not force:
        old = json.loads(meta_p.read_text())
        if (old.get("recipe_sha1") == r["sha1"] and old.get("rom_sha1") == rom_sha and
                (parent_meta is None or old.get("parent_state_sha1") == parent_meta.get("state_sha1"))):
            print(f"  {name}: up to date", file=out_stream)
            built[name] = old
            return old
    ns = argparse.Namespace(rom=str(rom), state=str(outdir / f"{parent}.ss") if parent != "boot" else None,
                            sram=None, frames=None, input=str(r["path"]), keys=None,
                            out=f"states-build-{name}-{secrets.token_hex(3)}",
                            keep=False, idle="ignore", shot=None, dump=None, regions=None, dump_range=None,
                            savestate=None, sheet=False)
    if r["meta"].get("frames") is None:
        die(f"recipe {name} needs a 'frames N' line")
    job = Job(ns, "states", run_dir=None)
    job.shots = []
    if r["meta"].get("stopat"):
        sa = r["meta"]["stopat"]
        job.lines.append(f"stopat 0x{syms().resolve(sa[0]):08X} {int(sa[1]) if len(sa) > 1 else 1}")
    if r["meta"].get("until"):
        add_until(job, r["meta"]["until"])
    if r["meta"].get("settle"):
        job.lines.append(f"settle {int(r['meta']['settle'])}")
    tmp_state = job.dir / f"{name}.ss"
    job.lines.append(f"savestate end {tmp_state}")
    job.lines.append(f"shot end {job.dir / (name + '.png')}")
    job.lines.append(f"dump end 0x03000040 0x488C {job.dir / 'gmain.bin'}")
    info = job.run(quiet=True)
    for cond in ("stopat", "until"):
        if r["meta"].get(cond) and info["stop_reason"] not in ("stopat", "until"):
            die(f"recipe {name}: {cond} {r['meta'][cond]} never reached within {job.frames} frames")
    outdir.mkdir(parents=True, exist_ok=True)
    install(tmp_state, out)  # atomic: other runs may be loading the old file right now
    install(job.dir / f"{name}.png", out.with_suffix(".png"))
    gmain = (job.dir / "gmain.bin").read_bytes()
    cb = int.from_bytes(gmain[0x410:0x414], "little")
    total = info["frames_run"] + (parent_meta["frame_from_boot"] if parent_meta else 0)
    meta = {"name": name, "description": r["desc"], "parent": parent, "recipe": rel(r["path"]),
            "recipe_sha1": r["sha1"], "rom": rel(rom), "rom_sha1": rom_sha,
            "parent_state_sha1": parent_meta.get("state_sha1") if parent_meta else None,
            "frames_from_parent": info["frames_run"], "frame_from_boot": total,
            "stop_reason": info["stop_reason"], "state_sha1": sha1_file(out),
            "main_callback": f"0x{cb:08X}", "main_callback_name": syms().label(cb & ~1),
            "built": time.strftime("%Y-%m-%d %H:%M:%S")}
    tmp_meta = job.dir / "meta.json"
    tmp_meta.write_text(json.dumps(meta, indent=2) + "\n")
    install(tmp_meta, meta_p)
    shutil.rmtree(job.dir, ignore_errors=True)
    print(f"  {name}: built ({info['frames_run']} frames from {parent}, frame {total} from boot, "
          f"scene {meta['main_callback_name']})", file=out_stream)
    built[name] = meta
    return meta


def cmd_states(args):
    recipes = load_recipes()
    outdir = Path(args.dir).resolve() if args.dir else STATES_DIR
    if args.action == "list":
        print(f"recipes: {rel(RECIPES_DIR)}/   states: {rel(outdir)}/")
        for name, r in recipes.items():
            mp = outdir / f"{name}.json"
            status = "missing"
            if mp.exists():
                m = json.loads(mp.read_text())
                status = "ok" if m.get("recipe_sha1") == r["sha1"] else "stale (recipe changed)"
                status += f", frame {m.get('frame_from_boot')}, scene {m.get('main_callback_name')}"
            print(f"  {name:<14} from {r['from']:<12} {status}\n      {r['desc']}")
        return
    if args.action == "show":
        for n in args.names:
            mp = outdir / f"{n}.json"
            if not mp.exists():
                die(f"no state {n} in {rel(outdir)}")
            print(mp.read_text())
        return
    if args.action == "diff":
        if len(args.names) != 2:
            die("states diff DIR_A DIR_B")
        return states_diff(Path(args.names[0]), Path(args.names[1]))
    # build
    names = list(recipes) if args.all or not args.names else args.names
    build_chain(names, recipes, rom_path(args.rom), outdir, force=args.force)


def states_diff(da, db):
    da, db = da.resolve(), db.resolve()
    names = sorted(p.stem for p in da.glob("*.ss") if (db / p.name).exists())
    if not names:
        die("no common states")
    bad = 0
    for n in names:
        outs = []
        for d in (da, db):
            m = d / f"{n}.json"
            rom = json.loads(m.read_text()).get("rom", "base") if m.exists() else "base"
            rom = str(REPO / rom) if not os.path.isabs(rom) else rom
            ns = argparse.Namespace(rom=rom, state=str(d / f"{n}.ss"), sram=None, frames=60, input=None,
                                    keys=None, out=f"statesdiff-{n}-{secrets.token_hex(3)}", keep=False,
                                    idle="ignore",
                                    shot="0,60", dump="0,60", regions="all", dump_range=None, savestate=None,
                                    sheet=False)
            job = Job(ns, "statesdiff")
            job.run(quiet=True)
            outs.append(job.dir)
        files = sorted(p.relative_to(outs[0]) for p in outs[0].rglob("*") if p.suffix in (".bin", ".png"))
        diffs = [str(f) for f in files if compare_files(outs[0] / f, outs[1] / f) is not None]
        bad += bool(diffs)
        print(f"  {n:<14} {'identical' if not diffs else 'DIFF: ' + ', '.join(diffs)}  "
              f"({len(files)} dumps/screens at load and +60 frames)")
        for o in outs:
            shutil.rmtree(o, ignore_errors=True)
    sys.exit(1 if bad else 0)


# --------------------------------------------------------------------------- CLI

def common(p, frames_default=None):
    g = p.add_argument_group("common")
    g.add_argument("--rom", default="base", help="base (baserom.gba, default), eds (eds.gba) or a path")
    g.add_argument("--state", help="start from a library state (build/emu/states/NAME.ss) or a .ss path")
    g.add_argument("--sram", help="initial SRAM image (default: blank; never written back)")
    g.add_argument("--frames", type=int, default=frames_default, help="frames to run")
    g.add_argument("--input", help="input script file (see README: '120 A', '130-140 RIGHT', '+20:5 DOWN')")
    g.add_argument("--keys", action="append", help="inline input lines, ';'-separated (repeatable)")
    g.add_argument("--out", help="run name: output in build/emu/NAME/ (replaced if it exists)")
    g.add_argument("--keep", action="store_true", help="do not clear an existing --out directory")
    g.add_argument("--idle", default="ignore", choices=["ignore", "detect", "remove"],
                   help="mGBA idle-loop optimisation (default ignore: exact emulation)")
    g.add_argument("--shot", help="screenshot frames, e.g. 0,60,100-400:50,end")
    g.add_argument("--sheet", action="store_true", help="also write shots/sheet.png (contact sheet)")
    g.add_argument("--dump", help="RAM dump frames (same syntax as --shot)")
    g.add_argument("--regions", help=f"regions to dump: {','.join(ALL_REGIONS)} or all (default all)")
    g.add_argument("--dump-range", action="append", help="extra dump ADDR:LEN[:NAME] (repeatable)")
    g.add_argument("--savestate", action="append", help="save a state at FRAME[:NAME] into the run dir")
    g.add_argument("--stopat", action="append",
                   help="FUNC[:COUNT]: stop at the end of the frame where FUNC ran COUNT times (repeatable)")
    g.add_argument("--until", help="ADDR[.b|.h|.w]=VALUE[:COUNT] (or !=): stop after the frame in which the "
                                   "condition becomes true for the COUNT-th time, e.g. 0x02015EE8=5")
    g.add_argument("--settle", type=int, help="with --stopat/--until: run N more frames with no input, then stop")


def main():
    if not in_container():
        reexec_in_docker()
    ap = argparse.ArgumentParser(prog="tools/emu.py", description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)

    p = sub.add_parser("run", help="run frames; screenshots, RAM dumps, savestates")
    common(p)
    p.set_defaults(fn=cmd_run)

    p = sub.add_parser("trace", help="which functions execute (every instruction checked)")
    common(p)
    p.add_argument("--trace-from", type=int, default=0, help="run frames before this at full speed, untraced")
    p.add_argument("--calls", action="store_true", help="also record caller->callee edges (calls.tsv)")
    p.add_argument("--top", type=int, default=25, help="rows to print")
    p.set_defaults(fn=cmd_trace)

    p = sub.add_parser("watch", help="memory watchpoints")
    common(p)
    p.add_argument("targets", nargs="+", help="ADDR[:LEN[:MODE]], MODE r, w (default) or rw; ADDR may be a name")
    p.add_argument("--reads", action="store_true", help="also log reads for every target")
    p.add_argument("--changes", action="store_true",
                   help="log only writes that change the value (the summary still counts everything)")
    p.add_argument("--max", type=int, default=100000, help="max events in watch.tsv (counting continues)")
    p.add_argument("--top", type=int, default=20)
    p.add_argument("--show", type=int, default=20, help="events to print")
    p.add_argument("--no-stack", action="store_true",
                   help="skip the shadow call stack (faster; no backtrace column)")
    p.set_defaults(fn=cmd_watch)

    p = sub.add_parser("break", help="log registers whenever an address executes")
    common(p)
    p.add_argument("targets", nargs="+", help="function names or addresses")
    p.add_argument("--deref", action="append", help="REG:LEN, dump LEN bytes at a register on each hit")
    p.add_argument("--max", type=int, default=10000, help="max logged hits (counting continues)")
    p.add_argument("--show", type=int, default=20, help="hits to print")
    p.add_argument("--backtrace", action="store_true", help="print the backtrace of each shown hit")
    p.add_argument("--no-stack", action="store_true", help="skip the shadow call stack")
    p.set_defaults(fn=cmd_break)

    p = sub.add_parser("states", help="savestate library: list | build [NAMES|--all] | show NAME | diff A B")
    p.add_argument("action", choices=["list", "build", "show", "diff"])
    p.add_argument("names", nargs="*")
    p.add_argument("--all", action="store_true")
    p.add_argument("--force", action="store_true", help="rebuild even if up to date")
    p.add_argument("--rom", default="base")
    p.add_argument("--dir", help="state directory (default build/emu/states)")
    p.set_defaults(fn=cmd_states)

    p = sub.add_parser("lua", help="run an mGBA Lua script headless")
    p.add_argument("script")
    common(p)
    p.set_defaults(fn=cmd_lua)

    p = sub.add_parser("gdb", help="GDB stub + gdb-multiarch in batch mode")
    common(p)
    p.add_argument("-x", action="append", help="gdb command file (repeatable)")
    p.add_argument("--ex", action="append", help="gdb command (repeatable)")
    p.add_argument("--port", type=int, default=2345)
    p.add_argument("--timeout", type=int, default=300)
    p.add_argument("--no-elf", action="store_true", help="do not load build/eds.elf symbols")
    p.add_argument("--interactive", action="store_true", help="interactive gdb (needs a TTY)")
    p.set_defaults(fn=cmd_gdb)

    p = sub.add_parser("compare", help="run the same job on two ROMs and compare dumps/screens (exit 1 if different)")
    common(p)
    p.add_argument("--rom-b", default="eds")
    p.add_argument("--trace", action="store_true", help="also compare function traces")
    p.set_defaults(fn=cmd_compare)

    p = sub.add_parser("peek", help="print memory at the end of a run (default: right after loading --state)")
    common(p)
    p.add_argument("ranges", nargs="+", help="ADDR[:LEN] (LEN default 16); ADDR may be a symbol")
    p.add_argument("--fmt", default="u8", choices=["u8", "u16", "u32"], help="word size for the dump")
    p.add_argument("--decimal", action="store_true", help="also print the words in decimal")
    p.set_defaults(fn=cmd_peek)

    p = sub.add_parser("tracediff", help="functions that run in one trace but not the other")
    p.add_argument("run_a")
    p.add_argument("run_b")
    p.add_argument("--top", type=int, default=60)
    p.set_defaults(fn=cmd_tracediff)

    p = sub.add_parser("selftest", help="check that trace/watch/break/lua do not change emulation; gdb smoke test")
    p.add_argument("--rom", default="base")
    p.add_argument("--out", help="name prefix for the run directories")
    p.add_argument("--keep", action="store_true", help="keep the run directories even when everything passes")
    p.set_defaults(fn=cmd_selftest)

    p = sub.add_parser("sym", help="resolve names/addresses")
    p.add_argument("names", nargs="+")
    p.set_defaults(fn=cmd_sym)

    args = ap.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
