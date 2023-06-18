#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_RESOURCES 10

typedef enum {
	BLOCKED, RUNNING, FINISHED
} State;

typedef struct {
	int id;
	int priority;
	State state;
	int *resources;
	pthread_t thread;
} pcb_t;

typedef struct {
	int available[MAX_RESOURCES];
	int allocated[MAX_TASKS][MAX_RESOURCES];
	int maximum[MAX_TASKS][MAX_RESOURCES];
	int need[MAX_TASKS][MAX_RESOURCES];
} resource_t;

pthread_t scheduler_thread;
pthread_t deadlock_thread;
pthread_mutex_t mutex;
pthread_cond_t cond;
sem_t resource_sem;
pcb_t task_list[MAX_TASKS];
int task_count = 0;
resource_t resource_table;

void* scheduler_fn(void *arg) {
  while (1) {
    pthread_mutex_lock(&mutex);
    int highest_priority = -1;
    pcb_t *next_task = NULL;
    for (int i = 0; i < MAX_TASKS; i++) {
      if (task_list[i].id != -1 && task_list[i].state == BLOCKED && task_list[i].priority > highest_priority) {
	    highest_priority = task_list[i].priority;
	    next_task = &task_list[i];
}
}
	if (next_task != NULL) {
	  next_task->state = RUNNING;
	  pthread_mutex_unlock(&mutex);
	  task_fn(next_task);
      pthread_mutex_lock(&mutex);
      next_task->state = FINISHED;
	  pthread_cond_broadcast(&cond);
}
	  pthread_mutex_unlock(&mutex);
}
	  return NULL;
}

void* deadlock_fn(void *arg) {
  while (1) {
    pthread_mutex_lock(&mutex);
    int deadlock_detected = 0;
	for (int i = 0; i < MAX_TASKS; i++) {
      if (task_list[i].id != -1 && task_list[i].state == RUNNING) {
	    int can_be_allocated = 1;
	    for (int j = 0; j < MAX_RESOURCES; j++) {
		  if (task_list[i].resources[j] > resource_table.available[j]) {
		    can_be_allocated = 0;
			break;
}
}
      if (can_be_allocated) {
	    task_list[i].state = BLOCKED;
	    deadlock_detected = 1;
		pthread_cond_broadcast(&cond);
		break;
}
}
}
      if (!deadlock_detected) {
	    pthread_cond_wait(&cond, &mutex);
}
		pthread_mutex_unlock(&mutex);
}
	    return NULL;
}

void initialize() {
  for (int i = 0; i < MAX_TASKS; i++) {
    task_list[i].id = -1;
}
	task_count = 0;

	pthread_create(&scheduler_thread, NULL, scheduler_fn, NULL);
	pthread_create(&deadlock_thread, NULL, deadlock_fn, NULL);
    pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
    sem_init(&resource_sem, 0, 1);
    printf("Enter resources:\n");
  for (int i = 0; i < MAX_RESOURCES; i++) {
	printf("Resource %d: ", i);
	scanf("%d", &resource_table.available[i]);
}
}

void destroy() {
  pthread_cancel(scheduler_thread);
  pthread_cancel(deadlock_thread);
  pthread_join(scheduler_thread, NULL);
  pthread_join(deadlock_thread, NULL);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  sem_destroy(&resource_sem);
}

void task_fn(pcb_t *task) {
  printf("Running Task %d\n", task->id);
  sem_wait(&resource_sem);
  for (int i = 0; i < MAX_RESOURCES; i++) {
    resource_table.available[i] -= task->resources[i];
    resource_table.allocated[task->id][i] += task->resources[i];
    resource_table.need[task->id][i] -= task->resources[i];
}
	sem_post(&resource_sem);
    sleep(20);
    sem_wait(&resource_sem);
  for (int i = 0; i < MAX_RESOURCES; i++) {
    resource_table.available[i] += task->resources[i];
    resource_table.allocated[task->id][i] -= task->resources[i];
    resource_table.need[task->id][i] += task->resources[i];
}
	sem_post(&resource_sem);
}

void create_task(int priority, int *resources, int num_resources) {
  pthread_mutex_lock(&mutex);
  if (task_count < MAX_TASKS) {
    for (int i = 0; i < MAX_TASKS; i++) {
      if (task_list[i].id != -1 && task_list[i].priority == priority) {
        printf("Tasks cant have the same priority.\n");
        pthread_mutex_unlock(&mutex);
        return;
}
}
    int higher_priority_task = -1;
    for (int i = 0; i < MAX_TASKS; i++) {
      if (task_list[i].id != -1 && task_list[i].priority > priority) {
        int same_resources = 1;
        for (int j = 0; j < num_resources; j++) {
          if (resources[j] != task_list[i].resources[j]) {
            same_resources = 0;
            break;
}
}
        if (same_resources) {
          higher_priority_task = i;
          break;
}
}
}
    if (higher_priority_task != -1) {
      task_list[task_count].id = task_count;
	  task_list[task_count].priority = priority;
      task_list[task_count].state = RUNNING;
      task_list[task_count].resources = malloc(
      sizeof(int) * num_resources);
      memcpy(task_list[task_count].resources, resources,
      sizeof(int) * num_resources);
      pthread_cond_broadcast(&cond);
      pthread_mutex_unlock(&mutex);
      pthread_mutex_lock(&mutex);
      while (task_list[task_count].state == RUNNING) {
        pthread_cond_wait(&cond, &mutex);
}
        pthread_mutex_unlock(&mutex);
} else {
    int task_id = -1;
	for (int i = 0; i < MAX_TASKS; i++) {
	  if (task_list[i].id == -1) {
	    task_id = i;
		break;
}
}
  if (task_id != -1) {
    task_list[task_id].id = task_id;
    task_list[task_id].priority = priority;
    task_list[task_id].state = BLOCKED;
    task_list[task_id].resources = malloc(
    sizeof(int) * num_resources);
    memcpy(task_list[task_id].resources, resources,
    sizeof(int) * num_resources);
    pthread_create(&task_list[task_id].thread, NULL, task_fn,&task_list[task_id]);
    task_count++;
    printf("Task %d created\n", task_id);
}
}
} else {
    printf("Cant create task. Max task amount reached.\n");
}
    pthread_mutex_unlock(&mutex);
}

void destroy_task(int task_id) {
  pthread_mutex_lock(&mutex);
  if (task_id >= 0 && task_id < MAX_TASKS && task_list[task_id].id != -1) {
    pthread_cancel(task_list[task_id].thread);
    pthread_join(task_list[task_id].thread, NULL);
    sem_wait(&resource_sem);
    for (int i = 0; i < MAX_RESOURCES; i++) {
      resource_table.available[i] += resource_table.allocated[task_id][i];
      resource_table.allocated[task_id][i] = 0;
      resource_table.need[task_id][i] = 0;
}
    sem_post(&resource_sem);
    free(task_list[task_id].resources);
    task_list[task_id].id = -1;
    task_count--;
    printf("Task %d destroyed\n", task_id);
} else {
    printf("Invalid task ID\n");
}
    pthread_mutex_unlock(&mutex);
}

void list_tasks() {
  pthread_mutex_lock(&mutex);
  printf("----------------------------------------\n");
  for (int i = 0; i < MAX_TASKS; i++) {
    if (task_list[i].id != -1) {
      printf("Task ID: %d, Priority: %d, State: ", task_list[i].id,
      task_list[i].priority);
      switch (task_list[i].state) {
        case BLOCKED:
          printf("Blocked\n");
          break;
        case RUNNING:
          printf("Running\n");
          break;
        case FINISHED:
          printf("Finished\n");
          break;
}
}
}
  printf("----------------------------------------\n");
  pthread_mutex_unlock(&mutex);
}

int main() {
  initialize();
  while (1) {
    printf("Enter a command ('c' = create, 'd' = destroy, 'l' = list, 'e' = exit): ");
    char command[10];
    scanf("%s", command);
    if (strcmp(command, "c") == 0) {
      int priority;
      int num_resources;
      int resources[MAX_RESOURCES];
      printf("Enter priority: ");
      scanf("%d", &priority);
      printf("Enter the amount of resources this task will use: ");
      scanf("%d", &num_resources);
      printf("Enter the required resources: ");
      for (int i = 0; i < num_resources; i++) {
        scanf("%d", &resources[i]);
}
      create_task(priority, resources, num_resources);
} else if (strcmp(command, "d") == 0) {
    int task_id;
    printf("Enter task ID: ");
    scanf("%d", &task_id);
    destroy_task(task_id);
} else if (strcmp(command, "l") == 0) {
    list_tasks();
} else if (strcmp(command, "e") == 0) {
    break;
} else {
    printf("Invalid command\n");
}
}
  destroy();
  return 0;
}
