using System;
using System.Runtime.InteropServices;

namespace feiping_sdk_demo_client_cs
{
    enum FPErrCode
    {
        kFPErrCodeSuccess = 0,
        kFPErrCodeUnInitialized = -0xFFFF, // 未初始化
        kFPErrCodeInitialized, // 已经初始化
        kFPErrCodeParameterInvalid, // 参数错误
        kFPErrCodeNotStartStream, // 未开始串流
        kFPErrCodeStreamStarted, // 已经开始串流
        kFPErrCodeFDInvalid, // FD错误
        kFPErrCodeLoginFail, // 公网登录失败
        kFPErrCodeLicenseInvalid, // license错误
        kFPErrCodeInner, // 内部错误
        kFPErrCodeAuthFail, // 认证失败
        kFPErrCodeAuthTimeout, // 认证超时
        kFPErrCodeCreateVideoStream, // 创建视频流失败
        kFPErrCodeRender, // 渲染失败
        kFPErrCodeWEBTimeout, // web连接超时
        kFPErrCodeWEBFail, // web连接失败
        kFPErrCodeSignalTimeout, // 连接信令超时
        kFPErrCodeP2PTimeout, // 打洞超时
        kFPErrCodeAddrNotSet, // 未设置地址
    };

    enum FPWebErrCode
    {
        kFPWebErrDeviceNoExist = 1007, // 设备不存在
        kFPWebErrUserLimit = 1010, // 远程用户已达上限
        kFPWebErrRemoteTypeError = 1014, // 远程类型有误
    };

    enum FPConnectType
    {
        kFPConnectTypeDesktopControl = 2, // 桌面控制
        kFPConnectTypeDesktopView, // 桌面观看
        kFPConnectTypeCamera, // 摄像头
        kFPConnectTypeFileTrans, // 远程文件
        kFPConnectTypeCmd, // 远程CMD
    };

    enum FPEvent
    {
        kFPEventWebConnected, // web连接成功
        kFPEventWebDisConnected, //WEB连接断开
        kFPEventWebReConnecting, //WEB重连中
        kFPEventConnected, // 建立连接
        kFPEventDisconnected, // 断开连接
        kFPEventAuthSuccessed, // 认证成功
        kFPEventAuthFailed, // 认证失败
        kFPEventCreateVideoStream, // 创建视频流
        kFPEventRendered, // 渲染成功
    };

    enum FPCursor
    {
        kFPCursorLocal, // 本地光标
        kFPCursorRemote, // 远端光标
        kFPCursorHide, // 隐藏光标
    };

    enum FPLanguage
    {
        kFPLanguageCN, // 中文
        kFPLanguageEN, // 英文
    };

    enum FPOption
    {
        // 0x101 避免和render::Option冲突
        kFPOptionScreenOriginalSize = 0x101, // 开启或关闭"原始屏幕比例"，默认关闭 
        kFPOptionFullScreen, // 开启或关闭"全屏"，默认关闭
        kFPOptionBlackScreen, // 开启或关闭"黑屏"，默认关闭
        kFPOptionVideoLossless, // 开启或关闭"无损画质"，默认关闭
        kFPOptionVideoBitrateMax, // 设置最大"视频码率", 单位Mbit/s
        kFPOptionVideoBitrateMin, // 设置最小"视频码率", 单位Mbit/s
        kFPOptionSound, // 开启或关闭"声音"，默认开启，暂不实现
        kFPOptionInput, // 开启或关闭"输入"，默认开启，暂不实现
        kFPOptionMicrophone, // 开启或关闭"麦克风"，默认关闭。备注：开启需要顺网声卡驱动支持
        kFPOptionRemoteDeviceMute, // 开启或关闭"被控设备静音"，默认关闭。备注：开启需要顺网声卡驱动支持
        kFPOptionFileTrans, // 启用或禁用"文件传输"，默认启用, 需要在connect之前设置
        kFPOptionCursor, // 设置"光标"，默认本地光标
        kFPOptionEnergySaving, // 开启或关闭"节能"，默认关闭
        kFPOptionControlPanelWnd, // 显示或隐藏"控制面板窗口"，默认显示
        kFPOptionFpsWnd, // 显示或隐藏"FPS窗口"，默认显示
        kFPOptionLanguage, // 设置"语言"，"1" 中文, "2" 英文, 默认中文
    };

    enum FPCommand
    {
        kFPCommandCtrlAltDelete, // Ctrl+Alt+Delete
        kFPCommandLock, // 锁屏
        kFPCommandLogout, // 注销
        kFPCommandRestart, // 重启
        kFPCommandShutdown, // 关机
        kFPCommandExplorer, // 资源管理器
        kFPCommandTaskmgr, // 任务管理器
        kFPCommandConsole, // 控制台
    };

    enum FPVirtualDisplayMode
    {
        kFPVirtualDisplayModeAuto, // 根据主控端显示器自动匹配
        kFPVirtualDisplayMode1K60HZ,
        kFPVirtualDisplayMode1K144HZ,
        kFPVirtualDisplayMode2K60HZ,
        kFPVirtualDisplayMode2K144HZ,
        kFPVirtualDisplayMode4K60HZ,
    };

    enum FPWatermarkType
    {
        kFPWatermarkTypeNone = 0, // 无水印
        kFPWatermarkTypeBright, // 明水印
        kFPWatermarkTypeDark, // 暗水印
    };

    class FeipingSdkDll
    {
        public delegate void fp_on_event(int fp, FPEvent on_event);

        [DllImport("fpsdk_client.dll", EntryPoint = "fp_init", CallingConvention = CallingConvention.StdCall)]
        public static extern int fp_init(String url, String company_id);

        [DllImport("fpsdk_client.dll", EntryPoint = "fp_uninit", CallingConvention = CallingConvention.StdCall)]
        public static extern void fp_uninit();

        [DllImport("fpsdk_client.dll", EntryPoint = "fp_create_client", CallingConvention = CallingConvention.StdCall)]
        public static extern int fp_create_client();

        [DllImport("fpsdk_client.dll", EntryPoint = "fp_connect", CallingConvention = CallingConvention.StdCall)]
        public static extern int fp_connect(int fd, String address, String token, FPConnectType connect_type);

        [DllImport("fpsdk_client.dll", EntryPoint = "fp_close", CallingConvention = CallingConvention.StdCall)]
        public static extern void fp_close(int fd);

        [DllImport("fpsdk_client.dll", EntryPoint = "fp_set_option", CallingConvention = CallingConvention.StdCall)]
        public static extern int fp_set_option(int fd, FPOption opt, String val, int len);

        [DllImport("fpsdk_client.dll", EntryPoint = "fp_set_on_event", CallingConvention = CallingConvention.StdCall)]
        public static extern void fp_set_on_event(fp_on_event on_event);

        [DllImport("fpsdk_client.dll", EntryPoint = "fp_set_port_mapping", CallingConvention = CallingConvention.StdCall)]
        public static extern void fp_set_port_mapping(int start_port, int end_port);
        // 以此类推添加函数
    }
}
