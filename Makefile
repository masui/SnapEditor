#
#	$Date: 2002/08/20 11:34:30 $
#	$Revision: 1.4 $
#

# # For Unix
# # LIBS = -lglut -lGLU -lGL -lXmu  -lXext -lX11 -lm
# # # CFLAGS = -Wall -I/usr/X11R6/include -L/usr/X11R6/lib -g
# # CFLAGS = -Xcpluscomm -I/usr/X11R6/include -L/usr/X11R6/lib -g
# 
# # for Windows/Cygwin
# LIBS = -lglut32 -lGLU32 -lOpenGL32 -lm

INCLUDE = -I/usr/X11R6/include
LIBS = -framework GLUT -framework OpenGL -lm
CC = gcc
CFLAGS = $(INCLUDE) -DOPENGL -DCOCOA -DTEXTDIR=../data/text


all: tangram4 tangram5

tangram: tangram.o vector.o data.o
	$(CC) $(CFLAGS) tangram.o vector.o data.o -o tangram $(LIBS)

# スナップなし
tangram1: vector.o data.o tangram.c
	$(CC) $(CFLAGS) -DNOSNAP -DUSEANGLE -o $@.o -c tangram.c
	$(CC) $(CFLAGS) $@.o vector.o data.o -o $@ $(LIBS)

# スナップつき
tangram2: vector.o data.o tangram.c
	$(CC) $(CFLAGS) -DUSEANGLE -o $@.o -c tangram.c
	$(CC) $(CFLAGS) $@.o vector.o data.o -o $@ $(LIBS)

# スナップつき / 衝突検出
tangram3: vector.o data.o tangram.c
	$(CC) $(CFLAGS) -DUSEINTERSECTION -o $@.o -c tangram.c
	$(CC) $(CFLAGS) $@.o vector.o data.o -o $@ $(LIBS)

# スナップつき / 可変グリッド
tangram4: vector.o data.o tangram4.o
	$(CC) $(CFLAGS) $@.o vector.o data.o -o $@ $(LIBS)
tangram4.o: tangram.c
	$(CC) $(CFLAGS) -DUSEANGLE -DOBJGRID -o $@ -c tangram.c

# スナップつき / 可変グリッド / Tangram
tangram5: vector.o datatangram.o tangram5.o tangramicon.o
	$(CC) $(CFLAGS) $@.o vector.o datatangram.o tangramicon.o -o $@ $(LIBS)
tangram5.o: tangram.c
	$(CC) $(CFLAGS) -DUSEANGLE -DOBJGRID -o $@ -c tangram.c
tangramicon.o: tangramicon.ico tangramicon.rc
	windres -i tangramicon.rc -o tangramicon.o

datatangram.o: data.o
	$(CC) $(CFLAGS) -DTANGRAM -c -o datatangram.o data.c

clean:
	-/bin/rm -f tangram tangram? tangram*.exe *.o *~ \#* *.stackdump
