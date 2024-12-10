#include "forms.h"
#include "gvs_gui.h"
#include <signal.h>

void initialize_gvs(FD_gvs *fd_gvs);

char jobs_dbfile[50];
FD_gvs *fd_gvs;
FD_cancel *fd_cancel;
FD_jobq *fd_jobq;
FD_options *fd_options;
FD_source *fd_source;
FD_times *fd_times;
FD_usage *fd_usage;

void timer(int n)
{
  /* Periodically do things. */
  /* Determine if there are Jobs queued and/or running. */
  set_queued_running_status(jobs_dbfile);

  alarm(30);
  signal(SIGALRM, timer);
}


int main(int argc, char *argv[])
{
  int dummy = 0;
  int i;
  
   fl_initialize(&argc, argv, 0, 0, 0);
   fd_gvs = create_form_gvs();
   fd_cancel = create_form_cancel();
   fd_jobq = create_form_jobq();

   fd_options  = create_form_options();

   fd_source = create_form_source();
   fd_times = create_form_times();
   fd_usage = create_form_usage();

   /* Set the name of the database file. Each user has their own file. */
   sprintf(jobs_dbfile, "/tmp/gvsjobs_%d.gdbm", getuid());
   /* fill-in form initialization code */
   initialize_gvs(fd_gvs);

   /* Periodic updates, semi-live job queue status. */
   signal(SIGALRM, timer);
   alarm(30);

   /* show the first form */
   fl_show_form(fd_gvs->gvs,FL_PLACE_CENTER,FL_FULLBORDER,"gvs");
#ifdef GVS_SOUND
   system("cat /user/merritt/au/welcome.au > /dev/audio&");
#endif
   while(1)
	 fl_do_forms();

   return 0;
}
