DODGE_LOCATION = ../Dodge

NAME = minefield
CC = g++
CFLAGS = -std=c++0x -O3 -Wall `sdl-config --cflags` -DLINUX -DGLEW
INCL = -I$(DODGE_LOCATION)/Dodge/include
LIBS = -L$(DODGE_LOCATION)/Dodge/lib -L/usr/lib -lDodge -lX11 -lGL -lGLEW -lpnglite -lz -lBox2D `sdl-config --libs` -lSDLmain -lSDL -lSDL_mixer
SRC = src
OBJS = $(SRC)/Application.o \
	$(SRC)/Clouds.o \
	$(SRC)/Collectable.o \
	$(SRC)/Counter.o \
	$(SRC)/CreditsMenu.o \
	$(SRC)/CSprite.o \
	$(SRC)/CTextEntity.o \
	$(SRC)/Exit.o \
	$(SRC)/GameLogic.o \
	$(SRC)/GameOptions.o \
	$(SRC)/GameOptionsMenu.o \
	$(SRC)/GameSettings.o \
	$(SRC)/Item.o \
	$(SRC)/main.o \
	$(SRC)/Menu.o \
	$(SRC)/MenuItem.o \
	$(SRC)/Mine.o \
	$(SRC)/NumericTile.o \
	$(SRC)/PauseMenu.o \
	$(SRC)/Player.o \
	$(SRC)/ProgressBar.o \
	$(SRC)/Soil.o \
	$(SRC)/SoundFx.o \
	$(SRC)/SettingsMenu.o \
	$(SRC)/StartMenu.o \
	$(SRC)/Throwable.o \
	$(SRC)/Zombie.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LIBS)

$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCL) $< -o $@

clean:
	rm -f $(SRC)/*.o
