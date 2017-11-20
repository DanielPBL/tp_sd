
# Trabalho Prático 3: Detecção de Falhas #

Implementação de um serviço P2P estruturado, com detecção de falhas.

## Compilação ##

O projeto vai acompanhado de um arquivo `Makefile` que gera o executável do
peer (que se encontra na pasta raiz `.`) . Desta forma, para compilar o programa
basta executar o comando `make` no diretório indicado.

## Execução ##

Dentro da pasta `build`, execute o comando:
```
peer.out IP PORTA (TAXA_ERRO)
```
* **IP**: IP do peer
* **PORTA**: porta utilizada para receber conexões
* **TAXA_ERRO** _**(Opcional)**_: taxa de perda de mensagens (simulada)

### Comandos disponíveis ###

* **ENTER "IP" PORTA**: entra na rede P2P do peer informado
* **STORE(<K,"V">)**: armazena a tupla <K,V> no peer
* **FIND(K)**: busca a tupla identificada por K na rede
* **LIST**: lista as tuplas armazenadas neste peer
* **MEMBERS**: lista todos os peers filiados ao grupo
* **INFO**: exibe informações de controle do peer
* **HELP**: exibe o texto de ajuda
* **QUIT**: encerra a aplicação
