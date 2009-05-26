/* Definitions for or32 running Linux-based GNU systems using ELF
   Copyright (C) 2002, 2005
   Free Software Foundation, Inc.
   Contributed by Marko Mlinar <markom@opencores.org>

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* elfos.h should have already been included.  Now just override
   any conflicting definitions and add any extras.  */

/* Run-time Target Specification.  */
#undef  TARGET_VERSION
#define TARGET_VERSION  fputs (" (OR32 GNU/Linux with ELF)", stderr);

/* Do not assume anything about header files.  */
#define NO_IMPLICIT_EXTERN_C

#undef  USER_LABEL_PREFIX
#define USER_LABEL_PREFIX "_"


/* This is how we tell the assembler that two symbols have the same value.  */
#define ASM_OUTPUT_DEF(FILE, NAME1, NAME2) \
  do                                       \
    {                                      \
      assemble_name (FILE, NAME1);         \
      fputs (" = ", FILE);                 \
      assemble_name (FILE, NAME2);         \
      fputc ('\n', FILE);                  \
    }                                      \
    while (0)


#if 0
/* Node: Label Output */

#define SET_ASM_OP      "\t.set\t"

#define ASM_OUTPUT_EXTERNAL_LIBCALL(FILE, FUN)  \
  (*targetm.asm_out.globalize_label) (FILE, XSTR (FUN, 0))

#define ASM_WEAKEN_LABEL(FILE, NAME)    \
  do                                    \
    {                                   \
      fputs ("\t.weak\t", (FILE));      \
      assemble_name ((FILE), (NAME));   \
      fputc ('\n', (FILE));             \
    }                                   \
  while (0)
#endif
