#include <iostream>
#include <vector> 
#include <complex> 
#include <string>
#include <iomanip>  // для std::setprecision и std::fixed

using Complex = std::complex<double>;

std::string toBinary(int x) 
{
    if (x == 0) return "0";

    std::string result;
    while (x > 0) 
    {
        result = char('0' + (x % 2)) + result;
        x /= 2;
    }
    return result;
}

void printState(const std::vector<Complex>& psi) 
{
    int N = psi.size();
    for (int x = 0; x < N; x++) 
    {
        std::cout << "|" << toBinary(x) << "> : "
            << std::fixed << std::setprecision(4)
            << psi[x].real();
        if (psi[x].imag() >= 0) std::cout << "+";
        std::cout << psi[x].imag() << "i\n";
    }
    std::cout << "\n";
}


std::vector<Complex> initEqualSuperposition() 
{
    int N = 4;                           // количество базисных состояний: 2^2 = 4, базисные состояния это: |00>, |01>, |10>, |11>
    std::vector<Complex> psi(N);         // вектор амплитуд длины 4
    double amp = 1.0 / std::sqrt(N);     // нормальная амплитуда 1/sqrt(4) = 0.5 нужно чтобы сумма всех вероятностей была равна 1 т.е. (0.5**2)*4=1
    for (int i = 0; i < N; i++) 
    {
        psi[i] = Complex(amp, 0);
    }
    return psi;
}


// Оракул для постоянной функции f(x) = 1 для всех x.
void oracleConstant(std::vector<Complex>& psi) 
{
    for (auto& amplitude : psi) 
    {
        amplitude = -amplitude;
    }
}

// Оракул для сбалансированной функции f(x0,x1) = x0 XOR x1.
void oracleBalanced(std::vector<Complex>& psi) 
{
    int N = psi.size();
    for (int x = 0; x < N; x++) // На нормальном квантовом компе тут цикла нет, так как x храниться в суперпозиции
    {
        int x0 = (x >> 1) & 1; // извлекаем старший бит
        int x1 = x & 1;        // извлекаем младший бит
        int fx = x0 ^ x1;      // ^ это XOR
        if (fx == 1) 
        {
            psi[x] = -psi[x];
        }
    }
}

int main() 
{    
    // psi — вектор амплитуд всей 2‑кубитной системы, psi это общее состояние квантовой системы:
    // psi = alfa00|00> + alfa01|01> + alfa10|10> + alfa11|11>
    // alfa это амплитуда, |alfa|^2 — вероятность получить этот результат.
    
    // Постоянная функция f(x)=1
    auto psi1 = initEqualSuperposition();
    std::cout << "Before Oracle:\n";
    printState(psi1);
    oracleConstant(psi1);
    std::cout << "After  Oracle (constant):\n";
    printState(psi1);

    // Сбалансированная функция f(x0,x1) = x0 XOR x1.
    auto psi2 = initEqualSuperposition();
    std::cout << "Before Oracle (balanced):\n";
    printState(psi2);
    oracleBalanced(psi2);
    std::cout << "After  Oracle (balanced):\n";
    printState(psi2);

    return 0;
}