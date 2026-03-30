# ComputacaoVisual

### Processador de Imagens com Histograma
Programa em C que carrega uma imagem, converte para escala de cinza, exibe o histograma e permite equalizar a imagem.

### Dependências

GCC (MinGW no Windows)
SDL3
SDL3_image
SDL3_ttf

### Estrutura do Projeto
ComputacaoVisual/
|-- main.c
|-- histograma.c
|-- histograma.h
|-- window.c
|-- window.h
|-- lib/
|   `-- SDL3/
|       |-- include/
|       `-- lib/
`-- output/
    |-- main.exe
    |-- SDL3.dll
    |-- SDL3_image.dll
    `-- SDL3_ttf.dll

### Compilação
gcc -Wall -g main.c histograma.c window.c -o output\main.exe -I"lib\SDL3\include" -L"lib\SDL3\lib" -lSDL3 -lSDL3_image -lSDL3_ttf -lm

### Exemplo de Uso
.\output\main.exe caminho\da\imagem.png

### O que o programa faz
Ao iniciar, o programa abre duas janelas:
- Janela principal — exibe a imagem em escala de cinza. Se a imagem já estiver em cinza, é usada diretamente; caso contrário, é convertida usando a fórmula Y = 0.2125R + 0.7154G + 0.0721B.
- Janela secundária — exibe o histograma da imagem com as seguintes informações:

1. Média de intensidade: classifica a imagem como clara, média ou escura
2. Desvio padrão: classifica o contraste como alto, médio ou baixo
3. Botão para equalizar/reverter o histograma

### Controles
- Ação Equalizar/Ver original: Botão na janela secundária.
- Ação Salvar imagem atual: Tecla S.
- Ação Fechar: Tecla ESC ou Q, ou fechar a janela.

A imagem é salva como output_image.png na pasta onde o programa foi executado. Se o arquivo já existir, é sobrescrito.

### Observações

1. As DLLs do SDL3 precisam estar na mesma pasta que o main.exe
2. A fonte usada é Arial (C:/Windows/Fonts/Arial.ttf); se não encontrada, o programa roda sem texto
3. O tamanho da janela principal se adapta à imagem, com limite de 1280×720
4. A janela secundária tem tamanho fixo de 420×520 e abre ao lado da janela principal.
