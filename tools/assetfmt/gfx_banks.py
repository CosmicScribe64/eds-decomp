"""Uncompressed graphics banks: card frames, banks A and B, the small graphics block and the Mode-4 bitmaps.

One manifest row per bank, type `gfx_banks_bank`, option `layout=<name>` (a key of LAYOUTS below). The bank is
split into items; each item becomes one standard file, and `<path>/index.json` lists the items in ROM order:

  pal      BGR555 colours                          <addr>.pal  (JASC-PAL), or the palette of the image that owns it
  bitmap   linear 8bpp picture (Mode 4)            <addr>.png  (indexed; palette = its `pal` item)
  tiles    4bpp / 8bpp 8x8 tiles                   <addr>.png  (indexed sheet; the palette is for viewing)
  map      BG map entries (u16)                    <addr>.json (rows of 4-digit hex)
  pack     image pack {palette, tiles, cell list}  <addr>.png  (the assembled picture with its own palette)
  sprite   sprite animation stream                 <addr>.png  (frames side by side, own palette) + frame list
  bin      bytes whose format is not known yet     <addr>.bin

Every item is rebuilt to exactly its ROM slot. A rebuilt item that comes out shorter is padded with zeros; one
that comes out longer is an error, because the layout of the ROM is fixed for now. During extraction each item is
built back and compared with the ROM; an item that does not round-trip is written as `bin` instead and listed in
the index's `fallback` field. The formats are described on wiki/tools/assets.md.
"""
import json
import struct
import zlib

A = None  # tools/assets.py, set by register()

# ------------------------------------------------------------------------------------------------- layouts
# One line per item: ADDRESS KIND [key=value ...] [# comment]. An item ends where the next one starts.
# Keys: name= (file name suffix), pal= (palette item address, optionally :bank for 16 colours of a larger
# palette), bpp=, cols=, frame=WxH (tiles grouped into sprite frames of W x H tiles), fcols= (frames per row),
# w=/h= (bitmap or map size), in= (palette stored in that image's PNG), why= (reason for a bin item).
LAYOUTS = {}

LAYOUTS['card_frames'] = """
08625460 pack bpp=8 name=frame_normal     # GetCardFrameGfx: normal monster
08627AF8 pack bpp=8 name=frame_effect     # effect monster
0862A190 pack bpp=8 name=frame_fusion     # fusion monster
0862C828 pack bpp=8 name=frame_ritual     # ritual monster
0862EEC0 pack bpp=8 name=frame_magic      # type 22 (magic)
08631558 pack bpp=8 name=frame_trap       # type 21 (trap)
08633BF0 pack bpp=8 name=frame_type23     # type 23
"""

LAYOUTS['bank_a'] = """
# Attribute palettes and 16x16 icons (tables 0x08198950 / 0x0819897C, indexed by attribute 1..6, then 3 type icons)
08636288 pal name=attr1
086362A8 pal name=attr2
086362C8 pal name=attr3
086362E8 pal name=attr4
08636308 pal name=attr5
08636328 pal name=attr6
08636348 pal name=trap
08636368 pal name=magic
08636388 pal name=type24
086363A8 tiles cols=2 pal=08636288 name=attr1_icon
08636428 tiles cols=2 pal=086362A8 name=attr2_icon
086364A8 tiles cols=2 pal=086362C8 name=attr3_icon
08636528 tiles cols=2 pal=086362E8 name=attr4_icon
086365A8 tiles cols=2 pal=08636308 name=attr5_icon
08636628 tiles cols=2 pal=08636328 name=attr6_icon
086366A8 tiles cols=2 pal=08636348 name=trap_icon
08636728 tiles cols=2 pal=08636368 name=magic_icon
086367A8 tiles cols=2 pal=08636388 name=type24_icon
# 4bpp image-pack icons (pointer table 0x081989AC)
08636828 pack bpp=4
086368F0 pack bpp=4
086369B8 pack bpp=4
08636A80 pack bpp=4
08636B48 pack bpp=4
08636C10 pack bpp=4
08636CD8 pack bpp=4
08636DA0 pack bpp=4
08636E68 pack bpp=4
08636F30 pack bpp=4
08636FEC pack bpp=4
086370A8 pack bpp=4
08637164 pack bpp=4
08637220 pack bpp=4
086372D8 pack bpp=4
08637394 tiles cols=6 pal=08637454         # 6 small icons, index-1 via 0x08637374 (CardDetail_DrawInfo)
08637454 pal
08637474 pack bpp=4                        # table 0x081989F0
0863753C pack bpp=4                        # table 0x081989F0
08637604 pack bpp=4                        # table 0x081989F0
086376CC pack bpp=4                        # table 0x081989F0
08637794 pack bpp=4                        # table 0x081989F0
0863785C pack bpp=4                        # table 0x081989F0
08637924 pack bpp=4                        # table 0x081989F0
086379EC pack bpp=4                        # table 0x081989F0
08637AB4 pack bpp=4                        # table 0x081989F0
08637B74 pack bpp=4                        # table 0x081989F0
08637C3C pack bpp=4                        # table 0x081989F0
08637D04 pack bpp=4                        # table 0x081989F0
08637DCC pack bpp=4                        # table 0x081989F0
08637E94 pack bpp=4                        # table 0x081989F0
08637F5C pack bpp=4                        # table 0x081989F0
08638024 pack bpp=4                        # table 0x081989F0
086380EC pack bpp=4                        # table 0x081989F0
086381B4 pack bpp=4                        # table 0x081989F0
0863827C pack bpp=4                        # table 0x081989F0
08638344 pack bpp=4                        # table 0x081989F0
0863840C pal
0863842C tiles cols=10 pal=0863840C         # digits 0-9
0863856C tiles cols=1 pal=0863840C
0863858C tiles cols=1 pal=0863840C
086385AC tiles cols=1 pal=0863840C
086385CC tiles cols=3 pal=0863840C
0863862C pack bpp=4
0863975C pack bpp=4
08639DFC pal
08639E1C tiles cols=32 pal=08639DFC         # 0x2C00 to OBJ 0x06010000 (Password_InitVideo)
0863CA1C pack bpp=4
0863CA9C pal
0863CABC tiles cols=4 pal=0863CA9C
0863CB3C pal
0863CB5C tiles cols=9 pal=0863CB3C
0863CC7C pal name=shop_bg                   # BG palette of the shop / pack covers (PackList_DrawBackground)
0863CE7C tiles bpp=8 cols=1 pal=0863CC7C
0863CEBC tiles bpp=8 cols=1 pal=0863CC7C
0863CEFC tiles bpp=8 cols=1 pal=0863CC7C
0863CF3C pack bpp=8
0863D12C pack bpp=8
0863E39C pack bpp=8
0863F54C pack bpp=8
# Booster pack covers: 56x112 8bpp, 98 tiles row-major 7 wide (PackInfo table 0x080865DC); 36 slots
0864073C tiles bpp=8 cols=7 pal=0863CC7C name=cover_vol1
08641FBC tiles bpp=8 cols=7 pal=0863CC7C name=cover_vol2
0864383C tiles bpp=8 cols=7 pal=0863CC7C name=cover_vol3
086450BC tiles bpp=8 cols=7 pal=0863CC7C name=cover_vol4
0864693C tiles bpp=8 cols=7 pal=0863CC7C name=cover_vol5
086481BC tiles bpp=8 cols=7 pal=0863CC7C name=cover_vol6
08649A3C tiles bpp=8 cols=7 pal=0863CC7C name=cover_vol7
0864B2BC tiles bpp=8 cols=7 pal=0863CC7C name=cover_lobewd
0864CB3C tiles bpp=8 cols=7 pal=0863CC7C name=cover_phantom_of_g
0864E3BC tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
0864FC3C tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
086514BC tiles bpp=8 cols=7 pal=0863CC7C name=cover_magicruler
08652D3C tiles bpp=8 cols=7 pal=0863CC7C name=cover_pharaos_survant
086545BC tiles bpp=8 cols=7 pal=0863CC7C name=cover_cursr_of_anubis
08655E3C tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
086576BC tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
08658F3C tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
0865A7BC tiles bpp=8 cols=7 pal=0863CC7C name=cover_premium3
0865C03C tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
0865D8BC tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
0865F13C tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
086609BC tiles bpp=8 cols=7 pal=0863CC7C name=cover_celemony
0866223C tiles bpp=8 cols=7 pal=0863CC7C name=cover_expert1
08663ABC tiles bpp=8 cols=7 pal=0863CC7C name=cover_expert2
0866533C tiles bpp=8 cols=7 pal=0863CC7C name=cover_expert3
08666BBC tiles bpp=8 cols=7 pal=0863CC7C name=cover_duelist
0866843C tiles bpp=8 cols=7 pal=0863CC7C name=cover_final_duelist
08669CBC tiles bpp=8 cols=7 pal=0863CC7C name=cover_rare_selections
0866B53C tiles bpp=8 cols=7 pal=0863CC7C name=cover_expert4
0866CDBC tiles bpp=8 cols=7 pal=0863CC7C name=cover_expert5
0866E63C tiles bpp=8 cols=7 pal=0863CC7C name=cover_limited_collection
0866FEBC tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
0867173C tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
08672FBC tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
0867483C tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
086760BC tiles bpp=8 cols=7 pal=0863CC7C name=cover_unused
0867793C pal
0867795C pal
0867797C tiles frame=4x8 pal=0867795C
0867817C tiles frame=4x8 pal=0867795C
0867897C tiles frame=4x8 pal=0867795C
0867917C tiles frame=4x8 pal=0867795C
0867997C tiles frame=4x8 pal=0867795C
0867A17C tiles frame=4x8 pal=0867795C
0867A97C tiles frame=4x8 pal=0867795C
0867B17C tiles frame=4x8 pal=0867795C
0867B97C tiles cols=4 pal=0867E4BC
0867BB7C pack bpp=4
0867CC0C pack bpp=4                        # table 0x08086550
0867CE84 pack bpp=4                        # table 0x08086550
0867D0FC pack bpp=4                        # table 0x08086550
0867D374 pack bpp=4                        # table 0x08086550
0867D5EC pack bpp=4                        # table 0x08086550
0867D864 pack bpp=4                        # table 0x08086550
0867DADC pack bpp=4                        # table 0x08086550
0867DD54 pack bpp=4                        # table 0x08086550
0867DFCC pack bpp=4                        # table 0x08086550
0867E244 pack bpp=4                        # table 0x08086550
0867E4BC pal
0867E6BC tiles cols=16 pal=0867E4BC
0867EE3C pal
0867EE5C tiles cols=2 pal=0867EE3C
0867EE9C tiles cols=4 pal=0867EE3C
0867F01C pal
0867F03C tiles frame=4x4 pal=0867F01C
0867F63C tiles frame=4x4 pal=0867F01C
0867FC3C pal
0867FC5C tiles cols=16 pal=0867FC3C
0868045C pal
0868047C tiles frame=4x2 pal=0868045C
0868147C pal
0868149C tiles cols=5 pal=0868147C
0868167C pal
0868187C tiles cols=8 pal=0868167C
08681D7C tiles cols=8 pal=0868167C
08681E7C tiles frame=4x2 pal=0868167C
0868247C pal
0868267C tiles frame=2x4 fcols=16 pal=0868247C
0868467C pal
0868487C tiles cols=4 pal=0868467C
086849FC tiles cols=4 pal=0868467C
08684B7C tiles cols=4 pal=0868467C
08684EFC tiles cols=4 pal=0868467C
0868557C pal
0868559C tiles cols=16 pal=0868557C
# Banner graphics {pal, gfx, bgm} (table 0x08198D68)
08685D9C pal
08685F9C tiles frame=8x4 fcols=2 pal=08685D9C
0868679C pal
0868699C tiles frame=8x4 fcols=2 pal=0868679C
0868719C pal
0868739C tiles frame=8x4 fcols=2 pal=0868719C
08687B9C pal
08687BBC tiles cols=8 pal=08687B9C
08687FBC tiles cols=8 pal=08687B9C
086883BC tiles cols=8 pal=08687B9C
086887BC tiles cols=8 pal=08687B9C
08688BBC tiles cols=8 pal=08687B9C
08688FBC pal
08688FD8 tiles cols=8 pal=08688FBC
086893D8 pack bpp=4
# 4bpp image-pack icons (table 0x081A41A8)
0868B670 pack bpp=4
0868B738 pack bpp=4
0868B800 pack bpp=4
0868B8C8 pack bpp=4
0868B990 pack bpp=4
0868BA58 pack bpp=4
0868BB20 pack bpp=4
0868BBE8 pack bpp=4
0868BCB0 pack bpp=4
0868BD78 pack bpp=4
0868BE40 pack bpp=4
0868BF08 pack bpp=4
0868BFD0 pack bpp=4
0868C098 pack bpp=4
0868C160 pack bpp=4
0868C228 pack bpp=4
0868C2F0 pack bpp=4
0868C3B8 pack bpp=4
0868C480 pack bpp=4
0868C548 pack bpp=4
0868C610 pack bpp=4
0868C6D8 pack bpp=4
0868C7A0 pack bpp=4
0868C868 pack bpp=4
0868C930 pack bpp=4
0868C9F8 pack bpp=4
# Sprite animation streams (SprAnimLoad via DuelAnim_PlayZoneEffect / DuelSprAnim_Load)
0868CAC0 sprite
0868DB94 sprite
0868EC38 sprite
08690D0C sprite
08694EA8 sprite
0869771C sprite
08698C7C pal
08698C9C tiles cols=32 pal=08698C7C       # 0x2000 to OBJ 0x06010000 (CardListView_InitScreen)
0869AC9C pack bpp=4
0869AD1C pal
0869AD3C tiles cols=32 pal=0869AD1C
0869B53C pal
0869B55C tiles cols=12 pal=0869B53C         # 5 x 0x300 (12x2 tiles) chosen by mode
0869C45C pack bpp=4
0869D758 pack bpp=4
0869E8E4 pack bpp=4
0869EECC pal
0869EEEC tiles cols=16 pal=0869EECC
086A12EC bitmap pal=086AA8EC                # Mode 4 240x160 (TurnOrder_Load)
086AA8EC pal in=086A12EC
086AAAEC pal                                 # OBJ palettes, loaded 0x20 at a time (they overlap)
086AAB00 pal                                 # OBJ palettes, loaded 0x20 at a time (they overlap)
086AAB20 pal                                 # OBJ palettes, loaded 0x20 at a time (they overlap)
086AAB40 pal                                 # OBJ palettes, loaded 0x20 at a time (they overlap)
086AAB60 pal                                 # OBJ palettes, loaded 0x20 at a time (they overlap)
086AAB80 pal                                 # OBJ palettes, loaded 0x20 at a time (they overlap)
086AABA0 pal                                 # OBJ palettes, loaded 0x20 at a time (they overlap)
086AABBC pal                                 # OBJ palettes, loaded 0x20 at a time (they overlap)
086AABDC pal                                 # OBJ palettes, loaded 0x20 at a time (they overlap)
086AAC00 pal                                 # OBJ palettes, loaded 0x20 at a time (they overlap)
086AAC20 pal                                 # OBJ palettes, loaded 0x20 at a time (they overlap)
086AAC28 tiles cols=4 pal=086AAAEC            # TurnOrder_LoadObjTiles (rows of 4 tiles)
086AB028 tiles cols=4 pal=086AAAEC            # TurnOrder_LoadObjTiles (rows of 4 tiles)
086AB428 tiles cols=4 pal=086AAAEC            # TurnOrder_LoadObjTiles (rows of 4 tiles)
086AB828 tiles cols=16 pal=086AAB80            # TurnOrder_LoadObjTiles (rows of 16 tiles)
086AC028 tiles cols=16 pal=086AAB80            # TurnOrder_LoadObjTiles (rows of 16 tiles)
086AC828 tiles cols=8 pal=086AAB80            # TurnOrder_LoadObjTiles (rows of 8 tiles)
086AD028 tiles cols=8 pal=086AAB80            # TurnOrder_LoadObjTiles (rows of 8 tiles)
086AD828 tiles cols=8 pal=086AAB80            # TurnOrder_LoadObjTiles (rows of 8 tiles)
086AE028 tiles cols=16 pal=086AAB80            # TurnOrder_LoadObjTiles (rows of 16 tiles)
086AE828 tiles cols=16 pal=086AAB80            # TurnOrder_LoadObjTiles (rows of 16 tiles)
086AF028 tiles cols=16 pal=086AAB80            # TurnOrder_LoadObjTiles (rows of 16 tiles)
086AF828 tiles cols=4 pal=086AAAEC            # TurnOrder_LoadObjTiles (rows of 4 tiles)
086AFA28 pal
086AFA48 tiles frame=4x4 pal=086AFA28
086AFC48 tiles frame=4x4 pal=086AFA28
086AFE48 tiles frame=4x4 pal=086AFA28
086B0048 tiles frame=4x4 pal=086AFA28
086B0248 tiles frame=4x4 pal=086AFA28
086B0448 tiles frame=4x4 pal=086AFA28
086B0648 tiles frame=4x4 pal=086AFA28
086B0848 tiles frame=4x4 pal=086AFA28
086B0A48 tiles frame=4x4 pal=086AFA28
086B0C48 tiles frame=4x4 pal=086AFA28
086B0E48 tiles frame=4x4 pal=086AFA28
086B1048 tiles frame=4x4 pal=086AFA28
086B1248 tiles frame=4x4 pal=086AFA28
086B1448 tiles frame=4x4 pal=086AFA28
086B1648 tiles frame=4x4 pal=086AFA28
086B1848 tiles frame=4x4 pal=086AFA28
086B1A48 tiles frame=4x4 pal=086AFA28
086B1C48 tiles frame=4x4 pal=086AFA28
086B1E48 tiles cols=2 pal=086AFA28
086B1EC8 tiles cols=2 pal=086AFA28
086B1F48 tiles cols=2 pal=086AFA28
086B1FC8 tiles cols=2 pal=086AFA28
086B2048 tiles cols=2 pal=086AFA28
086B20C8 tiles cols=2 pal=086AFA28
086B2148 pal
086B2168 tiles cols=16 pal=086B6168        # CopyTileSheetTo2D: 16 rows of 16 tiles
086B4168 tiles cols=16 pal=086B6168
086B6168 pal
086B6368 pal
086B6568 tiles cols=16 pal=086B6368
086B8568 bitmap pal=086C1B68                # Mode 4 240x160 (ExodiaScene_LoadEye)
086C1B68 pal in=086B8568
086C1D68 tiles cols=16 pal=086CAB78           # BG charblock (ExodiaScene_LoadFlames)
086C3D68 tiles cols=16 pal=086CAB78           # BG charblock (ExodiaScene_LoadFlames)
086C5D68 tiles cols=16 pal=086CAB78           # BG charblock (ExodiaScene_LoadFlames)
086C7D68 tiles cols=16 pal=086CAB78           # BG charblock (ExodiaScene_LoadFlames)
086C9D68 map w=30
086CA218 map w=30
086CA6C8 map w=30
086CAB78 pal
086CAD78 tiles cols=16 pal=086B6368
086CCD78 tiles cols=16 pal=086B6368
086CED78 tiles frame=4x4 pal=086B6368      # 32x32 sprites (LoadObjTileBlock4x4)
086CF778 tiles frame=4x4 pal=086B6368
086D0178 tiles cols=16 pal=086E22D0           # BG charblock (DestinyBoardScene_Load)
086D2178 tiles cols=16 pal=086E22D0           # BG charblock (DestinyBoardScene_Load)
086D4178 tiles cols=16 pal=086E22D0           # BG charblock (DestinyBoardScene_Load)
086D6178 tiles cols=16 pal=086E24D0           # CopyTileSheetTo2D
086D8178 tiles cols=16 pal=086E24D0           # CopyTileSheetTo2D
086DA178 tiles cols=16 pal=086E24D0           # CopyTileSheetTo2D
086DC178 tiles cols=16 pal=086E24D0           # CopyTileSheetTo2D
086DE178 tiles cols=16 pal=086E24D0           # CopyTileSheetTo2D
086E0178 map w=30                          # reel strips (table 0x08199DCC)
086E0C40 map w=30
086E11A4 map w=30
086E1708 map w=30
086E1C6C map w=30
086E21D0 map w=16
086E22D0 pal
086E24D0 pal
086E26D0 map w=30
086E3030 tiles cols=16 pal=086F1C60
086E5030 tiles cols=16 pal=086ED1B0           # CopyTileSheetTo2D
086E7030 tiles cols=16 pal=086ED1B0           # CopyTileSheetTo2D
086E9030 tiles cols=16 pal=086ED1B0           # CopyTileSheetTo2D
086EB030 tiles cols=16 pal=086ED1B0           # CopyTileSheetTo2D
086ED030 pal
086ED0B0 pal
086ED190 pal
086ED1B0 pal
086ED3B0 tiles cols=16 pal=086ED030
086EF3B0 tiles cols=16 pal=086F1C60        # 0x400 blocks (DeckEdit_CommandLabelVBlank)
086F0FB0 tiles cols=16 pal=086F1C60
086F13B0 tiles cols=16 pal=086F1C60
086F17B0 map w=30                          # one 30x20 map; code also reads it at +0x24, +0x30, +0x38, +0x360
086F1C60 pal
086F1E60 pal
086F2060 tiles cols=16 pal=086F1C60
086F4060 tiles cols=16 pal=086F1C60        # 0x2000 charblock; 0x086F41A0 is inside
086F6060 tiles cols=16 pal=086F1C60
086F8060 tiles cols=16 pal=086F1E60
086FA060 tiles cols=16 pal=086F1E60
086FC060 map w=8
086FC0E0 map w=30
086FC590 map w=30
086FCA40 map w=30
086FD0D0 map w=30
086FD580 map w=30
086FD850 map w=7
086FD924 pal
086FDB24 tiles cols=16 pal=086FD924
086FFB24 tiles cols=16 pal=086FD924
08701B24 map w=8
08701BA4 map w=30
08702054 map w=30
08702504 map w=7
087025D8 map w=30
08702A88 pal
08702B08 pal
08702BE8 pal
08702C08 tiles cols=16 pal=08702A88
# 16x16 metatile icons and their palettes (DeckEdit_LoadCardIconTiles, tables 0x08087394..0x08087440)
08704D48 tiles cols=2 pal=08704DC8
08704DC8 pal
08704DE8 tiles cols=2 pal=08704E68
08704E68 pal
08704E88 tiles cols=2 pal=08704EE8
08704EE8 pal
08704F08 tiles cols=2 pal=08704EE8
08704F88 pal
08704FA8 tiles cols=2 pal=08705028
08705028 pal
08705048 tiles cols=2 pal=087050C8
087050C8 pal
087050E8 tiles cols=2 pal=087050C8
08705168 pal
08705188 tiles cols=2 pal=08705208
08705208 pal
08705228 tiles cols=2 pal=08705208
087052A8 pal
087052C8 tiles cols=2 pal=08705348
08705348 pal
08705368 tiles cols=2 pal=08705348
087053E8 pal
08705408 tiles cols=2 pal=08705488
08705488 pal
087054A8 tiles cols=2 pal=08705488
08705528 pal
08705548 tiles cols=6 pal=08705528
08705608 pal
08705628 tiles cols=2 pal=087056A8
087056A8 pal
087056C8 tiles cols=2 pal=08705748
08705748 pal
08705768 tiles cols=2 pal=087057E8
087057E8 pal
08705808 tiles cols=2 pal=08705888
08705888 pal
087058A8 tiles cols=2 pal=08705928
08705928 pal
08705948 tiles cols=2 pal=087059C8
087059C8 pal
087059E8 tiles cols=2 pal=08705A68
08705A68 pal
08705A88 tiles cols=2 pal=08705B08
08705B08 pal
08705B28 tiles cols=2 pal=08705BA8
08705BA8 pal
08705BC8 tiles cols=2 pal=08705C48
08705C48 pal
08705C68 tiles cols=2 pal=08705CE8
08705CE8 pal
08705D08 tiles cols=2 pal=08705D88
08705D88 pal
08705DA8 tiles cols=2 pal=08705E28
08705E28 pal
08705E48 tiles cols=2 pal=08705EC8
08705EC8 pal
08705EE8 tiles cols=2 pal=08705F68
08705F68 pal
08705F88 tiles cols=2 pal=08706008
08706008 pal
08706028 tiles cols=2 pal=087060A8
087060A8 pal
087060C8 tiles cols=2 pal=08706148
08706148 pal
08706168 tiles cols=2 pal=087061E8
087061E8 pal
08706208 tiles cols=2 pal=08706288
08706288 pal
087062A8 tiles cols=2 pal=08706328
08706328 pal
08706348 tiles cols=2 pal=087063C8
087063C8 pal
087063E8 tiles cols=2 pal=08706468
08706468 pal
08706488 tiles cols=2 pal=08706468
08706508 pal
08706528 tiles cols=2 pal=087065A8
087065A8 pal
087065C8 tiles cols=2 pal=087065A8
08706648 pal
08706668 tiles cols=2 pal=087066E8
087066E8 pal
08706708 tiles cols=2 pal=087066E8
08706788 pal
087067A8 tiles cols=2 pal=08706828
08706828 pal
08706848 tiles cols=2 pal=08706828
087068C8 pal
087068E8 tiles cols=2 pal=08706968
08706968 pal
08706988 tiles cols=2 pal=08706968
08706A08 pal
08706A28 tiles cols=2 pal=08706AA8
08706AA8 pal
08706AC8 tiles cols=2 pal=08706AA8
08706B48 pal
08706B68 tiles cols=2 pal=08706BE8
08706BE8 pal
08706C08 tiles cols=2 pal=08706BE8
08706C88 pal
08706CA8 tiles cols=2 pal=08706D28
08706D28 pal
08706D48 tiles cols=2 pal=08706D28
08706DC8 pal
08706DE8 tiles cols=2 pal=08706E68
08706E68 pal
08706E88 tiles cols=2 pal=08706E68
08706F08 pal
08706F28 tiles cols=4 pal=0870B5E0            # DeckEdit_LoadCardBoxTiles
087070A8 tiles cols=4 pal=0870B5E0            # DeckEdit_LoadCardBoxTiles
08707228 tiles cols=4 pal=0870B5E0            # DeckEdit_LoadCardBoxTiles
087073A8 tiles cols=4 pal=0870B5E0            # DeckEdit_LoadCardBoxTiles
08707528 tiles cols=4 pal=0870B5E0            # DeckEdit_LoadCardBoxTiles
087076A8 tiles cols=4 pal=0870B5E0            # DeckEdit_LoadCardBoxTiles
08707828 tiles cols=2 pal=0870B5E0
087078A8 tiles cols=2 pal=0870B5E0
08707928 tiles cols=2 pal=0870B5E0
087079A8 tiles cols=2 pal=0870B5E0
08707A28 tiles cols=2 pal=0870B5E0
08707AA8 tiles cols=2 pal=0870B5E0
08707B28 pack bpp=4
087095E0 tiles cols=16 pal=0870B5E0        # CardTrading_LoadObjTiles: 8 rows of 16 tiles
0870A5E0 tiles cols=16 pal=0870B600
0870B5E0 pal
0870B600 pal
0870B620 tiles cols=16 pal=0870C620
0870C620 pal
"""

LAYOUTS['small_graphics'] = """
0871B650 pal                                # 0x60 loaded to OBJ palettes 0-2 (OpponentSelect_Init)
0871B850 tiles bpp=8 cols=16 pal=0871B650     # 0x1000 to OBJ 0x06014000; 8bpp, 2D mapping
0871C850 pal                                # 0x20 loaded to OBJ palette 3 (OpponentSelect_LoadPage)
0871CA50 tiles cols=10 pal=0871C850:0
0871CB90 tiles cols=10 pal=0871C850:0
"""

LAYOUTS['mode4_bitmaps'] = """
0871CE50 bitmap pal=08726450                # table 0x08198440: {palette, bitmap} x 5
08726450 pal in=0871CE50
08726650 bitmap pal=0872FC50
0872FC50 pal in=08726650
0872FE50 bitmap pal=08739450
08739450 pal in=0872FE50
08739650 bitmap pal=08742C50
08742C50 pal in=08739650
08742E50 bitmap pal=0874C450
0874C450 pal in=08742E50
"""

LAYOUTS['bank_b'] = """
087BDAA8 pack bpp=8                         # Title_LoadGraphics (LoadBgImage)
087C056C pack bpp=4                         # Title_FadeIn
087C0CD4 pack bpp=4 name=coin               # Title_LoadGraphics
087C1DCC pack bpp=4                         # Title_LoadGraphics
087C29D4 bitmap pal=087CBFD4                # Mode 4 240x160 (Title_ConfirmDeleteSave)
087CBFD4 pal in=087C29D4
087CC1D4 tiles cols=32 pal=087D01D4         # 0x4000 to OBJ 0x06014000 (2D mapping)
087D01D4 pal
087D01F4 pack bpp=8 name=license_logo1      # License_ShowKonamiLogo
087D292C pack bpp=8 name=license_logo2      # License_ShowKcejLogo
087D4B24 pack bpp=8 name=sky                # MainMenu_Init
087DE858 pal
087DE878 tiles cols=32 pal=087DE858         # 0x4000 to OBJ 0x06010000 (MainMenu_Init, 2D mapping)
087E2878 pal
087E2A78 pal
087E2C78 tiles cols=16 pal=087E2A78         # Record_LoadGfx: OBJ rows 0-2 (2D mapping)
087E2E78 tiles cols=16 pal=087E2A78
087E3078 tiles cols=16 pal=087E2878
087E3278 pal
087E3478 tiles cols=10 pal=087E3278
087E35B8 pack bpp=4                         # Record_LoadGfx (LoadBgImage4bppToMap)
087E4280 pack bpp=4
087E52A4 pack bpp=4                         # Record_DrawPage
087E5CF4 pack bpp=4
087E6610 pack bpp=4                         # table 0x08198600
087E6974 pack bpp=4
087E6C38 pack bpp=4
087E7078 pack bpp=4
087E743C pack bpp=4
087E77B4 pack bpp=4
087E795C pack bpp=4                         # 24 small packs (table 0x081985A0)
087E7B44 pack bpp=4
087E7D2C pack bpp=4
087E7F14 pack bpp=4
087E80FC pack bpp=4
087E82E4 pack bpp=4
087E84CC pack bpp=4
087E86B4 pack bpp=4
087E889C pack bpp=4
087E8A84 pack bpp=4
087E8C6C pack bpp=4
087E8E54 pack bpp=4
087E903C pack bpp=4
087E9224 pack bpp=4
087E940C pack bpp=4
087E95F4 pack bpp=4
087E97DC pack bpp=4
087E99C4 pack bpp=4
087E9BAC pack bpp=4
087E9D94 pack bpp=4
087E9F7C pack bpp=4
087EA164 pack bpp=4
087EA34C pack bpp=4
087EA530 pack bpp=4
087EA718 bitmap pal=087F3D18 name=calendar  # Mode 4 240x160 (Calendar_Init); rows 120-159 are also read as 0x087F1798
087F3D18 pal in=087EA718
087F3F18 pal
087F4118 tiles cols=32 pal=087F3F18         # calendar day numbers
087F4D18 pal                                # 6 OBJ palettes
087F4DD8 tiles bpp=8 cols=16 pal=087F4D18     # 8bpp OBJ icons
087F5DD8 pal
087F5DF8 tiles cols=32 pal=087F5DD8         # month names, 4 x 0x800, one per season (Calendar_DrawCursorAndHeader)
087F7DF8 pal
087F7E18 tiles cols=32 pal=087F7DF8         # weekday names; read as 0x800 bytes, the last 0xB0 from the padding
"""


# ------------------------------------------------------------------------------------------------- helpers
def h8(a):
    return f'0x{a:08X}'


def parse_layout(text, start, end):
    items = []
    note = None
    for line in text.splitlines():
        line, _, comment = line.partition('#')
        line, comment = line.strip(), comment.strip()
        if not line:  # a comment line describes the items below it
            note = comment or None
            continue
        f = line.split()
        it = {'addr': int(f[0], 16), 'kind': f[1]}
        for kv in f[2:]:
            k, v = kv.split('=', 1)
            it[k] = v
        if comment or note:
            it['note'] = comment or note
            note = None
        items.append(it)
    items.sort(key=lambda it: it['addr'])
    if not items or items[0]['addr'] != start:
        raise ValueError(f'layout must start at {h8(start)}')
    for a, b in zip(items, items[1:] + [{'addr': end}]):
        if b['addr'] <= a['addr']:
            raise ValueError(f'layout: {h8(b["addr"])} does not follow {h8(a["addr"])}')
        a['size'] = b['addr'] - a['addr']
    return items


def colours(raw):
    return list(struct.unpack(f'<{len(raw) // 2}H', raw[:len(raw) // 2 * 2]))


def rgb(cols):
    return [A.bgr555_to_rgb(c) for c in cols]


def jasc(cols):
    lines = ['JASC-PAL', '0100', str(len(cols))] + ['%d %d %d' % A.bgr555_to_rgb(c) for c in cols]
    return ('\r\n'.join(lines) + '\r\n').encode()


def read_jasc(data):
    v = data.decode().split()
    if v[0] != 'JASC-PAL':
        raise ValueError('not a JASC-PAL file')
    n = int(v[2])
    return [tuple(int(x) for x in v[3 + 3 * i:6 + 3 * i]) for i in range(n)]


def bit15(cols):
    return [i for i, c in enumerate(cols) if c & 0x8000]


def pack_colours(plte, n, hi):
    if len(plte) < n:
        raise ValueError(f'the palette has {len(plte)} colours, {n} are needed')
    cols = [A.rgb_to_bgr555(c) for c in plte[:n]]
    for i in hi or []:
        cols[i] |= 0x8000
    return struct.pack(f'<{n}H', *cols)


# tiles: 4bpp tiles are 32 bytes (low nibble = left pixel), 8bpp tiles 64 bytes
def tile_pixels(raw, bpp):
    if bpp == 8:
        return raw
    out = bytearray(64)
    for i, b in enumerate(raw):
        out[2 * i], out[2 * i + 1] = b & 15, b >> 4
    return bytes(out)


def tile_bytes(px, bpp):
    if bpp == 8:
        return bytes(px)
    if max(px) > 15:
        raise ValueError('4bpp graphics must use palette entries 0-15')
    return bytes((px[2 * i] & 15) | (px[2 * i + 1] & 15) << 4 for i in range(32))


class Sheet:
    """Places tile i of a sheet: tiles grouped into frames of fw x fh tiles, `fcols` frames per row."""
    def __init__(self, n, fw, fh, fcols):
        self.n, self.fw, self.fh = n, fw, fh
        frames = max(1, -(-n // (fw * fh)))
        self.fcols = max(1, min(fcols, frames))
        self.W = self.fcols * fw * 8
        self.H = -(-frames // self.fcols) * fh * 8

    def pos(self, i):
        f, k = divmod(i, self.fw * self.fh)
        fy, fx = divmod(f, self.fcols)
        return (fx * self.fw + k % self.fw) * 8, (fy * self.fh + k // self.fw) * 8


def sheet_of(it, n):
    if 'frame' in it:
        fw, fh = (int(v) for v in it['frame'].split('x'))
        return Sheet(n, fw, fh, int(it.get('fcols', 8)))
    cols = int(it.get('cols', 16))
    return Sheet(n, min(cols, max(n, 1)), 1, 1)


def blit(img, W, x, y, px, w=8):
    for r in range(len(px) // w):
        img[(y + r) * W + x:(y + r) * W + x + w] = px[r * w:(r + 1) * w]


def cut(img, W, x, y, w=8, h=8):
    return b''.join(bytes(img[(y + r) * W + x:(y + r) * W + x + w]) for r in range(h))


# ------------------------------------------------------------------------------------------------- context
class Ctx:
    """Extraction context: the whole ROM (for view palettes) and the bank's items by address."""
    def __init__(self, rom, items):
        self.rom = rom
        self.by_addr = {it['addr']: it for it in items}
        self.labels = {}
        for unit in A.DATA_UNITS:  # code labels, for the index only
            try:
                for name, addr in A.labels_of(f'data/{unit}.s'):
                    self.labels.setdefault(addr, []).append(name)
            except OSError:
                pass

    def pal_ref(self, ref):
        """`ADDR[:bank]` -> ("0x08XXXXXX[:bank]", file holding those colours)."""
        addr, _, bank = ref.partition(':')
        it = self.by_addr.get(int(addr, 16))
        f = None
        if it:
            f = self.by_addr[int(it['in'], 16)]['file'] if 'in' in it else it['file']
        return h8(int(addr, 16)) + (':' + bank if bank else ''), f

    def palette(self, ref, default_n):
        """View palette `ADDR[:bank]` -> list of RGB."""
        if not ref:
            return [(i * 17, i * 17, i * 17) for i in range(16)] if default_n == 16 else \
                   [(i, i, i) for i in range(256)]
        addr, _, bank = ref.partition(':')
        a = int(addr, 16)
        it = self.by_addr.get(a)
        n = it['size'] // 2 if it and it['kind'] == 'pal' else default_n
        if self.rom is None:
            cols = [0] * n
        else:
            cols = colours(self.rom[a - A.BASE:a - A.BASE + 2 * n])
        if bank:
            b = int(bank)
            cols = cols[16 * b:16 * b + 16]
        return (rgb(cols) + [(0, 0, 0)] * default_n)[:default_n]


# ------------------------------------------------------------------------------------------------- kinds
# x(raw, it, ctx) -> (files, meta); b(read, meta) -> bytes (at most the item's size; build_item pads it).

def x_bin(raw, it, ctx):
    return {it['file']: raw}, {}


def b_bin(read, m):
    return read(m['file'])


def x_pal(raw, it, ctx):
    cols = colours(raw)
    meta = {'colors': len(cols)}
    if bit15(cols):
        meta['bit15'] = bit15(cols)
    if len(raw) & 1:
        meta['tail'] = raw[-1:].hex()
    if 'in' in it:  # stored as the palette of the image item that owns it
        meta['file'] = ctx.by_addr[int(it['in'], 16)]['file']
        return {}, meta
    return {it['file']: jasc(cols)}, meta


def b_pal(read, m):
    data = read(m['file'])
    plte = A.png_read(data)[3] if m['file'].endswith('.png') else read_jasc(data)
    return pack_colours(plte, m['colors'], m.get('bit15'))


def x_bitmap(raw, it, ctx):
    w, h = int(it.get('w', 240)), int(it.get('h', 160))
    if len(raw) < w * h:
        raise ValueError('bitmap slot too small')
    pal = ctx.palette(it.get('pal'), 256)
    meta = {'width': w, 'height': h}
    if it.get('pal'):
        meta['palette'], f = ctx.pal_ref(it['pal'])
        if 'in' not in ctx.by_addr.get(int(it['pal'].split(':')[0], 16), {}):
            meta['palette_file'] = f
    if len(raw) > w * h:
        meta['tail'] = raw[w * h:].hex()
    return {it['file']: A.png_write(w, h, raw[:w * h], pal)}, meta


def b_bitmap(read, m):
    w, h, px, _ = A.png_read(read(m['file']))
    if (w, h) != (m['width'], m['height']):
        raise ValueError(f"{m['file']} must be {m['width']}x{m['height']}")
    return px + bytes.fromhex(m.get('tail', ''))


def x_tiles(raw, it, ctx):
    bpp = int(it.get('bpp', 4))
    ts = bpp * 8
    n = len(raw) // ts
    if n == 0:
        raise ValueError('no whole tile')
    sh = sheet_of(it, n)
    img = bytearray(sh.W * sh.H)
    for i in range(n):
        x, y = sh.pos(i)
        blit(img, sh.W, x, y, tile_pixels(raw[i * ts:(i + 1) * ts], bpp))
    pal = ctx.palette(it.get('pal'), 16 if bpp == 4 else 256)
    meta = {'bpp': bpp, 'tiles': n, 'frame': [sh.fw, sh.fh], 'frames_per_row': sh.fcols}
    if it.get('pal'):
        meta['palette'], meta['palette_file'] = ctx.pal_ref(it['pal'])
    if len(raw) > n * ts:
        meta['tail'] = raw[n * ts:].hex()
    return {it['file']: A.png_write(sh.W, sh.H, bytes(img), pal)}, meta


def b_tiles(read, m):
    W, H, px, _ = A.png_read(read(m['file']))
    sh = Sheet(m['tiles'], m['frame'][0], m['frame'][1], m['frames_per_row'])
    if (W, H) != (sh.W, sh.H):
        raise ValueError(f"{m['file']} must be {sh.W}x{sh.H}")
    out = bytearray()
    for i in range(m['tiles']):
        x, y = sh.pos(i)
        out += tile_bytes(cut(px, W, x, y), m['bpp'])
    return bytes(out) + bytes.fromhex(m.get('tail', ''))


def x_map(raw, it, ctx):
    w = int(it.get('w', 32))
    vals = colours(raw)
    rows = [' '.join('%04X' % v for v in vals[i:i + w]) for i in range(0, len(vals), w)]
    meta = {'width': w, 'entries': len(vals)}
    if len(raw) & 1:
        meta['tail'] = raw[-1:].hex()
    doc = {'width': w, 'height': len(rows),
           'format': 'GBA BG map entries: tile | hflip << 10 | vflip << 11 | palette << 12 (hex)',
           'rows': rows}
    return {it['file']: (json.dumps(doc, indent=1) + '\n').encode()}, meta


def b_map(read, m):
    doc = json.loads(read(m['file']))
    vals = [int(v, 16) for row in doc['rows'] for v in row.split()]
    if len(vals) != m['entries']:
        raise ValueError(f"{m['file']}: {len(vals)} entries, expected {m['entries']}")
    return struct.pack(f'<{len(vals)}H', *vals) + bytes.fromhex(m.get('tail', ''))


def rep4(raw, o):
    v = struct.unpack_from('<4H', raw, o)
    if not v[0] == v[1] == v[2] == v[3]:
        raise ValueError(f'pack count at +0x{o:X} is not stored 4 times')
    return v[0]


def x_pack(raw, it, ctx):
    """Image pack -> the picture it draws. Cells are 8x8 blocks of the picture in row-major order; blank blocks
    have no cell, and identical blocks share one tile (tiles are numbered in order of first use)."""
    bpp = int(it.get('bpp', 8))
    ts = bpp * 8
    nc = rep4(raw, 0)
    cols = colours(raw[8:8 + 2 * nc])
    o = 8 + 2 * nc
    nt = rep4(raw, o)
    tiles = [tile_pixels(raw[o + 8 + i * ts:o + 8 + (i + 1) * ts], bpp) for i in range(nt)]
    o += 8 + nt * ts
    ncell = rep4(raw, o)
    cells = [struct.unpack_from('<HH', raw, o + 8 + 4 * i) for i in range(ncell)]
    o += 8 + 4 * ncell
    W = (max((p & 0x3F for p, _ in cells), default=0) + 1) * 8
    H = (max((p >> 8 for p, _ in cells), default=0) + 1) * 8
    img = bytearray(W * H)
    for p, t in cells:
        blit(img, W, (p & 0x3F) * 8, (p >> 8) * 8, tiles[t])
    meta = {'bpp': bpp, 'colors': nc}
    if bit15(cols):
        meta['bit15'] = bit15(cols)
    if o < len(raw):
        meta['tail'] = raw[o:].hex()
    return {it['file']: A.png_write(W, H, bytes(img), rgb(cols))}, meta


def b_pack(read, m):
    bpp = m['bpp']
    W, H, px, plte = A.png_read(read(m['file']))
    if W % 8 or H % 8 or W > 64 * 8 or H > 256 * 8:
        raise ValueError(f"{m['file']}: size must be a multiple of 8, at most 512x2048")
    lim = 16 if bpp == 4 else 256
    nc = m['colors']
    if max(px, default=0) >= min(nc, lim):
        raise ValueError(f"{m['file']}: pixels must use palette entries 0-{min(nc, lim) - 1}")
    tiles, index, cells = [], {}, []
    for ty in range(H // 8):
        for tx in range(W // 8):
            blk = cut(px, W, tx * 8, ty * 8)
            if not any(blk):
                continue
            if blk not in index:
                index[blk] = len(tiles)
                tiles.append(blk)
            cells.append((tx | ty << 8, index[blk]))
    out = bytearray(struct.pack('<4H', *[nc] * 4) + pack_colours(plte, nc, m.get('bit15')))
    out += struct.pack('<4H', *[len(tiles)] * 4) + b''.join(tile_bytes(t, bpp) for t in tiles)
    out += struct.pack('<4H', *[len(cells)] * 4) + b''.join(struct.pack('<HH', *c) for c in cells)
    return bytes(out) + bytes.fromhex(m.get('tail', ''))


SIZE_CODE = {0: 8, 0x4000: 16, 0x8000: 32, 0xC000: 64}


def x_sprite(raw, it, ctx):
    """Sprite animation stream (SprAnimLoad): u16 palette[16]; u16 n; {u16 size, u16 b}[n];
    n x {u16 nTiles; 4bpp tiles}; u16 nFrames; nFrames x {u16 pieces; {u16 graphic; s16 dx; s16 dy}[pieces]}."""
    pal = colours(raw[:0x20])
    n = struct.unpack_from('<H', raw, 0x20)[0]
    table = [list(struct.unpack_from('<HH', raw, 0x22 + 4 * i)) for i in range(n)]
    o = 0x22 + 4 * n
    blocks = []
    for i in range(n):
        t = struct.unpack_from('<H', raw, o)[0]
        blocks.append([tile_pixels(raw[o + 2 + k * 32:o + 2 + (k + 1) * 32], 4) for k in range(t)])
        o += 2 + 32 * t
    nf = struct.unpack_from('<H', raw, o)[0]
    o += 2
    frames = []
    for _ in range(nf):
        k = struct.unpack_from('<H', raw, o)[0]
        frames.append([list(struct.unpack_from('<Hhh', raw, o + 2 + 6 * j)) for j in range(k)])
        o += 2 + 6 * k
    if o > len(raw):
        raise ValueError('sprite stream runs past its slot')
    sides = [SIZE_CODE.get(s & 0xC000, 8) for s, _ in table]
    cw = max(sides, default=8)
    ch = max([-(-len(b) // (s // 8)) * 8 for b, s in zip(blocks, sides)] + [8])
    W, H = cw * max(n, 1), ch
    img = bytearray(W * H)
    for i, (b, s) in enumerate(zip(blocks, sides)):
        c = s // 8
        for k, t in enumerate(b):
            blit(img, W, i * cw + (k % c) * 8, (k // c) * 8, t)
    meta = {'cell': [cw, ch], 'graphics': [{'size': '0x%04X' % s, 'b': '0x%04X' % b2, 'tiles': len(bl)}
                                           for (s, b2), bl in zip(table, blocks)],
            'frames': frames}
    if bit15(pal):
        meta['bit15'] = bit15(pal)
    if o < len(raw):
        meta['tail'] = raw[o:].hex()
    return {it['file']: A.png_write(W, H, bytes(img), rgb(pal))}, meta


def b_sprite(read, m):
    W, H, px, plte = A.png_read(read(m['file']))
    cw, ch = m['cell']
    g = m['graphics']
    if (W, H) != (cw * max(len(g), 1), ch):
        raise ValueError(f"{m['file']} must be {cw * max(len(g), 1)}x{ch}")
    out = bytearray(pack_colours(plte, 16, m.get('bit15')))
    out += struct.pack('<H', len(g))
    for e in g:
        out += struct.pack('<HH', int(e['size'], 16), int(e['b'], 16))
    for i, e in enumerate(g):
        c = SIZE_CODE.get(int(e['size'], 16) & 0xC000, 8) // 8
        out += struct.pack('<H', e['tiles'])
        for k in range(e['tiles']):
            out += tile_bytes(cut(px, W, i * cw + (k % c) * 8, (k // c) * 8), 4)
    out += struct.pack('<H', len(m['frames']))
    for f in m['frames']:
        out += struct.pack('<H', len(f)) + b''.join(struct.pack('<Hhh', *p) for p in f)
    return bytes(out) + bytes.fromhex(m.get('tail', ''))


KINDS = {
    'bin': (x_bin, b_bin, '.bin'), 'pal': (x_pal, b_pal, '.pal'), 'bitmap': (x_bitmap, b_bitmap, '.png'),
    'tiles': (x_tiles, b_tiles, '.png'), 'map': (x_map, b_map, '.json'), 'pack': (x_pack, b_pack, '.png'),
    'sprite': (x_sprite, b_sprite, '.png'),
}


def build_item(read, m):
    try:
        data = KINDS[m['kind']][1](read, m)
    except (IndexError, TypeError, struct.error, UnicodeDecodeError, zlib.error) as e:  # malformed edit
        raise ValueError(f"{m['file']}: {e}")
    if len(data) > m['size']:
        raise ValueError(f"{m['file']}: builds to 0x{len(data):X} bytes, its ROM slot holds 0x{m['size']:X} "
                         f"(the ROM layout is fixed; make the edit smaller)")
    return data + bytes(m['size'] - len(data))


# ------------------------------------------------------------------------------------------------- bank type
def x_bank(data, p, rom):
    items = parse_layout(LAYOUTS[p['layout']], p['start'], p['end'])
    ctx = Ctx(rom, items)
    for it in items:
        suffix = '_' + it['name'] if 'name' in it else ''
        it['file'] = f"{p['path']}/{it['addr']:08X}{suffix}{KINDS[it['kind']][2]}"
    files, index, fallback = {}, [None] * len(items), []
    order = sorted(range(len(items)), key=lambda i: 'in' in items[i])  # palettes kept in an image go last
    for i in order:
        it = items[i]
        raw = data[it['addr'] - p['start']:it['addr'] - p['start'] + it['size']]
        head = {'addr': h8(it['addr']), 'size': it['size'], 'kind': it['kind'], 'file': it['file']}
        try:
            fs, meta = KINDS[it['kind']][0](raw, it, ctx)
            m = dict(head, **meta)
            if build_item(lambda rel: fs[rel] if rel in fs else files[rel], m) != raw:
                raise ValueError('round trip differs')
        except (ValueError, KeyError, IndexError, struct.error) as e:
            if it['kind'] == 'bin':
                raise
            fallback.append(f"{h8(it['addr'])} {it['kind']}: {e}")
            it['file'] = it['file'].rsplit('.', 1)[0] + '.bin'
            fs, m = {it['file']: raw}, dict(head, kind='bin', file=it['file'], why=f'fallback: {e}')
        if it.get('why'):
            m['why'] = it['why']
        inside = [n for a in range(it['addr'], it['addr'] + it['size']) for n in ctx.labels.get(a, [])]
        if inside:
            m['labels'] = inside
        if it.get('note'):
            m['note'] = it['note']
        files.update(fs)
        index[i] = {k: (v[len(p['path']) + 1:] if k in ('file', 'palette_file') and v else v)
                    for k, v in m.items()}
    for f in fallback:
        print(f"note: {p['path']}: item {f} kept as raw bytes")
    doc = {'bank': p['path'], 'start': h8(p['start']), 'end': h8(p['end']),
           'doc': 'Items in ROM order; file names are relative to this folder. See tools/assetfmt/gfx_banks.py.',
           'fallback': fallback, 'items': index}
    files[f"{p['path']}/index.json"] = dump_index(doc)
    return files


def dump_index(doc):
    """JSON with one item per line, so the index stays readable."""
    items = doc['items']
    head = {k: v for k, v in doc.items() if k != 'items'}
    s = json.dumps(head, indent=1)[:-2] + ',\n "items": [\n'
    s += ',\n'.join('  ' + json.dumps(it, ensure_ascii=False) for it in items) + '\n ]\n}\n'
    return s.encode()


def b_bank(read, p):
    doc = json.loads(read(f"{p['path']}/index.json"))
    out = bytearray()
    for m in doc['items']:
        m = dict(m, file=f"{p['path']}/{m['file']}")
        if int(m['addr'], 16) != p['start'] + len(out):
            raise ValueError(f"{p['path']}/index.json: item {m['addr']} is not at offset 0x{len(out):X}")
        out += build_item(read, m)
    return bytes(out)


def register(assets):
    global A
    A = assets
    return {'gfx_banks_bank': (x_bank, b_bank)}
