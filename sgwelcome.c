#include "sgwelcome.h"


int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	readconf();
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), distrotitle);
		
	theme = gtk_icon_theme_get_default();
	info = gtk_icon_theme_lookup_icon(theme, distrologo, 48, 0);
	if (info != NULL) 
	{
		GdkPixbuf *icon = gtk_icon_info_load_icon(info, NULL);
		gtk_window_set_icon(GTK_WINDOW(window), icon);
		g_object_unref(icon);
		g_object_unref(info);
	}
	
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	notebook = gtk_notebook_new();
		switch (tabposition)
		{
			case 0:
				gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);
				break;
			case 1:
				gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_LEFT);
				break;
			case 2:
				gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_RIGHT);
				break;
			case 3:
				gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_BOTTOM);
				break;
			default:
				gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);
		}
	wgrid = gtk_grid_new();
	gtk_grid_attach(GTK_GRID(wgrid), notebook, 0, 0, 1, 1);
	gtk_container_add(GTK_CONTAINER(window), wgrid);
	const char *notes_path = mainfolder;
	load_folder_list(notes_path, notebook, 0);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
