CC=gcc
CFLAGS=-Wall -Wpedantic -Wextra -std=c99

default:
	make all

clean:
	rm -f *.o sport_events sport_athlete sport_registration sport_db

# To be implemented
all:
	gcc -c sport_events.c
	gcc -c sport_athlete.c
	gcc -c sport_registration.c
	gcc -c sport_db.c
	gcc -o sport_db sport_db.o sport_athlete.o sport_events.o sport_registration.o


sport_events.o: sport_events.h sport_events.c
				gcc -c sport_events.c

sport_athlete.o: sport_athlete.h sport_athlete.c
				gcc -c sport_athlete.c

sport_registration.o: sport_events.h sport_athlete.h sport_registration.h sport_registration.c
					gcc -c sport_registration.c

sport_db.o: sport_db.c sport_db.h sport_registration.h
			gcc -c sport_db.c

sport_db: 	sport_db.o sport_athlete.o sport_events.o sport_registration.o
			gcc -o sport_db sport_db.o sport_athlete.o sport_events.o sport_registration.o