#include<gtk/gtk.h>
#include<stdio.h>
#include<gdk/gdkkeysyms.h>
#include"btree.h"
#include<string.h>
GtkWidget *textView1,*textView2,*textView3,*entry;
GtkListStore *list;
char history[1000000];
static void load_css()
{
   GtkCssProvider *provider;
   GdkDisplay *display;
   GdkScreen *screen;
   const gchar *css_style_file="style.css";
   GFile *css_fp=g_file_new_for_path(css_style_file);
   GError *error=0;
   provider =gtk_css_provider_new();
   display=gdk_display_get_default();
   screen =gdk_display_get_default_screen(display);              				     
   gtk_style_context_add_provider_for_screen(screen,GTK_STYLE_PROVIDER(provider),
   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
   gtk_css_provider_load_from_file(provider,css_fp,&error);
   g_object_unref(provider);
}
void close_window(GtkWidget *widget, gpointer window) {
	gtk_widget_destroy(GTK_WIDGET(window));
}
const gchar *a;
const gchar *b;
const gchar *c;
BTA *btact;

void set_textView_text(char* text,GtkWidget* textView)  
{
	GtkTextBuffer *buffer;
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
	if (buffer == NULL)
		buffer = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buffer, text, -1);
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(textView), buffer);
}

int prefix(const char *big, const char *small)
{
	int small_len = strlen(small);
	int big_len = strlen(big);
	int i;
	if (big_len < small_len)
		return 0;
	for (i = 0; i < small_len; i++)
		if (big[i] != small[i])
			return 0;
		return 1;    
}	


void suggest(char * word, gboolean Tab_pressed,GtkWidget *textView)
{
	char nextword[100],preword[100];
	int i;
	int max, existed = 0;
	GtkTreeIter Iter;
	BTint value;
	strcpy(nextword,word);
        strcpy(preword,word);
	gtk_list_store_clear(GTK_LIST_STORE(list));
	if (bfndky(btact, word, &value) ==  0) 
        { 
            existed = 1;
            gtk_list_store_append(GTK_LIST_STORE(list), &Iter);
	    gtk_list_store_set(GTK_LIST_STORE(list), &Iter, 0, nextword, -1 ); 
	}
	if (existed==0)
		btins(btact, nextword, "", 1); 
	
	if (existed==0 && Tab_pressed) 
            { 
	      char suggestlist[1000] = "Có phải bạn muốn tìm:\n";
	      strcpy(nextword, word);
              max=5;
              i=0;
	      while(max!=0)
                 {
		   bfndky(btact, preword, &value);
                   if (bprvky(btact, preword, &value) == 0)
		    {
                          if(preword[0]!=word[0])
                          break;
			  strcat(suggestlist, preword);
			  strcat(suggestlist, "\n");
			  max--;
		    }	  
                       i++;          
                       if(i==1000)break;
		 }
              max=5;
              i=0;
	      while(max!=0)
                 {
		   bfndky(btact, nextword, &value); 
                   if (bnxtky(btact, nextword, &value) == 0)
	              {
                          if(nextword[0]!=word[0])
                          break;
			  strcat(suggestlist, nextword);
			  strcat(suggestlist, "\n");
			  max--;
		      } 
                          i++;          
                          if(i==1000)break;
		 }
	      set_textView_text(suggestlist,textView);  
            }
         else
          {
            for (i = 0; i < 9; i++) 
            {
                bfndky(btact,nextword,&value);
		if(bnxtky(btact,nextword,&value)==0)
                {  
		if (prefix(nextword, word))  
		 {
                  gtk_list_store_append(GTK_LIST_STORE(list), &Iter);
     		  gtk_list_store_set(GTK_LIST_STORE(list), &Iter, 0,nextword, -1 );
                 }
                else break;
                }       	
            }
          }
		if (existed==0)
		btdel(btact, word);
}
gboolean search_suggest(GtkWidget * entry1, GdkEvent * event,gpointer data) 
{
        GtkWidget *textView=((GtkWidget**)data)[2];
        set_textView_text("",textView);
	GdkEventKey *keyEvent = (GdkEventKey *)event;
	char word[50];
	int len;
	strcpy(word, gtk_entry_get_text(GTK_ENTRY(entry)));
	if (keyEvent->keyval == GDK_KEY_Tab) {
		suggest(word,TRUE,textView);
	}
	else {
		if (keyEvent->keyval != GDK_KEY_BackSpace) {
			len = strlen(word);
			word[len] = keyEvent->keyval;
			word[len + 1] = '\0';
		}
		else {
			len = strlen(word);
			word[len - 1] = '\0';
		}
		suggest(word,FALSE,textView);
	}
	return FALSE;
}
static void errormessages(GtkWidget *window1,gpointer data)
{
  GtkWidget *b1,*b2,*b3,*fixed1,*table,*label,*hbox,*label2,*theme;
  window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "Cảnh Báo");
  gtk_window_set_default_size(GTK_WINDOW(window1), 350, 50);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
  fixed1 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window1), fixed1);
   theme = gtk_image_new_from_file("3.jpg");
  gtk_container_add(GTK_CONTAINER(fixed1), theme);
  table = gtk_table_new(2,10,FALSE);
  gtk_container_add(GTK_CONTAINER(fixed1), table);

  label = gtk_label_new("                         Không tồn tại từ này trong từ điển");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL |  GTK_SHRINK, 20, 25);
  g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window1);
  gtk_main();
  return;
}
static void errormessage(GtkWidget *window1,gpointer data)
{
  GtkWidget *b1,*b2,*b3,*fixed1,*table,*label,*hbox,*label2,*theme;
  window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "Cảnh Báo");
  gtk_window_set_default_size(GTK_WINDOW(window1), 350, 50);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
  fixed1 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window1), fixed1);
  theme = gtk_image_new_from_file("3.jpg");
  gtk_container_add(GTK_CONTAINER(fixed1), theme);
  table = gtk_table_new(2,10,FALSE);
  gtk_container_add(GTK_CONTAINER(fixed1), table);

  label = gtk_label_new("                        Lỗi từ vừa nhập đã có trong từ điển");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL |  GTK_SHRINK, 20, 25);
  g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window1);
  gtk_main();
  return;
}
static void errormessage1(GtkWidget *window1,gpointer data)
{
  GtkWidget *b1,*b2,*b3,*fixed1,*table,*label,*hbox,*label2,*theme;
  window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "Cảnh Báo");
  gtk_window_set_default_size(GTK_WINDOW(window1), 350, 50);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
  fixed1 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window1), fixed1);
  theme = gtk_image_new_from_file("3.jpg");
  gtk_container_add(GTK_CONTAINER(fixed1), theme);
  table = gtk_table_new(2,10,FALSE);
  gtk_container_add(GTK_CONTAINER(fixed1), table);

  label = gtk_label_new("                       Lỗi chưa nhập nghĩa của từ");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL |  GTK_SHRINK, 60, 25);
  g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window1);
  gtk_main();
  return;
}
static void errormessage2(GtkWidget *window1,gpointer data)
{
  GtkWidget *b1,*b2,*b3,*fixed1,*table,*label,*hbox,*label2,*theme;
  window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "Cảnh báo");
  gtk_window_set_default_size(GTK_WINDOW(window1), 250, 50);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
  fixed1 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window1), fixed1);
  theme = gtk_image_new_from_file("3.jpg");
  gtk_container_add(GTK_CONTAINER(fixed1), theme);
  table = gtk_table_new(2,10,FALSE);
  gtk_container_add(GTK_CONTAINER(fixed1), table);

  label = gtk_label_new("Lỗi chưa nhập từ");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL |  GTK_SHRINK, 100, 25);
  g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window1);
  gtk_main();
  return;
}
static void correctmessage(GtkWidget *window1,gpointer data)
{
  GtkWidget *fixed1,*table,*label,*theme;
  window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "Thông báo");
  gtk_window_set_default_size(GTK_WINDOW(window1), 180, 50);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
  fixed1 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window1), fixed1);
  theme = gtk_image_new_from_file("4.jpg");
  gtk_container_add(GTK_CONTAINER(fixed1), theme);
  table = gtk_table_new(2,10,FALSE);
  gtk_container_add(GTK_CONTAINER(fixed1), table);

  label = gtk_label_new("                      Thêm từ thành công");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 30, 25);
  g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window1);
  gtk_main();
  return;
}
static void correctmessage1(GtkWidget *window1,gpointer data)
{
  GtkWidget *fixed1,*table,*label,*theme;
  window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "Thông báo");
  gtk_window_set_default_size(GTK_WINDOW(window1), 180, 50);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
  fixed1 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window1), fixed1);
  theme = gtk_image_new_from_file("4.jpg");
  gtk_container_add(GTK_CONTAINER(fixed1), theme);
  table = gtk_table_new(2,10,FALSE);
  gtk_container_add(GTK_CONTAINER(fixed1), table);

  label = gtk_label_new("                     Xóa từ thành công");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 30, 25);
  g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window1);
  gtk_main();
  return;
}
int btfind(char* word,GtkWidget *textView) 
{
        FILE *f;
        strcat(history,word);
        strcat(history,"\n");
        f=fopen("history","a+");
        fprintf(f,"%s\n",word);
        fclose(f);
        GtkWidget *w;
	char mean[200000];
	int size;
        
	if(btsel(btact,word,mean,200000,&size)==0)
	{
		set_textView_text(mean,textView);
		return 1;
	}
    else 
    {
        errormessages(w,NULL);
        return 0;
    }
}
static void search(GtkWidget *w, gpointer data)
{
 GtkWidget *w1;
 GtkWidget *entry1= ((GtkWidget**)data)[0];
 GtkWidget *window1=((GtkWidget**)data)[1];
 GtkWidget *textView=((GtkWidget**)data)[2];
 a = gtk_entry_get_text(GTK_ENTRY(entry1));
 char word[50];
 BTint x;
 strcpy(word,a);
 if(word[0]!=NULL)
 btfind(word,textView);
 else
   {
     errormessage2(w1,NULL);
   }
 
}

static void timkiemtu(GtkWidget* window1,gpointer data)
{
  GtkWidget *b1,*b2,*b3,*fixed1,*table,*label,*label2,*textView;
  window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "Tìm kiếm Từ");
  gtk_window_set_default_size(GTK_WINDOW(window1), 512, 512);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
  fixed1 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window1), fixed1);
 
  b1 = gtk_button_new_with_label("Tìm kiếm");
  gtk_fixed_put(GTK_FIXED(fixed1), b1, 430, 15);
  gtk_widget_set_size_request(b1, 80, 30);
  
  b2=gtk_button_new_with_label("<-Trở về");
  gtk_fixed_put(GTK_FIXED(fixed1), b2, 430, 50);
  gtk_widget_set_size_request(b2, 80, 30);
  table = gtk_table_new(1,1,FALSE);
  gtk_container_add(GTK_CONTAINER(fixed1), table);

  label = gtk_label_new("Nhập:");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 30, 15);
  entry=gtk_entry_new();
  gtk_widget_set_size_request(entry, 300, 30);
  gtk_entry_set_max_length(GTK_ENTRY(entry),100);
  gtk_table_attach(GTK_TABLE(table), entry, 1, 2, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 15, 15);
  GtkWidget *comple = gtk_entry_completion_new();
  gtk_entry_completion_set_text_column(comple, 0);
  list = gtk_list_store_new(10, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,      
 G_TYPE_STRING,G_TYPE_STRING, G_TYPE_STRING);
  gtk_entry_completion_set_model(comple, GTK_TREE_MODEL(list));
  gtk_entry_set_completion(GTK_ENTRY(entry), comple);
  label2 = gtk_label_new("Nghĩa:");
  gtk_table_attach(GTK_TABLE(table), label2, 0, 1, 1, 2,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 15, 200);
  textView = gtk_text_view_new();
  gtk_widget_set_size_request(textView, 300, 300);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD);
  GtkWidget *scrolling = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scrolling), textView);
  gtk_table_attach(GTK_TABLE(table), scrolling, 1, 2, 1, 2,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 15, 15);
  GtkWidget *data1[3];
  data1[0]= entry;
  data1[1]= window1;
  data1[2]= textView;
  g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(search),data1);
  g_signal_connect(entry, "key-press-event", G_CALLBACK(search_suggest), data1);
  g_signal_connect(G_OBJECT(b1),"clicked",G_CALLBACK(search),data1);
  g_signal_connect(b2,"clicked",G_CALLBACK(close_window),window1);
  g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window1);
  gtk_main();
  return;
}

static void thongtin(GtkWidget *window1,gpointer data)
{
  GtkWidget *fixed1,*table,*label;
  window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "Thông tin");
  gtk_window_set_default_size(GTK_WINDOW(window1), 250, 100);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
  fixed1 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window1), fixed1);
  table = gtk_table_new(2,10,FALSE);
  gtk_container_add(GTK_CONTAINER(fixed1), table);

  label = gtk_label_new("      Thành viên trong Nhóm\nNguyễn Mạnh Trường-20184208\nMai Minh Trọng");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 60, 10);
  
  g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window1);
  gtk_main();
  return;
}
static void add(GtkWidget *window1, gpointer data)
{
        
	GtkTextBuffer *buffer1,*buffer2;
	GtkTextIter start, end,iter;
	char mean[10000], word[50];
        mean[0]==NULL;
        word[0]==NULL;
	buffer1 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView1));
	gtk_text_buffer_get_iter_at_offset(buffer1, &iter, 0);

	gtk_text_buffer_insert(buffer1, &iter, "", -1);
	gtk_text_buffer_get_bounds (buffer1, &start, &end);
	b = gtk_text_buffer_get_text (buffer1, &start, &end, FALSE);

	strcpy(word,b);

	buffer2 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView2));
	gtk_text_buffer_get_iter_at_offset(buffer2, &iter, 0);

	gtk_text_buffer_insert(buffer2, &iter, "", -1);
	gtk_text_buffer_get_bounds (buffer2, &start, &end);
	b = gtk_text_buffer_get_text (buffer2, &start, &end, FALSE);

	strcpy(mean,b);
        if(word[0]==NULL)
         {
          errormessage2(window1,NULL);
           return;
         }
        if(mean[0]==NULL)
         {
           errormessage1(window1,NULL);
           return;
         }
	BTint x;
         if (bfndky(btact, word, &x )!= 0)
          {
	   btins(btact,word,mean,10000);
           correctmessage(window1,NULL);
          }
         else errormessage(window1,NULL);

	return;
}
static void del(GtkWidget *window1, gpointer data)
{
        
	GtkTextBuffer *buffer1,*buffer2;
	GtkTextIter start, end,iter;
	char word[50];
        word[0]==NULL;
	buffer1 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView3));
	gtk_text_buffer_get_iter_at_offset(buffer1, &iter, 0);

	gtk_text_buffer_insert(buffer1, &iter, "", -1);
	gtk_text_buffer_get_bounds (buffer1, &start, &end);
	c = gtk_text_buffer_get_text (buffer1, &start, &end, FALSE);

	strcpy(word,c);
        if(word[0]==NULL)
          errormessage2(window1,NULL);
	else
            {
              if(btdel(btact,word)==0)
              correctmessage1(window1,NULL);
              else errormessages(window1,NULL);
            }
         
	return;
}
static void themtu(GtkWidget* window1,gpointer data)
{
  
  GtkWidget *b1,*b2,*b3,*fixed1,*table,*label,*label2;
  window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "THÊM Từ");
  gtk_window_set_default_size(GTK_WINDOW(window1), 512, 512);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
  fixed1 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window1), fixed1);
  b1 = gtk_button_new_with_label("Thêm");
  gtk_fixed_put(GTK_FIXED(fixed1), b1, 222, 470);
  gtk_widget_set_size_request(b1, 80, 30);
  
  b2=gtk_button_new_with_label("<-Trở về");
  gtk_fixed_put(GTK_FIXED(fixed1), b2, 430, 15);
  gtk_widget_set_size_request(b2, 80, 30);
  table = gtk_table_new(2,10,FALSE);
  gtk_container_add(GTK_CONTAINER(fixed1), table);

  label = gtk_label_new("Nhập:");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 30, 15);
  
  textView1 = gtk_text_view_new();
  gtk_widget_set_size_request(textView1, 300, 20);
  gtk_table_attach(GTK_TABLE(table), textView1, 1, 2, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 15, 15);
  
  
  label2 = gtk_label_new("Nghĩa:");
  gtk_table_attach(GTK_TABLE(table), label2, 0, 1, 1, 2,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 15, 200);
  textView2 = gtk_text_view_new();
  gtk_widget_set_size_request(textView2, 300, 300);
  GtkWidget *scrolling = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scrolling), textView2);
   gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView2), GTK_WRAP_WORD);
  gtk_table_attach(GTK_TABLE(table), scrolling, 1, 2, 1, 2,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 15, 15);
  
  g_signal_connect(G_OBJECT(b1),"clicked",G_CALLBACK(add),NULL);
  g_signal_connect(b2,"clicked",G_CALLBACK(close_window),window1);
  g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window1);
  gtk_main();
  return;
}
static void xoatu(GtkWidget* window1,gpointer data)
{
  GtkWidget *b1,*b2,*b3,*fixed1,*table,*label,*label2;
  window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "Xóa từ");
  gtk_window_set_default_size(GTK_WINDOW(window1), 512, 200);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
  fixed1 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window1), fixed1);
  b1 = gtk_button_new_with_label("Xóa");
  gtk_fixed_put(GTK_FIXED(fixed1), b1, 222, 150);
  gtk_widget_set_size_request(b1, 80, 30);
  
  b2=gtk_button_new_with_label("<-Trở về");
  gtk_fixed_put(GTK_FIXED(fixed1), b2, 430, 15);
  gtk_widget_set_size_request(b2, 80, 30);
  table = gtk_table_new(2,10,FALSE);
  gtk_container_add(GTK_CONTAINER(fixed1), table);

  label = gtk_label_new("Nhập:");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 30, 15);
  textView3 = gtk_text_view_new();
  gtk_widget_set_size_request(textView3, 300, 20);
  gtk_table_attach(GTK_TABLE(table), textView3, 1, 2, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 15, 15);
  
  g_signal_connect(G_OBJECT(b1),"clicked",G_CALLBACK(del),NULL);
  g_signal_connect(b2,"clicked",G_CALLBACK(close_window),window1);
  g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window1);
  gtk_main();
  return;
}
static void xoalichsu(GtkWidget *window1,gpointer data)
{
 GtkWidget *textView=((GtkWidget**)data)[0];
 FILE *f;
 f=fopen("history","w");
 fprintf(f,"%s","");
 strcpy(history,"");
 set_textView_text("",textView);
 GtkWidget *fixed1,*table,*label,*theme;
  window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "Thông báo");
  gtk_window_set_default_size(GTK_WINDOW(window1), 200, 50);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
  fixed1 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window1), fixed1);
  table = gtk_table_new(2,10,FALSE);
  gtk_container_add(GTK_CONTAINER(fixed1), table);
  theme = gtk_image_new_from_file("4.jpg");
  gtk_container_add(GTK_CONTAINER(fixed1), theme);
  label = gtk_label_new("                          Đã xóa xong");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 30, 25);
  g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window1);
  gtk_main();
  return;
}
static void lichsutimkiem(GtkWidget *window1,gpointer data)
{
  if(history[0]=='\0')
  {
    char a;
    FILE *f;
    f=fopen("history","r");
    int i=0;
  while(a!=EOF)
  {
    a=fgetc(f);
    history[i++]=a;
  }
  history[i-1]='\0';
  }
  GtkWidget *fixed1,*table,*label,*b1,*textView,*label1,*label2;
  window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "Lịch sử tìm kiếm");
  gtk_window_set_default_size(GTK_WINDOW(window1), 400, 512);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
  fixed1 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window1), fixed1);
  b1=gtk_button_new_with_label("Xóa lịch sử");
  gtk_fixed_put(GTK_FIXED(fixed1), b1, 350, 50);
  gtk_widget_set_size_request(b1, 80, 30);
  table = gtk_table_new(2,10,FALSE);
  gtk_container_add(GTK_CONTAINER(fixed1), table);
  label = gtk_label_new("  ");
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 30, 15);
  label1 = gtk_label_new("                                                                                ");
  gtk_table_attach(GTK_TABLE(table), label1, 1, 2, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 15, 15);
  label2 = gtk_label_new("Lịch sử:");
  gtk_table_attach(GTK_TABLE(table), label2, 0, 1, 1, 2,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 15, 200);
  textView = gtk_text_view_new();
  gtk_widget_set_size_request(textView, 300, 300);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD);
  GtkWidget *scrolling = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scrolling), textView);
  gtk_table_attach(GTK_TABLE(table), scrolling, 1, 2, 1, 2,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 15, 15);
  GtkWidget *data1[0];
  data1[0]= textView;
  set_textView_text(history,textView);
  g_signal_connect(G_OBJECT(b1),"clicked",G_CALLBACK(xoalichsu),data1);
  g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window1);
  gtk_main();
  return;
}

int main()
{
   btinit();
  btact=btopn("Tudien",0,1);
  gtk_init(NULL,NULL);
  load_css();
  GtkWidget *window,*b1,*b2,*b3,*b4,*b5,*b6,*fixed,*theme;
  window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Từ điển Anh Việt");
  gtk_window_set_default_size(GTK_WINDOW(window), 700, 500);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  fixed = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), fixed);
  
  theme = gtk_image_new_from_file("2.jpg");
  gtk_container_add(GTK_CONTAINER(fixed), theme);

  b1=gtk_button_new_with_label("Tìm kiếm từ");
  gtk_fixed_put(GTK_FIXED(fixed), b1, 30, 30);
  gtk_widget_set_size_request(b1, 100, 100);
  g_signal_connect(b1,"clicked",G_CALLBACK(timkiemtu),NULL);
  
  b2=gtk_button_new_with_label("Bổ sung từ");
  gtk_fixed_put(GTK_FIXED(fixed), b2, 170, 30);
  gtk_widget_set_size_request(b2, 100, 100);
  g_signal_connect(b2,"clicked",G_CALLBACK(themtu),NULL);
 
  b3=gtk_button_new_with_label("Xóa từ");
  gtk_fixed_put(GTK_FIXED(fixed), b3, 310, 30);
  gtk_widget_set_size_request(b3, 100, 100);
  g_signal_connect(b3,"clicked",G_CALLBACK(xoatu),NULL);
  
  b4=gtk_button_new_with_label("Lịch sử \ntìm kiếm");
  gtk_fixed_put(GTK_FIXED(fixed), b4, 450, 30);
  gtk_widget_set_size_request(b4, 100, 100);
  g_signal_connect(b4,"clicked",G_CALLBACK(lichsutimkiem),NULL);
 
  b5=gtk_button_new_with_label("Thông tin");
  gtk_fixed_put(GTK_FIXED(fixed), b5, 590, 30);
  gtk_widget_set_size_request(b5, 100, 100);
  g_signal_connect(b5,"clicked",G_CALLBACK(thongtin),NULL);

  b6=gtk_button_new_with_label("Thoát");
  gtk_fixed_put(GTK_FIXED(fixed), b6, 300, 300);
  gtk_widget_set_size_request(b6, 100, 100);
  g_signal_connect(b6,"clicked",G_CALLBACK(gtk_main_quit),NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);
  gtk_main();
  return 0;
} 
