using System;
using System.IO;
using System.ServiceProcess;
using System.Threading;

namespace feiping_sdk_demo_server_cs
{
    public partial class SDKDemoService : ServiceBase
    {
        public delegate void on_service_start();
        public delegate void on_service_stop();

        private Thread thread_;
        private on_service_start on_start_;
        private on_service_stop on_stop_;

        public SDKDemoService(on_service_start on_start, on_service_stop on_stop)
        {
            on_start_ = on_start;
            on_stop_ = on_stop;
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            on_start_();
            // 需要新开一个线程，不然服务会卡在正在启动
            thread_ = new Thread(() => FeipingSdkDll.fp_start_service());
            thread_.Start();
        }

        protected override void OnStop()
        {
            on_stop_();
            thread_.Abort();
        }

        protected override void OnShutdown()
        {
            OnStop();
            base.OnShutdown();
        }
    }
}
