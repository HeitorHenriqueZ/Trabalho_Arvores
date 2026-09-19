from pathlib import Path
import re

import matplotlib.pyplot as plt


ID = r'"[^"]+"|[A-Za-z_][A-Za-z0-9_]*'


def clean_id(value):
    return value[1:-1] if value.startswith('"') else value


def parse_dot(path):
    nodes = {}
    edges = []
    node_re = re.compile(rf'^\s*({ID})\s+\[(.+)\];\s*$')
    edge_re = re.compile(rf'^\s*({ID})\s*->\s*({ID})\s*;\s*$')
    for line in path.read_text(encoding='utf-8').splitlines():
        edge = edge_re.match(line)
        if edge:
            edges.append((clean_id(edge.group(1)), clean_id(edge.group(2))))
            continue
        node = node_re.match(line)
        if not node or node.group(1) == 'node':
            continue
        node_id = clean_id(node.group(1))
        attrs = node.group(2)
        label_match = re.search(r'label="((?:\\.|[^"])*)"', attrs)
        label = label_match.group(1) if label_match else ''
        label = label.replace('\\n', '\n').replace('\\"', '"').replace('\\\\', '\\')
        shape_match = re.search(r'shape=([A-Za-z]+)', attrs)
        nodes[node_id] = {
            'label': label,
            'shape': shape_match.group(1) if shape_match else 'circle'
        }
    return nodes, edges


def layout(nodes, edges):
    children = {node: [] for node in nodes}
    child_nodes = set()
    for parent, child in edges:
        children.setdefault(parent, []).append(child)
        children.setdefault(child, [])
        child_nodes.add(child)
    roots = [node for node in children if node not in child_nodes]
    root = roots[0]
    positions = {}
    leaf_index = 0

    def visit(node, depth):
        nonlocal leaf_index
        visible = [child for child in children[node] if child in nodes]
        if not visible:
            positions[node] = (leaf_index, -depth)
            leaf_index += 1
            return positions[node][0]
        xs = [visit(child, depth + 1) for child in visible]
        positions[node] = (sum(xs) / len(xs), -depth)
        return positions[node][0]

    visit(root, 0)
    return positions


def render(path):
    nodes, edges = parse_dot(path)
    positions = layout(nodes, edges)
    max_depth = max(-y for _, y in positions.values())
    width = max(4.0, min(10.0, len([n for n in nodes if nodes[n]['shape'] == 'point']) * 0.5 + 3.0))
    height = max(2.4, max_depth * 0.8 + 1.1)
    fig, ax = plt.subplots(figsize=(width, height))
    for parent, child in edges:
        if parent in positions and child in positions:
            x1, y1 = positions[parent]
            x2, y2 = positions[child]
            ax.plot([x1, x2], [y1, y2], color='#374151', linewidth=1.6, zorder=1)
    for node, (x, y) in positions.items():
        data = nodes[node]
        if data['shape'] == 'point':
            ax.scatter([x], [y], s=18, color='#111827', zorder=3)
            continue
        terminal = data['shape'] == 'doublecircle'
        boxstyle = 'round,pad=0.28' if data['shape'] == 'box' else 'circle,pad=0.25'
        ax.text(
            x, y, data['label'], ha='center', va='center', fontsize=14,
            fontweight='bold' if terminal else 'normal', zorder=3,
            bbox=dict(
                boxstyle=boxstyle,
                facecolor='#e5e7eb' if terminal else 'white',
                edgecolor='#111827', linewidth=2.4 if terminal else 1.6
            )
        )
    ax.set_axis_off()
    ax.margins(0.15)
    fig.subplots_adjust(left=0.03, right=0.97, bottom=0.03, top=0.97)
    output = path.parent.parent / path.stem
    fig.savefig(output.with_suffix('.pdf'), bbox_inches='tight')
    fig.savefig(output.with_suffix('.png'), dpi=180, bbox_inches='tight')
    plt.close(fig)


def main():
    dot_dir = Path('docs/diagramas/dot')
    for path in sorted(dot_dir.glob('*.dot')):
        render(path)


if __name__ == '__main__':
    main()
