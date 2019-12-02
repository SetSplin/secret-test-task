#pragma once

class Complex {
private:
    int real_;
    int imaginary_;

public:
    Complex(int real, int imaginary) : real_(real), imaginary_(imaginary) {
    }

    Complex() : Complex(0, 0) {
    }

    int Real() const {
        return real_;
    }

    int Imaginary() const {
        return imaginary_;
    }
};
