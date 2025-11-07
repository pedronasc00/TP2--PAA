import random
import argparse
import os

def gerar_celula(prob_monstro, max_monstro):
    if random.random() < prob_monstro:
        forca = random.randint(1, max_monstro)
        return f"{forca:03d}"
    else:
        return "000"

def gerar_mapas(h, w, prob_muro, prob_ancora, prob_monstro, max_monstro):
    presente = [["" for _ in range(w)] for _ in range(h)]
    passado = [["" for _ in range(w)] for _ in range(h)]

    for i in range(h):
        for j in range(w):
            if random.random() < prob_muro:
                presente[i][j] = "***"
                passado[i][j] = "***"
            elif random.random() < prob_ancora:
                presente[i][j] = "AAA"
                passado[i][j] = "AAA"

    for i in range(h):
        for j in range(w):
            if presente[i][j] == "":
                presente[i][j] = gerar_celula(prob_monstro, max_monstro)
            
            if passado[i][j] == "":
                passado[i][j] = gerar_celula(prob_monstro, max_monstro)
                
    return presente, passado

def main():
    parser = argparse.ArgumentParser(description="Gerador de Mapas para o TP2 de PAA")
    
    parser.add_argument("-H", "--altura", type=int, default=10, help="Altura 'h' do mapa.")
    parser.add_argument("-W", "--largura", type=int, default=15, help="Largura 'w' do mapa.")
    parser.add_argument("-F", "--forca", type=int, default=100, help="Força 'F' inicial da tripulação.")
    parser.add_argument("-D", "--descanso", type=int, default=5, help="Recuperação 'D' no descanso.")
    parser.add_argument("-N", "--nikador", type=int, default=50, help="Força 'N' de Nikador.")
    
    parser.add_argument("-pm", "--prob-muro", type=float, default=0.2, help="Probabilidade de uma célula ser '***'. (0.0 a 1.0)")
    parser.add_argument("-pa", "--prob-ancora", type=float, default=0.1, help="Probabilidade de uma célula (não-muro) ser 'AAA'. (0.0 a 1.0)")
    parser.add_argument("-po", "--prob-monstro", type=float, default=0.5, help="Probabilidade de uma célula (não-muro/âncora) ser um monstro. (0.0 a 1.0)")
    parser.add_argument("-mm", "--max-monstro", type=int, default=15, help="Força máxima de um monstro (001 a X).")

    parser.add_argument("-o", "--output", type=str, default="mapas/mapa_gerado2.txt", help="Nome do arquivo de saída.")

    args = parser.parse_args()

    presente, passado = gerar_mapas(
        args.altura, args.largura, 
        args.prob_muro, args.prob_ancora, 
        args.prob_monstro, args.max_monstro
    )

    try:
        output_dir = os.path.dirname(args.output)
        if output_dir:
            os.makedirs(output_dir, exist_ok=True)

        with open(args.output, "w") as f:
            f.write(f"{args.altura} {args.largura} {args.forca} {args.descanso} {args.nikador}\n")
            
            for linha in presente:
                f.write(" ".join(linha) + "\n")
                
            f.write("///\n")
            
            for linha in passado:
                f.write(" ".join(linha) + "\n")
        
        print(f"Mapa gerado com sucesso e salvo em '{args.output}'")
        print(f"Parâmetros: h={args.altura}, w={args.largura}, F={args.forca}, D={args.descanso}, N={args.nikador}")

    except IOError as e:
        print(f"Erro ao escrever o arquivo: {e}")

if __name__ == "__main__":
    main()