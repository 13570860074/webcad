
#include "main.h"
#include "kernel.h"
#include "GePoint3d.h"
#include "RxManagerDictionary.h"
#include "RxWorkerThreadManager.h"
#include "EvEventManager.h"
#include "ApDocManager.h"
#include "arxHeaders.h"
#include "AcSystem.h"
#include "PrDXFParser.h"
#include <chrono>
#include <thread>
#include <functional>
using namespace std;


static void onInterval() {
	::kernel()->acevEventManager()->_emit(Ev::Interval::kInterval);
}

class A {
public:
	A() {
		this->v = 6;
	}
	~A() {
		int b = 6;
	}

	int v;
};
class B :public A {
public:
	B() {

	}
	~B() {
		int b = 6;
	}
};
void test(A *p) {
	B b = (B&)(*p);
	//B &b = (B&)(*p);
	b.v = 10;
}

int main()
{
	B* p = new B();
	test(p);

	::acrxManagerDictionary()->instantiate();


	AcSystem::setInterval(onInterval, 10);

	return 0;
	PrDXFParser* parser = new PrDXFParser();
	parser->load("E:/temp/前海店.dxf");

	return 0;
}

