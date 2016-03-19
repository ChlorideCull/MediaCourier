cmake_minimum_required(VERSION 3.3)
project(MediaCourier NONE)

install(DIRECTORY MC-CSG2 DESTINATION /srv/csg2 RENAME MCCSG2)
install(PROGRAM MC-StreamPlexer/mediacourier.py DESTINATION bin RENAME mediacourier)
install(FILES nginx-mediacourier.conf DESTINATION share/mediacourier)
