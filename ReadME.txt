--- SGBD-ITP ---

COMPONENTES
- Danilo Miranda de Medeiros Galv�o
- Jo�o Mendes Lopes Neto

FUNCIONALIDADES IMPLEMENTADAS
Todas as funcionalidades solicitadas foram implementadas

O QUE FAR�AMOS DE FORMA DIFERENTE
Poder�amos ter implementado nosso c�digo com uma modulariza��o melhor trabalhada (separando, por, exemplo, fun��es que escrevem algo na tela de fun��es que n�o escreve) e criado mais arquivos para conter fun��es de finalidades semelhantes. Al�m disso, poder�amos ter implementado o c�digo de forma que fossem aceitos acentos gr�ficos nas tabelas.

COMO COMPILAR
Para compilar o projeto, abra o terminal e se dirija � pasta do projeto. Depois, basta digitar o comando "make".p�s compilado, o comando gera o execut�vel "exe"
Para executar o programa, basta executar o execut�vel "exe"
No projeto, foram utilizadas as bibliotecas stdio.h, stdlib.h, string.h e math.h

EXTRAS
1)Chave prim�ria com auto-incremento ou n�o, � escolha do usu�rio
2)Colunas da tabela tem o par�metro not_null para aceitar ou n�o valores nulos, dependendo do desejo do usu�rio
3)Atualiza��o de registro
4)Comando "/cancelar" para cancelar a fun��o atual

ATRIBUI��O DE TAREFAS
1)Criar uma tabela - Danilo e Jo�o
2)Listar todas as tabelas - Danilo e Jo�o
3)Criar uma nova linha na tabela - Danilo e Jo�o
4)Listar todos os dados de uma tabela - Jo�o
5)Pesquisar valor em uma tabela - Danilo
6)Apagar valor de uma tabela - Danilo e Jo�o
7)Apagar uma tabela - Jo�o
8)Extra 1) - Danilo
9)Extra 2) - Jo�o
10)Extra 3) - Danilo e Jo�o
11)Extra 4) - Danilo e Jo�o
12)Corre��o de erros e revis�o do c�digo - Danilo e Jo�o

NOTAS
1)No item 5, a compara��o de strings pr�ximas � feita da seguinte forma: valor inserido pelo usu�rio � enviado junto ao valor da tabela a ser comparado para uma fun��o. Da�, a fun��o determina o valor que possui maior tamanho (m) e o que tem menor tamanho (n) e compara os n-�simos primeiros d�gitos do valor mais curto com os n-�simos d�gitos de cada uma das permuta��es circulares id�nticas do valor mais longo (permuta��es onde a sequ�ncia � a a mesma, por�m o in�cio � diferente. Exemplo: abc -> bca -> cab. O n�mero de permuta��es � dado pelo tamanho maior m), ao encontrar d�gitos iguais, a fun��o aumenta um contador e, caso seja maior que o maior contador anterior, atribui o novo contador ao maior contador(c). Por fim, a fun��o retorna uma divis�o n/c, transformada em inteiro, ou seja, arredondada para baixo e sem casas decimais. Caso essa divis�o seja menor que 2, a string inserida � tida como pr�xima da string que desejamos comparar e o programa imprime toda a linha. Resumindo: se metade dos caracteres da string inserida for igual a alguma combina��o de caracteres da string a ser comparada, a string � tida como pr�xima.

