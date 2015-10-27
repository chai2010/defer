# Go's defer operator for C++

Example:

```C++
#include "defer.h"

#include <stdio.h>

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

	FILE* fp = fopen("defer.h", "rt");
	if(fp == NULL) return false;
	defer [&](){ printf("fclose(\"defer.h\")\n"); fclose(fp); };

	char* buf = new char[1024];
	defer [&](){ printf("delete buf\n"); delete[] buf; };

	defer [](){ printf("defer a: %d\n", __LINE__); };
	defer [](){ printf("defer a: %d\n", __LINE__); };
	defer [](){ printf("defer a: %d\n", __LINE__); };

	{
		defer [](){ printf("local defer a: %d\n", __LINE__); };
		defer [](){ printf("local defer a: %d\n", __LINE__); };
		defer [](){ printf("local defer a: %d\n", __LINE__); };
	}

	defer [](){
		printf("defer c:\n");
		for(int i = 0; i < 3; ++i) {
			defer([&](){ defer([&](){
				printf("\ti = %d: begin\n", i);
				defer([&](){ printf("\ti = %d\n", i); });
				printf("\ti = %d: end\n", i);
			});});
		}
	};
}
```

Build and run:

```
g++ -std=c++11 hello.cc
./hello
```

Output:

```
local defer a: 41
local defer a: 40
local defer a: 39
defer c:
        i = 0: begin
        i = 0: end
        i = 0
        i = 1: begin
        i = 1: end
        i = 1
        i = 2: begin
        i = 2: end
        i = 2
defer a: 36
defer a: 35
defer a: 34
delete buf
fclose("defer.h")
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
