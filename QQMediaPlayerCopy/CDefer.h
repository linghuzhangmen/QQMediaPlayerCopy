/*

C++ 模拟go defer的实现

*/

#pragma once
#include <iostream>
#include <functional>

class CDefer
{
public:
    CDefer(std::function<void()> f) : f_(f) {}
    ~CDefer() { f_(); }

private:
    std::function<void()> f_;
};

#define DEFER(x) CDefer defer_##__LINE__(x)

