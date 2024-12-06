# Projeto Computação Gráfica 2024 OpenGL

Aluno: Guilherme Gimenes Liao
RA: 11201920847

## DESCRIÇÃO

Deve ser criado um modelo 3D que simule um objeto articulado, como um braço robótico, uma figura humana simplificada ou outro objeto com partes conectadas. As articulações devem ser controláveis por meio de teclado e mouse, e a cena deve incluir iluminação e texturas aplicadas nas partes do modelo.

### Compilar

```
g++ -Wall -Iinclude -g projeto.cpp -o projeto.exe -Llib -lopengl32 -lglu32 -lfreeglut
```


## Projeto

Modelo Articulado 3D de uma figura humana simplificada, apenas alguns movimentos foram limitados, pois o intuito é mostrar a articulação e a relação hierárquica de cada parte do modelo, focando em garantir que todas as transformações e rotações estão sendo executadas.

### Ações possíveis com o ***teclado***
  - Andar (Com movimentos e posições definidas)
  - Correr
  - Ficar parado (Reset das posições)
  - Mudar direção dos braços e elevação
  - Mudar direção dos antebraços
  - Mudar direção das pernas e joelhos
  - Mudar inclinação do tronco
  - Aumentar e diminuir intensidade da luz para a posição da câmera
  - Alterar tipo de sombra (Flat e Smooth)
  - Aumentar e diminuir velocidade da corrida
  - Parar animação de rotação da câmera

### Ações com as teclas especiais (GLUT_KEY)
  - Ajustar ângulos da câmera

### Ações com o mouse
  - Ajustar ângulos da câmera

### Opções do menu
  - Iniciar animação de andar
  - Parar animação com as posições atuais e parar a câmera
  - Reiniciar articulações
  - Aumentar e Diminuir iluminação
  - Sair do programa
