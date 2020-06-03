EE_BIN  = blank-unpacked.ELF
EE_BIN_PACKED = BLANK.ELF
EE_BIN_STRIPPED = blank-stripped.ELF
EE_OBJS = 	main.o gui.o textures.o pad.o sound.o \
			libsd_irx.o audsrv_irx.o \
			background_png.o cross_png.o circle_png.o font_png.o \
			cancel_adp.o confirm_adp.o

EE_INCS = -I$(PS2SDK)/ports/include -I$(PS2SDK)/sbv/include -I$(GSKIT)/include -I$(GSKIT)/ee/dma/include -I$(GSKIT)/ee/gs/include -I$(GSKIT)/ee/toolkit/include
EE_LIBS = -lpatches -lpad -laudsrv -L$(GSKIT)/lib -lgskit -ldmakit -lgskit_toolkit -L$(PS2SDK)/ports/lib -lpng -ljpeg -lz

all:
	$(MAKE) $(EE_BIN_PACKED)

clean:
	rm -f $(EE_OBJS) *.ELF *.s

audsrv_irx.s: $(PS2SDK)/iop/irx/audsrv.irx
	bin2s $< $@ audsrv_irx

libsd_irx.s: $(PS2SDK)/iop/irx/libsd.irx
	bin2s $< $@ libsd_irx

background_png.s: gfx/background.png
	bin2s $< $@ background_png

cross_png.s: gfx/cross.png
	bin2s $< $@ cross_png

circle_png.s: gfx/circle.png
	bin2s $< $@ circle_png

font_png.s: gfx/font.png
	bin2s $< $@ font_png

cancel_adp.s: sound/cancel.adp
	bin2s $< $@ cancel_adp

confirm_adp.s: sound/confirm.adp
	bin2s $< $@ confirm_adp

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

reset:
	ps2client reset

$(EE_BIN_STRIPPED): $(EE_BIN)
	$(EE_STRIP) -o $@ $<

$(EE_BIN_PACKED): $(EE_BIN_STRIPPED)
	~/ps2-packer/ps2-packer -v $< $@

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
