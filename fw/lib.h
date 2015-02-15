/*
   Phobia DC Motor Controller for RC and robotics.
   Copyright (C) 2014 Roman Belov <romblv@gmail.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _H_LIB_
#define _H_LIB_

#define NULL			((void *) 0L)
#define EOL			"\r\n"

char strcmp(const char *s, const char *p);
char strpcmp(const char *s, const char *p);
char *strcpy(char *d, const char *s);
int strlen(const char *s);
void putc(char c);
void puts(const char *s);
void printf(const char *fmt, ...);

#endif /* _H_LIB_ */
