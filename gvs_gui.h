/** Header file generated with fdesign on Sun Nov  7 14:36:53 1999.**/

#ifndef FD_gvs_h_
#define FD_gvs_h_

/** Callbacks, globals and object handlers **/
extern void do_option_button(FL_OBJECT *, long);
extern void do_tape_drive_input(FL_OBJECT *, long);
extern void do_tape_volid_input(FL_OBJECT *, long);
extern void do_at_input(FL_OBJECT *, long);
extern void set_run_cb(FL_OBJECT *, long);
extern void do_action_menu(FL_OBJECT *, long);
extern void do_configuration_menu(FL_OBJECT *, long);
extern void do_monitor_menu(FL_OBJECT *, long);
extern void do_help_menu(FL_OBJECT *, long);
extern void do_source_button(FL_OBJECT *, long);
extern void do_times_button(FL_OBJECT *, long);

extern void do_nothing(FL_OBJECT *, long);
extern void do_cancel_ok_button(FL_OBJECT *, long);
extern void do_close_button(FL_OBJECT *, long);

extern void do_jobq_close_button(FL_OBJECT *, long);

extern void do_option_ok_button(FL_OBJECT *, long);
extern void do_from_file_button(FL_OBJECT *, long);
extern void do_save_to_file_button(FL_OBJECT *, long);
extern void do_usage_button(FL_OBJECT *, long);
extern void do_cancel_button(FL_OBJECT *, long);

extern void do_source_ok_button(FL_OBJECT *, long);
extern void do_cancel_button(FL_OBJECT *, long);

extern void do_times_ok_button(FL_OBJECT *, long);
extern void do_cancel_button(FL_OBJECT *, long);

extern void do_close_button(FL_OBJECT *, long);


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *gvs;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *tape_drive_input;
	FL_OBJECT *tape_volid_input;
	FL_OBJECT *at_input;
	FL_OBJECT *working_dir_text;
	FL_OBJECT *computer_str_text;
	FL_OBJECT *action_menu;
	FL_OBJECT *configuration_menu;
	FL_OBJECT *monitor_menu;
	FL_OBJECT *help_menu;
	FL_OBJECT *job_status_text;
	FL_OBJECT *cd_disk_dir_text;
	FL_OBJECT *option_button[8];
	FL_OBJECT *run_button[8];
} FD_gvs;

extern FD_gvs * create_form_gvs(void);
typedef struct {
	FL_FORM *cancel;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *cancel_browser;
} FD_cancel;

extern FD_cancel * create_form_cancel(void);
typedef struct {
	FL_FORM *jobq;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *jobq_browser;
	FL_OBJECT *jobq_close_button;
} FD_jobq;

extern FD_jobq * create_form_jobq(void);
typedef struct {
	FL_FORM *options;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *option_label;
	FL_OBJECT *option_input;
} FD_options;

extern FD_options * create_form_options(void);
typedef struct {
	FL_FORM *source;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *button[6];
} FD_source;

extern FD_source * create_form_source(void);
typedef struct {
	FL_FORM *times;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *times_now_button;
	FL_OBJECT *times_am_button;
	FL_OBJECT *times_pm_button;
	FL_OBJECT *times_hh_input;
	FL_OBJECT *times_mm_input;
} FD_times;

extern FD_times * create_form_times(void);
typedef struct {
	FL_FORM *usage;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *browser;
	FL_OBJECT *label;
} FD_usage;

extern FD_usage * create_form_usage(void);

#endif /* FD_gvs_h_ */
