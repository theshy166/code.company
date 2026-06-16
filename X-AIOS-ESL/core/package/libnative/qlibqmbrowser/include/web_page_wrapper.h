#ifndef WEB_PAGE_WRAPPER_H
#define WEB_PAGE_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif


// Define opaque pointer types to handle C++ objects in C
typedef struct web_page web_page;

//stand for app
typedef struct html_widget {
    web_page *page;

    //callback for web_page
    void (*open_url)(const char *);
    void (*set_caption)(const char *);
    //..
}html_widget;


// Constructor and Destructor
web_page* web_page_new(html_widget *, int pool_size);
void web_page_delete(web_page* wp);

void web_page_runloop(web_page *wp, bool is_run);

// Methods wrapped as C functions
void web_page_open(web_page* wp, const char* url, const char* hash);
void web_page_stop_loading(web_page* wp);
int web_page_is_downloading(web_page* wp);

// void web_page_on_mouse_over(web_page* wp, int x, int y, int client_x, int client_y);
// void web_page_on_lbutton_down(web_page* wp, int x, int y, int client_x, int client_y);
// void web_page_on_lbutton_up(web_page* wp, int x, int y, int client_x, int client_y);



#ifdef __cplusplus
}
#endif

#endif // WEB_PAGE_WRAPPER_H
