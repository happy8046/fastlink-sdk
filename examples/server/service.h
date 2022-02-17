#pragma once

typedef void(*service_func)();

int run_as_service(service_func on_start, service_func run, service_func on_stop);