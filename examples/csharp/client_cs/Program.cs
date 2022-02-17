using System;
using System.Threading;


namespace feiping_sdk_demo_client_cs
{
    class Program
    {
        public static class Config
        {
            public static String url = "https://fprpc.icloud.cn";
            public static String company_id = "26132625";
            public static String address = "syh123456";
            public static String business_token = "123456789";
        }
        private static ManualResetEvent web_connect_event = new ManualResetEvent(false);
        private static ManualResetEvent close_game = new ManualResetEvent(false);

        static void Main(string[] args)
        {
            FeipingSdkDll.fp_init(Config.url, Config.company_id);
            FeipingSdkDll.fp_set_on_event(on_event);
            int fd = FeipingSdkDll.fp_create_client();

            if (fd < 0 )
            {
                return;
            }

            web_connect_event.WaitOne();
            if (FeipingSdkDll.fp_connect(fd, Config.address, Config.business_token, 
                FPConnectType.kFPConnectTypeDesktopControl) == 0)
            {
                // demo当串流窗口关闭时自动关闭进程
                // 用户可主动调用fp_close(fd)，只关闭串流连接;
                // 一个进程只能起一个串流窗口，fp_close后建议直接调用fp_uninit()
                close_game.WaitOne();

                FeipingSdkDll.fp_close(fd);
            }
            FeipingSdkDll.fp_uninit();
        }

        static void on_event(int fp, FPEvent event_type)
        {
            String status_str = "断开连接";
            switch (event_type)
            {
                case FPEvent.kFPEventWebConnected:
                    status_str = "连接web成功";
                    web_connect_event.Set();
                    break;
                // web发生错误时，请检查url设置是否正确，url服务器是否正常
                case FPEvent.kFPEventWebDisConnected:
                    status_str = "与web断开连接";
                    break;
                case FPEvent.kFPEventWebReConnecting:
                    status_str = "重连web中";
                    break;
                case FPEvent.kFPEventConnected:
                    status_str = "建立连接";
                    break;
                case FPEvent.kFPEventDisconnected:
                    status_str = "断开连接";
                    close_game.Set();
                    break;
                case FPEvent.kFPEventAuthSuccessed:
                    status_str = "认证成功";
                    break;
                case FPEvent.kFPEventAuthFailed:
                    status_str = "认证失败";
                    break;
                case FPEvent.kFPEventCreateVideoStream:
                    status_str = "创建视频流";
                    break;
                case FPEvent.kFPEventRendered:
                    status_str = "渲染成功";
                    break;
            }
            Console.Title = status_str;
        }

        // 常见错误处理，仅供参考
        void on_error(FPErrCode err)
        {
            switch (err)
            {
                case FPErrCode.kFPErrCodeUnInitialized:
                    // 未初始化，进行初始化
                    FeipingSdkDll.fp_init(Config.url, Config.company_id);
                    break;
                case FPErrCode.kFPErrCodeP2PTimeout:
                    // 打洞失败，开启端口映射进行尝试
                    FeipingSdkDll.fp_set_port_mapping(10086, 10100);
                    break;
                case FPErrCode.kFPErrCodeSignalTimeout:
                    // 连接信令超时，主动关闭连接
                    // 1 服务没启动
                    // 2 登录失败
                    // 3 并发数超时
                    // 4 授权到期
                    // 5 Token错误
                    // 6 设备不存在
                    // 7 企业不存在.
                    // FeipingSdkDll.fp_close(fd);
                    break;
                case FPErrCode.kFPErrCodeAuthTimeout:
                    // 认证超时，请检查授权是否过期或service的on_auth回调函数
                    break;
                case FPErrCode.kFPErrCodeCreateVideoStream:
                    // 创建视频流失败，一般不会发生
                    // 如果没安装显示器，服务可以尝试调用set_virtual_display
                    break;
                case FPErrCode.kFPErrCodeRender:
                    // 渲染失败，一般不会发生
                    break;
            }
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine("[error]error_code %d", err);
            Console.ForegroundColor = ConsoleColor.White;

        }
    }
}
