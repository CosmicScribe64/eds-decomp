# Yu-Gi-Oh! The Eternal Duelist Soul (USA) - matching decompilation.
# Run everything inside the toolchain container:
#     tools/dr make setup        # once: check your ROM (put it in roms/) and extract its data into assets/
#     tools/dr make -j8          # build eds.gba
#     tools/dr make compare      # build and check the SHA-1 against the original

ROM      := eds.gba
ELF      := build/eds.elf
MAP      := build/eds.map
SHA1     := 510fbba212aca9bab95ea12f8fd933e62ee34dea

PREFIX   := arm-none-eabi-
AS       := $(PREFIX)as
LD       := $(PREFIX)ld
OBJCOPY  := $(PREFIX)objcopy
CPP      := cpp
AGBCC_DIR ?= /opt/agbcc
CC1      := $(AGBCC_DIR)/bin/agbcc
CC1_OLD  := $(AGBCC_DIR)/bin/old_agbcc
AGBLIB   := $(AGBCC_DIR)/lib
PYTHON   := python3

ASFLAGS  := -mcpu=arm7tdmi -mthumb-interwork -I .
CPPFLAGS := -nostdinc -undef -I include -I $(AGBCC_DIR)/include -iquote .
# Game code: old_agbcc -O2. The Konami sound driver uses agbcc -O2 -fprologue-bugfix and
# AgbSram agbcc -O1; per-unit compiler+flags live in config/cflags.txt.
DEFAULT_CC1    := old_agbcc
DEFAULT_CFLAGS := -mthumb-interwork -Wimplicit -Wparentheses -O2 -fhex-asm
unit_cc   = $(AGBCC_DIR)/bin/$(or $(shell awk '$$1=="$(1)"{print $$2}' config/cflags.txt),$(DEFAULT_CC1))
unit_cflags = $(or $(shell awk '$$1=="$(1)"{$$1="";$$2="";print}' config/cflags.txt),$(DEFAULT_CFLAGS))

# Units in link order (see units.txt). A unit links the first of src/<u>.c, src/<u>.s,
# asm/<u>.s, data/<u>.s that exists.
# `make compare ASM_UNITS="code_08000228 ..."` forces those units back to asm (e.g. while
# someone is mid-edit on src/<unit>.c).
UNITS    := $(shell sed -e 's/\#.*//' -e '/^[[:space:]]*$$/d' -e '/^@/d' units.txt)
unit_obj  = $(if $(and $(wildcard src/$(1).c),$(if $(filter $(1),$(ASM_UNITS)),,y)),build/src/$(1).o,$(if $(wildcard src/$(1).s),build/srcasm/$(1).o,$(if $(wildcard asm/$(1).s),build/asm/$(1).o,build/data/$(1).o)))
OBJS     := $(foreach u,$(UNITS),$(call unit_obj,$(u)))

.PHONY: all compare clean tidy objdiff-report setup FORCE
.SECONDARY:
.DELETE_ON_ERROR:

all: $(ROM)

compare: $(ROM)
	@echo "$(SHA1)  $(ROM)" | sha1sum -c - || { [ ! -e baserom.gba ] || $(PYTHON) tools/romdiff.py $(ROM) baserom.gba $(MAP); exit 1; }

# Check your ROM and extract the game's data into assets/ (never committed).
setup:
	$(PYTHON) tools/setup.py

clean:
	rm -rf build $(ROM)

$(ROM): $(ELF)
	$(OBJCOPY) -O binary -j .text -j .rodata -j .data --gap-fill 0 $< $@

# Regenerated on every run, because the object list depends on which src/ files exist and on ASM_UNITS.
build/ld_script.ld: units.txt tools/mkld.py $(OBJS) FORCE
	@mkdir -p $(@D)
	$(PYTHON) tools/mkld.py units.txt $@ $(OBJS)

FORCE:

build/autosyms.s: $(OBJS) symbols.ld tools/autosyms.py
	@mkdir -p $(@D)
	$(PYTHON) tools/autosyms.py $@ $(OBJS)

build/autosyms.o: build/autosyms.s
	$(AS) $(ASFLAGS) -o $@ $<

$(ELF): build/ld_script.ld build/autosyms.o $(OBJS)
	$(LD) -T build/ld_script.ld -Map $(MAP) -o $@ $(OBJS) build/autosyms.o -L $(AGBLIB) -lgcc -lc
	$(PYTHON) tools/dumpsyms.py $@ config/symbols.txt

# The game's data (graphics, text, audio, tables) is never committed. `make setup` extracts it from your
# ROM into assets/, listed in config/assets.tsv; this step builds those files back into binary form.
ASSET_FILES  := $(shell find assets -type f 2>/dev/null)
ASSETS_BUILT := build/assets/.built
$(ASSETS_BUILT): config/assets.tsv tools/assets.py $(ASSET_FILES)
	@test -d assets || { echo "error: assets/ not found. Put your copy of the game in roms/ and run 'tools/dr make setup' (see README.md)."; exit 1; }
	$(PYTHON) tools/assets.py build

build/asm/crt0.o: $(ASSETS_BUILT)   # cartridge header

# asm units: each includes its functions from asm/nonmatching/<unit>/
build/asm/%.o: asm/%.s asm/macros.inc
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) --MD build/asm/$*.d -o $@ $<

build/data/%.o: data/%.s $(ASSETS_BUILT)
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) --MD build/data/$*.d -o $@ $<

# hand-written assembly sources (SDK stubs etc.)
build/srcasm/%.o: src/%.s
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) --MD build/srcasm/$*.d -o $@ $<

# C units go through cpp, agbcc and as. Functions not yet matched stay as INCLUDE_ASM(...).
build/src/%.o: src/%.c config/cflags.txt
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) -MMD -MP -MF build/src/$*.d -MT $@ $< -o build/src/$*.i
	$(call unit_cc,$*) $(call unit_cflags,$*) build/src/$*.i -o build/src/$*.s
	printf '\t.text\n\t.align 2, 0\n' >> build/src/$*.s
	$(AS) $(ASFLAGS) --MD build/src/$*.asm.d -o $@ build/src/$*.s

# objdiff / decomp.dev progress report (no baserom needed).
#   target = each unit's original assembly, base = its C with INCLUDE_ASM functions left out
#   (-DOBJDIFF_BASE); both get absolute data references resolved (tools/objdiff_resolve.py).
OBJDIFF_UNITS := $(patsubst asm/%.s,%,$(wildcard asm/code_*.s asm/sound_*.s))
OBJDIFF_OBJS  := $(foreach u,$(OBJDIFF_UNITS),build/objdiff/target/$(u).o $(if $(wildcard src/$(u).c),build/objdiff/base/$(u).o))

objdiff.json: units.txt tools/mkobjdiff.py FORCE
	$(PYTHON) tools/mkobjdiff.py > $@

objdiff-report: objdiff.json $(OBJDIFF_OBJS) $(patsubst src/%.s,build/srcasm/%.o,$(wildcard src/*.s src/sdk/*.s)) build/src/sdk/agb_sram.o
	objdiff-cli report generate -p . -o build/report.json
	$(PYTHON) tools/mkobjdiff.py --summary build/report.json
	$(PYTHON) tools/check_report.py build/report.json

build/objdiff/raw/%.o: src/%.c config/cflags.txt
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) -DOBJDIFF_BASE $< -o build/objdiff/raw/$*.i
	$(call unit_cc,$*) $(call unit_cflags,$*) build/objdiff/raw/$*.i -o build/objdiff/raw/$*.s
	printf '\t.text\n\t.align 2, 0\n' >> build/objdiff/raw/$*.s
	$(AS) $(ASFLAGS) -o $@ build/objdiff/raw/$*.s

build/objdiff/base/%.o: build/objdiff/raw/%.o tools/objdiff_resolve.py
	@mkdir -p $(@D)
	$(PYTHON) tools/objdiff_resolve.py $< $@

build/objdiff/target/%.o: build/asm/%.o tools/objdiff_resolve.py
	@mkdir -p $(@D)
	$(PYTHON) tools/objdiff_resolve.py $< $@

-include $(wildcard build/*/*.d build/*/*/*.d)

print-%:
	@echo '$*=$($*)'
