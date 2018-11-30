--- SGBD-ITP ---

COMPONENTES
- Danilo Miranda de Medeiros Galvão
- João Mendes Lopes Neto

FUNCIONALIDADES IMPLEMENTADAS
Todas as funcionalidades solicitadas foram implementadas

O QUE FARÍAMOS DE FORMA DIFERENTE
Poderíamos ter implementado nosso código com uma modularização melhor trabalhada (separando, por, exemplo, funções que escrevem algo na tela de funções que não escreve) e criado mais arquivos para conter funções de finalidades semelhantes. Além disso, poderíamos ter implementado o código de forma que fossem aceitos acentos gráficos nas tabelas.

COMO COMPILAR
Para compilar o projeto, abra o terminal e se dirija à pasta do projeto. Depois, basta digitar o comando "make".pós compilado, o comando gera o executável "exe"
Para executar o programa, basta executar o executável "exe"
No projeto, foram utilizadas as bibliotecas stdio.h, stdlib.h, string.h e math.h

EXTRAS
1)Chave primária com auto-incremento ou não, à escolha do usuário
2)Colunas da tabela tem o parâmetro not_null para aceitar ou não valores nulos, dependendo do desejo do usuário
3)Atualização de registro
4)Comando "/cancelar" para cancelar a função atual

ATRIBUIÇÃO DE TAREFAS
1)Criar uma tabela - Danilo e João
2)Listar todas as tabelas - Danilo e João
3)Criar uma nova linha na tabela - Danilo e João
4)Listar todos os dados de uma tabela - João
5)Pesquisar valor em uma tabela - Danilo
6)Apagar valor de uma tabela - Danilo e João
7)Apagar uma tabela - João
8)Extra 1) - Danilo
9)Extra 2) - João
10)Extra 3) - Danilo e João
11)Extra 4) - Danilo e João
12)Correção de erros e revisão do código - Danilo e João

NOTAS
1)No item 5, a comparação de strings próximas é feita da seguinte forma: valor inserido pelo usuário é enviado junto ao valor da tabela a ser comparado para uma função. Daí, a função determina o valor que possui maior tamanho (m) e o que tem menor tamanho (n) e compara os n-ésimos primeiros dígitos do valor mais curto com os n-ésimos dígitos de cada uma das permutações circulares idênticas do valor mais longo (permutações onde a sequência é a a mesma, porém o início é diferente. Exemplo: abc -> bca -> cab. O número de permutações é dado pelo tamanho maior m), ao encontrar dígitos iguais, a função aumenta um contador e, caso seja maior que o maior contador anterior, atribui o novo contador ao maior contador(c). Por fim, a função retorna uma divisão n/c, transformada em inteiro, ou seja, arredondada para baixo e sem casas decimais. Caso essa divisão seja menor que 2, a string inserida é tida como próxima da string que desejamos comparar e o programa imprime toda a linha. Resumindo: se metade dos caracteres da string inserida for igual a alguma combinação de caracteres da string a ser comparada, a string é tida como próxima.

