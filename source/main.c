#include "cc_array.h"
#include "t/calc.h"
#include <gtk/gtk.h>
#include <t/parser.h>

static GtkWidget *display;

num_t calculate(const char* expr, uint8_t *err)
{
    CC_Array* tokens = tokenize(expr);
    if(tokens == NULL)
    {
        *err = 1;
        return -1;
    }

    CC_Array* rpn = convert_rpn(tokens);
    if(rpn == NULL)
    {
        clear_token_arr(tokens);
        *err = 1;
        return -1;
    }

    return calc(rpn, err);
}

static void
on_button_clicked(GtkWidget *widget,
                 gpointer   data)
{
    const gchar *text = gtk_button_get_label(GTK_BUTTON(widget));
    const gchar *current_text = gtk_label_get_text(GTK_LABEL(display));
    
    gchar *new_text = g_strconcat(current_text, text, NULL);
    gtk_label_set_text(GTK_LABEL(display), new_text);
    g_free(new_text);
}

static void
on_clear_clicked(GtkWidget *widget,
                gpointer   data)
{
    gtk_label_set_text(GTK_LABEL(display), "");
}

void set_int64_to_label(GtkLabel *label, int64_t number) {
    char *text = g_strdup_printf("%" PRId64, number);
    gtk_label_set_text(label, text);
    g_free(text);
}

static void
on_equals_clicked(GtkWidget *widget,
                 gpointer   data)
{
    const gchar *current_text = gtk_label_get_text(GTK_LABEL(display));
    uint8_t err = 0;
    num_t res  = calculate((const char*) current_text, &err);

    if(err)
    {
        gtk_label_set_text(GTK_LABEL(display), "ERROR!!!");
    }
    else 
    {
        set_int64_to_label(GTK_LABEL(display), res);
    }
}

static void
create_calculator_button(GtkWidget *grid, const gchar *label, int col, int row)
{
    GtkWidget *button = gtk_button_new_with_label(label);
    
    // Make buttons expand to fill available space
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);
    
    // Set minimum size (will still scale up)
    gtk_widget_set_size_request(button, 40, 40);
    
    // Connect signals based on button type
    if (g_strcmp0(label, "C") == 0) {
        g_signal_connect(button, "clicked", G_CALLBACK(on_clear_clicked), NULL);
    } else if (g_strcmp0(label, "=") == 0) {
        g_signal_connect(button, "clicked", G_CALLBACK(on_equals_clicked), NULL);
    } else {
        g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);
    }
    
    // Add button to grid
    gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
}

static void
activate(GtkApplication *app,
        gpointer        user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    
    // Create the main window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK4 Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);
    
    // Set minimum window size
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 300);
    
    // Create the main container (vertical box)
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(vbox, 5);
    gtk_widget_set_margin_end(vbox, 5);
    gtk_widget_set_margin_top(vbox, 5);
    gtk_widget_set_margin_bottom(vbox, 5);
    gtk_window_set_child(GTK_WINDOW(window), vbox);
    
    // Create the display (a label)
    display = gtk_label_new("0");
    gtk_label_set_xalign(GTK_LABEL(display), 1.0); // Right align
    gtk_widget_set_hexpand(display, TRUE);
    
    // Style the display
    gtk_widget_add_css_class(display, "display");
    
    // Add display to the box
    gtk_box_append(GTK_BOX(vbox), display);
    
    // Create a separator between display and buttons
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(vbox), separator);
    
    // Create the button grid
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);
    gtk_box_append(GTK_BOX(vbox), grid);
    
    // // Calculator buttons layout
    // const gchar *buttons[4][4] = {
    //     {"7", "8", "9", "/"},
    //     {"4", "5", "6", "*"},
    //     {"1", "2", "3", "-"},
    //     {"C", "0", "=", "+"}
    // };
    const gchar *buttons[5][4] = {
        {"(", ")", "^", "%"},
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"C", "0", "=", "+"}

    };
    // Create and place buttons in the grid
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 4; col++) {
            create_calculator_button(grid, buttons[row][col], col, row);
        }
    }
    
    // Show the window
    gtk_window_present(GTK_WINDOW(window));
}

int
main(int    argc,
     char **argv)
{
    GtkApplication *app;
    int status;
    
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}