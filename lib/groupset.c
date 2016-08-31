/*
 * Copyright (c) 2014 Balabit
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
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

#include "groupset.h"

typedef struct _LogRewriteGroupSetCallbackData
{
  LogMessage *msg;
  LogTemplate *template;
} LogRewriteGroupSetCallbackData;

static gboolean
log_rewrite_groupset_foreach_func(const gchar *name, TypeHint type,
                                  const gchar *value, gsize value_len,
                                  gpointer user_data)
{
  LogRewriteGroupSetCallbackData *callback_data = (LogRewriteGroupSetCallbackData*) user_data;
  LogMessage *msg = callback_data->msg;
  LogTemplate *template = callback_data->template;
  GString *result;

  result = g_string_sized_new(64);

  log_template_format(template, msg, NULL, LTZ_LOCAL, 0, value, result);

  NVHandle handle = log_msg_get_value_handle(name);
  log_msg_set_value(msg, handle, result->str, result->len);

  g_string_free(result, TRUE);
  return FALSE;
}

static void
log_rewrite_groupset_process(LogRewrite *s, LogMessage *msg)
{
  LogRewriteGroupSet *self = (LogRewriteGroupSet *) s;
  LogRewriteGroupSetCallbackData userdata;
  userdata.msg = msg;
  userdata.template = self->replacement;
  value_pairs_foreach(self->query, log_rewrite_groupset_foreach_func, msg, 0, LTZ_LOCAL, NULL, &userdata);
}

static void
__free_field(gpointer field, gpointer user_data)
{
  g_free(field);
}


void
log_rewrite_groupset_add_fields(LogRewrite *rewrite, GList *fields)
{
  LogRewriteGroupSet *self = (LogRewriteGroupSet *) rewrite;
  GList *head;
  for (head = fields; head; head = head->next)
  {
    value_pairs_add_glob_pattern(self->query, head->data, TRUE);
  }
  g_list_foreach(fields, __free_field, NULL);
  g_list_free(fields);
}
static LogPipe *
log_rewrite_groupset_clone(LogProcessPipe *s)
{
   LogRewriteGroupSet *self = (LogRewriteGroupSet *) s;
   LogRewriteGroupSet *cloned = (LogRewriteGroupSet *)log_rewrite_groupset_new(self->replacement);
   value_pairs_unref(cloned->query);
   cloned->query = value_pairs_ref(self->query);
   return &cloned->super.super.super;
};

void
log_rewrite_groupset_free(LogPipe *s)
{
  LogRewriteGroupSet *self = (LogRewriteGroupSet *) s;
  value_pairs_unref(self->query);
  log_template_unref(self->replacement);
  log_rewrite_free_method(s);
}

LogRewrite *
log_rewrite_groupset_new(LogTemplate *template)
{
  LogRewriteGroupSet *self = g_new0(LogRewriteGroupSet, 1);

  log_rewrite_init(&self->super);

  self->super.super.super.free_fn = log_rewrite_groupset_free;
  self->super.process = log_rewrite_groupset_process;
  self->super.super.clone = log_rewrite_groupset_clone;

  self->replacement = log_template_ref(template);
  self->query = value_pairs_new();

  return &self->super;
}
