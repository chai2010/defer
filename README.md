- *Go语言QQ群: 102319854, 1055927514*
- *凹语言(凹读音“Wa”)(The Wa Programming Language): https://github.com/wa-lang/wa*

----

# Go's defer operator for C++

Build and run example:

```
make && ./hello
```

## Simple

```C++
#include "defer.h"

auto p = malloc(8<<20);
defer [&]{ free(p); p = NULL; };

do_some_thing(p);
```

## Example

```C++
#include "defer.h"

#include <stdio.h>
#include <vector>
#include <thread>
#include <mutex>

void done() {
	printf("done\n");
}

struct MyStruct {
	void MethodA() { printf("MyStruct.MethodA\n"); }
	void MethodB() { printf("MyStruct.MethodB\n"); }
};

int main() {
	defer done;

	defer []{ printf("lambda 1\n"); };
	defer []{ printf("lambda 2\n"); };

	defer std::bind(&MyStruct::MethodA, MyStruct());
	defer std::bind(&MyStruct::MethodB, MyStruct());
	defer std::bind(printf, "printf %s %d\n", "hello world", 2015);

	FILE* fp = fopen("defer.h", "rt");
	if(fp == NULL) {
		printf("open \"defer.h\" failed!\n");
		return -1;
	}
	defer [&]{ printf("fclose(\"defer.h\")\n"); fclose(fp); };

	char* buf = new char[1024];
	defer [&]{ printf("delete buf\n"); delete[] buf; };

	defer []{ printf("defer a: %d\n", 1); };
	defer []{ printf("defer a: %d\n", 2); };
	defer []{ printf("defer a: %d\n", 3); };

	{
		defer []{ printf("local defer a: %d\n", 1); };
		defer []{ printf("local defer a: %d\n", 2); };
		defer []{ printf("local defer a: %d\n", 3); };
	}

	defer []{
		printf("defer c:\n");
		for(int i = 0; i < 3; ++i) {
			defer [&]{
				defer [&]{
					printf("\ti = %d: begin\n", i);
					defer [&]{ printf("\ti = %d: end\n", i); };
					printf("\ti = %d: ...\n", i);
				};
			};
		}
	};

	std::mutex mutex;
	int sum_1_100 = 0; // 1 + 2 + 3 + ... + 100 = 5050
	defer [&]{ printf("sum_1_100 = %d\n", sum_1_100); };

	std::vector<std::thread> threads;
	for(int i = 1; i <= 100; ++i) {
		threads.push_back(std::thread([&](int step){
			mutex.lock();
			defer [&]{ mutex.unlock(); };

			sum_1_100 += step;
		}, i));
	}
	for(auto& th : threads) {
		th.join();
	}

	return 0;
}
```

Build and run:

```
g++ -std=c++11 hello.cc
./hello
```

Output:

```
local defer a: 3
local defer a: 2
local defer a: 1
sum_1_100 = 5050
defer c:
        i = 0: begin
        i = 0: ...
        i = 0: end
        i = 1: begin
        i = 1: ...
        i = 1: end
        i = 2: begin
        i = 2: ...
        i = 2: end
defer a: 3
defer a: 2
defer a: 1
delete buf
fclose("defer.h")
printf hello world 2015
MyStruct.MethodB
MyStruct.MethodA
lambda 2
lambda 1
done
```

BUGS
====

Report bugs to <chaishushan@gmail.com>.

Thanks!
