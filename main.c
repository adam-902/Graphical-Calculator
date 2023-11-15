#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <gtk/gtk.h>
#include <math.h>

#define MAX 1000

static float last_result;

typedef struct Node
{
    char op;
    float num;
    struct Node *left;
    struct Node *right;
} Node;

Node *newNode(char op, float num)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->op = op;
    node->num = num;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Helper function to check if a character is an operator
int isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// Recursive descent parser for constructing the expression tree
Node *parseExpression(const char *expression, int *index)
{
    // Parse the left operand
    Node *leftOperand = NULL;
    if (expression[*index] == '(')
    {
        (*index)++;
        leftOperand = parseExpression(expression, index);
        (*index)++;
    }
    else
    {
        leftOperand = newNode('\0', atof(expression + *index));
        while (expression[*index] != '\0' && !isOperator(expression[*index]) && expression[*index] != ')')
        {
            (*index)++;
        }
    }

    // Parse the operator
    char op = expression[*index];

    // Parse the right operand
    (*index)++;
    Node *rightOperand = NULL;
    if (isOperator(expression[*index]))
    {
        (*index)++;
        rightOperand = parseExpression(expression, index);
        (*index)++;
    }
    else
    {
        rightOperand = newNode('\0', atof(expression + *index));
        while (expression[*index] != '\0' && !isOperator(expression[*index]) && expression[*index] != ')')
        {
            (*index)++;
        }
    }

    // Create the current node
    Node *node = newNode(op, 0);
    node->left = leftOperand;
    node->right = rightOperand;

    return node;
}

Node *construct(const char *expression)
{
    int index = 0;
    return parseExpression(expression, &index);
}

float evaluate(Node *node)
{
    if (node->op == '+')
    {
        return evaluate(node->left) + evaluate(node->right);
    }
    else if (node->op == '-')
    {
        return evaluate(node->left) - evaluate(node->right);
    }
    else if (node->op == '*')
    {
        return evaluate(node->left) * evaluate(node->right);
    }
    else if (node->op == '/')
    {
        return evaluate(node->left) / evaluate(node->right);
    }
    else if (node->op == '^')
    {
        return pow(evaluate(node->left), evaluate(node->right));
    }
    else
    {
        return node->num;
    }
}

void plotGraph(const char *expression)
{
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set title '%s'\n", expression);

    char gnuplotExpression[MAX * 2];
    size_t j = 0;

    for (size_t i = 0; expression[i] != '\0'; ++i)
    {
        if (expression[i] == '^')
        {
            // Replace '^' with '**' in Gnuplot context
            gnuplotExpression[j++] = '*';
            gnuplotExpression[j++] = '*';
        }
        else
        {
            gnuplotExpression[j++] = expression[i];
        }
    }

    gnuplotExpression[j] = '\0';

    fprintf(gnuplotPipe, "plot [-10:10] %s with lines\n", gnuplotExpression);
    pclose(gnuplotPipe);
}

static void on_button_clicked(GtkButton *button, GtkEntry *entry)
{
    const gchar *button_text = gtk_button_get_label(button);

    if (strcmp(button_text, "=") == 0)
    {
        const char *entry_text = gtk_editable_get_text(GTK_EDITABLE(entry));

        if (strstr(entry_text, "sin("))
        {
            float angle_degrees = 0.0;
            sscanf(entry_text, "sin(%f)", &angle_degrees);

            // Convert the angle from degrees to radians
            float angle_radians = angle_degrees * (M_PI / 180.0);

            float result = sin(angle_radians);

            gtk_editable_set_text(GTK_EDITABLE(entry), "");

            char result_str[MAX];
            snprintf(result_str, MAX, "%g", result);
            gtk_editable_set_text(GTK_EDITABLE(entry), result_str);
        }
        else if (strstr(entry_text, "cos("))
        {
            float angle_degrees = 0.0;
            sscanf(entry_text, "cos(%f)", &angle_degrees);
            float angle_radians = angle_degrees * (M_PI / 180.0);

            float result = cos(angle_radians);

            gtk_editable_set_text(GTK_EDITABLE(entry), "");

            char result_str[MAX];
            snprintf(result_str, MAX, "%g", result);
            gtk_editable_set_text(GTK_EDITABLE(entry), result_str);
        }
        else if (strstr(entry_text, "tan("))
        {
            float angle_degrees = 0.0;
            sscanf(entry_text, "tan(%f)", &angle_degrees);
            float angle_radians = angle_degrees * (M_PI / 180.0);

            float result = tan(angle_radians);

            gtk_editable_set_text(GTK_EDITABLE(entry), "");

            char result_str[MAX];
            snprintf(result_str, MAX, "%g", result);
            gtk_editable_set_text(GTK_EDITABLE(entry), result_str);
        }
        else
        {
            Node *bt = construct(entry_text);
            float result = evaluate(bt);
            last_result = result;
            gtk_editable_set_text(GTK_EDITABLE(entry), "");
            char result_str[256];
            snprintf(result_str, 256, "%g", result);
            gtk_editable_set_text(GTK_EDITABLE(entry), result_str);
        }
    }
    else if (strcmp(button_text, "C") == 0)
    {
        const char *entry_text = gtk_editable_get_text(GTK_EDITABLE(entry));
        int length = strlen(entry_text);
        char *new_text = g_strndup(entry_text, length - 1);
        gtk_editable_set_text(GTK_EDITABLE(entry), new_text);
        g_free(new_text);
    }
    else if (strcmp(button_text, "AC") == 0)
    {
        gtk_editable_set_text(GTK_EDITABLE(entry), "");
    }
    else if (strcmp(button_text, "sin") == 0)
    {
        const char *entry_text = gtk_editable_get_text(GTK_EDITABLE(entry));
        gchar *new_text = g_strconcat(entry_text, "sin(", NULL);
        gtk_editable_set_text(GTK_EDITABLE(entry), new_text);
        g_free(new_text);
    }
    else if (strcmp(button_text, "cos") == 0)
    {
        const char *entry_text = gtk_editable_get_text(GTK_EDITABLE(entry));
        gchar *new_text = g_strconcat(entry_text, "cos(", NULL);
        gtk_editable_set_text(GTK_EDITABLE(entry), new_text);
        g_free(new_text);
    }
    else if (strcmp(button_text, "tan") == 0)
    {
        const char *entry_text = gtk_editable_get_text(GTK_EDITABLE(entry));
        gchar *new_text = g_strconcat(entry_text, "tan(", NULL);
        gtk_editable_set_text(GTK_EDITABLE(entry), new_text);
        g_free(new_text);
    }
    else if (strcmp(button_text, "PLOT") == 0)
    {
        const char *entry_text = gtk_editable_get_text(GTK_EDITABLE(entry));
        plotGraph(entry_text);
    }
    else if (strcmp(button_text, "Ans") == 0)
    {

        char result_str[MAX];
        snprintf(result_str, MAX, "%g", last_result);
        gtk_editable_set_text(GTK_EDITABLE(entry), result_str);
    }
    else
    {
        const char *entry_text = gtk_editable_get_text(GTK_EDITABLE(entry));
        gchar *new_text = g_strjoin("", entry_text, button_text, NULL);
        gtk_editable_set_text(GTK_EDITABLE(entry), new_text);
        g_free(new_text);
    }
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *entry;
    GtkWidget *vbox;
    GtkWidget *grid;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    entry = gtk_entry_new();
    gtk_box_append(GTK_BOX(vbox), entry);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

    const gchar *labels[] = {"^", "x", "y", "PLOT", "sin", "cos", "tan", "Ans", "C", "(", ")", "/", "7", "8", "9", "*", "4", "5", "6", "-", "1", "2", "3", "+", "AC", "0", ".", "="};

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gchar *label = g_strdup(labels[i * 4 + j]);
            GtkWidget *button = gtk_button_new_with_label(label);
            g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), entry);
            gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
            g_free(label);
        }
    }

    gtk_box_append(GTK_BOX(vbox), grid);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
