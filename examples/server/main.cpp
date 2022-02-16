#include <tchar.h>
#include <string>
#include <Windows.h>
#include "service.h"
#include <feiping_sdk_constants.h>

#if !(defined(_WIN32) && defined(UNICODE))
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")//不显示窗口
#else
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")//不显示窗口
#endif

const static char url[] = "https://fprpc.icloud.cn";
const static char company_id[] = { "26132625" };
const static char company_token[] = { "8ebb9b4883eb437cb83a850a07586ca7" };
const static char global_device_id[] = "syh123456";
const static char global_business_token[] = "123456789";

class FpSdkService
{
public:
    FpSdkService() {};
    ~FpSdkService();

    bool init();
private:
    // 推荐用C的方式定义函数指针
    typedef int(*fp_init_t)(const char* url, const char* company_id, const char* token);
    typedef int(*fp_uninit_t)();
    typedef int(*fp_start_service_t)();
    typedef int(*fp_start_worker_t)(int argc, void ** argv);
    typedef int(*fp_set_address_t)(const char* addr);
    typedef int(*fp_close_t)(int fd);
    typedef void(*fp_on_event_t)(int fd, FPEvent event);
    typedef int(*fp_set_on_event_t)(fp_on_event_t on_event);
    typedef int(*fp_on_auth_t)(int fd, const char* token, FPConnectType type);
    typedef int(*fp_set_on_auth_t)(fp_on_auth_t on_auth);
    typedef void(*fp_on_error_t)(int err);
    typedef int(*fp_set_on_error_t)(fp_on_error_t on_error);
    typedef int(*fp_get_last_error_t)();
    typedef int(*fp_set_port_mapping_t)(unsigned start_port, unsigned end_port);

public:
    // FIXME：demo未做函数指针空值判断，用户可以加上
    int fp_init(const char* url, const char* company_id, const char* token)
    {
        return fp_init_func_(url, company_id, token);
    }
    int fp_uninit()
    {
        return fp_uninit_func_();
    }
    int fp_start_service()
    {
        return fp_start_service_func_();
    }
    int fp_start_worker(int argc, void ** argv)
    {
        return fp_start_worker_func_(argc, argv);
    }
    int fp_set_address(const char* addr)
    {
        return fp_set_address_func_(addr);
    }
    int fp_close(int fd)
    {
        return fp_close_func_(fd);
    }
    int fp_set_on_auth(fp_on_auth_t on_auth)
    {
        return fp_set_on_auth_func_(on_auth);
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
private:
    HINSTANCE hdll_ = NULL;
    fp_init_t fp_init_func_;
    fp_uninit_t fp_uninit_func_;
    fp_start_service_t fp_start_service_func_;
    fp_start_worker_t fp_start_worker_func_;
    fp_set_address_t fp_set_address_func_;
    fp_close_t fp_close_func_;
    fp_set_on_auth_t fp_set_on_auth_func_;
    fp_set_on_event_t fp_set_on_event_func_;
    fp_set_on_error_t fp_set_on_error_func_;
    fp_get_last_error_t fp_get_last_error_func_;
    fp_set_port_mapping_t fp_set_port_mapping_func_;
};

bool FpSdkService::init()
{
    hdll_ = LoadLibrary(_T("..\\..\\sdk\\windows\\fpsdk_server.dll"));

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
    fp_start_service_func_ = (fp_start_service_t)GetProcAddress(hdll_, "fp_start_service");
    if (!fp_start_service_func_)
    {
        printf("load fp_start_service fail\n");
        return false;
    }
    fp_start_worker_func_ = (fp_start_worker_t)GetProcAddress(hdll_, "fp_start_worker");
    if (!fp_start_worker_func_)
    {
        printf("load fp_start_service fail\n");
        return false;
    }
    fp_set_address_func_ = (fp_set_address_t)GetProcAddress(hdll_, "fp_set_address");
    if (!fp_set_address_func_)
    {
        printf("load fp_start_service fail\n");
        return false;
    }
    fp_close_func_ = (fp_close_t)GetProcAddress(hdll_, "fp_close");
    if (!fp_close_func_)
    {
        printf("load fp_close fail\n");
        return false;
    }
    fp_set_on_auth_func_ = (fp_set_on_auth_t)GetProcAddress(hdll_, "fp_set_on_auth");
    if (!fp_set_on_auth_func_)
    {
        printf("load fp_set_on_auth fail\n");
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
    return true;
}

FpSdkService::~FpSdkService()
{
    FreeLibrary(hdll_);
}

// 初始化一个sdk全局变量
FpSdkService g_fp_sdk;

int on_auth(int fd, const char* token, FPConnectType type)
{
    if (strcmp(token, global_business_token) == 0)
    {
        return 0;
    }
    return -1;
}

void on_error(int error)
{
    switch (error)
    {
    case kFPErrCodeLoginFail:
        // 公网登录失败，检查cpmany_id和company_token是否正确
        break;
    case kFPErrCodeLicenseInvalid:
        // license认证失败，是否授权
        break;
    }
}

//函数返回之前服务状态是正在启动，之后才会显示服务正在运行
void on_start()
{
    //加载fpsdk动态库
    g_fp_sdk.fp_init(url, company_id, company_token);
    g_fp_sdk.fp_set_address(global_device_id);
    g_fp_sdk.fp_set_on_auth(on_auth);
    g_fp_sdk.fp_set_on_error(on_error);
    // 设置虚拟显示器
    // fp_set_virtual_display(kFPVirtualDisplayMode1K144HZ);
}

//run函数返回或者停止服务时会运行此函数
void on_stop()
{
    g_fp_sdk.fp_uninit();
}

//服务运行过程的函数，需要一直阻塞
void run()
{
    g_fp_sdk.fp_start_service();
}

int _tmain(int argc, const TCHAR* argv[])
{
    //加载fpsdk动态库
    if (!g_fp_sdk.init())
    {
        return 0;
    }

    auto res = 0;
    if (argc < 2)
    {
        res = run_as_service(on_start, run, on_stop);
    }
    else
    {
        g_fp_sdk.fp_init(url, company_id, company_token);

        res = g_fp_sdk.fp_start_worker(argc, (void**)argv);

        g_fp_sdk.fp_uninit();
    }

    return res;
}