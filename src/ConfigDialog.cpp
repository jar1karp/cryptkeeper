#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <string>
#include "ConfigDialog.h"
#include "defines.h"
#include "cryptkeeper.h"

static gboolean on_window_close (GtkWidget *window, GdkEvent *event, ConfigDialog *w)
{
	w->Hide ();
	return TRUE;
}

static void on_close_clicked (GtkButton *button, ConfigDialog *w)
{
	w->Hide ();
}

ConfigDialog::ConfigDialog ()
{
	GtkWidget *w, *hbox;

	m_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (m_window), UI_WINDOW_BORDER);
	gtk_window_set_title (GTK_WINDOW (m_window), _("Cryptkeeper Preferences"));

	g_signal_connect(G_OBJECT(m_window), "delete-event", G_CALLBACK(on_window_close), this);

	GtkWidget *vbox = gtk_vbox_new (FALSE, UI_SPACING);
	gtk_container_add (GTK_CONTAINER (m_window), vbox);

	{
		hbox = gtk_hbox_new (FALSE, UI_SPACING);
		gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, UI_SPACING);

		w = gtk_image_new_from_stock (GTK_STOCK_PREFERENCES, GTK_ICON_SIZE_DIALOG);
		gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, UI_SPACING);

		w = gtk_label_new (NULL);

		char buf[512];
		snprintf (buf, sizeof(buf), "<span weight=\"bold\" size=\"large\">%s</span>", _("Cryptkeeper Preferences"));
		gtk_label_set_markup (GTK_LABEL (w), buf);
		gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, UI_SPACING);
	}

	hbox = gtk_hbox_new (FALSE, UI_SPACING);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, UI_SPACING);

	GtkWidget *label = gtk_label_new (_("File browser:"));
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, UI_SPACING);

	m_filemanager_entry = gtk_entry_new ();
	gtk_entry_set_text (GTK_ENTRY (m_filemanager_entry), config_filemanager);
	gtk_box_pack_start (GTK_BOX (hbox), m_filemanager_entry, FALSE, FALSE, UI_SPACING);

	{
		hbox = gtk_hbox_new (FALSE, UI_SPACING);
		gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, UI_SPACING);

		w = gtk_label_new (_("Unmount after idle (minutes):"));
		gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, UI_SPACING);

		m_idle_spinbutton = gtk_spin_button_new_with_range (0.0, 60.0, 1.0);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (m_idle_spinbutton), config_idletime);
		gtk_box_pack_start (GTK_BOX (hbox), m_idle_spinbutton, FALSE, FALSE, UI_SPACING);
	}

	GtkWidget *buttonBox = gtk_hbutton_box_new ();
	gtk_box_pack_end (GTK_BOX (vbox), buttonBox, FALSE, FALSE, UI_SPACING);

	GtkWidget *button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
	g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (on_close_clicked), this);
	gtk_box_pack_end (GTK_BOX (buttonBox), button, FALSE, FALSE, UI_SPACING);
}

ConfigDialog::~ConfigDialog ()
{
	gtk_widget_destroy (m_window);
}

void ConfigDialog::Hide ()
{
	config_idletime = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (m_idle_spinbutton));
	free (config_filemanager);
	config_filemanager = strdup (gtk_entry_get_text (GTK_ENTRY (m_filemanager_entry)));
	write_config ();
	gtk_widget_hide (m_window);
}

void ConfigDialog::Show ()
{
	gtk_widget_show_all (m_window);
}