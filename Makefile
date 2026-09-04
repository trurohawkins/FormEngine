TARGET = editor

LIBDIR = lib/
SRCDIR = src/
INCDIR = include/

RENDERER = TUI
RENDERDIR = $(RENDERER)/
RENDERLIB = $(RENDERDIR)lib/
RENDERINC = $(RENDERDIR)include/

AUDIO = AudioMan
AUDIODIR = $(AUDIO)/
AUDIOLIB = $(AUDIODIR)lib/
AUDIOINC = $(AUDIODIR)include/
AUDIOFLAGS = -lportaudio -lsndfile

HELPERDIR =HelperFuncs/
HELPERINC = $(HELPERDIR)include/
HELPERLIB = $(HELPERDIR)lib/

MOLTNDIR = MOLTN/
MOLTNINC = $(MOLTNDIR)include/
MOLTNLIB = $(MOLTNDIR)lib/

OIBDIR = OIB/
OIBINC = $(OIBDIR)include/
OIBLIB = $(OIBDIR)lib/

GAMEDIR = GameCore/
GAMEINC = $(GAMEDIR)include/
GAMELIB = $(GAMEDIR)lib/

DEV_CFLAGS = -g -fsanitize=address,undefined -fno-omit-frame-pointer
DEV_LDFLAGS = -fsanitize=address,undefined

TSAN_CFLAGS = -g -O1 -fsanitize=thread -fno-omit-frame-pointer
TSAN_LDFLAGS = -fsanitize=thread

PROD_CFLAGS = -O2
PROD_LDFLAGS =

CFLAGS = -MMD -MP -I$(HELPERINC) -I$(MOLTNINC) -I$(OIBINC) -I$(GAMEINC) -I$(AUDIOINC) -I$(RENDERINC) -I$(INCDIR) -I$(SRCDIR)
LDFLAGS =

dev: CFLAGS += $(DEV_CFLAGS)
dev: LDFLAGS += $(DEV_LDFLAGS)
dev: $(TARGET)

tsan: CFLAGS += $(TSAN_CFLAGS)
tsan: LDFLAGS += $(TSAN_LDFLAGS)
tsan: $(TARGET)

prod: CFLAGS += $(PROD_CFLAGS)
prod: LDFLAGS += $(PROD_LDFLAGS)
prod: $(TARGET)

# Linking
$(TARGET): $(INCDIR)FormEngine.h $(LIBDIR)libFormEngine.a $(RENDERLIB)lib$(RENDERER).a $(RENDERINC)$(RENDERER).h $(AUDIOLIB)lib$(AUDIO).a $(AUDIOINC)$(AUDIO).h $(GAMEINC)GameCore.h $(GAMELIB)libGameCore.a $(OIBINC)OIB.h $(OIBLIB)libOIB.a $(MOLTNLIB)libMoltnCore.a $(MOLTNINC)MoltnCore.h $(HELPERLIB)libHelper.a  $(HELPERINC)helper.h main.o  
	gcc main.o -o $@ $(LDFLAGS) $(LIBDIR)libFormEngine.a -L$(AUDIOLIB) -l$(AUDIO) -L$(RENDERLIB) -l$(RENDERER) -L$(GAMELIB) -lGameCore -L$(OIBLIB) -lOIB -L$(MOLTNLIB) -lMoltnCore -L$(HELPERLIB) -lHelper $(AUDIOFLAGS) -lm

main.o: main.c constants.h guy.c guy.h move.c move.h
	cp $(AUDIODIR)lsan.supp .
	gcc $(CFLAGS) -c main.c -o $@

$(HELPERLIB)libHelper.a:
	$(MAKE) -C $(HELPERDIR)

$(MOLTNLIB)libMoltnCore.a:
	$(MAKE) -C $(MOLTNDIR)

$(OIBLIB)libOIB.a:
	$(MAKE) -C $(OIBDIR)

$(GAMELIB)libGameCore.a:
	$(MAKE) -C $(GAMEDIR)

$(RENDERLIB)lib$(RENDERER).a:
	$(MAKE) -C $(RENDERDIR)

$(AUDIOLIB)lib$(AUDIO).a:
	$(MAKE) -C $(AUDIODIR)

# Static lib
$(LIBDIR)libFormEngine.a: form.o cell.o world.o view.o WorldManager.o | $(LIBDIR)
	ar rs $@ $^

# Compiling

form.o: $(SRCDIR)form.c $(INCDIR)form.h
	gcc $(CFLAGS) -c $(SRCDIR)form.c -o $@

cell.o: $(SRCDIR)cell.c $(INCDIR)cell.h
	gcc $(CFLAGS) -c $(SRCDIR)cell.c -o $@

world.o: $(SRCDIR)world.c $(INCDIR)world.h
	gcc $(CFLAGS) -c $(SRCDIR)world.c -o $@

view.o: $(SRCDIR)view.c $(INCDIR)view.h
	gcc $(CFLAGS) -c $(SRCDIR)view.c -o $@

WorldManager.o: $(SRCDIR)WorldManager.c $(INCDIR)WorldManager.h
	gcc $(CFLAGS) -c $(SRCDIR)WorldManager.c -o $@

$(LIBDIR):
	mkdir -p $(LIBDIR)

# tools
clean:
	rm -f *.o *.d
	git submodule foreach '$(MAKE) clean'

fclean:
	rm -f $(TARGET) *.o *.d $(LIBDIR)libFormEngine.a lsan.supp
	git submodule foreach '$(MAKE) fclean'

fixTerminal:
	stty sane

# merges .d files into dependency graph
-include *.d
