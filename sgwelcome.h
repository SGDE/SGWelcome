#include <gtk/gtk.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define ML 4096
GtkIconTheme *theme;
GtkIconInfo *info;
GtkWidget *window;

char distrotitle[ML];
char distrologo[ML];
char mainfolder[ML];
char config_file_path[ML];
char current_folder[ML] = "";
char data_folder[ML] = "";
char entry_folder[ML] = "";

int format;
int tabposition;

GtkWidget *wgrid;
GtkWidget *notebook;

int checkname(const char *str)
{
    while (*str) {
        if (!isdigit(*str)) {
            return 0; // No es un número
        }
        str++;
    }
    return 1; // Es un número
}
void load_folder_list(const char *notes_path, GtkWidget *parent_notebook, int parent_page)
{
	DIR *dir;
	struct dirent *entry;

	if ((dir = opendir(notes_path)) != NULL)
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_type == DT_DIR)
			{
				const char *filename = entry->d_name;

				if (filename[0] != '.')
				{
					// Remove numbers and dots from the filename
					char filtered_filename[256];
					int j = 0;
					for (int i = 0; i < strlen(filename); ++i)
					{
						if (!isdigit(filename[i]) && filename[i] != '.')
						{
							filtered_filename[j++] = filename[i];
						}
					}
					filtered_filename[j] = '\0';

					GtkWidget *notebook_grid = gtk_grid_new();

					gtk_container_set_border_width(GTK_CONTAINER(notebook_grid), 10);
					gtk_container_set_border_width(GTK_CONTAINER(window), 10);
					GtkWidget *list = gtk_list_box_new();
					GtkWidget *scrolled_list = gtk_scrolled_window_new(NULL, NULL);
					gtk_widget_set_vexpand(scrolled_list, TRUE);
					gtk_container_add(GTK_CONTAINER(scrolled_list), list);
					gtk_notebook_append_page(GTK_NOTEBOOK(parent_notebook), GTK_WIDGET(notebook_grid), gtk_label_new(filtered_filename));

					char subfolder_path[ML];
					snprintf(subfolder_path, sizeof(subfolder_path), "%s/%s", notes_path, filename);

					DIR *subdir;
					struct dirent *subentry;

					if ((subdir = opendir(subfolder_path)) != NULL)
					{
						while ((subentry = readdir(subdir)) != NULL)
						{
							if (subentry->d_type == DT_REG) // Disable showing directories
							{
								char cfile[9999];
								const char *infilename = subentry->d_name;

								snprintf(cfile, sizeof(cfile), "%s/%s", subfolder_path, infilename);
								//g_print(cfile);
								FILE *file = fopen(cfile, "r");

								char line[ML];
								char content[ML];
								content[0] = '\0';

								while (fgets(line, sizeof(line), file) != NULL)
								{
									strcat(content, line); // Concatenate lines to form content
								}


								GtkWidget *label_content = gtk_label_new(NULL);
								gtk_label_set_xalign(GTK_LABEL(label_content), 0.0);
								gtk_widget_set_hexpand(label_content, TRUE);
								int position = atoi(infilename);
								gtk_label_set_markup(GTK_LABEL(label_content), content);
								gtk_grid_attach(GTK_GRID(notebook_grid), label_content, 0, position, 1, 1);
							}
							else if (subentry->d_type == DT_DIR && strcmp(subentry->d_name, "data") == 0)
							{
								// Handle "data" directory
								char data_folder_path[PATH_MAX];
								snprintf(data_folder_path, sizeof(data_folder_path), "%s/%s", subfolder_path, subentry->d_name);
					
								DIR *data_dir;
								struct dirent *data_entry;

								if ((data_dir = opendir(data_folder_path)) != NULL)
								{
									while ((data_entry = readdir(data_dir)) != NULL)
									{
										if (data_entry->d_type == DT_REG)
										{
											char cfile[PATH_MAX];
											const char *infilename = data_entry->d_name;
					
											snprintf(cfile, sizeof(cfile), "%s/%s", data_folder_path, infilename);

											GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(cfile, NULL);
											int position = atoi(infilename);
											if (pixbuf != NULL)
											{
												GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
												gtk_grid_attach(GTK_GRID(notebook_grid), image, 1, position, 1, 1);
												g_object_unref(pixbuf);
											}
										}
										if (data_entry->d_type == DT_DIR)
										{
	DIR *indir;
	struct dirent *inentry;
	const char *infilename = data_entry->d_name;
	snprintf(current_folder, sizeof(current_folder), "%s/%s/data/%s", notes_path, filename, infilename);
	if (checkname(infilename) && (indir = opendir(current_folder)) != NULL) 
	{
		while ((inentry = readdir(indir)) != NULL)
		{
			if (inentry->d_type == DT_REG) // Disable showing directories
			{
				g_print("\n\n\n");
				g_print("current_folder:\n");
				g_print(current_folder);
				g_print("\n\n\n");
				char cfile[9999];
				const char *rinfilename = inentry->d_name;

				snprintf(cfile, sizeof(cfile), "%s/%s", current_folder, rinfilename );
				g_print("Next File:\n");
				g_print(cfile);
				FILE *file = fopen(cfile, "r");

				char line[ML];
				char ActName[ML];
				char ActNote[ML];
				char ActNotif[ML];
				char ActIcon[ML];

				while (fgets(line, sizeof(line), file) != NULL)
				{
					char *name = strtok(line, "=");
					char *value_str = strtok(NULL, "=");

					if (name != NULL && value_str != NULL)
					{
						// Set the value of the corresponding variable based on the name
						if (strcmp(name, "Name") == 0)
						{
							strncpy(ActName, value_str, sizeof(ActName));
						}
						else if (strcmp(name, "Comment") == 0)
						{
							strncpy(ActNote, value_str, sizeof(ActNote));
						}
						else if (strcmp(name, "Icon") == 0)
						{
							strncpy(ActIcon, value_str, sizeof(ActIcon));
							// fix GtkIcon deleting newline
								char *newline = strchr(ActIcon, '\n');
								if (newline != NULL)
								{
									*newline = '\0'; 
								}
						}
						else if (strcmp(name, "Exec") == 0)
						{
							strncpy(ActNotif, value_str, sizeof(ActNotif));
						}
					}
				}


GtkWidget *ingrid = gtk_grid_new();
gtk_grid_set_column_homogeneous(GTK_GRID(ingrid), TRUE);
gtk_grid_set_row_spacing(GTK_GRID(ingrid), 5);
GtkWidget *icon = gtk_image_new_from_icon_name(ActIcon, GTK_ICON_SIZE_MENU);
GtkWidget *label_filename = gtk_label_new(ActName);
GtkWidget *label_time = gtk_label_new(ActNote);
GtkWidget *Installbutton = gtk_button_new_with_label("Install");

gtk_grid_attach(GTK_GRID(ingrid), icon, 0, 0, 1, 1);
gtk_grid_attach(GTK_GRID(ingrid), label_filename, 1, 0, 1, 1);
gtk_grid_attach(GTK_GRID(ingrid), label_time, 2, 0, 1, 1);
gtk_grid_attach(GTK_GRID(ingrid), Installbutton, 3, 0, 1, 1);

int position = atoi(infilename);
gtk_list_box_insert(GTK_LIST_BOX(list), ingrid, -1);
gtk_grid_attach(GTK_GRID(notebook_grid), scrolled_list, 0, position, 2, 1);
			}
		}
		closedir(indir);
	}

										}
									}
									closedir(data_dir);
								}
							}
						}
						closedir(subdir);
					}
				}
			}
		}
		closedir(dir);
	}
}


void readconf()
{
	//READ THE CONF

	snprintf(config_file_path, sizeof(config_file_path), "/etc/sgwelcome.conf");

	FILE *file = fopen(config_file_path, "r");
	if (file == NULL)
	{
		if (file == NULL)
		{
			FILE *default_conf = fopen("/etc/sgwelcome.conf", "r");
			if (default_conf == NULL)
			{
				fprintf(stderr, "Error: could not open default configuration file /etc/sgwelcome.conf, please reinstall the program or put a config file in /etc/sgwelcome.conf.\n");
				exit(1);
			}
		fclose(default_conf);
		}
		else
		{
			fclose(file);
			printf("%s exists and can be read.\n", config_file_path);
		}
	}

// Open the file for reading
	char line[ML];
		if (file != NULL)
		{
			while (fgets(line, ML, file) != NULL)
			{
				char *name = strtok(line, "=");
				char *value_str = strtok(NULL, "=");

				if (name != NULL && value_str != NULL)
				{
					// Set the value of the corresponding variable based on the name
					if (strcmp(name, "distrotitle") == 0) 
					{
						strncpy(distrotitle, value_str, sizeof(distrotitle));
							char *newline = strchr(distrotitle, '\n');
							if (newline != NULL)
							{
								*newline = '\0'; 
							}
					} 
					else if (strcmp(name, "distrologo") == 0) 
					{
						strncpy(distrologo, value_str, sizeof(distrologo));
						char *newline = strchr(distrologo, '\n');
							if (newline != NULL)
							{
								*newline = '\0'; 
							}
					} 
					else if (strcmp(name, "mainfolder") == 0)
					{
						strncpy(mainfolder, value_str, sizeof(mainfolder));
						char *newline = strchr(mainfolder, '\n');
							if (newline != NULL)
							{
								*newline = '\0'; 
							}
					}
					else if (strcmp(name, "format") == 0) 
					{
						format = atoi(value_str);
					}
					else if (strcmp(name, "tabposition") == 0) 
					{
						tabposition = atoi(value_str);
					}
				}
			}

	fclose(file); // Close the file
	}
	else
	{
		printf("Error opening file");
	}
	printf("distrotitle: %s\ndistrologo: %s\nmainfolder: %s\nformat: %d\ntabposition: %d\n",
			distrotitle,distrologo,mainfolder,format,tabposition);
}
