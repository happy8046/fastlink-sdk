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
    unsigned fps; // 渲染FPS
    unsigned long bitrate; // 接收的码率
} FPVideoInfo;

/**
 * 创建主控端或被控端环境
 *
 * @param url 授权服务器地址
 * @param company_id 企业ID
 *
 * @return 0 成功
 */
int FPAPI fp_init(const char* url, const char* company_id);

/**
 * 销毁主控端或被控端环境
 *
 */
int FPAPI fp_uninit();

/*
 * 创建连接描述符
 *
 * @param 主控端调用
 * @return 连接描述符
 */
int FPAPI fp_create_client();

/*
 * 连接被控端
 *
 * @param fd 连接描述符
 * @param address 被控端地址
 * @param token 业务认证token
 * @param connect_type 连接方式
 *
 * @return 0 成功
 */
int FPAPI fp_connect(int fd, const char* address, const char* token, FPConnectType connect_type);

/*
 * 关闭连接
 *
 * @param fd 连接描述符
 */
int FPAPI fp_close(int fd);

/*
 * 设置选项
 *
 * @param fd 连接描述符
 * @param opt 选项
 * @param val 值
 * @param len 值长度
 *
 * @return 0 成功
 */
int FPAPI fp_set_option(int fd, FPOption opt, const char* val, unsigned len);

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
 * 发送命令
 *
 * @param fd 连接描述符
 * @param command 命令
 *
 * @return 0 成功
 */
int FPAPI fp_send_command(int fd, FPCommand command);

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
 * 设置远程桌面窗口位置
 *
 * @param fd 连接描述符
 * @param x,y,width,height 坐标、宽高
 */
int FPAPI fp_set_desktop_wnd_pos(int fd, int x, int y, int width, int height);

/*
 * 设置远程CMD窗口位置
 *
 * @param fd 连接描述符
 * @param x,y,width,height 坐标、宽高
 */
int FPAPI fp_set_cmd_wnd_pos(int fd, int x, int y, int width, int height);

/*
 * 设置远程文件窗口位置
 *
 * @param fd 连接描述符
 * @param x,y,width,height 坐标、宽高
 *
 * @return 0 成功
 */
int FPAPI fp_set_file_trans_wnd_pos(int fd, int x, int y, int width, int height);

/*
 * 截屏（仅支持png）
 *
 * @param fd 连接描述符
 * @param path 保存路径
 *
 * @return 0 成功
 */
int FPAPI fp_screenshot(int fd, const char* path);

/*
 * 开始录像（仅支持MP4）
 *
 * @param fd 连接描述符
 * @param path 保存路径
 *
 * @return 0 成功
 */
int FPAPI fp_start_screen_record(int fd, const char* path);

/*
 * 结束录像
 *
 * @param fd 连接描述符
 *
 * @return 0 成功
 */
int FPAPI fp_stop_screen_record(int fd);

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