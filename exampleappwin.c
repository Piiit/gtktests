#include <gtk/gtk.h>
#include <evince-view.h>
#include <evince-document.h>


#include "exampleapp.h"
#include "exampleappwin.h"

struct _ExampleAppWindow
{
  GtkApplicationWindow parent;
};

struct _ExampleAppWindowClass
{
  GtkApplicationWindowClass parent_class;
};

typedef struct _ExampleAppWindowPrivate ExampleAppWindowPrivate;

struct _ExampleAppWindowPrivate
{
  GtkWidget *scrolledwindow;
  GtkWidget *appmenu;
};

G_DEFINE_TYPE_WITH_PRIVATE(ExampleAppWindow, example_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void
example_app_window_init (ExampleAppWindow *win)
{
	gtk_widget_init_template (GTK_WIDGET (win));
}

static void
example_app_window_class_init (ExampleAppWindowClass *class)
{
	gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (class), "/org/gtk/exampleapp/xreader.ui");
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), ExampleAppWindow, scrolledwindow);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), ExampleAppWindow, appmenu);
}

ExampleAppWindow *
example_app_window_new (ExampleApp *app)
{
  return g_object_new (EXAMPLE_APP_WINDOW_TYPE, "application", app, NULL);
}

void
example_app_window_open (ExampleAppWindow *win,
                         GFile            *file)
{
	g_print("INIT: %d\n", ev_init());
	ExampleAppWindowPrivate *priv = example_app_window_get_instance_private (win);
	GError *error = NULL;
	gchar *uri = g_file_get_uri(file);
	gchar *basename = g_file_get_basename(file);
	g_print("URI = %s; file = %s\n", uri, basename);

	EvDocument *doc = ev_document_factory_get_document(uri, &error);
	if (error) {
		g_printerr("ERROR: %s\n", error->message);
		g_error_free (error);
		return;
	}

	EvDocumentModel *model = ev_document_model_new_with_document(doc);
	EvView *view = EV_VIEW(ev_view_new());
	gtk_widget_show(GTK_WIDGET(view));
	gtk_container_add(GTK_CONTAINER(priv->scrolledwindow), GTK_WIDGET(view));
	ev_view_set_model(view, model);

	g_free(uri);
	g_free(basename);
}
