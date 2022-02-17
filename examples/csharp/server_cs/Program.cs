using System.ServiceProcess;
using System.Threading;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.Diagnostics;

namespace feiping_sdk_demo_server_cs
{
    public static class ServiceConfig
    {
        public static String url = "https://fprpc.icloud.cn";
        public static String company_id = "26132625";
        public static String company_token = "8ebb9b4883eb437cb83a850a07586ca7";
        public static String address = "syh123456";
        public static String token = "123456789";
    }

    static class Program
    {
        static bool is_run_as_service()
        {
            return Process.GetCurrentProcess().SessionId == 0;
        }

        static void on_start()
        {
            FeipingSdkDll.fp_init(ServiceConfig.url, ServiceConfig.company_id, ServiceConfig.company_token);
            FeipingSdkDll.fp_set_address(ServiceConfig.address);
            FeipingSdkDll.fp_set_on_auth(on_auth);
            FeipingSdkDll.fp_set_on_error(on_error);
            // 设置虚拟显示器
            // FeipingSdkDll.fp_set_virtual_display(FPVirtualDisplayMode.kFPVirtualDisplayMode1K144HZ);

        }

        static void on_stop()
        {
            FeipingSdkDll.fp_uninit();
        }

        static int on_auth(int fd, String token, FPConnectType type)
        {
            return String.Compare(token, ServiceConfig.token) == 0 ? 0 : -1;
        }

        static void on_error(FPErrCode error)
        {
            switch (error)
            {
                case FPErrCode.kFPErrCodeLoginFail:
                    // 公网登录失败，检查cpmany_id和company_token是否正确
                    break;
                case FPErrCode.kFPErrCodeLicenseInvalid:
                    // license认证失败，是否授权
                    break;
            }
        }
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        static int Main()
        {
            string[] args = Environment.GetCommandLineArgs();
            int res = 0;

            if (args.Length < 2)
            {
                if (is_run_as_service())
                {
                    ServiceBase.Run(new SDKDemoService(on_start, on_stop));
                }
                else
                {
                    on_start();
                    FeipingSdkDll.fp_start_service();
                    on_stop();
                }
            }
            else
            {
                FeipingSdkDll.fp_init(ServiceConfig.url, ServiceConfig.company_id, ServiceConfig.company_token);
                IntPtr[] str_arr = new IntPtr[args.Length];

                IntPtr argv = Marshal.AllocHGlobal(Marshal.SizeOf<IntPtr>() * args.Length);

                for (int i = 0; i < args.Length; ++i)
                {
                    var arg = args[i];
                    IntPtr arg_ptr = Marshal.StringToHGlobalUni(arg);
                    Marshal.WriteIntPtr(argv + i * Marshal.SizeOf<IntPtr>(), arg_ptr);
                    //存下来防止内存泄漏
                    str_arr[i] = arg_ptr;
                }
                res = FeipingSdkDll.fp_start_worker(args.Length, argv);

                //释放资源
                foreach (var str_p in str_arr)
                {
                    Marshal.FreeHGlobal(str_p);
                }
                Marshal.FreeHGlobal(argv);
                FeipingSdkDll.fp_uninit();
            }
            return res;
        }
    }
}
