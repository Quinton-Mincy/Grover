#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <bitset>
#include <math.h>
#include <complex>
#include <fstream>

//m(col,row)
#define PI 3.1415926535
// Print out matrix with each value separated by a tab space
void ppComplexMatrix(Eigen::MatrixXcd m) {
    int rows = m.rows();
    int cols = m.cols();

    for (size_t j = 0; j < cols; j++) {
        for (size_t i = 0; i < rows; i++) {
            auto el = m(j, i);
            auto real = el.real();
            auto imag = el.imag();
            
            if (real != 0 && imag != 0) {
                if (imag > 0) std::cout << real << '+' << imag << "i\t";
                else std::cout << real << imag << "i\t";
            }
            
            else if (real != 0) std::cout << real << "\t";
            else if (imag == 0) std::cout << "0\t";
            else std::cout << imag << "i\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}

// Use QFT formula to generate omega and the full QFT Matrix
Eigen::MatrixXcd generateQftMatrix(int m, int n) {
    Eigen::MatrixXcd U_qft(m, n);
    double th = (2 * PI) / m;
    std::complex<double>w(cos(th), sin(th));//e^(i*theta)
    for (size_t j = 0; j < n; j++) {//columns
        for (size_t i = 0; i < m; i++) {//rows
            auto ij = std::pow(w, i * j) / sqrt(m);//i^(col*row)
            U_qft(i, j) = ij;
        }
    }
    
    return U_qft;
}

// Generate Uquery based on the target bit
Eigen::MatrixXd generateQueryMatrix(int m, int n, int target) {
    Eigen::MatrixXd U_query = Eigen::MatrixXd::Identity(m, n);
    U_query(target, target) = -1.0;
    return U_query;
}

// Generate U0 based on the input bit
Eigen::MatrixXd generateFlipMatrix(int m, int n, int input) {
    Eigen::MatrixXd U_0 = -1.0 * Eigen::MatrixXd::Identity(m, n);
    U_0(input, input) = 1.0;
    return U_0;
}

// Do Grover's Algorithm for numIters number of iterations, returning back the amplitude values
Eigen::MatrixXcd grover(int m, int input, int target, int numIters) {
    Eigen::VectorXd d0 = Eigen::VectorXd::Unit(m, input);

    Eigen::MatrixXcd U_qft = generateQftMatrix(m, m);
    Eigen::MatrixXcd U_qfti = U_qft.inverse();
    
    // debug for checking matrix contents
    //ppComplexMatrix(U_qft);
    //ppComplexMatrix(U_qfti);

    Eigen::MatrixXd U_query = generateQueryMatrix(m, m, target);
    Eigen::MatrixXd U_0 = generateFlipMatrix(m, m, input);

    Eigen::VectorXcd acc = U_qft * d0;
    Eigen::MatrixXcd groverOperator = U_qfti * U_0 * U_qft * U_query;

    // Handle 2 qubit situation
    if (m == 4) {
        acc = groverOperator * acc;
    }
    else {
        // otherwise, Iterate numIters number of times
        for (size_t i = 0; i < numIters; i++) {
            acc = groverOperator * acc;
        }
    }

    return acc;
}

// Calculate probability values from amplitude results
Eigen::VectorXd probabilityValues(Eigen::VectorXcd results) {
    /*Should it be the amplitude squared or just the real part squared?*/
    //Eigen::VectorXd ret = results.real();
    //for (size_t i = 0; i < ret.size(); i++) {
    //    ret(i) = ret(i) * ret(i);
    //}
    //return ret;
    Eigen::VectorXd ret(results.size());

    for (int i = 0; i < results.size(); i++) {
        std::complex<double> amp = results(i);
        ret(i) = std::norm(amp);
    }

    return ret;
}

// Function for gathering probability values of the target bit after each iteration
//    of Grover's operator usage
std::vector<double> groverStatistics(int numQubits, int input, int target, int numIters) {
    size_t  m = std::pow(2, numQubits);

    std::vector<double> ret;
    Eigen::VectorXd d0 = Eigen::VectorXd::Unit(m, input);

    Eigen::MatrixXcd U_qft = generateQftMatrix(m, m);
    Eigen::MatrixXcd U_qfti = U_qft.inverse();

    Eigen::MatrixXd U_query = generateQueryMatrix(m, m, target);
    Eigen::MatrixXd U_0 = generateFlipMatrix(m, m, input);

    // Append first 'balanced' probability at time step 0
    Eigen::VectorXcd acc = U_qft * d0;
    Eigen::VectorXd prob = probabilityValues(acc);
    ret.push_back(prob[target]);

    Eigen::MatrixXcd groverOperator = U_qfti * U_0 * U_qft * U_query;


    std::ofstream outFile("data.txt");
    outFile << numQubits << " " << input << " " << target << " " << numIters << "\n";
      
    outFile << 0 << " " << prob[target] << "\n";
    // Iterate numIters number of times, appending probabilites in-between
    for (size_t i = 0; i < numIters; i++) {
        acc = groverOperator * acc;
        prob = probabilityValues(acc);
        ret.push_back(prob[target]);

        outFile << i+1 << " " << prob[target] << "\n";

    }
    outFile.close();

    return ret;
}

int main() {
    // Number of Qubits
    int numQubits;

    // Input bit and target bit
    int input;
    int target;

    // Variable for number of grover iterations for better control
   // int numIters = std::floor(sqrt(std::pow(2, numQubits)));
   // 
   // Eigen::MatrixXcd result = grover(std::pow(2, numQubits), input, target, numIters);

   // // Turn result amplitude coefficients into probability values
   // Eigen::VectorXd probs = probabilityValues(result);
   // Eigen::VectorXd coefs = result.real();

   // std::cout << "Final Coefficients:" << std::endl;
   // for (int i = 0; i < coefs.size(); i++) {
   //     std::cout << coefs[i] <<"\t|" << std::bitset<numQubits>(i).to_string() << ">" << std::endl;
   // }

   // std::cout << std::endl << "Probability of each Result:" << std::endl;
   // for (int i = 0; i < probs.size(); i++) {
   //     std::cout << "Bit " << i << ": " << probs[i] * 100 << "%" << std::endl;
   // }
    

    std::cout << "Number of Qubits: ";
    std::cin >> numQubits;

    int N = std::pow(2, numQubits);

    std::cout << "Input State: (0-"<<N-1<<"): ";
    std::cin >> input;

    if(input<0 || input > N-1){
        std::cout << "Invalid input state provided. Exiting program.\n";
        return 1;
    }

    std::cout << "Target State: (0-"<<N-1<<"): ";
    std::cin >> target;

    if(target<0 || target > N-1){
        std::cout << "Invalid target state provided. Exiting program.\n";
        return 1;
    }

    int minIters = std::floor(sqrt(std::pow(2, numQubits)));
    int testingIters;

    std::cout << "Enter number of iterations (minimum  " << minIters << "): ";
    std::cin >> testingIters;

    if(testingIters<=0){
        std::cout << "Iterations must be a positive, non-zero value.\n";
        return 1;
    }


    // Code for running statistics generation in order to create tables for Part 3

    std::vector<double> probs = groverStatistics(numQubits, input, target, testingIters);

    //for (int i = 0; i < probs.size(); i++) {
    //    std::cout << probs[i] << std::endl;
    //}
   
    
    return 0;
}
