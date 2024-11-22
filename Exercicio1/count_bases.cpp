#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <omp.h>
#include <unordered_map>

void base_paralela(const std::string &filename, std::unordered_map<char, size_t> &base_conta) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::string line;
    #pragma omp parallel
    {
        std::unordered_map<char, size_t> conta_local;
        conta_local['A'] = 0;
        conta_local['T'] = 0;
        conta_local['C'] = 0;
        conta_local['G'] = 0;

        #pragma omp for schedule(dynamic)
        for (int i = 0; i < 1; ++i) {
            while (std::getline(file, line)) {
                if (line.empty() || line[0] == '>') continue; 


                for (char base : line) {
                    base = toupper(base);
                    if (conta_local.find(base) != conta_local.end()) {
                        conta_local[base]++;
                    }
                }
            }
        }


        #pragma omp critical
        {
            for (const auto &pair : conta_local) {
                base_conta[pair.first] += pair.second;
            }
        }
    }
    file.close();
}

int main() {

    std::string folder_path = "dna_files/";


    std::vector<std::string> filenames;
    for (int i = 1; i <= 22; ++i) {
        filenames.push_back(folder_path + "chr" + std::to_string(i) + ".subst.fa");
    }

    std::unordered_map<char, size_t> g_base_conta;
    g_base_conta['A'] = 0;
    g_base_conta['T'] = 0;
    g_base_conta['C'] = 0;
    g_base_conta['G'] = 0;


    for (const std::string &filename : filenames) {
        std::cout << "Processando arquivo: " << filename << std::endl;

        std::unordered_map<char, size_t> local_base_counts;
        local_base_counts['A'] = 0;
        local_base_counts['T'] = 0;
        local_base_counts['C'] = 0;
        local_base_counts['G'] = 0;

        base_paralela(filename, local_base_counts);


        for (const auto &pair : local_base_counts) {
            g_base_conta[pair.first] += pair.second;
        }
    }

    std::cout << "Contagem total de bases em todos os arquivos:\n";
    for (const auto &pair : g_base_conta) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }

    return 0;
}
