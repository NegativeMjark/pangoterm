#include "conf.h"

#include <stdlib.h>

#include <glib.h>
#include <gtk/gtk.h>

int conf_parse(ConfigEntry *entries, int *argcp, char ***argvp)
{
  int n_entries;
  for(n_entries = 0; entries[n_entries].longname; n_entries++)
    ;

  GOptionEntry *option_entries = malloc(sizeof(GOptionEntry) * (n_entries + 1));

  for(int i = 0; i < n_entries; i++) {
    option_entries[i].long_name  = entries[i].longname;
    option_entries[i].short_name = entries[i].shortname;
    option_entries[i].flags      = 0;
    switch(entries[i].type) {
      case CONF_TYPE_STRING:
        option_entries[i].arg = G_OPTION_ARG_STRING;
        break;
      case CONF_TYPE_INT:
        option_entries[i].arg = G_OPTION_ARG_INT;
        break;
      case CONF_TYPE_DOUBLE:
        option_entries[i].arg = G_OPTION_ARG_DOUBLE;
        break;
    }
    option_entries[i].arg_data        = entries[i].var;
    option_entries[i].description     = entries[i].desc;
    option_entries[i].arg_description = entries[i].argdesc;
  }

  option_entries[n_entries].long_name = NULL;

  GError *args_error = NULL;
  GOptionContext *args_context;

  args_context = g_option_context_new("commandline...");
  g_option_context_add_main_entries(args_context, option_entries, NULL);
  free(option_entries);

  g_option_context_add_group(args_context, gtk_get_option_group(TRUE));
  if(!g_option_context_parse(args_context, argcp, argvp, &args_error)) {
    fprintf(stderr, "Option parsing failed: %s\n", args_error->message);
    return 0;
  }

  return 1;
}
