#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>

#include <sys/inotify.h>

#define BUFFSIZE 4096

#define INOTIFY_MASK IN_CREATE | IN_DELETE | IN_ACCESS | IN_CLOSE_WRITE | IN_MODIFY | IN_MOVE_SELF

#define ERREXT(ERR_CODE, ERR_MSG) \
  {                               \
    fprintf(stderr, ERR_MSG);     \
    exit(ERR_CODE);               \
  }

#define SIGEXT                     \
  signal(SIGABRT, sigext_handler); \
  signal(SIGINT, sigext_handler);  \
  signal(SIGTERM, sigext_handler);

#define ERR_TOO_FEW_ARGS 1
#define ERR_PATH_NULL 2
#define ERR_FILENAME_NULL 3
#define ERR_INOTIFY_INIT 4
#define ERR_INOTIFY_WATCH 5
#define ERR_READ_INOTIFY 6

int inotify_event_queue = -1;
int inotify_event_status = -1;

void sigext_handler(int s);

int main(int argc, char *argv[])
{
  if (argc < 2)
    ERREXT(ERR_TOO_FEW_ARGS, "USAGE: watchd PATH\n");

  const struct inotify_event *watchEvent;

  char buffer[BUFFSIZE] = {0};

  char *path = (char *)malloc(sizeof(char) * (strlen(argv[1]) + 1));

  if (path == NULL)
    ERREXT(ERR_PATH_NULL, "ERR getting path\n");

  strcpy(path, argv[1]);

  char *token = strtok(argv[1], "/");
  char *tokens[] = {NULL};

  int t_count = 0;
  do
    tokens[t_count++] = token;
  while ((token = strtok(NULL, "/")) != NULL);

  char *filename = NULL;

  if ((filename = tokens[t_count - 1]) == NULL)
    ERREXT(ERR_FILENAME_NULL, "ERR getting filename");

  if ((inotify_event_queue = inotify_init()) < 0)
    ERREXT(ERR_INOTIFY_INIT, "ERR initializing inotify instance\n")

  if ((inotify_event_status = inotify_add_watch(inotify_event_queue, path, INOTIFY_MASK)) < 0)
    ERREXT(ERR_INOTIFY_WATCH, "ERR adding inotify watch\n");

  SIGEXT;

  free(path);

  while (true)
  {
    int read_len = -1;
    if ((read_len = read(inotify_event_queue, buffer, BUFFSIZE)) < 0)
      ERREXT(ERR_READ_INOTIFY, "ERR reading inotify instance\n");

    for (char *buffer_ptr = buffer; buffer_ptr < buffer + read_len; buffer_ptr += sizeof(struct inotify_event) + watchEvent->len)
    {
      char *notification_message = NULL;

      watchEvent = (const struct inotify_event *)buffer_ptr;

      if (watchEvent->mask & IN_CREATE)
        notification_message = "FILE created";

      if (watchEvent->mask & IN_DELETE)
        notification_message = "FILE deleted";

      if (watchEvent->mask & IN_ACCESS)
        notification_message = "FILE accessed";

      if (watchEvent->mask & IN_CLOSE_WRITE)
        notification_message = "FILE written and closed";

      if (watchEvent->mask & IN_MODIFY)
        notification_message = "FILE modified";

      if (watchEvent->mask & IN_MOVE_SELF)
        notification_message = "FILE moved";

      if (notification_message == NULL)
        continue;

      printf("%s: %s\n", filename, notification_message);
    }
  }
}

void sigext_handler(int s)
{
  int inotify_event_remove = -1;

  if ((inotify_event_remove = inotify_rm_watch(inotify_event_queue, inotify_event_status)) < 0)
    fprintf(stderr, "ERR removing inotify watch\n");
  else
  {
    printf("\nSIGNAL %i: Cleaning up...\n", s);

    close(inotify_event_queue);

    exit(EXIT_SUCCESS);
  }
}
