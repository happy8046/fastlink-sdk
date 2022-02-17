using System.ServiceProcess;
using System.Threading;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;

namespace feiping_sdk_demo_server_cs
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        static int Main()
        {
            string[] args = Environment.GetCommandLineArgs();
            int res = 0;

            if (args.Length < 2)
            {
                ServiceBase.Run(new SDKDemoService());
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
