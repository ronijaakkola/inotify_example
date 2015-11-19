#include <stdio.h>
#include <errno.h>

#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
/* TODO: What the size of the buffer should be? */
#define BUFFER_SIZE (10 * (sizeof(struct inotify_event) + 16))

int main()
{
   int i;
   int readLength;
   int fd, wd;
   char buffer[BUFFER_SIZE];

   printf( "inotify example program\n");
   
   /* Initialize inotify instance */
   fd = inotify_init();
   if ( fd < 0 ) {
      perror("inotify_init");
   }

   /* Add directory to the watch list.
      In this case, as a mask parameter we specify what events we want to
      add a watch for. */
   wd = inotify_add_watch(fd, "/tmp", IN_CREATE | IN_DELETE);
   if ( fd < 0 ) {
      perror("inotify_add_watch");
   }

   /* Read from the event buffer indefinitely. */
   for (;;) {
      readLength = read(fd, buffer, BUFFER_SIZE);
      if (readLength < 0) {
         perror("read");
      }
      else {
         /* TODO: Recognize the event here */
         printf( "event read from the buffer\n");
      }
   }

   /* Remove the watch from the watch list */
   inotify_rm_watch(fd, wd);

   /* Close the instance */
   close(fd);

   return 0;
}
