#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <omp.h>


std::unordered_map<std::string, std::string> codon_to_aminoacid = {
    {"CCA", "Prolina"}, {"CCG", "Prolina"}, {"CCU", "Prolina"}, {"CCC", "Prolina"},
    {"UCU", "Serina"}, {"UCC", "Serina"}, {"UCA", "Serina"}, {"UCG", "Serina"},
    {"CAG", "Glutamina"},
    {"ACA", "Treonina"}, {"ACC", "Treonina"}, {"ACU", "Treonina"}, {"ACG", "Treonina"},
    {"AUG", "Metionina"}, 
    {"UGA", "Stop"}, {"UAA", "Stop"}, {"UAG", "Stop"}, 
    {"GUG", "Valina"}, {"GUA", "Valina"}, {"GUU", "Valina"}, {"GUC", "Valina"}
};

void rna_pra_proteina(const std::string &input_filename, const std::string &output_filename) {
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
            continue;
        }

        std::vector<std::string> sequencia_proteina;
        bool stop_codon_reached = false;

      
        #pragma omp parallel for shared(sequencia_proteina, stop_codon_reached)
        for (size_t i = 0; i <= line.size() - 3; i += 3) {
            if (stop_codon_reached) continue; 

            std::string codon = line.substr(i, 3);
            std::string aminoacido = codon_to_aminoacid[codon];

            if (aminoacido == "Stop") {
                #pragma omp critical
                stop_codon_reached = true;
                continue;
            }

            #pragma omp critical
            {
                sequencia_proteina.push_back(aminoacido);
            }
        }

        for (const std::string &aminoacido : sequencia_proteina) {
            outfile << aminoacido << " ";
        }
        outfile << "\n";
    }

    infile.close();
    outfile.close();
    std::cout << "Tradução concluída e salva em: " << output_filename << std::endl;
}

int main() {
    std::string folder_path = "dna_files/";

    std::vector<std::string> filenames;
    for (int i = 1; i <= 22; ++i) {
        filenames.push_back(folder_path + "chr" + std::to_string(i) + ".subst.fa_rna.fa");
    }

    for (const std::string &input_filename : filenames) {
        std::string output_filename = input_filename + "_protein.fa";
        std::cout << "Traduzindo arquivo: " << input_filename << std::endl;
        rna_pra_proteina(input_filename, output_filename);
    }

    return 0;
}
