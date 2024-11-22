#!/bin/bash
#SBATCH --job-name=count_proteins                # Nome do job
#SBATCH --output=count_proteins_%j.txt          # Nome do arquivo de saída (%j será substituído pelo ID do job)
#SBATCH --error=count_proteins_%j.err           # Nome do arquivo de erro (%j será substituído pelo ID do job)
#SBATCH --ntasks=1                              # Número de tarefas (1 processo)
#SBATCH --cpus-per-task=4                       # Número de threads por tarefa (4 threads OpenMP)
#SBATCH --mem=4G                                # Memória total alocada por nó (4 GB)
#SBATCH --time=00:15:00                         # Tempo máximo de execução (15 minutos)
#SBATCH --partition=normal                      # Fila do cluster a ser utilizada

# Exporta a variável de ambiente para configurar o número de threads OpenMP
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Executa o programa
echo "Iniciando contagem de proteínas iniciadas pelos códons AUG..."
srun ./count_proteins
