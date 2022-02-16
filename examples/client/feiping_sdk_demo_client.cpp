#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <string>
#include <functional>
#include <fp_dll.h>

const static std::basic_string<TCHAR> dll_path = _T("..\\..\\sdk\\windows\\fpsdk_client.dll");

const static char url[] = "https://fprpc.icloud.cn";
const static char company_id[] = { "26132625" };
const static char global_device_id[] = "syh123456";
const static char global_business_token[] = "123456789";

HANDLE global_web_connected = NULL, g_close_connected = NULL;
HANDLE g_error_event_p2p_timeout = NULL, g_error_event_uninit = NULL, g_error_event_signal_timeout = NULL;
HANDLE g_error_create_video_stream = NULL, g_error_render = NULL;

// 初始化一个sdk全局变量
FpSdkClient g_fp_sdk(dll_path);

void on_event(int fd, FPEvent event)
{
    std::wstring event_str;
    switch (event)
    {
    case kFPEventWebConnected:
        event_str = _T("连接web成功");
        SetEvent(global_web_connected);
        break;
        // web发生错误时，请检查url设置是否正确，url服务器是否正常
    case kFPEventWebDisConnected:
        event_str = _T("与web断开连接");
        break;
    case kFPEventWebReConnecting:
        event_str = _T("重连web中");
        break;
    case kFPEventConnected:
        event_str = _T("建立连接");
        break;
    case kFPEventDisconnected:
        event_str = _T("断开连接");
        break;
    case kFPEventAuthSuccessed:
        event_str = _T("认证成功");
        break;
    case kFPEventAuthFailed:
        event_str = _T("认证失败");
        break;
    case kFPEventCreateVideoStream:
        event_str = _T("创建视频流");
        break;
    case kFPEventRendered:
        event_str = _T("渲染成功");
        break;
    case kFPEventStreamStop:
        event_str = _T("串流停止");
        SetEvent(g_close_connected);
        break;
    }
    SetConsoleTitle(event_str.c_str());
}

// 常见错误处理，仅供参考
void on_error(int err)
{
    std::string str;
    switch (err)
    {
    case kFPErrCodeUnInitialized:
        SetEvent(g_error_event_uninit);
        break;
    case kFPErrCodeP2PTimeout:
        SetEvent(g_error_event_p2p_timeout);
        break;
    case kFPErrCodeSignalTimeout:
        SetEvent(g_error_event_signal_timeout);
        break;
    case kFPErrCodeAuthTimeout:
        // 认证超时，请检查授权是否过期或service的on_auth回调函数
        break;
    case kFPErrCodeCreateVideoStream:
        SetEvent(g_error_create_video_stream);
        break;
    case kFPErrCodeRender:
        SetEvent(g_error_render);
        break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    printf("[error]error_code %d \n", err);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

}

// demo在此函数中测试sdk接口
void test_api(int fd)
{
    static bool first = true;
    if (!first)
    {
        return;
    }
    first = false;
    Sleep(5000);

    // 测试修改码率
    //std::string min_birate = "50", max_birate = "80";
    //g_fp_sdk.fp_set_option(fd, FPOption::kFPOptionVideoBitrateMin, min_birate.c_str(), min_birate.size());
    //g_fp_sdk.fp_set_option(fd, FPOption::kFPOptionVideoBitrateMax, max_birate.c_str(), max_birate.size());
    // 测试无损开关
    //std::string use_yuv444 = "1";
    //g_fp_sdk.fp_set_option(fd, FPOption::kFPOptionVideoLossless, use_yuv444.c_str(), use_yuv444.size());
    // 测试录像
    //g_fp_sdk.fp_start_screen_record(fd, "D:\\test.mp4");
    //Sleep(5000);
    //g_fp_sdk.fp_stop_screen_record(fd);
    // 测试客户端信息
    /*FPClientInfo client_info;
    g_fp_sdk.fp_get_client_info(fd, &client_info);
    printf("mac:%s ip:%s port:%d token:%s connect_type:%d\n",
        client_info.mac, client_info.ip, client_info.port, client_info.token, client_info.connect_type);*/
    // 测试网络信息
    /*FPNetInfo net_info;
    g_fp_sdk.fp_get_net_info(fd, &net_info);
    printf("is_lan:%d is_relay:%d rtt:%d lost_rate:%d send_bw:%d\n", 
        net_info.is_lan, net_info.is_relay, net_info.rtt, net_info.lost_rate, net_info.send_bw);*/
    // 测试视频信息
    /*FPVideoInfo video_info;
    g_fp_sdk.fp_get_video_info(fd, &video_info);
    printf("fps:%d bitrate:%d\n", video_info.fps, video_info.bitrate);*/
    // 测试命令
    //g_fp_sdk.fp_send_command(fd, kFPCommandConsole);
    // 测试截屏
    //g_fp_sdk.fp_screenshot(fd, "123.png");
}

void start_game(int fd)
{
    static bool start = false;
    if (fd < 0 || start)
    {
        return;
    }
    
    int error_code = g_fp_sdk.fp_connect(fd, global_device_id, global_business_token, kFPConnectTypeDesktopControl);

    if (error_code == kFPErrCodeSuccess)
    {
        start = true;
        return;
    }

    // connect连接失败主要为web错误，获取错误码，检查参数重新调用fp_connect即可
    error_code = g_fp_sdk.fp_get_last_error();
    switch (error_code)
    {
    case kFPWebErrDeviceNoExist:
        // 设备不存在, 通过管理员平台查看设备是否注册
    case kFPWebErrUserLimit:
        // 远程用户已达上限,重新申请授权数
    case kFPWebErrRemoteTypeError:
        // 远程类型有误
        g_fp_sdk.fp_connect(fd, global_device_id, global_business_token, FPConnectType::kFPConnectTypeDesktopControl);
        break;
    }
}

int main()
{
    // 动态加载dll
    if (!g_fp_sdk.init())
    {
        return 0;
    }

    global_web_connected = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_close_connected = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_error_event_p2p_timeout = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_error_event_uninit = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_error_event_signal_timeout = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_error_create_video_stream = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_error_render = ::CreateEvent(NULL, FALSE, FALSE, NULL);

    // 初始化
    g_fp_sdk.fp_init(url, company_id);
    g_fp_sdk.fp_set_on_event(on_event);
    g_fp_sdk.fp_set_on_error(on_error);
    int fd = g_fp_sdk.fp_create_client();

    //demo采用事件同步，用户可以更换自己的方式
    const HANDLE handle_array[] = { global_web_connected, g_close_connected,
        g_error_event_p2p_timeout, g_error_event_uninit,
        g_error_event_signal_timeout, g_error_create_video_stream,
        g_error_render };
    bool stop = false;
    while (!stop)
    {
        DWORD ret = WaitForMultipleObjects(sizeof(handle_array) / sizeof(HANDLE), handle_array, FALSE, INFINITE);
        printf("res %d\n", ret);
        switch (ret - WAIT_OBJECT_0)
        {
        case 0:
        {
            start_game(fd);
            test_api(fd);
            break;
        }
        case 1:
            // demo当串流窗口关闭时自动关闭进程
            // 用户可主动调用fp_close(fd)，只关闭串流连接;
            // 一个进程只能起一个串流窗口，fp_close后建议直接调用fp_uninit()
            stop = true;
            break;
            // 常见错误处理
        case 2:    // 未初始化，进行初始化
            //g_fp_sdk.fp_init(url, company_id);
            break;
        case 3:   // 打洞失败，开启端口映射进行尝试
            g_fp_sdk.fp_set_port_mapping(10086, 10100);
            break;
        case 4:   // 连接信令超时，主动关闭连接
            // 1 服务没启动
            // 2 登录失败
            // 3 并发数超时
            // 4 授权到期
            // 5 Token错误
            // 6 设备不存在
            // 7 企业不存在.
            g_fp_sdk.fp_close(fd);
            stop = true;
            break;
        case 5:  // 创建视频流失败，一般不会发生
            // 如果没安装显示器，服务可以尝试调用set_virtual_display
            break;
        case 6:  // 渲染失败，一般不会发生
            break;
        }
    }
    g_fp_sdk.fp_uninit();
    return 0;
}