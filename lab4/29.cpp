#include <iostream>
#include <fstream>
#include <vector>

int main() {
    int N; // Количество узлов
    double t_end, L, lamda, ro, c, T0, Tl, Tr; // Параметры задачи
    
    // Ввод параметров с клавиатуры
    std::cout << "Введите количество пространственных узлов, N: ";
    std::cin >> N;
    std::cout << "Введите окончание по времени, t_end: ";
    std::cin >> t_end;
    std::cout << "Введите толщину пластины, L: ";
    std::cin >> L;
    std::cout << "Введите коэффициент теплопроводности материала пластины, lamda: ";
    std::cin >> lamda;
    std::cout << "Введите плотность материала пластины, ro: ";
    std::cin >> ro;
    std::cout << "Введите теплоемкость материала пластины, c: ";
    std::cin >> c;
    std::cout << "Введите начальную температуру, T0: ";
    std::cin >> T0;
    std::cout << "Введите температуру на границе x=0, Tl: ";
    std::cin >> Tl;
    std::cout << "Введите температуру на границе x=L, Tr: ";
    std::cin >> Tr;

    // Параметры сетки
    double h = L / (N - 1); // Шаг по пространству
    double tau = t_end / 100.0; // Шаг по времени

    // Инициализация массивов температуры и прогоночных коэффициентов
    std::vector<double> T(N, T0), alfa(N, 0.0), beta(N, 0.0);
    double ai, bi, ci, fi;
    double time = 0.0; // Временная переменная

    // Итерации по времени
    while (time < t_end) {
        time += tau;

        // Определение начальных прогоночных коэффициентов
        alfa[0] = 0.0;
        beta[0] = Tl;

        // Прямой ход прогонки
        for (int i = 1; i < N - 1; ++i) {
            ai = lamda / (h * h);
            bi = 2.0 * lamda / (h * h) + ro * c / tau;
            ci = lamda / (h * h);
            fi = -ro * c * T[i] / tau;

            alfa[i] = ai / (bi - ci * alfa[i - 1]);
            beta[i] = (ci * beta[i - 1] - fi) / (bi - ci * alfa[i - 1]);
        }

        // Учет правого граничного условия
        T[N - 1] = Tr;

        // Обратный ход прогонки
        for (int i = N - 2; i >= 0; --i) {
            T[i] = alfa[i] * T[i + 1] + beta[i];
        }
    }

    // Запись результатов в файлы
    std::ofstream res("res.txt");
    res << "Толщина пластины L = " << L << "\n"
        << "Число узлов по координате N = " << N << "\n"
        << "Коэффициент теплопроводности lamda = " << lamda << "\n"
        << "Плотность ro = " << ro << "\n"
        << "Теплоемкость c = " << c << "\n"
        << "Начальная температура T0 = " << T0 << "\n"
        << "Температура на границе x = 0, Tl = " << Tl << "\n"
        << "Температура на границе x = L, Tr = " << Tr << "\n"
        << "Шаг по координате h = " << h << "\n"
        << "Шаг по времени tau = " << tau << "\n"
        << "Температурное поле в момент времени t = " << t_end << "\n";
    res.close();

    std::ofstream tempr("tempr.txt");
    for (int i = 0; i < N; ++i) {
        tempr << h * i << " " << T[i] << "\n";
    }
    tempr.close();

    return 0;
}