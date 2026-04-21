#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <math.h>
#include <complex>

//m(col,row)

void ppComplexMatrix(Eigen::MatrixXcd m) {
    int rows = m.rows();
    int cols = m.cols();

    for (size_t j = 0; j < cols; j++) {
        for (size_t i = 0; i < rows; i++) {
            auto el = m(j, i);
            auto real = el.real();
            auto imag = el.imag();
            //untested...
            if (real != 0 && imag != 0) {
                if (imag > 0) std::cout << real << '+' << imag << "i\t";
                else std::cout << real << imag << "i\t";
            }
            //untested...
            //TODO: Padding not correct
            else if (real != 0) std::cout << real << "\t";
            else if (imag == 0) std::cout << "0\t";
            else std::cout << imag << "i\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}



Eigen::MatrixXcd generateQftMatrix(int m, int n) {
    Eigen::MatrixXcd U_qft(m, n);
    double th = (2 * M_PI) / m;
    std::complex<double>w(cos(th), sin(th));//e^(i*theta)
    for (size_t j = 0; j < n; j++) {//columns
        for (size_t i = 0; i < m; i++) {//rows
            auto ij = std::pow(w, i * j) / sqrt(m);//i^(col*row)
            //round off floating point errors
            if (fabs(ij.real()) < 1e-15) ij.real(0);
            if (fabs(ij.imag()) < 1e-15) ij.imag(0);
            U_qft(i, j) = ij;
        }
    }
    //int N = std::pow(2,m);
    return U_qft;

}

Eigen::MatrixXd generateQueryMatrix(int m, int n, int target) {
    Eigen::MatrixXd U_query = Eigen::MatrixXd::Identity(m, n);
    U_query(target, target) = -1.0;
    return U_query;
}


Eigen::MatrixXd generateFlipMatrix(int m, int n, int input) {
    Eigen::MatrixXd U_0 = -1.0 * Eigen::MatrixXd::Identity(m, n);
    U_0(input, input) = 1.0;
    return U_0;
}

Eigen::MatrixXcd grover(int m, int input, int target, int numIters) {
    Eigen::VectorXd d0 = Eigen::VectorXd::Unit(m, input);

    Eigen::MatrixXcd U_qft = generateQftMatrix(m, m);
    Eigen::MatrixXcd U_qfti = U_qft.inverse();
    
    // debug for checking matrix contents
    // ppComplexMatrix(U_qft);

    Eigen::MatrixXd U_query = generateQueryMatrix(m, m, target);
    Eigen::MatrixXd U_0 = generateFlipMatrix(m, m, input);

    Eigen::VectorXcd acc = U_qft * d0;
    Eigen::MatrixXcd groverOperator = U_qfti * U_0 * U_qft * U_query;

    // Iterate numIters number of times
    for (size_t i = 1; i < numIters; i++) {
        acc = groverOperator * acc;
    }

    return acc;
}

Eigen::MatrixXd probabilityValues(Eigen::MatrixXcd results) {
    Eigen::VectorXd ret = results.real();
    for (size_t i = 0; i < ret.size(); i++) {
        ret(i) = ret(i) * ret(i);
    }
    return ret;
}

int main() {
    // Number of Qubits
    int N = 3;

    // Input bit and target bit
    int input = 0;
    int target = 7;

    // Variable for number of grover iterations for better control
    int numIters = std::floor(sqrt(std::pow(2, N)));

    Eigen::MatrixXcd result = grover(std::pow(2, N), input, target, numIters);

    // Turn result amplitude coefficients into probability values
    Eigen::VectorXd probs = probabilityValues(result);
  
    std::cout << "Final Coefficients:" << std::endl << result.real() << std::endl;
    std::cout << "Probability of each Result:" << std::endl << probs << std::endl;

    /*
    Not working as expected. Here is current output for 3 bit system:


    -0.0883883
    -0.0883883
    -0.0883883
    -0.0883883
    -0.0883883
    -0.0883883
    -0.0883883
      0.972272

    Do not think negative values are accepptable. May be somthing with amount of iterations.
    If you do sqrt(N) - 1 iterations, here is the output


    0.176777
    0.176777
    0.176777
    0.176777
    0.176777
    0.176777
    0.176777
    0.883883

    which also should not be acceptable.


    This actually is acceptable, as they're amplitude values and not probability values
    When you square it, you get the probability that the correlated bit will be the one that
       is found when measuring. When changing N to 2, and the target to a valid bit, it works
       perfectly. When changing N to 3 and choosing a valid bit, it also works properly, but
       because sqrt(2^3) == 2.82, it's not perfect after a specific number of iterations, so
       we choose to stop at the lower bound (according to Dr. Cahay), but if you run it one
       more time after that in this configuration then it acutally does better than at the
       lower bound. If you run it one more past that, though, then the probability of getting
       the correct element goes down again.

    */

    return 0;
}
