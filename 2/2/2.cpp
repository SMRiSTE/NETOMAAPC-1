#include<iostream>
#include<thread>
#include<vector>
#include<chrono>

void sumVec(std::vector<int> &vec1, std::vector <int> &vec2, std::vector<int> &result, int start, int end) {
    for (int i = start; i < end; ++i) {
        result[i] = vec1[i] + vec2[i];
    }
}

int main() {

    setlocale(LC_ALL, "Russian");


    int num_threads[5] = {1, 2, 4, 8, 16 };
    std::vector <int> num_elements = { 1000, 10000, 100000, 1000000 };

    std::cout << "Количество аппаратных ядер - " << std::thread::hardware_concurrency() << std::endl;

    for (int i = 0; i < num_elements.size(); ++i) {
        std::vector<int> vec1(num_elements[i], 1);
        std::vector<int> vec2(num_elements[i], 2);
        std::vector<int> result(num_elements[i]);

        for (int j = 0; j < 5; ++j) {
            auto start_time = std::chrono::high_resolution_clock::now();

            std::vector<std::thread> threads;
            for (int k = 0; k < num_threads[j]; ++k) {
                int start = k * (num_elements[i] / num_threads[j]);
                int end = (k + 1) * (num_elements[i] / num_threads[j]);
                threads.push_back(std::thread(sumVec, std::ref(vec1), std::ref(vec2), std::ref(result), start, end));
            }

            for (auto& thread : threads) {
                thread.join();
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end_time - start_time;

            std::cout << num_threads[j] << " поток, " << num_elements[i] << " элементов: " << elapsed.count() << " секунд" << std::endl;
        }
    }

    return 0;
}