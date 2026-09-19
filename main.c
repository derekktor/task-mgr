#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S_T 20
#define S_D 100

typedef struct Task
{
    int priority;
    char title[S_T];
    char desc[S_D];
} task;

typedef struct Node
{
    task data;
    struct Node *next;
} node;

/**
 * Removes last newline char
 */
void trim(char t[])
{
    int idx = 0;
    while (t[idx] != '\n')
    {
        idx++;
    }
    t[idx] = '\0';
}

/**
 * Prints task details: priority, title, description
 */
void printTask(int i, task t)
{
    printf("%d\t%d\t%s\t\t%s\n", i, t.priority, t.title, t.desc);
}

/**
 * Prompts the user to enter task details and adds it to the list
 */
void createTask(node **n)
{
    printf("--- Creating task ---\n");
    task t = {0};

    printf("title:\n");
    fgets(t.title, S_T, stdin);
    trim(t.title);

    printf("description:\n");
    fgets(t.desc, S_D, stdin);
    trim(t.desc);

    printf("priority = ");
    scanf("%d", &t.priority);

    node *newNode = malloc(sizeof(node));

    if (newNode == NULL)
    {
        perror("ERR: can't allocate memory for pointer\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = t;
    newNode->next = NULL;

    // place at the front
    if (*n == NULL || t.priority > (*n)->data.priority)
    {
        newNode->next = *n;
        *n = newNode;
        return;
    }

    node *current = *n;

    // place at the back
    while (current->next != NULL && current->next->data.priority >= t.priority)
    {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

/**
 * Frees the dynamically created memory
 */
void freeList(node *n)
{
    while (n != NULL)
    {
        node *next = n->next;
        free(n);
        n = next;
    }
}

/**
 * Traverses the list and prints task details
 */
void printTasks(node *n)
{
    printf("--- TASKS ---\n");
    int counter = 1;
    while (n != NULL)
    {
        printTask(counter, n->data);
        n = n->next;
        counter++;
    }
    printf("-------------\n");
}

/**
 * Saves current list of tasks to tasks.txt file
 */
void saveTasks(node *n)
{
    printf("--- Saving tasks ----\n");
    FILE *f = fopen("tasks.txt", "w");

    while (n != NULL)
    {
        fprintf(f, "%d\n", n->data.priority);
        fprintf(f, "%s\n", n->data.title);
        fprintf(f, "%s\n", n->data.desc);
        n = n->next;
    }

    fclose(f);
}

node *loadTasks()
{
    FILE *f = fopen("tasks.txt", "r");

    if (f == NULL)
    {
        perror("ERR: no file found\n");
        exit(EXIT_FAILURE);
    }

    node *root = NULL;
    node *tail = NULL;

    while (1)
    {
        node *newNode = malloc(sizeof(node));

        if (newNode == NULL)
        {
            perror("ERR: can't allocate memory for new node\n");
            exit(EXIT_FAILURE);
        }

        if (fscanf(f, "%d\n", &newNode->data.priority) != 1)
        {
            free(newNode);
            break;
        }

        if (fgets(newNode->data.title, S_T, f) == NULL)
        {
            free(newNode);
            break;
        }

        trim(newNode->data.title);

        if (fgets(newNode->data.desc, S_D, f) == NULL)
        {
            free(newNode);
            break;
        }

        trim(newNode->data.title);

        newNode->next = NULL;

        if (root == NULL)
        {
            root = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }

    fclose(f);
    return root;
}

void showInstructions()
{
    printf("q - quit, p - print tasks, c - create task, d - delete task, e - edit desc, u - update priority\n");
}

void deleteTask(node **root, int idx)
{
    printf("--- Deleting task %d ---\n", idx);
    // if the list is empty
    if (*root == NULL)
    {
        printf("Task list is empty\n");
        return;
    }

    // if it's the first node
    if (idx == 1)
    {
        node *temp = *root;
        *root = (*root)->next;
        printf("%s\t%s\n", temp->data.title, temp->data.desc);
        free(temp);
        return;
    }

    // find node before target node
    node *current = *root;
    int i = 1;
    while (current->next != NULL)
    {
        if (i + 1 == idx)
        {
            node *temp = current->next;
            current->next = temp->next;
            printf("%s\t%s\n", temp->data.title, temp->data.desc);
            free(temp);
            return;
        }

        current = current->next;
        i++;
    }
}

void editDescription(node *root, int idx)
{
    int i = 1;

    while (root != NULL)
    {
        if (i == idx)
        {
            printf("new description: (%s, %s)\n", root->data.title, root->data.desc);
            fgets(root->data.desc, S_D, stdin);
            trim(root->data.desc);
            return;
        }

        root = root->next;
        i++;
    }

    printf("ERR: task index %d not found\n", idx);
}

void updatePriority(node **root, int idx)
{
    int i = 1;
    node *current = *root;
    node *prev = NULL;

    // Find task
    while (current != NULL && i < idx)
    {
        prev = current;
        current = current->next;
        i++;
    }

    if (current == NULL)
    {
        printf("ERR: task index %d not found\n", idx);
        return;
    }

    // Remove current node from list
    if (prev == NULL)
        *root = current->next;
    else
        prev->next = current->next;

    // Get new priority
    printf("new priority = ");
    scanf("%d", &current->data.priority);
    getchar();

    // Reinsert at correct position
    if (*root == NULL ||
        current->data.priority > (*root)->data.priority)
    {
        current->next = *root;
        *root = current;
        return;
    }

    node *p = *root;

    while (p->next != NULL &&
           p->next->data.priority >= current->data.priority)
    {
        p = p->next;
    }

    current->next = p->next;
    p->next = current;
}

int main(int argc, char *argv[])
{
    printf("Tasks:\n");
    node *root = loadTasks();
    printTasks(root);

    char res;
    while (res != 'q')
    {
        showInstructions();
        scanf(" %c", &res);
        getchar();

        if (res == 'c')
        {
            createTask(&root);
        }
        else if (res == 'p')
        {
            printTasks(root);
        }
        else if (res == 'd')
        {
            int taskIdx;
            printf("Task idx = ");
            scanf("%d", &taskIdx);
            getchar();
            deleteTask(&root, taskIdx);
        }
        else if (res == 'e')
        {
            int taskIdx;
            printf("Task idx = ");
            scanf("%d", &taskIdx);
            getchar();
            editDescription(root, taskIdx);
        }
        else if (res == 'u')
        {
            int taskIdx;
            printf("Task idx = ");
            scanf("%d", &taskIdx);
            getchar();

            updatePriority(&root, taskIdx);
        }
    }

    saveTasks(root);
    freeList(root);

    return 0;
}