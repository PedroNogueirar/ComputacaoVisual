# ComputacaoVisual

### Integrantes
1. Pedro Nogueira Ribeiro - 10324924
2. Rafael Santos Lourenço da Silva - 10403588
3. Thiago Siqueira - 10390176

### Processador de Imagens com Histograma
Programa em C que carrega uma imagem, converte para escala de cinza, exibe o histograma e permite equalizar a imagem.

### Dependências

GCC (MinGW no Windows)
SDL3
SDL3_image
SDL3_ttf

### Estrutura do Projeto
```text
ComputacaoVisual/
├── main.c
├── histograma.c
├── histograma.h
├── window.c
├── window.h
├── lib/
│   └── SDL3/
│       ├── include/
│       └── lib/
└── output/
    ├── main.exe
    ├── SDL3.dll
    ├── SDL3_image.dll
    └── SDL3_ttf.dll

```

### Compilação
gcc -Wall -g "ComputacaoVisual\main.c" "ComputacaoVisual\histograma.c" "ComputacaoVisual\window.c" -o "ComputacaoVisual\output\main.exe" -I "ComputacaoVisual\lib\SDL3\include" -L "ComputacaoVisual\lib\SDL3\lib" -lSDL3 -lSDL3_image -lSDL3_ttf

### Exemplo de Uso
.\output\main.exe caminho\da\imagem.png

### O que o programa faz
Ao iniciar, o programa abre duas janelas:
- Janela principal — exibe a imagem em escala de cinza. Se a imagem já estiver em cinza, é usada diretamente; caso contrário, é convertida usando a fórmula Y = 0.2125R + 0.7154G + 0.0721B.
- Janela secundária — exibe o histograma da imagem com as seguintes informações:

1. Média de intensidade: classifica a imagem como clara, média ou escura.
2. Desvio padrão: classifica o contraste como alto, médio ou baixo.
3. Botão para equalizar/reverter o histograma.

### Controles
- Ação Equalizar/Ver original: Botão na janela secundária.
- Ação Salvar imagem atual: Tecla S.
- Ação Fechar: Tecla ESC ou Q, ou fechar a janela.

A imagem é salva como output_image.png na pasta onde o programa foi executado. Se o arquivo já existir, é sobrescrito.

### Explicação do Código

**`main.c`**
Ponto de entrada do programa. Valida a extensão do arquivo recebido por argumento, carrega a imagem com SDL_image e converte para escala de cinza caso necessário. Cria a janela principal e a janela secundária, e gerencia o loop de eventos de teclado, mouse e fechamento de janelas. Ao pressionar `S`, salva a imagem atual como `output_image.png`.


**`histograma.c` / `histograma.h`**
Calcula a frequência de cada nível de cinza (0–255) da imagem, a média de intensidade classificando-a como *clara*, *média* ou *escura*, e o desvio padrão classificando o contraste como *alto*, *médio* ou *baixo*. A equalização é feita via tabela CDF/LUT para redistribuir os tons. Também é responsável por renderizar as barras do histograma e os textos informativos na janela secundária.


**`window.c` / `window.h`**
Cria a janela secundária e a posiciona automaticamente ao lado da janela principal. Desenha o botão com primitivas SDL, sem uso de imagens externas, e gerencia seus três estados visuais: azul para o estado normal, azul claro quando o mouse está sobre o botão e azul escuro quando pressionado. O texto do botão alterna entre *"Equalizar"* e *"Ver original"* conforme o estado atual.

### Observações

1. As DLLs do SDL3 precisam estar na mesma pasta que o main.exe.
2. A fonte usada é Arial (C:/Windows/Fonts/Arial.ttf); se não encontrada, o programa roda sem texto.
3. O tamanho da janela principal se adapta à imagem, com limite de 1280×720.
4. A janela secundária tem tamanho fixo de 420×520 e abre ao lado da janela principal.
