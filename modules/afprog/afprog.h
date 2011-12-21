/*
 * Copyright (c) 2002-2010 BalaBit IT Ltd, Budapest, Hungary
 * Copyright (c) 1998-2010 Balázs Scheidler
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */
  
#ifndef AFPROG_H_INCLUDED
#define AFPROG_H_INCLUDED

#include "driver.h"
#include "logwriter.h"
#include "logreader.h"

typedef struct _AFProgramSourceDriver
{
  LogSrcDriver super;
  GString *cmdline;
  LogPipe *reader;
  pid_t pid;
  LogReaderOptions reader_options;
  LogProtoOptions proto_options;
  LogProtoFactory *proto_factory;
} AFProgramSourceDriver;

typedef struct _AFProgramDestDriver
{
  LogDestDriver super;
  GString *cmdline;
  LogPipe *writer;
  pid_t pid;
  LogWriterOptions writer_options;
  LogProtoOptions proto_options;
  LogProtoFactory *proto_factory;
} AFProgramDestDriver;

LogDriver *afprogram_sd_new(gchar *cmdline);
LogDriver *afprogram_dd_new(gchar *cmdline);

#endif
