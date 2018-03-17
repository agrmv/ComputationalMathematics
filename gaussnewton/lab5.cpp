#include <iostream>
#include <vector>

using namespace std;
using vector_t = vector<double>;

double q(const vector_t &vec, double x, size_t n) {
    if (n >= vec.size())
        return 1;
    double result = 1;
    for (size_t i = 0; i < vec.size(); ++i)
        if (i != n)
            result *= (x - vec[i]) / (vec[n] - vec[i]);
    return result;
}

double p(const vector_t &_x, const vector_t &y, double x, double n) {
    double res = 0;
    for (size_t i = 0; i <= n; ++i)
        res += y[i] * q(_x, x, i);
    return res;
}

int main() {
    size_t n;
    cout << "Enter n: ";
    cin >> n;
    vector_t _x(n + 1), y(n + 1);
    for (size_t i = 0; i < n + 1; ++i) {
        cout << "Enter x" << i << ": ";
        cin >> _x[i];
        cout << "Enter y" << i << ": ";
        cin >> y[i];
    }
    double x;
    cout << "Enter x: ";
    cin >> x;
    cout << "Result: " << p(_x, y, x, n);
    return 0;
}