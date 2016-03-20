cmake_minimum_required(VERSION 3.0)
project(MediaCourier NONE)

install(DIRECTORY MC-CSG2/ DESTINATION /srv/csg2/MCCSG2)
install(PROGRAMS MC-StreamPlexer/mediacourier.py DESTINATION bin RENAME mediacourier)
install(FILES nginx-mediacourier.conf DESTINATION share/mediacourier)
