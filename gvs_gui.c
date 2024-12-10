/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "gvs_gui.h"

FD_gvs *create_form_gvs(void)
{
  FL_OBJECT *obj;
  FD_gvs *fdui = (FD_gvs *) fl_calloc(1, sizeof(*fdui));

  fdui->gvs = fl_bgn_form(FL_NO_BOX, 490, 530);
  obj = fl_add_box(FL_UP_BOX,0,0,490,530,"");
    fl_set_object_color(obj,FL_DARKCYAN,FL_COL1);
  obj = fl_add_box(FL_FRAME_BOX,10,10,470,41,"");
    fl_set_object_color(obj,FL_DARKCYAN,FL_COL1);
  obj = fl_add_frame(FL_ENGRAVED_FRAME,30,90,170,290,"");
    fl_set_object_color(obj,FL_COL1,FL_COL1);
  obj = fl_add_text(FL_NORMAL_TEXT,50,50,377,50,"Ground Validation System");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_HUGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->option_button[0] = obj = fl_add_button(FL_NORMAL_BUTTON,140,140,20,20,"");
    fl_set_object_callback(obj,do_option_button,0);
  fdui->tape_drive_input = obj = fl_add_input(FL_NORMAL_INPUT,250,120,150,30,"Source drive");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,do_tape_drive_input,0);
  fdui->tape_volid_input = obj = fl_add_input(FL_NORMAL_INPUT,250,180,150,30,"Tape label string");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,do_tape_volid_input,0);
  fdui->at_input = obj = fl_add_input(FL_NORMAL_INPUT,250,240,150,30,"Run at time (Unix 'at')");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,do_at_input,0);
  obj = fl_add_text(FL_NORMAL_TEXT,93,120,42,20,"Run?");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,135,120,41,20,"Options");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
  fdui->run_button[0] = obj = fl_add_button(FL_PUSH_BUTTON,100,140,20,20,"1C-51");
    fl_set_object_color(obj,FL_RED,FL_GREEN);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_callback(obj,set_run_cb,0);
  fdui->run_button[1] = obj = fl_add_button(FL_PUSH_BUTTON,100,160,20,20,"2A-53-v4");
    fl_set_object_color(obj,FL_RED,FL_GREEN);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_callback(obj,set_run_cb,1);
  fdui->run_button[2] = obj = fl_add_button(FL_PUSH_BUTTON,100,180,20,20,"2A-54");
    fl_set_object_color(obj,FL_RED,FL_GREEN);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_callback(obj,set_run_cb,2);
  obj = fl_add_text(FL_NORMAL_TEXT,51,100,136,20,"Select Products");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->option_button[1] = obj = fl_add_button(FL_NORMAL_BUTTON,140,160,20,20,"");
    fl_set_object_callback(obj,do_option_button,1);
  fdui->option_button[2] = obj = fl_add_button(FL_NORMAL_BUTTON,140,180,20,20,"");
    fl_set_object_callback(obj,do_option_button,2);
  obj = fl_add_text(FL_NORMAL_TEXT,20,390,110,30,"Computer:");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
  fdui->working_dir_text = obj = fl_add_text(FL_NORMAL_TEXT,130,420,340,30,"");
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,20,420,110,30,"Working Directory:");
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
  fdui->computer_str_text = obj = fl_add_text(FL_NORMAL_TEXT,130,390,260,30,"");
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->action_menu = obj = fl_add_menu(FL_PULLDOWN_MENU,20,20,70,20,"Action");
    fl_set_object_color(obj,FL_DARKCYAN,FL_DARKCYAN);
    fl_set_object_lcolor(obj,FL_YELLOW);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,do_action_menu,0);
  fdui->configuration_menu = obj = fl_add_menu(FL_PULLDOWN_MENU,100,20,110,20,"Configuration");
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lcolor(obj,FL_YELLOW);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,do_configuration_menu,0);
  fdui->monitor_menu = obj = fl_add_menu(FL_PULLDOWN_MENU,220,20,70,20,"Monitor");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lcolor(obj,FL_YELLOW);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,do_monitor_menu,0);
  fdui->help_menu = obj = fl_add_menu(FL_PULLDOWN_MENU,400,21,70,20,"Help");
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lcolor(obj,FL_YELLOW);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,do_help_menu,0);
  obj = fl_add_box(FL_FLAT_BOX,80,360,10,10,"Run");
    fl_set_object_color(obj,FL_GREEN,FL_COL1);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
  obj = fl_add_box(FL_FLAT_BOX,170,360,10,10,"Don't Run");
    fl_set_object_color(obj,FL_RED,FL_COL1);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
  obj = fl_add_text(FL_NORMAL_TEXT,20,480,110,30,"Job status:");
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
  fdui->job_status_text = obj = fl_add_text(FL_NORMAL_TEXT,130,480,340,30,"");
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,50,340,60,20,"Legend:");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_button(FL_NORMAL_BUTTON,220,290,220,30,"Options for all_tape_level_1n2");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,do_option_button,8);
  fdui->run_button[3] = obj = fl_add_button(FL_PUSH_BUTTON,100,200,20,20,"2A-55");
    fl_set_object_color(obj,FL_RED,FL_GREEN);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_callback(obj,set_run_cb,3);
  fdui->option_button[3] = obj = fl_add_button(FL_NORMAL_BUTTON,140,200,20,20,"");
    fl_set_object_callback(obj,do_option_button,3);
  fdui->cd_disk_dir_text = obj = fl_add_text(FL_NORMAL_TEXT,130,450,340,30,"");
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,20,450,110,30,"CD/disk directory:");
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,410,110,48,15,"Change");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_button(FL_NORMAL_BUTTON,415,125,22,23,"");
    fl_set_object_callback(obj,do_source_button,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,415,245,22,23,"");
    fl_set_object_callback(obj,do_times_button,0);
  fdui->run_button[5] = obj = fl_add_button(FL_PUSH_BUTTON,100,270,20,20,"3A-53");
    fl_set_object_color(obj,FL_RED,FL_GREEN);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_callback(obj,set_run_cb,5);
  fdui->run_button[6] = obj = fl_add_button(FL_PUSH_BUTTON,100,290,20,20,"3A-54");
    fl_set_object_color(obj,FL_RED,FL_GREEN);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_callback(obj,set_run_cb,6);
  fdui->run_button[7] = obj = fl_add_button(FL_PUSH_BUTTON,100,310,20,20,"3A-55");
    fl_set_object_color(obj,FL_RED,FL_GREEN);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_callback(obj,set_run_cb,7);
  fdui->run_button[4] = obj = fl_add_button(FL_PUSH_BUTTON,100,250,20,20,"2A-52i");
    fl_set_object_color(obj,FL_RED,FL_GREEN);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_callback(obj,set_run_cb,4);
  fdui->option_button[4] = obj = fl_add_button(FL_NORMAL_BUTTON,140,250,20,20,"");
    fl_set_object_callback(obj,do_option_button,4);
  fdui->option_button[5] = obj = fl_add_button(FL_NORMAL_BUTTON,140,270,20,20,"");
    fl_set_object_callback(obj,do_option_button,5);
  fdui->option_button[6] = obj = fl_add_button(FL_NORMAL_BUTTON,140,290,20,20,"");
    fl_set_object_callback(obj,do_option_button,6);
  fdui->option_button[7] = obj = fl_add_button(FL_NORMAL_BUTTON,140,310,20,20,"");
    fl_set_object_callback(obj,do_option_button,7);
  obj = fl_add_text(FL_NORMAL_TEXT,30,230,90,20,"Monthly products");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_TINY_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
  obj = fl_add_button(FL_NORMAL_BUTTON,220,320,220,30,"Options for all_tape_2A-52");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,do_option_button,9);
  obj = fl_add_text(FL_NORMAL_TEXT,410,230,48,15,"Change");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_button(FL_NORMAL_BUTTON,220,350,220,30,"Options for level_3");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,do_option_button,10);
  fl_end_form();

  fdui->gvs->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_cancel *create_form_cancel(void)
{
  FL_OBJECT *obj;
  FD_cancel *fdui = (FD_cancel *) fl_calloc(1, sizeof(*fdui));

  fdui->cancel = fl_bgn_form(FL_NO_BOX, 380, 330);
  obj = fl_add_box(FL_UP_BOX,0,0,380,330,"");
    fl_set_object_color(obj,FL_DARKCYAN,FL_COL1);
  fdui->cancel_browser = obj = fl_add_browser(FL_MULTI_BROWSER,40,60,300,190,"");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,do_nothing,0);
  obj = fl_add_text(FL_NORMAL_TEXT,70,10,250,50,"Select Jobs to Cancel");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_HUGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_button(FL_NORMAL_BUTTON,40,260,100,50,"OK");
    fl_set_object_color(obj,FL_GREEN,FL_COL1);
    fl_set_object_lsize(obj,FL_HUGE_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,do_cancel_ok_button,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,240,260,100,50,"Close");
    fl_set_object_color(obj,FL_RED,FL_COL1);
    fl_set_object_lsize(obj,FL_HUGE_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,do_close_button,0);
  fl_end_form();

  fdui->cancel->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_jobq *create_form_jobq(void)
{
  FL_OBJECT *obj;
  FD_jobq *fdui = (FD_jobq *) fl_calloc(1, sizeof(*fdui));

  fdui->jobq = fl_bgn_form(FL_NO_BOX, 380, 330);
  obj = fl_add_box(FL_UP_BOX,0,0,380,330,"");
    fl_set_object_color(obj,FL_DARKCYAN,FL_COL1);
  fdui->jobq_browser = obj = fl_add_browser(FL_NORMAL_BROWSER,30,60,310,210,"");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
  obj = fl_add_text(FL_NORMAL_TEXT,80,10,200,40,"Job Queue for 'at'");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->jobq_close_button = obj = fl_add_button(FL_NORMAL_BUTTON,120,280,130,40,"Close");
    fl_set_object_color(obj,FL_RED,FL_COL1);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_callback(obj,do_jobq_close_button,0);
  fl_end_form();

  fdui->jobq->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_options *create_form_options(void)
{
  FL_OBJECT *obj;
  FD_options *fdui = (FD_options *) fl_calloc(1, sizeof(*fdui));

  fdui->options = fl_bgn_form(FL_NO_BOX, 650, 190);
  obj = fl_add_box(FL_UP_BOX,0,0,650,190,"");
  fdui->option_label = obj = fl_add_text(FL_NORMAL_TEXT,200,10,280,60,"Product Options");
    fl_set_object_lsize(obj,FL_HUGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_button(FL_NORMAL_BUTTON,90,130,70,40,"OK");
    fl_set_object_callback(obj,do_option_ok_button,0);
  fdui->option_input = obj = fl_add_input(FL_NORMAL_INPUT,90,70,530,40,"Options:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_button(FL_NORMAL_BUTTON,170,130,70,40,"From file");
    fl_set_object_callback(obj,do_from_file_button,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,250,130,70,40,"Save to file");
    fl_set_object_callback(obj,do_save_to_file_button,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,330,130,70,40,"Usage");
    fl_set_object_callback(obj,do_usage_button,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,410,130,70,40,"Cancel");
    fl_set_object_callback(obj,do_cancel_button,0);
  fl_end_form();

  fdui->options->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_source *create_form_source(void)
{
  FL_OBJECT *obj;
  FD_source *fdui = (FD_source *) fl_calloc(1, sizeof(*fdui));

  fdui->source = fl_bgn_form(FL_NO_BOX, 270, 300);
  obj = fl_add_box(FL_UP_BOX,0,0,270,300,"");
  obj = fl_add_text(FL_NORMAL_TEXT,20,10,220,50,"Pick a source");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->button[0] = obj = fl_add_checkbutton(FL_RADIO_BUTTON,70,60,130,40,"disk");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->button[1] = obj = fl_add_checkbutton(FL_RADIO_BUTTON,70,90,130,40,"/dev/8mm1");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->button[2] = obj = fl_add_checkbutton(FL_RADIO_BUTTON,70,120,130,40,"/dev/8mm2");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->button[3] = obj = fl_add_checkbutton(FL_RADIO_BUTTON,70,150,130,40,"/dev/8mm3");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->button[4] = obj = fl_add_checkbutton(FL_RADIO_BUTTON,70,180,130,40,"/dev/8mm4");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->button[5] = obj = fl_add_checkbutton(FL_RADIO_BUTTON,70,210,130,40,"CD (/dev/cdrom)");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_button(FL_NORMAL_BUTTON,10,250,70,40,"OK");
    fl_set_object_callback(obj,do_source_ok_button,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,190,250,70,40,"Cancel");
    fl_set_object_callback(obj,do_cancel_button,0);
  fl_end_form();

  fdui->source->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_times *create_form_times(void)
{
  FL_OBJECT *obj;
  FD_times *fdui = (FD_times *) fl_calloc(1, sizeof(*fdui));

  fdui->times = fl_bgn_form(FL_NO_BOX, 230, 250);
  obj = fl_add_box(FL_UP_BOX,0,0,230,250,"");
  obj = fl_add_frame(FL_ENGRAVED_FRAME,20,20,190,130,"");
    fl_set_object_color(obj,FL_COL1,FL_COL1);
  fdui->times_now_button = obj = fl_add_checkbutton(FL_RADIO_BUTTON,40,110,150,40,"now + 1 minute");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->times_am_button = obj = fl_add_checkbutton(FL_RADIO_BUTTON,140,40,60,40,"am");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->times_pm_button = obj = fl_add_checkbutton(FL_RADIO_BUTTON,140,70,60,40,"pm");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->times_hh_input = obj = fl_add_input(FL_NORMAL_INPUT,40,60,40,30,"Hour");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->times_mm_input = obj = fl_add_input(FL_NORMAL_INPUT,90,60,40,30,"Minute");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_text(FL_NORMAL_TEXT,90,90,30,30,"or");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,180,50,50,"OK");
    fl_set_object_callback(obj,do_times_ok_button,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,170,180,50,50,"Cancel");
    fl_set_object_callback(obj,do_cancel_button,0);
  obj = fl_add_clock(FL_ANALOG_CLOCK,80,170,80,70,"");
    fl_set_object_boxtype(obj,FL_DOWN_BOX);
  fl_end_form();

  fdui->times->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_usage *create_form_usage(void)
{
  FL_OBJECT *obj;
  FD_usage *fdui = (FD_usage *) fl_calloc(1, sizeof(*fdui));

  fdui->usage = fl_bgn_form(FL_NO_BOX, 660, 610);
  obj = fl_add_box(FL_UP_BOX,0,0,660,610,"");
  fdui->browser = obj = fl_add_browser(FL_NORMAL_BROWSER,60,60,550,480,"");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_FIXED_STYLE);
  fdui->label = obj = fl_add_text(FL_NORMAL_TEXT,220,20,230,30,"Usage");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
  obj = fl_add_button(FL_NORMAL_BUTTON,60,550,90,20,"Close");
    fl_set_object_callback(obj,do_close_button,0);
  fl_end_form();

  fdui->usage->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

