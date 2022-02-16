/**
 *  飞屏SDK API定义
 */

#pragma once

#ifdef _WIN32
#define FPAPI __stdcall
#else
#define FPAPI
#endif

#include "feiping_sdk_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char mac[20];
    char ip[20];
    int port;
    char token[256];
    unsigned connect_type;
} FPClientInfo;

typedef struct {
    int is_lan; // 是否局域网通信
    int is_relay; // 是否走网络中继
    unsigned rtt; // 网络往返时间
    unsigned lost_rate; // 网络丢包率
    unsigned long send_bw; // 网络发送带宽
} FPNetInfo;

typedef struct {
    unsigned fps; // 编码的FPS
    unsigned long bitrate; // 编码的码率
} FPVideoInfo;

/**
 * 创建主控端或被控端环境
 *
 * @param url 授权服务器地址
 * @param company_id 企业ID
 * @param token 企业token
 *
 * @return 0 成功
 */
int FPAPI fp_init(const char* url, const char* company_id, const char* token);

/**
 * 销毁主控端或被控端环境
 *
 */
int FPAPI fp_uninit();

/**
 * 启动服务时调用
 *
 * @param end_type 端类型
 */
int FPAPI fp_start_service();

/**
 * 启动工作进程时调用
 *
 */
int FPAPI fp_start_worker(int argc, void ** argv);

/**
 * 设置本地地址, 长度不超过32位
 *
 * @return 0 成功
 */
int FPAPI fp_set_address(const char* addr);

/*
 * 关闭连接
 *
 * @param fd 连接描述符
 */
int FPAPI fp_close(int fd);

/*
 * 回调事件
 *
 * @param fd 连接描述符
 * @param event 事件类型
 */
typedef void (FPAPI* fp_on_event)(int fd, FPEvent event);

/*
 * 设置事件回调函数
 *
 * @param on_event 事件回调函数地址
 */
int FPAPI fp_set_on_event(fp_on_event on_event);

/*
 * 回调业务认证
 *
 * @param fd 连接描述符
 * @param
 * #return 0 成功
 */
typedef int (FPAPI* fp_on_auth)(int fd, const char* token, FPConnectType type);

/*
 * 设置业务认证回调函数
 *
 * @param on_auth 业务认证回调函数地址
 */
int FPAPI fp_set_on_auth(fp_on_auth on_auth);

/*
 * 发送业务数据
 *
 * @param fd 连接描述符
 * @param data 发送的数据
 * @param len 发送的数据长度
 *
 * @return 0 成功
 */
int FPAPI fp_send_data(int fd, const char *data, unsigned len);

/*
 * 回调业务数据
 *
 * @param fd 连接描述符
 * @param data 接收的数据
 * @param len 接收的数据长度
 */
typedef void (FPAPI* fp_on_recv_data)(int fd, const char *data, unsigned len);

/*
 * 设置业务数据接收回调函数
 *
 * @param on_recv_data 业务数据接收回调函数地址
 */
int FPAPI fp_set_on_recv_data(fp_on_recv_data on_recv_data);

/*
 * 获取错误码
 * @return 错误码
 */
int FPAPI fp_get_last_error();

/*
 * 回调错误码
 *
 * @param err 错误码
 */
typedef void (FPAPI *fp_on_error)(int err);

/*
 * 设置错误回调函数
 *
 * @param on_error 错误回调函数地址
 */
int FPAPI fp_set_on_error(fp_on_error on_error);

/*
 * 获取错误码详细说明
 *
 * @return 详细信息
 */
const char* FPAPI fp_get_error_desc(int err);

/*
 * 设置虚拟显示器，即使用虚拟显示器
 *
 * @param mode 虚拟显示器分辨率、刷新率
 */
int FPAPI fp_set_virtual_display(FPVirtualDisplayMode mode);

/*
 * 设置远程桌面水印图像
 *
 * @param type 水印类型
 * @param width 水印图像宽度
 * @param height 水印图像高度
 * @param alpha 透明度(0-255)
 * @param data 水印数据
 * @param len 水印数据长度
 */
int FPAPI fp_set_desktop_watermark(FPWatermarkType type, int width, int height, int alpha, const char* data, unsigned len);

/*
 * 设置端口映射
 *
 * @param start_port 开始端口
 * @param end_port 结束端口
 */
int FPAPI fp_set_port_mapping(unsigned start_port, unsigned end_port);

/*
 * 获取客户端信息
 *
 * @param fd 连接描述符
 * @param info 客户端信息结构
 *
 * @return 0 成功
 */
int FPAPI fp_get_client_info(int fd, FPClientInfo* info);

/*
 * 获取网络信息
 *
 * @param fd 连接描述符
 * @param info 网络信息结构
 *
 * @return 0 成功
 */
int FPAPI fp_get_net_info(int fd, FPNetInfo* info);

/*
 * 获取视频信息
 *
 * @param fd 连接描述符
 * @param info 视频信息结构
 *
 * @return 0 成功
 */
int FPAPI fp_get_video_info(int fd, FPVideoInfo* info);

#ifdef __cplusplus
}
#endif