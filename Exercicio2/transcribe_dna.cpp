#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <omp.h>

void transcribe_dna_to_rna(const std::string &input_filename, const std::string &output_filename) {
    std::ifstream infile(input_filename);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << input_filename << std::endl;
        return;
    }

    std::ofstream outfile(output_filename);
    if (!outfile.is_open()) {
        std::cerr << "Erro ao criar o arquivo: " << output_filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '>') {
            outfile << line << "\n";
        } else {
            #pragma omp parallel for
            for (size_t i = 0; i < line.size(); ++i) {
                line[i] = toupper(line[i]);

                if (line[i] == 'T') {
                    line[i] = 'U';
                }
            }
            outfile << line << "\n";
        }
    }

    infile.close();
    outfile.close();
    std::cout << "Arquivo transcrito e salvo em: " << output_filename << std::endl;
}

int main() {
    std::string folder_path = "dna_files/";

    std::vector<std::string> filenames;
    for (int i = 1; i <= 22; ++i) {
        filenames.push_back(folder_path + "chr" + std::to_string(i) + ".subst.fa");
    }

    for (const std::string &input_filename : filenames) {
        std::string output_filename = input_filename + "_rna.fa";
        std::cout << "Transcrevendo arquivo: " << input_filename << std::endl;
        transcribe_dna_to_rna(input_filename, output_filename);
    }

    return 0;
}