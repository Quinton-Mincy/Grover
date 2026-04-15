#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <math.h>
#include <complex>

//m(col,row)

void ppComplexMatrix(Eigen::MatrixXcd m){
    int rows = m.rows();
    int cols = m.cols();

    for(size_t j = 0; j < cols; ++j){
        for(size_t i = 0; i < rows; i++){
            //all elements will be either purely real or imaginary.
            auto el = m(j,i);
            if(el.real() != 0) std::cout<<el.real()<< " ";
            else if(el.imag() == 0) std::cout<< "0 ";
            else std::cout<<el.imag()<< "i ";
       }
       std::cout<<"\n";
    }
   std::cout<<"\n\n";
}

Eigen::MatrixXcd generateQftMatrix(int n, int m){
    Eigen::MatrixXcd U_qft(n, m);
    double th = (2*M_PI)/m;
    std::complex<double>w(cos(th),sin(th));//e^(i*theta)
    for(size_t j = 0; j < n; j++){//columns
        for(size_t i = 0; i < m; i++){//rows
            auto ij = std::pow(w,i*j)/sqrt(m);//i^(col*row)
            //round off floating point errors
            if(fabs(ij.real()) < 1e-15) ij.real(0);
            if(fabs(ij.imag()) < 1e-15) ij.imag(0);
            U_qft(j,i) = ij;
        }
    } 
    //int N = std::pow(2,m);
    return U_qft; 

}

int main() {
    //works for 1 and 2 qubits at least... 
    Eigen::MatrixXcd U_qft  = generateQftMatrix(4,4);
    Eigen::MatrixXcd U_qfti = U_qft.inverse();

    ppComplexMatrix(U_qft);
    ppComplexMatrix(U_qfti);
    
    /*identity*/
    //ppComplexMatrix(U_qft*U_qfti);

    return 0;
}

