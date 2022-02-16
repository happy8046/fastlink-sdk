#include <fp_dll.h>

bool FpSdkClient::init()
{
    hdll_ = LoadLibrary(dll_path_.c_str());
    if (!hdll_)
    {
        printf("load library fail");
        return false;
    }
    fp_init_func_ = (fp_init_t)GetProcAddress(hdll_, "fp_init");
    if (!fp_init_func_)
    {
        printf("load fp_init fail\n");
        return false;
    }
    fp_uninit_func_ = (fp_uninit_t)GetProcAddress(hdll_, "fp_uninit");
    if (!fp_uninit_func_)
    {
        printf("load fp_uninit fail\n");
        return false;
    }
    fp_create_client_func_ = (fp_create_client_t)GetProcAddress(hdll_, "fp_create_client");
    if (!fp_create_client_func_)
    {
        printf("load fp_create_client fail\n");
        return false;
    }
    fp_connect_func_ = (fp_connect_t)GetProcAddress(hdll_, "fp_connect");
    if (!fp_connect_func_)
    {
        printf("load fp_connect fail\n");
        return false;
    }
    fp_close_func_ = (fp_close_t)GetProcAddress(hdll_, "fp_close");
    if (!fp_close_func_)
    {
        printf("load fp_close fail\n");
        return false;
    }
    fp_set_option_func_ = (fp_set_option_t)GetProcAddress(hdll_, "fp_set_option");
    if (!fp_set_option_func_)
    {
        printf("load fp_set_option fail\n");
        return false;
    }
    fp_set_on_event_func_ = (fp_set_on_event_t)GetProcAddress(hdll_, "fp_set_on_event");
    if (!fp_set_on_event_func_)
    {
        printf("load fp_set_on_event fail\n");
        return false;
    }
    fp_set_on_error_func_ = (fp_set_on_error_t)GetProcAddress(hdll_, "fp_set_on_error");
    if (!fp_set_on_error_func_)
    {
        printf("load fp_set_on_error fail\n");
        return false;
    }
    fp_get_last_error_func_ = (fp_get_last_error_t)GetProcAddress(hdll_, "fp_get_last_error");
    if (!fp_get_last_error_func_)
    {
        printf("load fp_get_last_error fail\n");
        return false;
    }
    fp_set_port_mapping_func_ = (fp_set_port_mapping_t)GetProcAddress(hdll_, "fp_set_port_mapping");
    if (!fp_set_port_mapping_func_)
    {
        printf("load fp_set_port_mapping fail\n");
        return false;
    }
    fp_start_screen_record_func_ = (fp_start_screen_record_t)GetProcAddress(hdll_, "fp_start_screen_record");
    if (!fp_start_screen_record_func_)
    {
        printf("load fp_start_screen_record fail\n");
        return false;
    }
    fp_stop_screen_record_func_ = (fp_stop_screen_record_t)GetProcAddress(hdll_, "fp_stop_screen_record");
    if (!fp_stop_screen_record_func_)
    {
        printf("load fp_stop_screen_record fail\n");
        return false;
    }
    fp_get_client_info_func_ = (fp_get_client_info_t)GetProcAddress(hdll_, "fp_get_client_info");
    if (!fp_get_client_info_func_)
    {
        printf("load fp_get_client_info fail\n");
        return false;
    }
    fp_get_net_info_func_ = (fp_get_net_info_t)GetProcAddress(hdll_, "fp_get_net_info");
    if (!fp_get_net_info_func_)
    {
        printf("load fp_get_net_info fail\n");
        return false;
    }
    fp_get_video_info_func_ = (fp_get_video_info_t)GetProcAddress(hdll_, "fp_get_video_info");
    if (!fp_get_video_info_func_)
    {
        printf("load fp_get_video_info fail\n");
        return false;
    }    
    fp_send_command_func_ = (fp_send_command_t)GetProcAddress(hdll_, "fp_send_command");
    if (!fp_send_command_func_)
    {
        printf("load fp_send_command fail\n");
        return false;
    }    
    fp_screenshot_func_ = (fp_screenshot_t)GetProcAddress(hdll_, "fp_screenshot");
    if (!fp_screenshot_func_)
    {
        printf("load fp_screenshot fail\n");
        return false;
    }
    return true;
}

FpSdkClient::~FpSdkClient()
{
    FreeLibrary(hdll_);
}