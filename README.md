## 使用手册

#### 1.dll存放

将fpsdk_client.dll、fpsdk_server.dll存放至sdk\windows\目录下

将其他dll存放至bin\windows\目录下

#### 2.配置参数

feiping_sdk_demo_client_cpp\feiping_sdk_demo_client.cpp

feiping_sdk_demo_server_cpp\source\main.cpp

```C++
const static char url[] = "https://fprpc.icloud.cn";
const static char company_id[] = { "" };
const static char company_token[] = { "" };
const static char global_device_id[] = "";
const static char global_business_token[] = "";
```

| 变量名                | 说明                                     |
| --------------------- | ---------------------------------------- |
| url                   | sdk公网地址，默认https://fprpc.icloud.cn |
| company_id            | 公司id，申请授权后下发                   |
| company_token         | 公司token，申请授权后下发，仅sdk服务需要 |
| global_device_id      | 设备id，用户自定义                       |
| global_business_token | 业务token，用户自定义                    |

####  3.创建和启动service服务

用管理员方式启动cmd，输入命令

```C++
sc create sdk_service binPath="E:\sdk_demo\bin\windows\feiping_sdk_demo_service_cpp.exe"
sc start sdk_service
```

#### 4.启动客户端

启动cmd，输入命令

```c
feiping_sdk_demo_client_cpp.exe
```
