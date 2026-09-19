import matplotlib.pyplot as plt
import numpy as np
import os
import csv

# Configuração de estilo acadêmico
plt.rcParams.update({
    'font.family': 'sans-serif',
    'font.sans-serif': ['DejaVu Sans', 'Arial', 'Helvetica'],
    'font.size': 11,
    'axes.titlesize': 13,
    'axes.titleweight': 'bold',
    'axes.labelsize': 11,
    'axes.labelweight': 'bold',
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'legend.fontsize': 10,
    'figure.titlesize': 14,
    'figure.titleweight': 'bold',
    'figure.dpi': 300
})

COLORS = {
    'ins': '#2b5c8f',    # Azul acadêmico
    'busca': '#e76f51',  # Coral / Terracota
    'rem': '#2a9d8f'     # Verde azulado / Teal
}

PALETTE_STRUCTURES = {
    'Trie': '#e63946',
    'Patricia': '#1d3557',
    'Splay': '#f4a261',
    'Treap': '#2a9d8f',
    'KDTree': '#457b9d'
}

def read_all_results(filepath):
    """Lê todos os dados de benchmark agrupados por N e por estrutura."""
    data = {}
    if not os.path.exists(filepath):
        print(f"Erro: Arquivo {filepath} não encontrado!")
        return data
        
    with open(filepath, 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            n = int(row['N'])
            struct = row['Estrutura']
            if n not in data:
                data[n] = {}
            data[n][struct] = {
                'ins': int(row['Insercao_ms']),
                'busca': int(row['Busca_ms']),
                'rem': int(row['Remocao_ms'])
            }
    return data

def plot_bar_chart(title, labels, time_insert, time_search, time_remove, filename, output_dir):
    """Gera gráfico de barras agrupadas com acabamento acadêmico refinado."""
    x = np.arange(len(labels))
    width = 0.25

    fig, ax = plt.subplots(figsize=(8, 5.5), dpi=300)
    
    # Grid sutil no eixo Y
    ax.yaxis.grid(True, linestyle='--', alpha=0.5, color='#b0b0b0', zorder=0)
    ax.set_axisbelow(True)

    rects1 = ax.bar(x - width, time_insert, width, label='Inserção', color=COLORS['ins'], edgecolor='#1a3a5a', linewidth=0.8, zorder=3)
    rects2 = ax.bar(x, time_search, width, label='Busca', color=COLORS['busca'], edgecolor='#a24832', linewidth=0.8, zorder=3)
    rects3 = ax.bar(x + width, time_remove, width, label='Remoção', color=COLORS['rem'], edgecolor='#1a6259', linewidth=0.8, zorder=3)

    ax.set_ylabel('Tempo de Execução (ms)')
    ax.set_title(title, pad=15)
    ax.set_xticks(x)
    ax.set_xticklabels(labels, fontweight='bold')
    
    # Remover bordas superior e direita
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_color('#888888')
    ax.spines['bottom'].set_color('#888888')

    # Ajustar margem superior para acomodar os labels e a legenda
    max_val = max(max(time_insert), max(time_search), max(time_remove))
    ax.set_ylim(0, max(max_val * 1.30, 10))

    # Labels de valores sobre as barras com formatação
    def add_labels(rects):
        for rect in rects:
            height = rect.get_height()
            label_text = f"{height} ms"
            ax.annotate(label_text,
                        xy=(rect.get_x() + rect.get_width() / 2, height),
                        xytext=(0, 5),
                        textcoords="offset points",
                        ha='center', va='bottom',
                        fontsize=9.5, fontweight='bold', color='#222222')

    add_labels(rects1)
    add_labels(rects2)
    add_labels(rects3)

    # Determina melhor posição da legenda para evitar sobreposição
    # Se o valor mais alto estiver à direita (remoção), coloca legenda na esquerda
    if time_remove[-1] == max_val and len(labels) == 1:
        legend_loc = 'upper left'
    else:
        legend_loc = 'upper right'

    ax.legend(frameon=True, facecolor='white', edgecolor='#cccccc', framealpha=0.9, loc=legend_loc)

    fig.tight_layout()
    output_path = os.path.join(output_dir, filename)
    plt.savefig(output_path, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"[OK] Gráfico salvo em: {output_path}")

def plot_scalability_chart(all_data, filename, output_dir):
    """Gera gráfico de curvas de escalabilidade temporal com N nos eixos logarítmicos."""
    ns = sorted(all_data.keys())
    structures = ['Trie', 'Patricia', 'Splay', 'Treap', 'KDTree']

    fig, axes = plt.subplots(1, 3, figsize=(15, 5), dpi=300, sharey=False)
    operations = [('ins', 'Inserção'), ('busca', 'Busca'), ('rem', 'Remoção')]
    markers = {'Trie': 'o', 'Patricia': 's', 'Splay': '^', 'Treap': 'D', 'KDTree': 'v'}

    for idx, (op_key, op_name) in enumerate(operations):
        ax = axes[idx]
        ax.grid(True, linestyle='--', alpha=0.5, color='#b0b0b0', zorder=0)
        ax.set_axisbelow(True)

        for struct in structures:
            times = []
            valid_ns = []
            for n in ns:
                if struct in all_data[n]:
                    value = all_data[n][struct][op_key]
                    times.append(0.5 if value == 0 else value)
                    valid_ns.append(n)
            
            ax.plot(valid_ns, times, marker=markers.get(struct, 'o'), markersize=6,
                    label=struct, color=PALETTE_STRUCTURES.get(struct, '#333333'),
                    linewidth=2, zorder=3)

        ax.set_title(f'Tempo de {op_name} vs. N', pad=12)
        ax.set_xlabel('Quantidade de Elementos (N)')
        ax.set_ylabel('Tempo (ms)')
        ax.set_xscale('log')
        ax.set_yscale('log')
        ax.set_xticks(ns)
        ax.set_xticklabels([f'{n//1000}k' if n < 1000000 else f'{n//1000000}M' for n in ns])
        
        ax.spines['top'].set_visible(False)
        ax.spines['right'].set_visible(False)
        ax.spines['left'].set_color('#888888')
        ax.spines['bottom'].set_color('#888888')

        if idx == 0:
            ax.legend(frameon=True, facecolor='white', edgecolor='#cccccc', framealpha=0.9, loc='upper left')

    fig.suptitle('Escalabilidade Temporal das Estruturas de Dados ($N = 10^4, 10^5, 5 \\times 10^5$)', fontsize=14, fontweight='bold', y=1.02)
    fig.tight_layout()
    output_path = os.path.join(output_dir, filename)
    plt.savefig(output_path, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"[OK] Gráfico de escalabilidade salvo em: {output_path}")

if __name__ == '__main__':
    base_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
    csv_path = os.path.join(base_dir, 'data', 'benchmark_results.csv')
    graficos_dir = os.path.join(base_dir, 'docs', 'graficos')
    os.makedirs(graficos_dir, exist_ok=True)

    print("Carregando resultados de benchmark...")
    all_data = read_all_results(csv_path)

    if not all_data:
        print("Erro: Nenhum dado encontrado. Execute o binário de benchmark primeiro.")
        exit(1)

    target_n = 500000
    if target_n not in all_data:
        # Pega o maior N disponível se 500000 não estiver presente
        target_n = max(all_data.keys())

    res = all_data[target_n]
    target_label = f'{target_n:,}'.replace(',', '.')
    print(f"Gerando gráficos acadêmicos para N = {target_label}...")

    # Gráfico 1: Trie vs Patricia (Strings)
    if 'Trie' in res and 'Patricia' in res:
        labels = ['Trie', 'Patricia']
        t_ins = [res['Trie']['ins'], res['Patricia']['ins']]
        t_bus = [res['Trie']['busca'], res['Patricia']['busca']]
        t_rem = [res['Trie']['rem'], res['Patricia']['rem']]
        plot_bar_chart(f'Desempenho: Trie vs. Patricia (Strings, N = {target_label})',
                       labels, t_ins, t_bus, t_rem, 'benchmark_strings.png', graficos_dir)

    # Gráfico 2: Splay vs Treap (Inteiros)
    if 'Splay' in res and 'Treap' in res:
        labels = ['Splay', 'Treap']
        t_ins = [res['Splay']['ins'], res['Treap']['ins']]
        t_bus = [res['Splay']['busca'], res['Treap']['busca']]
        t_rem = [res['Splay']['rem'], res['Treap']['rem']]
        plot_bar_chart(f'Desempenho: Splay vs. Treap (Inteiros, N = {target_label})',
                       labels, t_ins, t_bus, t_rem, 'benchmark_inteiros.png', graficos_dir)

    # Gráfico 3: KDTree (Pontos 2D)
    if 'KDTree' in res:
        labels = ['KD-Tree']
        t_ins = [res['KDTree']['ins']]
        t_bus = [res['KDTree']['busca']]
        t_rem = [res['KDTree']['rem']]
        plot_bar_chart(f'Desempenho: KD-Tree (Pontos 2D, N = {target_label})',
                       labels, t_ins, t_bus, t_rem, 'benchmark_kdtree.png', graficos_dir)

    # Gráfico 4: Escalabilidade temporal global em função de N
    plot_scalability_chart(all_data, 'benchmark_escalabilidade.png', graficos_dir)

    print("Todos os gráficos foram gerados e salvos com sucesso em docs/graficos/")
