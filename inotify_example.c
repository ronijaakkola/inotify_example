#include <stdio.h>
#include <errno.h>

#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
/* TODO: What the size of the buffer should be? */
#define BUFFER_SIZE (10 * (sizeof(struct inotify_event) + 16))

static void
eventInformation(struct inotify_event *e)
{
  printf("File%2d; ", e->wd);

  // Different event types
  printf("mask = ");
    if (e->mask & IN_ACCESS)        printf("IN_ACCESS ");
    if (e->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
    if (e->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
    if (e->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE ");
    if (e->mask & IN_CREATE)        printf("IN_CREATE ");
    if (e->mask & IN_DELETE)        printf("IN_DELETE ");
    if (e->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
    if (e->mask & IN_IGNORED)       printf("IN_IGNORED ");
    if (e->mask & IN_ISDIR)         printf("IN_ISDIR ");
    if (e->mask & IN_MODIFY)        printf("IN_MODIFY ");
    if (e->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
    if (e->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
    if (e->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");
    if (e->mask & IN_OPEN)          printf("IN_OPEN ");
    if (e->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
    if (e->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
    printf("\n");
}

int main(int argc, char **argv)
{
  int i;
  int j = 1;
  char *k;
  ssize_t readLength;
  int fd, wd;
  char buffer[BUFFER_SIZE];
  struct inotify_event *event;

  printf("Inotify example program. Give file names as parameters for the program.\n");

  if(argc < 2)
  {
    printf("No files given.\n");
    return 0;
  }

  /* Initialize inotify instance */
  fd = inotify_init();
  if ( fd < 0 ) {
  perror("inotify_init");
  }
    for(j; j < argc; ++j)
    {
      printf("Starting to watch file: %s\n", argv[j]);

      /* Add directory to the watch list.
      In this case, as a mask parameter we specify what events we want to
      add a watch for. */

      wd = inotify_add_watch(fd, argv[j], IN_ALL_EVENTS);
      if ( wd < 0 )
      {
        perror("inotify_add_watch");
      }
  }

  printf("All files initialized.\n");
  printf("---------------------------------------------\n");

  /* Read from the event buffer indefinitely. */
  for (;;) {
    readLength = read(fd, buffer, BUFFER_SIZE);
    if (readLength < 0) {
      perror("read");
    }
    else {
      /* Event found */
      printf("Read %ld bytes from inotify fd\n", (long) readLength);
      // Get information from event
      for(k = buffer; k < buffer + readLength;)
      {
        event = (struct inotify_event *) k;
        eventInformation(event);
        k += sizeof(struct inotify_event) + event->len;
      }
      printf("---------------------------------------------\n");
    }
  }

  /* Remove the watch from the watch list */
  inotify_rm_watch(fd, wd);

  /* Close the instance */
  close(fd);

  return 0;
}
