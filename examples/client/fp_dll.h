#ifndef FEIPING_SDK_DEMO_CLIENT_FP_DLL_H_
#define FEIPING_SDK_DEMO_CLIENT_FP_DLL_H_

#include <Windows.h>
#include <iostream>
#include <feiping_sdk_client.h>

class FpSdkClient
{
public:
    explicit FpSdkClient(const std::basic_string<TCHAR>& dll_path) : dll_path_(dll_path){};
    ~FpSdkClient();

    bool init();
private:
    // 推荐用C的方式定义函数指针
    typedef int(*fp_init_t)(const char* url, const char* company_id);
    typedef int(*fp_uninit_t)();
    typedef int(*fp_create_client_t)();
    typedef int(*fp_connect_t)(int fd, const char* address, const char* token, FPConnectType connect_type);
    typedef int(*fp_close_t)(int fd);
    typedef int(*fp_set_option_t)(int fd, FPOption opt, const char* val, unsigned len);
    typedef void(*fp_on_event_t)(int fd, FPEvent event);
    typedef int(*fp_set_on_event_t)(fp_on_event_t on_event);
    typedef void(*fp_on_error_t)(int err);
    typedef int(*fp_set_on_error_t)(fp_on_error_t on_error);
    typedef int(*fp_get_last_error_t)();
    typedef int(*fp_set_port_mapping_t)(unsigned start_port, unsigned end_port);
    typedef int(*fp_start_screen_record_t)(int fd, const char* path);
    typedef int(*fp_stop_screen_record_t)(int fd);
    typedef int(*fp_get_client_info_t)(int fd, FPClientInfo* info);
    typedef int(*fp_get_net_info_t)(int fd, FPNetInfo* info);
    typedef int(*fp_get_video_info_t)(int fd, FPVideoInfo* info); 
    typedef int(*fp_send_command_t)(int fd, FPCommand command);
    typedef int(*fp_screenshot_t)(int fd, const char* path);
public:
    // FIXME：demo未做函数指针空值判断，用户可以加上
    int fp_init(const char* url, const char* company_id)
    {
        return fp_init_func_(url, company_id);
    }
    int fp_uninit()
    {
        return fp_uninit_func_();
    }
    int fp_create_client()
    {
        return fp_create_client_func_();
    }
    int fp_connect(int fd, const char* address, const char* token, FPConnectType connect_type)
    {
        return fp_connect_func_(fd, address, token, connect_type);
    }
    int fp_close(int fd)
    {
        return fp_close_func_(fd);
    }
    int fp_set_option(int fd, FPOption opt, const char* val, unsigned len)
    {
        return fp_set_option_func_(fd, opt, val, len);
    }
    int fp_set_on_event(fp_on_event_t on_event)
    {
        return fp_set_on_event_func_(on_event);
    }
    int fp_set_on_error(fp_on_error_t on_error)
    {
        return fp_set_on_error_func_(on_error);
    }
    int fp_get_last_error()
    {
        return fp_get_last_error_func_();
    }
    int fp_set_port_mapping(unsigned start_port, unsigned end_port)
    {
        return fp_set_port_mapping_func_(start_port, end_port);
    }
    int fp_start_screen_record(int fd, const char* path)
    {
        return fp_start_screen_record_func_(fd, path);
    }
    int fp_stop_screen_record(int fd)
    {
        return fp_stop_screen_record_func_(fd);
    }
    int fp_get_client_info(int fd, FPClientInfo* info)
    {
        return fp_get_client_info_func_(fd, info);
    }
    int fp_get_net_info(int fd, FPNetInfo* info)
    {
        return fp_get_net_info_func_(fd, info);
    }
    int fp_get_video_info(int fd, FPVideoInfo* info)
    {
        return fp_get_video_info_func_(fd, info);
    } 
    int fp_send_command(int fd, FPCommand command)
    {
        return fp_send_command_func_(fd, command);
    }
    int fp_screenshot(int fd, const char* path)
    {
        return fp_screenshot_func_(fd, path);
    }
private:
    std::basic_string<TCHAR> dll_path_;
    HINSTANCE hdll_ = NULL;
    fp_init_t fp_init_func_;
    fp_uninit_t fp_uninit_func_;
    fp_create_client_t fp_create_client_func_;
    fp_connect_t fp_connect_func_;
    fp_close_t fp_close_func_;
    fp_set_option_t fp_set_option_func_;
    fp_set_on_event_t fp_set_on_event_func_;
    fp_set_on_error_t fp_set_on_error_func_;
    fp_get_last_error_t fp_get_last_error_func_;
    fp_set_port_mapping_t fp_set_port_mapping_func_;
    fp_start_screen_record_t fp_start_screen_record_func_;
    fp_stop_screen_record_t fp_stop_screen_record_func_;
    fp_get_client_info_t fp_get_client_info_func_;
    fp_get_net_info_t fp_get_net_info_func_;
    fp_get_video_info_t fp_get_video_info_func_;
    fp_send_command_t fp_send_command_func_;
    fp_screenshot_t fp_screenshot_func_;
};

#endif // ! FEIPING_SDK_DEMO_CLIENT_FP_DLL_H_