# Non-Permutation Flow Shop Scheduler

Trabalho educacional da disciplina de Algoritmos em Grafos. O objetivo do projeto e avaliar e melhorar sequencias para o problema Flow Shop nao permutacional (FSP), usando a funcao objetivo:

```text
objetivo = flowtime + makespan
```

No FSP nao permutacional cada maquina pode ter sua propria permutacao dos jobs. Por isso, uma solucao do programa e um conjunto de sequencias: uma sequencia sem repeticao para cada maquina da instancia.

## Ideia Do Modelo

Cada operacao de um job em uma maquina vira um vertice do grafo, com peso igual ao tempo de processamento daquela operacao. O grafo e direcionado e aciclico (DAG).

As arestas representam duas dependencias:

- Dentro de um mesmo job, a operacao na maquina seguinte so pode iniciar depois da maquina anterior.
- Em uma mesma maquina, os jobs seguem a ordem definida pela permutacao especifica daquela maquina.

Depois que o DAG e montado, o programa calcula uma ordenacao topologica e usa caminho maximo no DAG para obter os tempos de conclusao. O makespan e o maior tempo de conclusao; o flowtime e a soma dos tempos de conclusao dos jobs.

Essa abordagem se relaciona diretamente com os topicos de DAG, ordenacao topologica e caminhos em DAG apresentados na apostila da disciplina.

## Heuristica

A solucao implementada e uma heuristica, nao um algoritmo exato. Ela nao garante otimo global.

O metodo atual parte da sequencia natural em todas as maquinas:

```text
M1: 1, 2, 3, ..., n
M2: 1, 2, 3, ..., n
...
Mm: 1, 2, 3, ..., n
```

Em seguida aplica uma busca local por trocas adjacentes em cada maquina. A cada passo, o programa troca dois jobs vizinhos em uma maquina, reavalia o DAG e aceita a troca apenas se:

- o grafo continuar aciclico;
- houver melhora estrita em:

```text
flowtime + makespan
```

Para manter o tempo de execucao controlado, a busca faz uma passada pelas sequencias.

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

Mostrar a caminhada topologica e o caminho maximo usados para uma instancia:

```bash
./build/bin/flowshop --details ta001
```

Rodar o teste do grafo fixo de 15 vertices do PDF:

```bash
./build/bin/flowshop --fixed-graph
```

Mostrar ajuda:

```bash
./build/bin/flowshop --help
```

## Saida

Cada linha mostra a comparacao entre as sequencias naturais e as sequencias melhoradas:

```text
Instance: ta001 | InitialObjective: 19734 | FinalObjective: 19544 | Improvement: 190 | Makespan: 1455 | Flowtime: 18089
```

Campos:

- `InitialObjective`: valor de `flowtime + makespan` para a sequencia natural.
- `FinalObjective`: valor apos a busca local.
- `Improvement`: reducao obtida pela heuristica.
- `Makespan`: makespan final.
- `Flowtime`: flowtime final.

Ao rodar uma unica instancia, a saida tambem inclui:

- `MachineSequences`: sequencias finais dos jobs em cada maquina, impressas com indices iniciando em 1.

No modo `--details`, o programa tambem imprime:

- `TopologicalOrder`: ordem em que os vertices do DAG sao percorridos.
- `LongestPathLength`: comprimento do caminho maximo.
- `LongestPath`: caminho maximo encontrado no DAG.

No modo `--fixed-graph`, o programa constroi o grafo fixo do enunciado, executa a ordenacao topologica e responde:

- caminho maximo de um elemento minimal para um elemento maximal;
- caminho maximo de um elemento minimal para cada elemento no final de cada linha.

## Limitacoes

Esta implementacao prioriza clareza e relacao com a disciplina. Ela remonta e reavalia o DAG a cada troca testada, o que e simples de entender, mas nao e a forma mais eficiente possivel. Como diferentes maquinas podem ter ordens diferentes, algumas trocas podem criar ciclos no grafo; nesses casos a troca e descartada.

Melhorias futuras possiveis:

- Testar trocas nao adjacentes.
- Implementar uma heuristica construtiva, como insercao gulosa.
- Comparar diferentes heuristicas em uma tabela de resultados.
- Salvar resultados em CSV para facilitar a analise.
