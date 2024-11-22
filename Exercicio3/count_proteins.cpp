#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <omp.h>

size_t conta_AUG(const std::string &filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return 0;
    }

    size_t aug_count = 0; 
    std::string line;

    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '>') continue; 

        #pragma omp parallel for reduction(+:aug_count)
        for (size_t i = 0; i <= line.size() - 3; ++i) {
            if (line.substr(i, 3) == "AUG") {
                aug_count++;
            }
        }
    }

    infile.close();
    return aug_count;
}

int main() {

    std::string folder_path = "dna_files/";

  
    std::vector<std::string> filenames;
    for (int i = 1; i <= 22; ++i) {
        filenames.push_back(folder_path + "chr" + std::to_string(i) + ".subst.fa_rna.fa");
    }


    size_t total_aug_count = 0;


    for (const std::string &filename : filenames) {
        std::cout << "Processando arquivo: " << filename << std::endl;
        size_t aug_count = conta_AUG(filename);
        std::cout << "Códons AUG encontrados em " << filename << ": " << aug_count << std::endl;
        total_aug_count += aug_count;
    }

    std::cout << "Número total de proteínas iniciadas (AUG) em todos os arquivos: " << total_aug_count << std::endl;

    return 0;
}
