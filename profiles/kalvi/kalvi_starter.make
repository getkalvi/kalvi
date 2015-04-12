; Stub makefile for Kalvi
; Get Drupal  7 core and the profile which contains a .make file that will
; download all the required projects/libraries.

core = 7.x
api = 2

projects[] = drupal


; Setup kalvi  profile from this make file stub.
projects[kalvi][type] = profile
projects[kalvi][download][type] = git
projects[kalvi][download][url] = http://git.drupal.org/project/kalvi.git
