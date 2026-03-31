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

O que o programa faz

Ao ser executado, o programa abre duas janelas simultaneamente:
Janela principal — apresenta a imagem em tons de cinza. Caso a imagem já esteja em escala de cinza, ela é utilizada diretamente; do contrário, a conversão é realizada aplicando a fórmula Y = 0.2125R + 0.7154G + 0.0721B.
Janela secundária — exibe o histograma da imagem, contendo as seguintes informações:
Média de intensidade: indica se a imagem é clara, média ou escura.
Desvio padrão: indica se o contraste é alto, médio ou baixo.
Botão para equalizar o histograma ou retornar à versão original.

Controles
Equalizar/Ver original: Botão localizado na janela secundária.
Salvar imagem atual: Tecla S.
Encerrar: Tecla ESC ou Q, ou pelo fechamento da janela.
A imagem é gravada com o nome output_image.png no diretório em que o programa foi executado. Caso o arquivo já exista, será substituído.


Explicação do Código

main.c
Ponto de entrada do programa. Verifica a extensão do arquivo recebido como argumento, carrega a imagem via SDL_image e realiza a conversão para escala de cinza quando necessário. Instancia a janela principal e a janela secundária, e conduz o loop de eventos de teclado, mouse e fechamento de janelas. Ao acionar a tecla S, a imagem atual é salva como output_image.png.

histograma.c / histograma.h
Responsável por calcular a frequência de cada nível de cinza (0–255) da imagem, a média de intensidade — classificando-a como clara, média ou escura — e o desvio padrão — classificando o contraste como alto, médio ou baixo. A equalização é executada por meio de uma tabela CDF/LUT que redistribui os tons da imagem. Também cuida da renderização das barras do histograma e dos textos informativos na janela secundária.

window.c / window.h
Cria a janela secundária e a posiciona automaticamente junto à janela principal. O botão é desenhado com primitivas SDL, sem depender de imagens externas, e possui três estados visuais distintos: azul no estado padrão, azul claro ao passar o mouse sobre ele e azul escuro ao ser clicado. O rótulo do botão alterna entre "Equalizar" e "Ver original" de acordo com o estado vigente.


Observações
As DLLs do SDL3 devem estar no mesmo diretório que o arquivo main.exe.
A fonte utilizada é Arial (C:/Windows/Fonts/Arial.ttf); caso não seja encontrada, o programa continua funcionando sem exibir texto.
O tamanho da janela principal se ajusta às dimensões da imagem, respeitando o limite de 1280×720.
A janela secundária possui tamanho fixo de 420×520 e é aberta ao lado da janela principal.
