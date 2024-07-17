#include "task.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Busca pela task na árvore baseando-se no id fornecido.
 *
 * Utiliza recursão indo para a direita caso o id fornecido
 * seja maior do que o id da task atual, ou para a esquerda
 * caso contrário.
 */
Task *findNodeInTree(Task *root, int taskId) {
    if (root == NULL || root->id == taskId) {
        return root;
    }
    if (taskId > root->id) {
        return findNodeInTree(root->right, taskId);
    }
    return findNodeInTree(root->left, taskId);
}

/**
 * Busca pela task com o menor id na árvore.
 *
 * Implementa um while loop, indo sempre para a esquerda,
 * até encontrar o fim da árvore, onde está a task
 * com o menor id.
 */
Task *findMinimumIdTask(Task *task) {
    Task *current = task;
    while (current != NULL && current->left != NULL) {
        current = current->left;
    }
    return current;
}

/**
 * Adiciona uma nova task na árvore.
 *
 * Se o id da task a ser adicionada for menor do que o id
 * da task root, a função é re-chamada na sub-árvore da esquerda,
 * caso contrário, é re-chamada na sub-árvore da direita.
 *
 * A task é adicionada quando um nó vazio é encontrado.
 */
Task *addTaskToTree(Task *root, Task *task) {
    if (root == NULL) {
        root = task;
        return root;
    }
    if (task->id < root->id) {
        root->left = addTaskToTree(root->left, task);
    } else {
        root->right = addTaskToTree(root->right, task);
    }
    return root;
}

/**
 * Remove uma task da árvore.
 *
 * Utiliza recursão para encontrar a task com o id fornecido,
 * se o id fornecido for menor do que o id da task atual a função
 * é re-chamada na sub-árvore da esquerda, caso contrário, é re-chamada
 * na sub-árvore da direita.
 *
 * Quando a task é encontrada ela é removida da árvore.
 */
Task *removeTaskFromTree(Task *root, int taskId) {
    if (root == NULL) {
        return root;
    }
    if (taskId < root->id) {
        root->left = removeTaskFromTree(root->left, taskId);
    } else if (taskId > root->id) {
        root->right = removeTaskFromTree(root->right, taskId);
    } else {
        Task *temp;
        if (root->left == NULL) {
            temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            temp = root->left;
            free(root);
            return temp;
        }
        temp = findMinimumIdTask(root->right);
        root->id = temp->id;
        strcpy(root->description, temp->description);
        root->timeLimit = temp->timeLimit;
        root->status = temp->status;
        root->right = removeTaskFromTree(root->right, temp->id);
    }
    return root;
}

/**
 * Remove todas as tasks da árvore.
 *
 * Percorre toda a árvore, removendo todas as tasks encontradas.
 */
void removeAllTasksFromTree(Task *root) {
    if (root == NULL) {
        return;
    }
    removeAllTasksFromTree(root->left);
    removeAllTasksFromTree(root->right);
    free(root);
}

/**
 * Imprime todas as tasks concluídas.
 *
 * Percorre a árvore imprimindo todas as tasks com status == 0.
 */
void printFinishedTasks(Task *root) {
    if (root == NULL) {
        return;
    }
    printFinishedTasks(root->left);
    if (root->status == 0) {
        printf("| %-5d | %-20s | %-16d | %-9s|\n", root->id, root->description, root->timeLimit, "Concluida");
    }
    printFinishedTasks(root->right);
}

/**
 * Adiciona as tasks para a lista encadeada temporária
 *
 * Cria um novo nó contendo a task e percorre a lista encadeada temporária até
 * encontrar o fim, adicionando a task
 */
ActiveTaskList *appendTaskToActiveTaskList(ActiveTaskList *head, Task *task) {
    ActiveTaskList *node = malloc(sizeof(ActiveTaskList));
    node->task = task;
    node->next = NULL;
    ActiveTaskList *currentNode = head;
    if (currentNode == NULL) {
        return node;
    }
    while (currentNode->next != NULL) {
        currentNode = currentNode->next;
    }
    currentNode->next = node;
    return head;
}

/**
 * Cria uma lista encadeada temporária com as tasks ativas
 *
 * Recursivamente percorre toda a árvore buscando por tasks ativas (com status == 1),
 * quando uma task é encontrada, ela é adicionada na lista encadeada temporária
 */
ActiveTaskList *createActiveTaskList(ActiveTaskList *activeTaskList, Task *root) {
    if (root == NULL) {
        return activeTaskList;
    }
    if (root->status == 1) {
        activeTaskList = appendTaskToActiveTaskList(activeTaskList, root);
    }
    activeTaskList = createActiveTaskList(activeTaskList, root->left);
    activeTaskList = createActiveTaskList(activeTaskList, root->right);
    return activeTaskList;
}

/**
 * Ordena a lista encadeada temporária pelo timeLimit
 *
 * Implementa o algoritmo Bubble Sort para ordenar as tasks na lista encadeada temporária,
 * baseando-se pelo timeLimit, organizando em ordem crescente
 *
 * Complexidade: 0(n^2)
 */
ActiveTaskList *sortActiveTaskList(ActiveTaskList *head) {
    // Cria uma variável armazenando o primeiro nó da lista.
    ActiveTaskList *currentNode = head;
    // Executa um loop até que o nó atual seja NULL (fim da lista).
    while (currentNode != NULL) {
        // Cria uma variável para armazenar o próximo nó da lista.
        ActiveTaskList *nextNode = currentNode->next;
        // Executa um loop até que o próximo nó seja NULL (fim da lista).
        while (nextNode != NULL) {
            // Se o timeLimit da task atual for maior que o da próxima task
            // inverte a posição das tasks.
            if (currentNode->task->timeLimit > nextNode->task->timeLimit) {
                // Cria uma váriavel temporária para armazenar a task atual.
                Task *temp = currentNode->task;
                // Atribui a próxima task ao nó atual.
                currentNode->task = nextNode->task;
                // Atribui a task atual (salva na variável temporária) ao próximo nó.
                nextNode->task = temp;
            }
            // Re-atribui o próximo nó para continuar a iteração
            nextNode = nextNode->next;
        }
        // Re-atribui o nó atual para continuar a iteração
        currentNode = currentNode->next;
    }
    return head;
}

/**
 * Deleta a lista encadeada temporária
 *
 * Percorre toda a lista encadeada temporária, liberando todos os valores da memória
 */
void deleteActiveTaskList(ActiveTaskList *head) {
    ActiveTaskList *currentTask = head;
    while (currentTask != NULL) {
        ActiveTaskList *nextTask = currentTask->next;
        free(currentTask);
        currentTask = nextTask;
    }
}

/**
 * Imprime todas as tasks ativas.
 *
 * Primeiro é criada uma Linked List temporária para armezar as tasks ativas,
 * após isso, a lista é ordenada pelo timeLimit em ordem crescente, quando feito
 * as tasks são imprimidas na tela, e ao final, a Linked List temporária é apagada
 */
void printActiveTasks(Task *root) {
    ActiveTaskList *taskList = NULL;
    taskList = createActiveTaskList(taskList, root);
    taskList = sortActiveTaskList(taskList);
    while (taskList != NULL) {
        Task *task = taskList->task;
        printf("| %-5d | %-20s | %-16d | %-9s|\n", task->id, task->description, task->timeLimit, "Ativa");
        taskList = taskList->next;
    }
    deleteActiveTaskList(taskList);
    taskList = NULL;
}
