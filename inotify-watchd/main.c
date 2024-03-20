#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>

#include <sys/inotify.h>

#define ERR_TOO_FEW_ARGS 1
#define ERR_BASE_PATH_NULL 2
#define ERR_INOTIFY_INIT 3
#define ERR_INOTIFY_ADD_WATCH 4
#define ERR_READ_INOTIFY 5

#define INOTIFY_MASK IN_CREATE | IN_DELETE | IN_ACCESS | IN_CLOSE_WRITE | IN_MODIFY | IN_MOVE_SELF

#define EXIT(ERR_CODE, ERR_MSG) \
  {                             \
    fprintf(stderr, ERR_MSG);   \
    exit(ERR_CODE);             \
  }

#define EXITSIG                  \
  signal(SIGABRT, exit_handler); \
  signal(SIGINT, exit_handler);  \
  signal(SIGTERM, exit_handler);

int inotify_event_queue = -1;
int inotify_event_status = -1;

void exit_handler(int signal)
{
  printf("\nCleaning up...\n");
  int inotify_event_rm = -1;
  if ((inotify_event_rm = inotify_rm_watch(inotify_event_queue, inotify_event_status)) < 0)
    fprintf(stderr, "ERR removing inotify watch");

  close(inotify_event_queue);
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
    EXIT(ERR_TOO_FEW_ARGS, "USAGE: watcher PATH\n");

  const struct inotify_event *watchEvent;

  char buffer[4096] = {0};
  char *base_path = (char *)malloc(sizeof(char) * (strlen(argv[1]) + 1));

  strcpy(base_path, argv[1]);

  char *token = strtok(base_path, "/");
  while (token != NULL)
  {
    base_path = token;
    token = strtok(NULL, "/");
  }

  if (base_path == NULL)
    EXIT(ERR_BASE_PATH_NULL, "ERR getting base_path\n");

  if ((inotify_event_queue = inotify_init()) < 0)
    EXIT(ERR_INOTIFY_INIT, "ERR initializing inotify instance\n")

  if ((inotify_event_status = inotify_add_watch(inotify_event_queue, argv[1], INOTIFY_MASK)) < 0)
    EXIT(ERR_INOTIFY_ADD_WATCH, "ERR adding inotify watch\n");

  EXITSIG

  while (true)
  {
    int read_len;
    if ((read_len = read(inotify_event_queue, buffer, sizeof(buffer))) < 0)
      EXIT(ERR_READ_INOTIFY, "ERR reading inotify instance\n");

    for (char *buffer_ptr = buffer; buffer_ptr < buffer + read_len; buffer_ptr += sizeof(struct inotify_event) + watchEvent->len)
    {
      char *notification_message = NULL;

      watchEvent = (const struct inotify_event *)buffer_ptr;

      if (watchEvent->mask & IN_CREATE)
        notification_message = "FILE created\n";

      if (watchEvent->mask & IN_DELETE)
        notification_message = "FILE deleted\n";

      if (watchEvent->mask & IN_ACCESS)
        notification_message = "FILE accessed\n";

      if (watchEvent->mask & IN_CLOSE_WRITE)
        notification_message = "FILE written and closed\n";

      if (watchEvent->mask & IN_MODIFY)
        notification_message = "FILE modified\n";

      if (watchEvent->mask & IN_MOVE_SELF)
        notification_message = "FILE moved\n";

      if (notification_message == NULL)
        continue;

      printf("%s %s\n", argv[1], notification_message);
    }
  }
}