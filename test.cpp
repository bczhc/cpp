#include "./FourierSeries.h"
#include "ComplexValue.h"

int main() {
	class Callback : public FourierSeriesCallback {
		public:
			void callback(double n, double re, double im) override {
				cout << n << " " << re << " " << im << endl;
			}
	} cb;
	class FunctionImpl : public ComplexFunctionInterface {
		void x(ComplexValue &dest, double t) override {
			dest.setValue(t, t);
		}
	} f;
	FourierSeries fs(f, 100, 50);
	fs.calc(cb, 100000, 1);
}
