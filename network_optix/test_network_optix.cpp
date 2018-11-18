Denis Sherstennikov
1. Which of the C lines define a variable which is a pointer to a function that accepts int and returns char?
( ) a)	char *func(int);
(+) b)	char (*func)(int);
( ) c)	char *(func)(int);
( ) d)	char (*)func(int);
( ) e)	char (*)(func)(int);

2. Which lines will cause C compiler to produce errors?
[ ] a)	char *const p1 = "abc"; // ok
[ ] b)	const char *p2 = p1;    // ok
[+] c)	char *p3 = p2;          // losing const-ness of data (left const)
[ ] d)	p1[1] = 'B';            // ok: tmp expr (p1+1) = 'B'
[+] e)	p2[2] = 'C';            // error data is accessed through const
[+] f)	p1++;                   // error: p1 is const-ptr
[ ] g)	p2++;                   // ok: p2 is not const-ptr, though assigned p1 and has const-ness of data

3. What will be printed to stdout as a result of the following C program compiled with MSVC on x86?

// int = 2 B
// 00000000 00000001 00000000 00000002 00000000 00000003

// int = 4 B
// 00000000 00000000 00000000 00000001 00000000 00000000 00000000 00000002 00000000 00000000 00000000 00000003

// perhaps depends on endian-ness

int main()                              // x86 sizeof(int) = 2 - 4 B, sizeof(short) = 2 B
{
    const int a[] = {1, 2, 3};          // equiv. to (const int)* a; 6 - 12 B allocated depending on system configuration
    const short* p = (const short*) a;  // (a)if (sz_short == sz_int == 2B) p == {1,2,3} (b)else if (sz_short == 2B && sz_int == 4B) p = {0, 1, 0}
    char c = *(p + 2);                  // (a) c = (char)3 (b) c = (char)0
    return printf(“%d”, (int) c);       // int cast does not change char, appending 00000000 byte to the left
}

(a) int=short=2B                3
(b) short=2B, int=4B            0

4. What will be printed to stdout as a result of the following C program compiled on x86 in 32-bit or 64 bit mode?

// in C++ our team used types a la uint32_t to be specific about size
// in 32bit mode sizeof(int) is 2 B to 4 B
// in 64bit mode sizeof(int) is 4 B to 8 B

// endian-ness dependence left out

int main() 
{
    int a = 2 & 4; // 0000 0010 & 0000 0100
    int b = 2 | 3; // 0000 0010 | 0000 0011
    int c = ~2;    // 1111 1101
    
    return printf(“%d %d %d”, a, b, c);
}

0 3 (2^16-3)       // int=2B // 32bit
0 3 (2^32-3)       // int=4B // 32bit && 64bit
0 3 (2^64-3)       // int=8B // 64bit

5. What will happen in attempt to compile and run the following C program?


void append_endl(char *s)
{
	s[strlen(s)] = '\n';      // strlen returns 11
}

int main()
{
 	char s[] = "some string"; // some string\0
	append_endl(s);           // some string\n

    return printf(s);
}

( ) a)	The program will be compiled and executed normally.
( ) b)	The program will be compiled but produce a run-time error.
(+) c)	The program will be compiled but run-time behavior is undefined. // because we can get lucky if s[strlen(s)+1] = '\0'
( ) d)	Compilation will fail with error.

6. What will be printed to stdout as a result of the following C++ program?

struct A
{
    virtual void foo() { std::cout << “A”; }
};

struct B: public A
{
    B() { foo(); }
    virtual void foo() override { std::cout << “B”; }
};

struct C: public B
{
    virtual void foo() override { std::cout << “C”; }
};


int main() 
{
    C c; // A default ctor; B default ctor calls foo override; C default ctor
}

B

7. What will be printed to stdout as a result of the following C++ program?

struct A 
{
    A() { std::cout << “A “; }
    ~A() { std::cout << "~A "; }
};

struct B: public A
{
    B() { std::cout << “B “; }
    ~B() { std::cout << “~B “; } // not virtual
};


int main() 
{
    A* obj = new B();
    delete obj;
}

BA~A

8. What will be printed to stdout as a result of the following C++ program?

struct A 
{
    A() { std::cout << "N "; }
    A(const A&) { std::cout << "C "; }
    A(A&&) { std::cout << "M "; }
    
    void p() { std::cout << "P "; }
};

void p(A a)
{
    a.p();
}

void f(A a)
{
    a.p();
    p(std::move(a)); // by rvalue, move-ctor
}


int main() 
{
    A a;  // default-ctor
    f(a); // by value, copy-ctor
}

NCPMP

9. What can be written in place of TYPE to make the following C++ program compile and work?

struct A 
{
    double i = 0;
};

void p(const A& a) 
{
    std::cout << a.i << std::endl;
};


int main() 
{
    std::function<TYPE> f = [](double i) { return A{i}; }; // returns A // gets double or float or any-kind-of-int (perhaps with warnings)
    p(f(7.7)); // 7.7 can be rounded // f must return A by-value
}

[+] a)	A(int)
[ ] b)	A*(float)
[ ] c)	A, double
[?] d)	decltype(A())(decltype(0)) // I don't remember what will be decltype(0) - if double/float/integral then '+'

10. What can be written in place of ARGS to make the following C++ program compile and work?

template<typename... T>
std::vector<std::string> make(T ... arg) 
{
    return std::vector<std::string>{std::forward<T>(arg)...}; // as far as I remember char* forwards to string ok
}

int main() 
{
    for (const auto& s: make(ARGS)) // ARGS are something-that-can-be-forwarded-to-construct-string
		std::cout << s;
}

[ ] a)	std::vector()
[+] b)	std::string()
[+] c)	"a", "b", "c"
[ ] d)	‘a’, ‘b’, ‘c’
