// Copyright 2013 <chaishushan{AT}gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#pragma once

#ifndef __DEFER_H__
#define __DEFER_H__

#include <functional>

//
// Go's defer operator for C++
//
// Examples:
//
//	#include "defer.h"
//
//	#include <stdio.h>
//
//	void done() {
//		printf("done\n");
//	}
//
//	struct MyStruct {
//		void MethodA() { printf("MyStruct.MethodA\n"); }
//		void MethodB() { printf("MyStruct.MethodB\n"); }
//	};
//
//	int main() {
//		defer done;
//
//		defer []{ printf("lambda 1\n"); };
//		defer []{ printf("lambda 2\n"); };
//
//		defer std::bind(&MyStruct::MethodA, MyStruct());
//		defer std::bind(&MyStruct::MethodB, MyStruct());
//		defer std::bind(printf, "printf %s %d\n", "hello world", 2015);
//
//		FILE* fp = fopen("defer.h", "rt");
//		if(fp == NULL) {
//			printf("open \"defer.h\" failed!\n");
//			return -1;
//		}
//		defer [&]{ printf("fclose(\"defer.h\")\n"); fclose(fp); };
//
//		char* buf = new char[1024];
//		defer [&]{ printf("delete buf\n"); delete[] buf; };
//
//		defer []{ printf("defer a: %d\n", 1); };
//		defer []{ printf("defer a: %d\n", 2); };
//		defer []{ printf("defer a: %d\n", 3); };
//
//		{
//			defer []{ printf("local defer a: %d\n", 1); };
//			defer []{ printf("local defer a: %d\n", 2); };
//			defer []{ printf("local defer a: %d\n", 3); };
//		}
//
//		defer []{
//			printf("defer c:\n");
//			for(int i = 0; i < 3; ++i) {
//				defer [&]{
//					defer [&]{
//						printf("\ti = %d: begin\n", i);
//						defer [&]{ printf("\ti = %d: end\n", i); };
//						printf("\ti = %d: ...\n", i);
//					};
//				};
//			}
//		};
//
//		return 0;
//	}
//
// Reference:
//
// http://github.com/chai2010/defer
// http://golang.org/doc/effective_go.html#defer
// http://golang.org/ref/spec#Defer_statements
// http://blog.korfuri.fr/post/go-defer-in-cpp/
// http://blog.korfuri.fr/attachments/go-defer-in-cpp/defer.hh
// http://blogs.msdn.com/b/vcblog/archive/2011/09/12/10209291.aspx
// http://github.com/topcpporg/cpp_features/blob/master/coroutine/coroutine.h
//

// auto __defer_action_line_???__ = __Defer()- []{ ... };
#define defer auto __DEFER_ACTION_VAR(__defer_action_line_, __LINE__, __) = ::__Defer()-
#define __DEFER_ACTION_VAR(a, b, c) __DEFER_TOKEN_CONNECT(a, b, c)
#define __DEFER_TOKEN_CONNECT(a, b, c) a ## b ## c

// Hold defered func
struct __DeferredAction {
	std::function<void()> func_;

	template<typename T>
	__DeferredAction(T&& p): func_(std::function<void()>(p)) {}

	__DeferredAction();
	__DeferredAction(__DeferredAction const&);
	__DeferredAction& operator=(__DeferredAction const&);
	__DeferredAction& operator=(__DeferredAction&&);

	__DeferredAction(__DeferredAction&& other):
		func_(std::forward<std::function<void()>>(other.func_)) {
		other.func_ = nullptr;
	}
	~__DeferredAction() {
		if(func_) { func_(); }
	}
};

template<typename T>
__DeferredAction __DeferredActionCtor(T&& p) {
	return __DeferredAction(std::forward<T>(p));
}

struct __Defer {
	template <typename Arg>
	inline __DeferredAction operator-(Arg const& arg) {
		return __DeferredActionCtor(arg);
	}
};

#endif  // __DEFER_H__
