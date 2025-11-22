//
// Created by murandido on 11/4/25.
//

#include "client.h"
    int validarCNPJ() {
    PessoaJuridicaId empresa; 
    // Variaveis axiliares fora da struct.
    // A leitura precisa ser da direita para esquerda logo no loop ambas posições serão invertidas.
    // DigitoVerificador 1.
    int soma=0, Divisao, DigitoVerificador;
    int pesos[14]= {1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 2, 3, 4, 5};
    // DigitoVerificador 2.
    int soma2=0, Divisao2, DigitoVerificador2;
    int pesos2 [14]= {2, 3, 4, 5, 6, 7, 8, 9, 2, 3, 4, 5,};
    
    printf("Digite o CNPJ ( Sem caracteres especiais.):\n");

    for (int i = 0; i < 14; i++) {
        scanf("%1d", &empresa.CNPJ[i]);
        empresa.CNPJclone[i]= empresa.CNPJ[i];
    }   
    // calculo com pesos.
    for (int i = 13; i >= 0; i--){
        empresa.CNPJ[i]= empresa.CNPJ[i] * pesos[i];
        empresa.CNPJclone[i]= empresa.CNPJclone[i] * pesos2[i];
        
        //soma dos resultados.
        soma = soma + empresa.CNPJ[i];
        soma2 = soma2 + empresa.CNPJclone[i];
    }
    
    // Dividir o total por 11 e obter o resto.
    Divisao = soma % 11;
   
    // 1 Verificação.
    if (Divisao < 2) 
    {
        DigitoVerificador=0;
    }
    else {DigitoVerificador = 11 - Divisao;}
    // 2 Verificação.
    Divisao2 = soma2 % 11;
    if (Divisao2 < 2)
    {
        DigitoVerificador2=0;
    }
    else {DigitoVerificador2 = 11 - Divisao2;}
    // O CNPJ e valido apenas quando ambos digitos verificadores baterem com o os 2 ultimos digitos.
    if (DigitoVerificador == empresa.CNPJ[12] && DigitoVerificador2 == empresa.CNPJ[13]){
        printf ("CNPJ Valido.\n");
    }
    else {
        printf ("CNPJ Invalido.\n");
    }
    
    return 0;
    }