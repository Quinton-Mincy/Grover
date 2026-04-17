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
            auto el = m(j,i);
            auto real = el.real();
            auto imag = el.imag();
            //untested...
            if(real != 0 && imag != 0){
                if(imag > 0) std::cout<< real << '+' << imag<<"i ";
                else std::cout<< real << imag<<"i ";
            }
            //untested...
            //TODO: Padding not correct
            else if(real != 0) std::cout<<real<< " ";
            else if(imag == 0) std::cout<< "0 ";
            else std::cout<<imag<< "i ";
       }
       std::cout<<"\n";
    }
   std::cout<<"\n\n";
}



Eigen::MatrixXcd generateQftMatrix(int m, int n){
    Eigen::MatrixXcd U_qft(m,n);
    double th = (2*M_PI)/m;
    std::complex<double>w(cos(th),sin(th));//e^(i*theta)
    for(size_t j = 0; j < n; j++){//columns
        for(size_t i = 0; i < m; i++){//rows
            auto ij = std::pow(w,i*j)/sqrt(m);//i^(col*row)
            //round off floating point errors
            if(fabs(ij.real()) < 1e-15) ij.real(0);
            if(fabs(ij.imag()) < 1e-15) ij.imag(0);
            U_qft(i,j) = ij;
        }
    } 
    //int N = std::pow(2,m);
    return U_qft; 

}

Eigen::MatrixXd generateQueryMatrix(int m, int n, int target){
    Eigen::MatrixXd U_query = Eigen::MatrixXd::Identity(m, n);
    U_query(target,target) = -1.0;
    return U_query;
}


Eigen::MatrixXd generateFlipMatrix(int m, int n, int input){
    Eigen::MatrixXd U_0 = -1.0 * Eigen::MatrixXd::Identity(m, n);
    U_0(input,input) = 1.0;
    return U_0;
}

Eigen::MatrixXcd grover(int m, int n, int input, int target){
    Eigen::VectorXd d0 = Eigen::VectorXd::Unit(m, input);

    Eigen::MatrixXcd U_qft  = generateQftMatrix(m,n);
    Eigen::MatrixXcd U_qfti = U_qft.inverse();

    Eigen::MatrixXd U_query = generateQueryMatrix(m,n,target);
    Eigen::MatrixXd U_0 = generateFlipMatrix(m,n,input);

    Eigen::VectorXcd acc = U_qft*d0;
    Eigen::MatrixXcd groverOperator = U_qfti*U_0*U_qft*U_query;

    int iter = sqrt(m);
    for(size_t i = 0; i < iter-1; i++){
        acc = groverOperator*acc;    
    }
    return acc;
}

int main() {
    int m = 8;//rows,
    int n = 8;//cols
    int target = 7;
    int input = 0;
  
    Eigen::MatrixXcd result = grover(m,n,input,target); 
    std::cout<<result.real()<<"\n";
   
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


    */

    return 0;
}

