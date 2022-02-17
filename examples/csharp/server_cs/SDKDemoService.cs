using System;
using System.IO;
using System.ServiceProcess;
using System.Threading;

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

    public partial class SDKDemoService : ServiceBase
    {
        private Thread thread_;
        public SDKDemoService()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            FeipingSdkDll.fp_init(ServiceConfig.url, ServiceConfig.company_id, ServiceConfig.company_token);
            FeipingSdkDll.fp_set_address(ServiceConfig.address);
            FeipingSdkDll.fp_set_on_auth(on_auth);
            FeipingSdkDll.fp_set_on_error(on_error);
            // 设置虚拟显示器
            // FeipingSdkDll.fp_set_virtual_display(FPVirtualDisplayMode.kFPVirtualDisplayMode1K144HZ);
            // 需要新开一个线程，不然服务会卡在正在启动
            thread_ = new Thread(() => FeipingSdkDll.fp_start_service());
            thread_.Start();
        }

        protected override void OnStop()
        {
            FeipingSdkDll.fp_uninit();
            thread_.Abort();
        }

        protected override void OnShutdown()
        {
            OnStop();
            base.OnShutdown();
        }

        int on_auth(int fd, String token, FPConnectType type)
        {
            return String.Compare(token, ServiceConfig.token) == 0 ? 0 : -1;
        }

        void on_error(FPErrCode error)
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
    }
}
