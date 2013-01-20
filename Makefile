NAME = demo
CC = g++
CFLAGS = -std=c++0x -Wall -g -DDEBUG -O3
INCL = -I../Dodge/Dodge/include
LIBS = -L../Dodge/Dodge/lib -L/usr/lib -lDodge -lX11 -lGLESv2 -lEGL -lpnglite -lz -lBox2D
OBJS = Application.o \
	Collectable.o \
	Counter.o \
	CreditsMenu.o \
	CSprite.o \
	CTextEntity.o \
	Exit.o \
	Item.o \
	main.o \
	Menu.o \
	MenuItem.o \
	Mine.o \
	NumericTile.o \
	Player.o \
	Soil.o \
	SettingsMenu.o \
	StartMenu.o \
	Throwable.o \
	Zombie.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LIBS)

$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCL) $< -o $@

clean:
	rm -f $(NAME)
	rm -f *.o
