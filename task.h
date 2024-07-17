#ifndef TASK_H
#define TASK_H

typedef struct tTask {
    int id;
    char description[21]; // máx 20 caracteres, o 1 adicional é para o espaço em branco obrigatório
    int timeLimit;
    int status; // 1 - ativa | 0 - concluida
    struct tTask *left, *right;
} Task;

typedef struct tActiveTaskList {
    Task *task;
    struct tActiveTaskList *next;
} ActiveTaskList;

Task *findNodeInTree(Task *root, int taskId);

Task *findMinimumIdTask(Task *task);

Task *addTaskToTree(Task *root, Task *task);

Task *removeTaskFromTree(Task *root, int taskId);

void removeAllTasksFromTree(Task *root);

void printActiveTasks(Task *root);

void printFinishedTasks(Task *root);

#endif
