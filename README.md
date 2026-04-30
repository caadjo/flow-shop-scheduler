# Flow Shop Scheduler

Trabalho educacional da disciplina de Algoritmos em Grafos. O objetivo do projeto e avaliar e melhorar sequencias para o problema Flow Shop Permutacional (FSP), usando a funcao objetivo:

```text
objetivo = flowtime + makespan
```

## Ideia Do Modelo

Cada operacao de um job em uma maquina vira um vertice do grafo, com peso igual ao tempo de processamento daquela operacao. O grafo e direcionado e aciclico (DAG).

As arestas representam duas dependencias:

- Dentro de um mesmo job, a operacao na maquina seguinte so pode iniciar depois da maquina anterior.
- Em uma mesma maquina, os jobs seguem a ordem definida pela sequencia testada.

Depois que o DAG e montado, o programa calcula uma ordenacao topologica e usa caminho maximo no DAG para obter os tempos de conclusao. O makespan e o maior tempo de conclusao; o flowtime e a soma dos tempos de conclusao dos jobs.

Essa abordagem se relaciona diretamente com os topicos de DAG, ordenacao topologica e caminhos em DAG apresentados na apostila da disciplina.

## Heuristica

A solucao implementada e uma heuristica, nao um algoritmo exato. Ela nao garante otimo global.

O metodo atual parte da sequencia natural:

```text
1, 2, 3, ..., n
```

Em seguida aplica uma busca local por trocas adjacentes. A cada passo, o programa troca dois jobs vizinhos, reavalia a sequencia e aceita a troca apenas se houver melhora estrita em:

```text
flowtime + makespan
```

Para manter o tempo de execucao controlado, a busca faz no maximo 5 passadas pela sequencia.

## Como Compilar

```bash
make
```

Para limpar os arquivos gerados:

```bash
make clean
```

## Como Executar

Rodar todas as instancias em `data/instances`:

```bash
./build/bin/flowshop
```

Rodar uma instancia pelo nome:

```bash
./build/bin/flowshop ta001
```

Rodar uma instancia pelo caminho:

```bash
./build/bin/flowshop data/instances/ta001
```

Mostrar ajuda:

```bash
./build/bin/flowshop --help
```

## Saida

Cada linha mostra a comparacao entre a sequencia natural e a sequencia melhorada:

```text
Instance: ta001 | InitialObjective: 19734 | FinalObjective: 18097 | Improvement: 1637 | Makespan: 1366 | Flowtime: 16731 | Sequence: ...
```

Campos:

- `InitialObjective`: valor de `flowtime + makespan` para a sequencia natural.
- `FinalObjective`: valor apos a busca local.
- `Improvement`: reducao obtida pela heuristica.
- `Makespan`: makespan final.
- `Flowtime`: flowtime final.
- `Sequence`: sequencia final dos jobs, impressa com indices iniciando em 1.

## Limitacoes

Esta implementacao prioriza clareza e relacao com a disciplina. Ela remonta e reavalia o DAG a cada troca testada, o que e simples de entender, mas nao e a forma mais eficiente possivel.

Melhorias futuras possiveis:

- Testar trocas nao adjacentes.
- Implementar uma heuristica construtiva, como insercao gulosa.
- Comparar diferentes heuristicas em uma tabela de resultados.
- Salvar resultados em CSV para facilitar a analise.
