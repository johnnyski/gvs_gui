#include "forms.h"
#include "gvs_gui.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <gdbm.h>
#include <errno.h>

#define GVS_VERSION_NUM "1.7"
#define GVS_VERSION_STR "GVS user interface version " GVS_VERSION_NUM " January 9, 1998."
#define MAX_STR_LEN 256

/*
 * The number of actual products is one less than NUM_PRODUCTS. 
 * I use the extra index to store general options for 'all_tape_level_1n2'.
 */
#define NUM_L1_PRODUCTS 1
#define NUM_L2_PRODUCTS 3
#define NUM_L3_PRODUCTS 4
#define NUM_SCRIPTS 3
#define NUM_PRODUCTS (NUM_L1_PRODUCTS + NUM_L2_PRODUCTS + NUM_L3_PRODUCTS + NUM_SCRIPTS)

char *product[NUM_PRODUCTS] = {  "level_1",
								 "2A-53-v4",
								 "2A-54",
								 "2A-55",
								 "2A-52i",
								 "3A-53",
								 "3A-54",
								 "3A-55",
								 "all_tape_level_1n2",
								 "all_tape_2A-52", 
								 "level_3"
							   };

char *options[NUM_PRODUCTS];
int n_inputs[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
char *working_dir;
extern char jobs_dbfile[];

char *button_stat[]  = {"NOT PUSHED", "PUSHED"};
char *computer_str   = NULL;
char *tape_drive_str = NULL;
char *tape_volid_str = NULL;
char *at_str         = NULL;
static int cmd_pid = -1;

extern FD_gvs *fd_gvs;
extern FD_cancel *fd_cancel;
extern FD_jobq *fd_jobq;
extern FD_options *fd_options;
extern FD_source *fd_source;
extern FD_times *fd_times;
extern FD_usage *fd_usage;

/* Commands that when executed w/ no command options, print their usage.
 * Feed the output to the browser for display.
 */
char *usage_text[] = {
  "level_1",
  "singlevos_2A-53-v4",
  "singlevos_2A-54",
  "singlevos_2A-55",
  "2A-52i",
  "3A-53",
  "3A-54",
  "3A-55",
  "all_tape_level_1n2",  /* First script. */
  "all_tape_2A-52",
  "level_3"              /* Last. */
};

/* This global variable is set by 'do_option_button' and is used
 * by 'do_option_ok_button', 'do_save_to_file_button', 
 * 'do_from_file_button', etc. (all buttons on the option form),
 * to identify which product is targeted for the options.
 *
 * It helps identify the default 'option file' in the file selection
 * when reading or saving options to a file.
 *
 * We know that this number is properly set because there are only
 * definite ways to get to the option form.
 */
static int product_ordinal;  /* 0 - 8 for 1c-51, 2a-52i, ..., 2a-55, and all_tape */
int kill_pid(long pid);
void end_log_cmd(long pid);
void do_option_ok_button(FL_OBJECT *ob, long n)
{
  /* Read all options and construct the command line options.
   * The global variable 'options' is for this.  Nearly all the
   * option screens are similiar.  There are only minor differences.
   * Where there are differences, 'if' statements check which 
   * form is being used.
   */

  char *s;

  /* We expect the order of each option in ~/.gvs to match the 
   * predefined ordinal ordering of the products.
   */
  s = (char *)fl_get_input(fd_options->option_input);
  options[product_ordinal] = strdup(s);  /* For saving into .gvs and for
										  * building the command line.
										  */
  fl_hide_form(ob->form);
}

/**********************************************************************/
/*                                                                    */
/*                       do_save_to_file_button                       */
/*                                                                    */
/**********************************************************************/
void do_save_to_file_button(FL_OBJECT *ob, long n)
{
  /* Pertains to 1C-51, for now. */
  char *file_str, *s;
  FILE *fp;
  char *errstr;
  char *options;

  /* Open a file browser and read the file and load the input field. */
  file_str = (char *)fl_show_fselector("Save to file name:", ".", "*.opt", "1C-51.opt");
  if (file_str == NULL) return;
  fp = fopen(file_str, "w");
  if (fp == NULL) {
	errstr =  strerror(errno);
	fl_show_alert(errstr, NULL, NULL, 0);
	return;
  }

  options = (char *) fl_get_input(fd_options->option_input);

  /* Write to the file and append a \n. */
  fwrite(options, sizeof(char), strlen(options), fp);
  fwrite("\n", 1, 1, fp);
  fclose(fp);

}

/**********************************************************************/
/*                                                                    */
/*                          do_usage_button                           */
/*                                                                    */
/**********************************************************************/
void do_usage_button(FL_OBJECT *ob, long n)
{
  char cmd[100];

  fl_set_object_label(fd_usage->label, product[product_ordinal]);
  fl_set_browser_fontsize(fd_usage->browser, 14);
  sprintf(cmd, "%s >& /tmp/usage.txt", usage_text[product_ordinal]);
  system(cmd);
  fl_load_browser(fd_usage->browser, "/tmp/usage.txt");
  unlink("/tmp/usage.txt");
  fl_show_form(fd_usage->usage, FL_PLACE_MOUSE, FL_OFF, NULL);
}

/**********************************************************************/
/*                                                                    */
/*                         do_from_file_button                        */
/*                                                                    */
/**********************************************************************/
void do_from_file_button(FL_OBJECT *ob, long n)
{
  /* Pertains to 1C-51, for now. */
  char *file_str, *s;
  FILE *fp;
  char *errstr;
  char options[1000]; /* Way big. */

  /* Open a file browser and read the file and load the input field. */
  file_str = (char *)fl_show_fselector("Enter file name:", ".", "*.opt", "1C-51.opt");
  if (file_str == NULL) return;
  fp = fopen(file_str, "r");
  if (fp == NULL) {
	errstr =  strerror(errno);
	fl_show_alert(errstr, NULL, NULL, 0);
	return;
  }

  memset(options, '\0', sizeof(options));
  /* Read the file and insert the contents into the input field. */
  fread(options, sizeof(char), sizeof(options), fp);
  fclose(fp);
  /* Replace the \n with \0. */
  s = strchr(options, '\n');
  if (s) *s = '\0';

  fl_set_input(fd_options->option_input, options);
}
  

/**********************************************************************/
/*                                                                    */
/*                        do_source_button                            */
/*                                                                    */
/**********************************************************************/
void do_source_button(FL_OBJECT *ob, long prod_num)
{
  fl_show_form(fd_source->source, FL_PLACE_MOUSE, FL_OFF, NULL);
}

/**********************************************************************/
/*                                                                    */
/*                         do_times_button                            */
/*                                                                    */
/**********************************************************************/
void do_times_button(FL_OBJECT *ob, long prod_num)
{
  fl_show_form(fd_times->times, FL_PLACE_MOUSE, FL_OFF, NULL);
}

char *get_raw_dir_opt_str(char *s)
{
  /* Return the parameter to '-raw_dir parameter'.  The word after. */
  char *s1, *s2;
  char *out;
  int len;
  char *s_copy;

  s_copy = strdup(s);
  s1 = strstr(s_copy, "-raw_dir");
  s2 = NULL;
  if (s1 != NULL) {
	/* Eliminate the original -raw_dir */
    /* Up to position s1 is to be saved. */
	s2 = s1+1;
	*s1 = '\0';
	s1 = s;
	/* Starting at s2, skip up and over -raw_dir ___ */
	while (*s2 != ' ' && *s2 != '\0') s2++;  /* Testing '\0' is paranoid. */

	while (*s2 == ' ' && *s2 != '\0') s2++;  /* Testing '\0' is paranoid. */

	s1 = s2; /* This is the start of the parameter. */
	/* Now find another ' ' or '\0', from there on out it's s2 */
	while (*s1 != ' ' && *s1 != '\0') s1++; /* Not so paranoid, here. */
	*s1 = '\0';
  }
  if (s2) out = strdup(s2);
  else out = NULL;
  return out;
}

char *remove_raw_dir_opt(char *s)
{
  char *s1, *s2;
  char *out;
  int len;
  char *s_copy;

  s_copy = strdup(s);
  s1 = strstr(s_copy, "-raw_dir");
  if (s1 == NULL) {
	s1 = s;
	s2 = NULL;
	return s;
  } else {
	/* Eliminate the original -raw_dir */
    /* Up to position s1 is to be saved. */
	s2 = s1+1;
	if (s1 == s_copy) {
	  *s1 = '\0';
	} else {
	  *s1 = '\0';
	  s1 = s_copy;
	}
	/* Starting at s2, skip up and over -raw_dir ___ */
	while (*s2 != ' ' && *s2 != '\0') s2++;  /* Testing '\0' is paranoid. */

	while (*s2 == ' ' && *s2 != '\0') s2++;  /* Testing '\0' is paranoid. */

	/* Now find another ' ' or '\0', from there on out it's s2 */
	while (*s2 != ' ' && *s2 != '\0') s2++; /* Not so paranoid, here. */
  }

  len = 0;
  if (s1) len = strlen(s1);
  if (s2) len += strlen(s2);
  out = (char *)calloc(len+1, sizeof(char));
  sprintf(out,"%s%s", s1, s2);
  return out;
}

int cd_is_mounted(void)
{
  FILE *fp;
  char s[100];
  fp = fopen("/etc/mtab", "r");
  if (fp == NULL) return 0;

  /* Count lines.  Should be only one line. */
  while (fgets(s, 100, fp))
	if (strstr(s, "/cdrom") != 0) {
	  fclose(fp);
	  return 1;
	}
  fclose(fp);
  return 0;
}

void set_raw_dir_opt(char **opt, char *str)
{
  /* Replace '-raw_dir <whatever>' with contents of 'str' */
  char *s;
  int len;
  char *s_opt;

  /* Remember space for 'opt' is allocated on the fly.  It may not
   * be big enough.
   */

  /* s1 is the first part of the result. */
  /* s2 is the second part of the result. */
  /* s is just a temp buffer. */

  s = strdup(*opt);
  
  s = remove_raw_dir_opt(s);
  len = 0;
  if (s) len += strlen(s);
  if (str) len += strlen(str);
  len += 11;  /* Adding -raw_dir and 2 spaces. */
  s_opt = (char *)calloc(len, sizeof(char));
  sprintf(s_opt, "%s -raw_dir %s", s, str);
  *opt = s_opt;
}

/**********************************************************************/
/*                                                                    */
/*                     do_cd_disk_dir_button                          */
/*                                                                    */
/**********************************************************************/
void do_cd_disk_dir_button(FL_OBJECT *ob, long n)
{
  /* Several possibilities:
   *   1. This matches the '-raw_dir <whatever>' option in General Options.
   *   2. Browse for this information.
   *   3. Ask to mount the /cdrom, if not mounted and only if source is CD.
   *   4. Default to current directory, except if CD.  Browse, if CD.
   */
  int ans;
  char cd_disk_dir[200];
  char *s_ans;
  char msg[MAX_STR_LEN];

  if (strstr(tape_drive_str, "8mm") != 0) {
	fl_show_alert("Setting -raw_dir does not make sense for", tape_drive_str, NULL, 0);
	return;
  }

  /* Check if the -raw_dir option already exists, for all_tape_level_1n2 only. */
  if (strstr(options[NUM_PRODUCTS-3], "-raw_dir")) {
	/* Warn. */
	memset(msg, '\0', MAX_STR_LEN);
	sprintf(msg, "General Options contains the -raw_dir parameter.\n\
Level 1n2 options are: %s.\nDo you want to change it?",
			options[NUM_PRODUCTS-3]);
	ans = fl_show_question(msg, 2);
	if (ans == 0) return;
  }

  /* fl_show_fselector will default to `pwd` */
  getcwd(cd_disk_dir, 200);
  /* Ok, if the source is a CD, ask if we should mount it. */
  if (strstr(tape_drive_str, "CD") != 0) { /* Match at begining. */
	/* Check if it is mounted. */
	if (cd_is_mounted() == 0) {
	  /* Ask to mount it. */
	  ans = fl_show_question("I detected that /dev/cdrom is not mounted at /cdrom.\nShould I try to mount it?\nI will try: mount /cdrom", 1);
	TRYMOUNT:
	  if (ans == 1) {
		/* Try to mount it. */
		system("mount /cdrom");
		if (cd_is_mounted() == 0) {
		  /* Ask to try again. */
		  ans = fl_show_question("I could not mount /dev/cdrom onto /cdrom.\nShould I try again?",1);
		  goto TRYMOUNT;
		}
	  }
	}
	/* It doesn't matter if the CD got mounted.  Just check if it is
	 * and set the default directory to /cdrom.
	 */
	
	if (cd_is_mounted()) strcpy(cd_disk_dir, "/cdrom");
  }

  s_ans = (char *)fl_show_fselector("Go to a directory where the input data resides.",
									cd_disk_dir,
									NULL,
									NULL);

  if (s_ans == NULL) /* User aborts this option. */
	return;

  strcpy(cd_disk_dir, (char *)fl_get_directory());
  fl_set_object_label(fd_gvs->cd_disk_dir_text, cd_disk_dir);
  /* And set the option[...] */
  set_raw_dir_opt(&options[NUM_PRODUCTS-3], cd_disk_dir);
}


/**********************************************************************/
/*                                                                    */
/*                       do_source_ok_button                          */
/*                                                                    */
/**********************************************************************/
void do_source_ok_button(FL_OBJECT *ob, long prod_num)
{
  /* There are 6 possible buttons.
   * 'Disk', '/dev/8mm[1-4]', '/dev/cdrom'.
   * The user could have entered one too.  But, that doesn't come
   * through this form.
   */
  /* Poll the buttons available, set 'tape_drive_str'. */
  int nbuttons = 6;
  int i;
  char *src_str[] = {"disk", "/dev/8mm1", "/dev/8mm2",
					 "/dev/8mm3", "/dev/8mm4", "CD"};
  /* These are radio buttons.  Only one is pressed. */
  for(i=0; i<nbuttons; i++) {
	if (fl_get_button(fd_source->button[i]) != 0) break;
	continue;
  }
  /* Button at 'i' */
  if (i == nbuttons) return;

  fl_set_input(fd_gvs->tape_drive_input, src_str[i]);
  tape_drive_str = strdup(src_str[i]);
  fl_hide_form(ob->form);

  if (strstr(src_str[i], "/dev/8mm") != 0)
	options[NUM_PRODUCTS-3] = remove_raw_dir_opt(options[NUM_PRODUCTS-3]);
  else
	do_cd_disk_dir_button(ob, prod_num);
}

/**********************************************************************/
/*                                                                    */
/*                        do_times_ok_button                          */
/*                                                                    */
/**********************************************************************/
void do_times_ok_button(FL_OBJECT *ob, long prod_num)
{
  /* Time can be anything that 'at' can handle.  
   * However, from this form, the time selection is restricted to:
   *    hh:mm am/pm
   *    now + 1 min
   *
   */
  char buf[100];
  char *s;
  /* ONE of the radio buttons should be pressed: am/pm/now */
  if (fl_get_button(fd_times->times_am_button) ||
	  fl_get_button(fd_times->times_pm_button)) {
	s = (char *)fl_get_input(fd_times->times_hh_input);
	if (strcmp(s,"") == 0) {
	  fl_show_message("Please select an hour.", NULL, NULL);
	  return;
	}
	sprintf(buf, "%s", s);
	s = (char *)fl_get_input(fd_times->times_mm_input);
	if (strcmp(s,"") != 0) sprintf(buf, "%s:%s", buf, s);
	if (fl_get_button(fd_times->times_am_button))
	  sprintf(buf, "%s am", buf);
	else
	  sprintf(buf, "%s pm", buf);
	at_str = strdup(buf);
  } else if (fl_get_button(fd_times->times_now_button))
	at_str = strdup("now + 1 minute");

  /* Otherwise, don't change the 'at_str' -- no buttons pressed. */
  fl_set_input(fd_gvs->at_input, at_str);
  fl_hide_form(ob->form);

}

/**********************************************************************/
/*                                                                    */
/*                     do_working_dir_button                          */
/*                                                                    */
/**********************************************************************/
void do_working_dir_button(FL_OBJECT *ob, long n)
{
  const char *s;
  s = fl_show_fselector("Do not enter anything here.  Change the Directory above.",
						NULL,
						NULL,
						NULL);
  if (s == NULL) return;
  working_dir = (char *)fl_get_directory();
  fl_set_object_label(fd_gvs->working_dir_text, working_dir);
  
}

/**********************************************************************/
/*                                                                    */
/*                       tokenize_option_string                       */
/*                                                                    */
/**********************************************************************/
void tokenize_option_string(char *opt_string, char *word[], int *nwords)
{
  char *s;
  int i = 0;

  s = strdup(opt_string); /* Safe copy. */
  s = strtok(s, " ");
  word[i++] = strdup(s);
  while((s = strtok(NULL, " "))) {
	word[i++] = strdup(s);
  }
  *nwords = i;
}

/**********************************************************************/
/*                                                                    */
/*                          load_options                              */
/*                                                                    */
/**********************************************************************/
void load_options(int form_ordinal)
{
  /* 'form_ordinal' is the ordinal number of the form.
   * The order is 1c51, 2a52i, 2a53w, 2a53d, 2a54, 2a55.
   */
  int i, j, n;
  FL_OBJECT *in;
  FL_OBJECT *verbose;
  FL_OBJECT *gif;

  char *word[100]; /* Max 100 options. */
  int nwords;

  gif == NULL;
  n = n_inputs[form_ordinal];

  /* Tokenize the option string, set buttons and input fields appropriately. */
  tokenize_option_string(options[form_ordinal], word, &nwords);
  /* The number of words should not exceed the number of input fields
   * we plan to set for this form.  It should be <= 'n+(# toggle_button)'. 
   */

  i = 0;
  /* The first option is always '-v' */
  if (strcmp(word[i++], "-v") == 0) {
	fl_set_button(verbose, 1);
  } else  fl_set_button(verbose, 0);
  /*
  if (fd_options->opt1c51 == form_option_list[form_ordinal]) 
	if (strcmp(word[i++], "-G") == 0) {
	  fl_set_button(gif, 1);
	} else
	  fl_set_button(gif, 0);
	  */
}

/*************************************************************/
/*                                                           */
/*                       do_option_button                    */
/*                                                           */
/*************************************************************/
void do_option_button(FL_OBJECT *ob, long prod_num)
{
  /*  load_options(prod_num); */
  product_ordinal = prod_num;
  fl_set_input(fd_options->option_input, options[product_ordinal]);
  fl_set_object_label(fd_options->option_label, usage_text[product_ordinal]);
  fl_show_form(fd_options->options, FL_PLACE_MOUSE, FL_OFF, NULL);
  free(options[product_ordinal]);
  options[product_ordinal] = strdup((char *)fl_get_input(fd_options->option_input));
}

/**********************************************************************/
/*                                                                    */
/*                     add_jobinfo_to_db                              */
/*                                                                    */
/**********************************************************************/
void add_jobinfo_to_db(char *msg, char *csh_name)
{
  /* On Linux msg = "Job 65 will be executed using /bin/sh N00777.csh"
   * On HP    msg = "job 829782000.a at Wed Apr 17 19:00:00 1996 N00777.csh"
   */

  /* Database will be: key = csh_name
                       con = msg, if queued.
					       = PID, if running.
   */
  
  char cmd[250];
  int n;

/* If '@' preceeds, then it is an 'at' job#.  Otherwise, it is 
 * a PID of a running process.
 */
  sprintf(cmd, "gdbm_store -f %s %s '@%s'", jobs_dbfile, csh_name, msg);
  system(cmd);

}

int fetch_pid(char *gdbmfile, char *key_str)
{
  FILE *fpipe;
  char cmd[100];
  char cmd_cpy[100];
  char *s;
  int rc;

  /* Return PID.  If job is queued, return NEGATIVE. */  
  sprintf(cmd, "gdbm_fetch -f %s %s", gdbmfile, key_str);
  if ((fpipe = popen(cmd, "r")) == NULL) {
	perror(cmd);
	return 0;
  }

  if (fgets(cmd, sizeof(cmd), fpipe) == NULL) rc = 0;
  else {
	memcpy(cmd_cpy, cmd, sizeof(cmd));
	s = strtok(cmd_cpy, " ");	
	s = strtok(NULL, " ");
	s = strtok(NULL, " ");
	if (cmd[0] == '@') { /* Job is queued. */
	  /* Third token is job id # */
	  sscanf(s, "%d", &rc);
	  rc = -rc;
	}
	else
	  /* PID is what we read. */
	  sscanf(cmd, "%d", &rc);
  }
  pclose(fpipe);
  return rc;
}

/*************************************************************/
/*                                                           */
/*                       do_ok_button                        */
/*                                                           */
/*************************************************************/
void do_ok_button(FL_OBJECT *ob, long data)
{
  FILE *fp, *fp2;
  int i, j, n;
  char msg[500], csh_name[20];
  char new_msg[500];
  int something_selected;
  char *s;
  char *gvs_data_env;
  struct stat stat_buf;
  char *word[500];
  int nwords;
  int q_level1n2_selected;
  int q_2a52i_selected;
  int q_level3_selected;

  /*
   * 1. Query which buttonsare pushed.
   * 2. Get computer, 8mm drive, volid, time to start.
   * 3. Construct all_tape_level_1n2 command line, using options.
   */

  something_selected = 0;

/* Query the buttons.  Which products are selected? */
  for (i=0; i<NUM_PRODUCTS-NUM_SCRIPTS; i++) {
	n = fl_get_button(fd_gvs->run_button[i]);
	if (n == 1)	something_selected = 1;
  }

  if (something_selected == 0) {
	fl_show_message("No products selected for processing.", NULL, NULL);
	return;
  }


/* Get 8mm drive, volid, time_to_start */
	
  if (tape_drive_str == NULL || strcmp(tape_drive_str, "") == 0) {
	fl_show_message("No 8mm drive selected.", NULL, NULL);
	return;
  }
	
  if (tape_volid_str == NULL || strcmp(tape_volid_str, "") == 0) {
	fl_show_message("No Tape Label specified.", NULL, NULL);
	return;
  }
	
  if (at_str == NULL || strcmp(at_str, "") == 0) {
	fl_show_message("No time selected.  Assumming NOW", NULL, NULL);
	return;
  }
  
  gvs_data_env = getenv("GVS_DATA_PATH");
  if (gvs_data_env == NULL) {
    gvs_data_env = (char *)strdup((char *)fl_show_input("$GVS_DATA_PATH is not set. Please set it here.", "/usr/local/trmm/data"));
  }

  stat(gvs_data_env, &stat_buf);
  if (S_ISDIR(stat_buf.st_mode) == 0) {
	fl_show_message("GVS_DATA_PATH", gvs_data_env, "is not a directory.");
	return;
  }

  sprintf(csh_name, "%s.csh", tape_volid_str);
  if (fetch_pid(jobs_dbfile, csh_name) != 0) {
	fl_show_message("Job already submitted.", csh_name, "Cancel first, if desired.");
	return;
  }

  fp = fopen(csh_name, "w");
  if (fp == NULL) {
	fl_show_message("Unable to open file", csh_name, NULL);
	return;
  }
  /* Construct the all_tape_level_1n2 command. */
  fprintf(fp, "#!/bin/csh -f\n\n");
  fprintf(fp, "setenv GVS_DATA_PATH %s\n", gvs_data_env);
  fprintf(fp, "# Save the PID for possible deletion by GVS\n");
  fprintf(fp, "gdbm_store -f %s %s $$\n", jobs_dbfile, csh_name);
  fprintf(fp, "chdir %s\n", working_dir);

/* Query the buttons.  Which products are selected? */
  q_level1n2_selected = 0;
  for (i=0; i<NUM_L1_PRODUCTS+ NUM_L2_PRODUCTS; i++) {
	n = fl_get_button(fd_gvs->run_button[i]);
	q_level1n2_selected = n == 1;
	if (q_level1n2_selected) break;
  }

  q_level3_selected = 0;
  q_2a52i_selected = 0; /* Deal w/ 2A-52i as an exception for L3 command. */
  for (i=0; i<NUM_L3_PRODUCTS; i++) {
	j = i+NUM_L1_PRODUCTS+NUM_L2_PRODUCTS;
	n = fl_get_button(fd_gvs->run_button[j]);
	q_level3_selected = n == 1 && i > 0;
	if (!q_2a52i_selected) q_2a52i_selected = n == 1 && i == 0;
	if (q_level3_selected) break;
  }


  for (i=0; i<NUM_PRODUCTS-NUM_SCRIPTS; i++) {
	n = fl_get_button(fd_gvs->run_button[i]);
	if (n == 1) {
	  fprintf(fp, "echo '");
	  tokenize_option_string(options[i], word, &nwords);
	  for (j=0; j<nwords; j++)
		fprintf(fp, " %s", word[j]);
	  fprintf(fp,"' > %s.opt$$\n", product[i]);
	}
  }

  for (i=NUM_PRODUCTS-NUM_SCRIPTS; i<NUM_PRODUCTS; i++) {
	fprintf(fp,"echo ' %s' > %s.opt$$\n", options[i],
			product[i]);
  }

  if (strcasecmp(tape_drive_str, "disk") == 0) {
	tape_drive_str = "disk";
  }
  if (q_level1n2_selected) {
	/* Select level 1 and level 2 products for all_tape_level_1n2. */
	fprintf(fp, "%s %s %s `cat %s.opt$$` ",
			product[NUM_PRODUCTS-3],
			tape_drive_str, tape_volid_str, product[NUM_PRODUCTS-3]);
	for (i=0; i<NUM_L1_PRODUCTS + NUM_L2_PRODUCTS; i++) {
	  n = fl_get_button(fd_gvs->run_button[i]);
	  if (n == 1)	fprintf(fp, "%s %s.opt$$ ", product[i], product[i]);
	}
	fprintf(fp, ">& %s.log\n\n", tape_volid_str);
  }

  if (q_2a52i_selected) {
	/* Select 2A-52i product. */
	j = NUM_L1_PRODUCTS+NUM_L2_PRODUCTS;
	n = fl_get_button(fd_gvs->run_button[j]);
	if (n == 1)	{
	  fprintf(fp, "%s %s `cat %s.opt$$` %s %s.opt$$",
			  product[NUM_PRODUCTS-2], tape_volid_str, product[NUM_PRODUCTS-2],
			  product[j], product[j]);
	  fprintf(fp, ">& all_tape_2A-52_%s.log\n\n", tape_volid_str);

	}
  }

  if (q_level3_selected) {
	/* Select level 3 products. */
	fprintf(fp, "%s %s `cat %s.opt$$` ",
			product[NUM_PRODUCTS-1],
		    tape_volid_str, product[NUM_PRODUCTS-1]);
	for (i=1; i<NUM_L3_PRODUCTS; i++) { /* Start at 1 because 2A-52i is 0. */
	  j = i+NUM_L1_PRODUCTS+NUM_L2_PRODUCTS;
	  n = fl_get_button(fd_gvs->run_button[j]);
	  if (n == 1)	{
		fprintf(fp, "%s %s.opt$$ ", product[j], product[j]);
	  }
	}
	fprintf(fp, ">& %s_%s.log\n\n", product[NUM_PRODUCTS-1],tape_volid_str);
  }

  for (i=0; i<NUM_PRODUCTS-NUM_SCRIPTS; i++) {
	n = fl_get_button(fd_gvs->run_button[i]);
	if (n == 1)	fprintf(fp, "rm %s.opt$$\n", product[i]);
  }
  for (i=NUM_PRODUCTS-NUM_SCRIPTS; i<NUM_PRODUCTS; i++) {
	fprintf(fp, "rm %s.opt$$\n", product[i]);
  }

  fprintf(fp, "# Remove this PID entry from the database.\n");
  fprintf(fp, "# This is not absolutely necessary, but, a nice thing.\n");
  fprintf(fp, "gdbm_delete -f %s %s\n", jobs_dbfile, csh_name);

  fclose(fp);
  chmod(csh_name, 0755);

  if (data == 1) { /* Really submit the job to 'at'. */

	/* Now create the 'at' job. */
	/* Catch stderr for 'cancel' capability. */
	sprintf(msg, "at %s > at.msg 2>&1", at_str);
	fp = popen(msg, "w");
	fprintf(fp, "%s.csh\n", tape_volid_str);
	pclose(fp);
	
	fp = fopen("at.msg", "r");
	fgets(msg, sizeof(msg), fp);
#if defined(__linux)
#else
	fgets(msg, sizeof(msg), fp); /* Data is on the second line. */
#endif
	fclose(fp);
	unlink("at.msg");
	/* Null at \n */
	s = strchr(msg, '\n');
	if (s != NULL) *s = '\0';
	/* Prepend the csh filename. */
	/*
	  fprintf(stderr,"MSG <%s>\n", msg);
	  fprintf(stderr,"CSH <%s>\n", csh_name);
	  */
	sprintf(new_msg, "%s %s", csh_name, msg);
	
	fl_show_message("Job submission message", new_msg, NULL);

	fl_add_browser_line(fd_cancel->cancel_browser, new_msg);
	add_jobinfo_to_db(new_msg, csh_name); /* Relate at jobid's to the csh. */
  }
}


/*************************************************************/
/*                                                           */
/*                       set_run_cb                          */
/*                                                           */
/*************************************************************/
void set_run_cb(FL_OBJECT *ob, long data)
{
  char msg[100];
  int i;
/* Do nothing.  When the job commands (volid.csh) are generated,
 * then I will poll the status of the buttons or products selected.
 */
}

/*************************************************************/
/*                                                           */
/*  Misc buttons: cancel, jobs, products, help, close.       */
/*                                                           */
/*************************************************************/
void do_cancel_button(FL_OBJECT *ob, long data)
{
  /* Re-read saved configuration and set the display. */
  fl_hide_form(ob->form);
}

void do_cancel_job_button(FL_OBJECT *ob, long data)
{
  /* Re-read saved configuration and set the display. */
  fl_show_form(fd_cancel->cancel, FL_PLACE_MOUSE, FL_OFF, NULL);
}

void do_jobs_button(FL_OBJECT *ob, long data)
{
  system("gms &");
}

void do_nothing(FL_OBJECT *ob, long data)
{
  /* Dummy call back; catch all. */
}

void do_products_button(FL_OBJECT *ob, long data)
{
  fl_show_message("There are no products at this time.", NULL, NULL);
}

void do_help_button(FL_OBJECT *ob, long data)
{
  fl_show_message(GVS_VERSION_STR, "There is no help at this time.", NULL);
}

void do_close_button(FL_OBJECT *ob, long data)
{
  fl_hide_form(ob->form);
}


void murder(int pid)
{
  /* Kill at and all it's children. */
}

void do_cancel_ok_button(FL_OBJECT *ob, long data)
{
  /* 1. Find what was selected from the cancel_browser.
   * 2. Parse the 'at' job number.
   * 3. Send the 'atrm' command.
   */
  int i;
  char *line, *s, *line_cpy;
  char cmd[100];
  char *csh_name;
  int pid;
  int qselect;


  qselect = 0;
  for (i=1; i<= fl_get_browser_maxline(fd_cancel->cancel_browser); i++)
	if (fl_isselected_browser_line(fd_cancel->cancel_browser, i)) {
	  /* Parse the line. */
	  qselect = 1;
      line = (char *)fl_get_browser_line(fd_cancel->cancel_browser, i);
	  csh_name = strdup(line);
	  s = strchr(csh_name, ' ');
	  *s = '\0';

	  /* Lines are shifted, see i-- */
	  fl_delete_browser_line(fd_cancel->cancel_browser, i--);

	  /* Now, if we find the job in the database, we must kill the PID and
	   * all of its children.
	   */
	  pid = fetch_pid(jobs_dbfile, csh_name);
	  if (pid > 0) {
		if (fl_show_choice("Job is running ", csh_name, "Kill anyway?",
						   2, "Yes", "No", NULL, 0) == 1) {
		  sprintf(cmd, "murder.pl %d", pid);
		  system(cmd);
		}
	  } else { /* There is a possible race condition here.
				* The job could start before we have a chance to remove it.
				*/
		line_cpy = strdup(line);
		s = strtok(line_cpy, " ");	
		s = strtok(NULL, " ");
		s = strtok(NULL, " ");
#if defined(__linux)
		sprintf(cmd, "atrm %s", s);
#else
		sprintf(cmd, "at -r %s", s);
#endif
		system(cmd);
	  }

	  sprintf(cmd, "gdbm_delete -f %s %s", jobs_dbfile, csh_name);
	  system(cmd);
	}
  
  fl_hide_form(ob->form);
}


void load_the_cancel_browser(FL_OBJECT *browser)
{
  FILE *fpipe;
  GDBM_FILE gf;
  datum key, content;
  int qqueue = 0;
  int qjob   = 0;
  char msg[100];

  gf = gdbm_open(jobs_dbfile, 512, GDBM_READER, 0, 0);
  if (gf != NULL) {

	for (key=gdbm_firstkey(gf); key.dptr; key=gdbm_nextkey(gf, key)) {
	  content = gdbm_fetch(gf, key);
	  if (content.dptr) {
		if (content.dptr[0] == '@')
		  sprintf(msg,"%s", &content.dptr[1]);
		else
		  sprintf(msg,"%s RUNNING.", key.dptr);
		fl_add_browser_line(browser, msg);
	  }
	}
	gdbm_close(gf);
  }
  return;
}

static char *cd_disk_dir; /* Used in get_gvs_conf... and initialize_gvs */

void get_gvs_configuration(void)
{
  /* The inverse of 'save_gvs_configuration. */
  /* 1. Save status of buttons for selected products.
   * 2. Save options.
   * 3. Save 8mm, volid, at-time
   * 4. Save cancel_browser information.
   */
  char config_file[100];
  char *home_dir;
  char msg[100];
  FILE *fp;
  int i, n;
  char *line = (char *)calloc(100, sizeof(char));
  char *parm_str;

  if ((home_dir = getenv("HOME")) == NULL) {
	fl_show_message("Environment variable HOME doesn't exist.", NULL, NULL);
	return;
  }
  sprintf(config_file, "%s/.gvs", home_dir);

  if ((fp = fopen(config_file, "r")) == NULL) {
	sprintf(msg,"%s: %s", config_file, strerror(errno));
	fl_show_message(msg, "Creating ~/.gvs automatically.", NULL);
	return;
  }

  /* Determine if .gvs is compatable. */
  fgets(line, 100, fp);
  if (strcmp("GVS config file version 1.5 or greater.\n", line) == 0) {
	/* Then ok. */
  } else {
	fl_show_message(msg, "Previous ~/.gvs is not compatable.", "I will rename it to ~/.gvs.bak and create a new one.");
	system("mv ~/.gvs ~/.gvs.bak");
	return;
  }

/* Button states. */
  for (i=0; i<NUM_PRODUCTS-NUM_SCRIPTS; i++) {
	fgets(line, 100, fp);
	sscanf(line, "%d\n", &n);
	fl_set_button(fd_gvs->run_button[i], n);
  }

/* Options, including all_tape options. */
  for (i=0; i<NUM_PRODUCTS; i++) {
	fgets(line, 100, fp);
	line[strlen(line)-1] = '\0';
	options[i] = (char *)strdup(line);
  }

  /* Option NUM_PRODUCTS-NUM_SCRIPTS is reserved for the 'all_tape...' options.
   * But, we need to parse the -raw_dir option, which has one argument.
   * That one argument is placed on the main window.
   */
  parm_str = get_raw_dir_opt_str(options[NUM_PRODUCTS-3]);
  if (parm_str) {
	cd_disk_dir = parm_str;
  }

/* 8mm, volid, at-time */
  fgets(line, 100, fp);
  line[strlen(line)-1] = '\0';
  tape_drive_str = (char *)strdup(line);
  fgets(line, 100, fp);
  line[strlen(line)-1] = '\0';
  tape_volid_str = (char *)strdup(line);
  fgets(line, 100, fp);
  line[strlen(line)-1] = '\0';
  at_str = (char *)strdup(line);

  load_the_cancel_browser(fd_cancel->cancel_browser);

  fclose(fp);

}

char *str_first_non_white(char *s)
{
  /* Return pointer to first non-white character, Null otherwise. */
  while(*s) {
	if (*s == ' ' || *s == '\t') s++;
	else break;
  }
  return s;
}

void save_gvs_configuration(void)
{
  /* 1. Save status of buttons for selected products.
   * 2. Save options.
   * 3. Save 8mm, volid, at-time
   * 4. Save cancel_browser information.
   */
  char config_file[100];
  char *home_dir;
  char msg[100];
  FILE *fp;
  int i, n;
  char *line;
  char *non_blank;

  if ((home_dir = getenv("HOME")) == NULL) {
	fl_show_message("Environment variable HOME doesn't exist.", NULL, NULL);
	return;
  }
  sprintf(config_file, "%s/.gvs", home_dir);

  if ((fp = fopen(config_file, "w")) == NULL) {
	sprintf(msg,"%s: %s", config_file, strerror(errno));
	fl_show_message(msg, NULL, NULL);
	return;
  }

  /* Output version string. */
  fprintf(fp, "GVS config file version 1.5 or greater.\n");

/* Button states. */
  for (i=0; i<NUM_PRODUCTS-NUM_SCRIPTS; i++) {
	n = fl_get_button(fd_gvs->run_button[i]);
	fprintf(fp, "%d\n", n);
  }

/* Options, including all_tape, level_3 options */
  for (i=0; i<NUM_PRODUCTS; i++) {
	non_blank = str_first_non_white(options[i]);
	if (non_blank) fprintf(fp, "%s\n", non_blank);
	else fprintf(fp,"\n");
  }

/* 8mm, volid, at-time */
  fprintf(fp, "%s\n%s\n%s\n", tape_drive_str, tape_volid_str, at_str);

  fclose(fp);
}

void do_exit_button(FL_OBJECT *ob, long data)
{
  /* Save configuration to $HOME/.gvs */
  save_gvs_configuration();
  
  /* Kill process created by fl_exe_command() */
  end_log_cmd(cmd_pid);
  exit(0);
}

void do_tape_drive_input(FL_OBJECT *ob, long data)
{
  tape_drive_str = (char *)fl_get_input(ob);
}

void do_tape_volid_input(FL_OBJECT *ob, long data)
{
  tape_volid_str = (char *)fl_get_input(ob);
}

void do_at_input(FL_OBJECT *ob, long data)
{
  at_str = (char *)fl_get_input(ob);

}

void set_queued_running_status(char *jobs_dbfile)
{
  /* Status messages are:
   *    Some jobs are queued.
   *    No jobs are queued.
   *    Some jobs are running.
   *    No jobs are running.
   *
   * Message placed into widget has the queued,running format.
   * Ex.
   *    No jobs are queued. Some jobs are running.
   */
  FILE *fpipe;
  GDBM_FILE gf;
  datum key, content;
  int qqueue = 0;
  int qjob   = 0;
  char *qmsg[] = {"No jobs are queued.", "Some Jobs are queued."};
  char *jmsg[] = {"No jobs are running.", "Some Jobs are running."};
  char msg[100];

  gf = gdbm_open(jobs_dbfile, 512, GDBM_READER, 0, 0);
  if (gf != NULL) {

	for (key=gdbm_firstkey(gf); key.dptr; key=gdbm_nextkey(gf, key)) {
	  content = gdbm_fetch(gf, key);
	  if (content.dptr) {
		if (content.dptr[0] == '@') qqueue = 1;
		else qjob = 1;
	  }
	}
	gdbm_close(gf);
  }
  sprintf(msg, "%s %s", qmsg[qqueue], jmsg[qjob]);
  fl_set_object_label(fd_gvs->job_status_text, msg);
  return;
}

void initialize_gvs(FD_gvs *fd_gvs)
{
/*
 * Read previously save config information.
 */
  char *hostname = (char *)calloc(100, sizeof(char));
  int i;
  char cwd[100];

  for (i=0; i<NUM_PRODUCTS-NUM_SCRIPTS; i++) /* Do not include 'all_tape...' */
	options[i] = (char *)strdup("-v");

  for (i=NUM_PRODUCTS-NUM_SCRIPTS; i<NUM_PRODUCTS; i++)
	options[i] = (char *)strdup(" ");

  gethostname(hostname, 100);
  fl_set_object_label(fd_gvs->computer_str_text, hostname);
  computer_str = hostname;

  getcwd(cwd, sizeof(cwd));
  fl_set_object_label(fd_gvs->working_dir_text, cwd);
  working_dir = strdup(cwd);

  cd_disk_dir = strdup(cwd);

  tape_drive_str = (char *)strdup("/dev/8mm1");
  tape_volid_str = (char *)strdup("N00000");
  at_str         = (char *)strdup("now + 1 minute");

  get_gvs_configuration();  /* This could change the hard coded defaults. */
  unlink(at_str);  /* This is the collection of *.csh files to
					* execute (yes, spaces in the filename).
					* It is submitted to 'at'.
					*/

  fl_set_object_label(fd_gvs->cd_disk_dir_text, cd_disk_dir);

  fl_set_input(fd_gvs->tape_drive_input, tape_drive_str);
  fl_set_input_return(fd_gvs->tape_drive_input, 1);

  fl_set_input(fd_gvs->tape_volid_input, tape_volid_str);
  fl_set_input_return(fd_gvs->tape_volid_input, 1);

  fl_set_input(fd_gvs->at_input, at_str);
  fl_set_input_return(fd_gvs->at_input, 1);

  /* The pulldown menus. */
  fl_set_menu(fd_gvs->action_menu,        "Save to File|Submit|Cancel|JobQ|Exit");
  fl_set_menu(fd_gvs->configuration_menu, "Source drive|'at' Time|Working Dir|CD/disk Dir");
  fl_set_menu(fd_gvs->monitor_menu,       "Jobs|Products|Log File");
  fl_set_menu(fd_gvs->help_menu,       "About GVS");

  /* Determine if there are Jobs queued and/or running. */
  set_queued_running_status(jobs_dbfile);


}

void do_jobq_button(FL_OBJECT *ob, long data)
{
  FILE *fpipe;
  char cmd[100];
#ifdef __linux
  sprintf(cmd, "atq");
#endif
#ifdef __hpux
  sprintf(cmd, "at -l");
#endif

  fpipe = popen(cmd, "r");
  if (fpipe == NULL) {
	fl_show_message("Unable to get job queue.", NULL, NULL);
	return;
  }
  fl_clear_browser(fd_jobq->jobq_browser);
  while(fgets(cmd, sizeof(cmd), fpipe) != NULL) {
	fl_add_browser_line(fd_jobq->jobq_browser, cmd);
  }
  pclose(fpipe);

  fl_show_form(fd_jobq->jobq, FL_PLACE_MOUSE, FL_OFF, NULL);
}

void close_log_form(FL_OBJECT *ob, long data)
{
  end_log_cmd(cmd_pid);
}

void do_logfile_button(FL_OBJECT *ob, long data)
{
  /* Show logfile <working_dir/tapeID.log> in a window. */
  char cmd[MAX_STR_LEN];

  FD_CMDLOG * cmd_log_data = NULL;

  memset(cmd, '\0', 256);
  if (working_dir && tape_volid_str) {
	sprintf(cmd, "tail +1f %s/%s.log", working_dir, tape_volid_str);
    /*  Kill the process invoked last time.*/
	end_log_cmd(cmd_pid);
	cmd_pid = fl_exe_command(cmd, 0);
	/* Set log window attributes. */
	cmd_log_data = fl_get_command_log_fdstruct();
	if (cmd_log_data) {
	  if (cmd_log_data->close_browser) {
		fl_set_object_callback(cmd_log_data->close_browser, close_log_form, 0);
	  }
	  if (cmd_log_data->form) {
		fl_set_form_title(cmd_log_data->form, "Log");
	  }
	}
	
	fl_set_command_log_position(200,200);
	fl_show_command_log(FL_FULLBORDER);
  }
  else
	fl_show_messages("Unable to show logfile. Working dir or 'tape label' is not set.");
}

void do_jobq_close_button(FL_OBJECT *ob, long data)
{
  fl_hide_form(ob->form);
}

/**************************************************************************/
/*                                                                        */
/*                           M E N U S                                    */
/*                                                                        */
/**************************************************************************/

void do_action_menu(FL_OBJECT *ob, long n)
{
  int i;
  /* Submit
   * Cancel
   * JobQ
   * Exit
   */
  i = fl_get_menu(ob);
  switch (i) {
  case 1: do_ok_button(ob, 0);         break;
  case 2: do_ok_button(ob, 1);         break;
  case 3: do_cancel_job_button(ob, 0); break;
  case 4: do_jobq_button(ob, 0);       break;
  case 5: do_exit_button(ob, 0);       break;
  }
}

void do_configuration_menu(FL_OBJECT *ob, long n)
{
  int i;
  /* Working Dir
   * Source Drive
   * 'at' times
   */

  i = fl_get_menu(ob);
  switch (i) {
  case 1: do_source_button(ob, 0);     break;
  case 2: do_times_button(ob, 0);      break;
  case 3: do_working_dir_button(ob, 0);break;
  case 4: do_cd_disk_dir_button(ob, 0);break;
  }
}

void do_monitor_menu(FL_OBJECT *ob, long n)
{
  int i;
  /* Jobs
   * Products
   */
  i = fl_get_menu(ob);
  switch (i) {
  case 1: do_jobs_button(ob, 0);     break;
  case 2: do_products_button(ob, 0); break;
  case 3: do_logfile_button(ob, 0); break;
  }
}


void do_help_menu(FL_OBJECT *ob, long n)
{
  int i;
  /* About GVS
   */
  i = fl_get_menu(ob);
  switch (i) {
  case 1: do_help_button(ob, 0);     break;
  }
}


int kill_pid(long pid)
{
  /* Kill the process ID if it's still running and return 1; otherwise,
   * return 0.
   */
  char cmd[MAX_STR_LEN], line[MAX_STR_LEN];
  FILE *fp;

  if (pid <= 0) return 0;
  memset(cmd, '\0', MAX_STR_LEN);
  sprintf(cmd, "ps -h%d\n", cmd_pid);
  if ((fp = popen(cmd, "r")) == NULL) {
	return 0;
  }
  if (fgets(line, MAX_STR_LEN, fp) == NULL) {
	pclose(fp);
	return 0;
  }
  pclose(fp);
  if (strstr(line, "No processes available.") != NULL) {
	return 0;
  }
  kill(pid, SIGTERM);
  return 1;
}

void end_log_cmd(long pid)
{
  if (kill_pid(pid)) {
	fl_end_command(pid);
	fl_clear_command_log();
	fl_hide_command_log();
  }

}
