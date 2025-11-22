//
// Created by murandido on 11/4/25.
//

/* Criação do menu do produto, passo a passo do que fazer.
    * 1 - Criar o struct do produto. > V
    * 2 - Criar a interface do menu do produto. > V
    * 3 - Linkar o menu do produto ao menu principal. > F
    * 4 - Criar a função de adicionar produto. > F
    * 5 - Criar a função de listar produto. > F
    * 6 - Criar a função de editar produto. > F
    * 7 - Criar a função de remover produto. > F
    * 8 - Testar todas as funções. > F
*/
// Criar switch case para o menu. 

int MenuProduto() {
    // espaço para o link do menu principal.
    int opcao;
    printf("==================================\n");
    printf("        Menu Produtos        \n");
    printf("==================================\n");
    printf(" 1. InserirProduto\n");
    printf(" 2. Listar Produtos\n");
    printf(" 3. Editar Produto\n");
    printf(" 4. Remover Produto\n");
    printf(" 5.SalvarProdutosCSV\n");
    printf(" 6. CarregarProdutosCSV\n");
    printf(" 7. Voltar ao Menu Principal\n");
    printf("==================================\n");
    printf(" Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}